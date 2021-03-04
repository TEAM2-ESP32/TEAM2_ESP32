#include "driver/uart.h"
#include "driver/gpio.h"

void myuart_init(void)
{
    uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_APB,
    };

    uart_driver_install(UART_NUM_2, UART_FIFO_LEN, 0, 0, NULL, 0);
    uart_param_config(UART_NUM_2, &uart_config);
    uart_set_pin(UART_NUM_2, GPIO_NUM_23, GPIO_NUM_22, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
}

void myuart_test(void)
{
    while (1) {
        uint8_t buf[128];
        int len = uart_read_bytes(UART_NUM_2, buf, sizeof(buf), 20 / portTICK_RATE_MS);
        uart_write_bytes(UART_NUM_2, (const char *)buf, len);
    }
}