#include "relay.h"
#include "driver/gpio.h"

#define RELAY1_PIN				GPIO_NUM_32
#define RELAY2_PIN				GPIO_NUM_33
#define RELAY1_BIT				GPIO_SEL_32
#define RELAY2_BIT				GPIO_SEL_33

static void set_level(int pin, bool level)
{
	gpio_set_level(pin, level);
}

void relay1_set(bool level)
{
	set_level(RELAY1_PIN, level);
}

void relay2_set(bool level)
{
	set_level(RELAY2_PIN, level);
}

void relay_init(void)
{
	gpio_config_t io_conf;
	io_conf.intr_type = GPIO_INTR_DISABLE;
	io_conf.mode = GPIO_MODE_OUTPUT;
	io_conf.pin_bit_mask = RELAY1_BIT | RELAY2_BIT;
	io_conf.pull_down_en = 0;
	io_conf.pull_up_en = 0;
	gpio_config(&io_conf);
}
