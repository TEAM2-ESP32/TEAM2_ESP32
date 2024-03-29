#include <stdio.h>
#include "adcread.h"
#include "driver/adc.h"
#include "esp_adc_cal.h"

#define DEFAULT_VREF    1100
#define NO_OF_SAMPLES   64
#define A1_CHANNEL			ADC_CHANNEL_0     //GPIO36(SVP) if ADC1
#define A2_CHANNEL			ADC_CHANNEL_3     //GPIO39(SCN) if ADC1
#define A3_CHANNEL			ADC_CHANNEL_6     //GPIO34 if ADC1

static esp_adc_cal_characteristics_t *adc_chars;
static const adc_bits_width_t width = ADC_WIDTH_BIT_12;
static const adc_atten_t atten = ADC_ATTEN_DB_11;
static const adc_unit_t unit = ADC_UNIT_1;

static void check_efuse(void)
{
  //Check if TP is burned into eFuse
  if (esp_adc_cal_check_efuse(ESP_ADC_CAL_VAL_EFUSE_TP) == ESP_OK) {
    printf("eFuse Two Point: Supported\n");
  } else {
    printf("eFuse Two Point: NOT supported\n");
  }
  //Check Vref is burned into eFuse
  if (esp_adc_cal_check_efuse(ESP_ADC_CAL_VAL_EFUSE_VREF) == ESP_OK) {
    printf("eFuse Vref: Supported\n");
  } else {
    printf("eFuse Vref: NOT supported\n");
  }
}

static void print_char_val_type(esp_adc_cal_value_t val_type)
{
  if (val_type == ESP_ADC_CAL_VAL_EFUSE_TP) {
    printf("Characterized using Two Point Value\n");
  } else if (val_type == ESP_ADC_CAL_VAL_EFUSE_VREF) {
    printf("Characterized using eFuse Vref\n");
  } else {
    printf("Characterized using Default Vref\n");
  }
}

void adc_init(void)
{
  //Check if Two Point or Vref are burned into eFuse
  check_efuse();

  //Configure ADC
  if (unit == ADC_UNIT_1) {
    adc1_config_width(width);
  }

	//Characterize ADC
  adc_chars = calloc(1, sizeof(esp_adc_cal_characteristics_t));
  esp_adc_cal_value_t val_type = esp_adc_cal_characterize(unit, atten, width, DEFAULT_VREF, adc_chars);
  print_char_val_type(val_type);
}

void adc_read(int ch)
{
  static adc_channel_t channel;

  switch (ch) {
	case 1:
		channel = A1_CHANNEL;
		break;
	case 2:
		channel = A2_CHANNEL;
		break;
  case 3:
    channel = A3_CHANNEL;
    break;
	default:
		break;
	}

  //Configure ADC
  if (unit == ADC_UNIT_1) {
    adc1_config_channel_atten(channel, atten);
  } else {
    adc2_config_channel_atten((adc2_channel_t)channel, atten);
  }

  uint32_t adc_reading = 0;
  //Multisampling
  for (int i = 0; i < NO_OF_SAMPLES; i++) {
    if (unit == ADC_UNIT_1) {
      adc_reading += adc1_get_raw((adc1_channel_t)channel);
      //adc_reading += hall_sensor_read();
    } else {
      int raw;
      adc2_get_raw((adc2_channel_t)channel, width, &raw);
      adc_reading += raw;
    }
  }
  adc_reading /= NO_OF_SAMPLES;
  //Convert adc_reading to voltage in mV
  uint32_t voltage = esp_adc_cal_raw_to_voltage(adc_reading, adc_chars);
  float Ampere = (float)adc_reading / 136.5;
  switch(ch) {
    case 1: 
      printf("Raw: %d\tVoltage: %dmV\n", adc_reading, voltage);
      break;
    case 2:
      printf("Raw: %d\tVoltage: %dmV\n", adc_reading, voltage);
      break;
    case 3:
      printf("Raw: %d\tVoltage: %dmV\tAmpere: %.1f A\n", adc_reading, voltage, Ampere);
      //printf("Raw: %d\tVoltage: %dmV\n", adc_reading, voltage);
      break;
    default:
      break;
  }
  //vTaskDelay(pdMS_TO_TICKS(1000));
}
