#include "myi2c.h"
#include "driver/i2c.h"

#define I2C_NUM             I2C_NUM_0

void myi2c_init(void)
{
    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = 26,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_io_num = 27,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = 100000,
    };
    esp_err_t err = i2c_param_config(I2C_NUM, &conf);
    if (err != ESP_OK || i2c_driver_install(I2C_NUM, conf.mode, 0, 0, 0) != ESP_OK) {
        // error
    }
}

/**
 * @brief test code to read esp-i2c-slave
 *        We need to fill the buffer of esp slave device, then master can read them out.
 *
 * _______________________________________________________________________________________
 * | start | slave_addr + rd_bit +ack | read n-1 bytes + ack | read 1 byte + nack | stop |
 * --------|--------------------------|----------------------|--------------------|------|
 *
 */
#define ESP_SLAVE_ADDR 0

bool i2c_master_read_slave(uint8_t *data_rd, size_t size)
{
    if (size == 0) {
        return false;
    }

    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (ESP_SLAVE_ADDR << 1) | I2C_MASTER_READ, 1);
    if (size > 1) {
        i2c_master_read(cmd, data_rd, size - 1, 0);
    }
    i2c_master_read_byte(cmd, data_rd + size - 1, 1);
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(I2C_NUM, cmd, 1000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);
    return ret == ESP_OK;
}

/**
 * @brief Test code to write esp-i2c-slave
 *        Master device write data to slave(both esp32),
 *        the data will be stored in slave buffer.
 *        We can read them out from slave buffer.
 *
 * ___________________________________________________________________
 * | start | slave_addr + wr_bit + ack | write n bytes + ack  | stop |
 * --------|---------------------------|----------------------|------|
 *
 */
bool i2c_master_write_slave(uint8_t *data_wr, size_t size)
{
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (ESP_SLAVE_ADDR << 1) | I2C_MASTER_WRITE, 1);
    i2c_master_write(cmd, data_wr, size, 1);
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(I2C_NUM, cmd, 1000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);
    return ret == ESP_OK;
}