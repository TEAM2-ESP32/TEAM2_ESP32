#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <esp_err.h>
#include <esp_log.h>
#include "dht_espidf.h"
#include "dht.h"

#define DHT_PIN           GPIO_NUM_13

void dht_read(void)
{
    static const char *TAG = "TEAM2_ESP32";
    struct dht_reading dht_data;

    dht_result_t res = read_dht_sensor_data((gpio_num_t)DHT_PIN, DHT11, &dht_data);
    //vTaskDelay(200 / portTICK_PERIOD_MS);             // In the event of periodic "DHT sensor reading failed". I think it's the difference between hardware(dht).
 
    if (res != DHT_OK) {
        ESP_LOGW(TAG, "DHT sensor reading failed");
    } else {
        double temperature = dht_data.temperature;
        double humidity = dht_data.humidity;
        ESP_LOGI(TAG, "DHT sensor reading: %4.1f° / %4.1f%%", temperature, humidity);
    }
}