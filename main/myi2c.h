#ifndef MYI2C_H
#define MYI2C_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

void myi2c_init(void);
bool i2c_master_read_slave(uint8_t *data_rd, size_t size);
bool i2c_master_write_slave(uint8_t *data_wr, size_t size);

#endif