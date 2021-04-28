#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "commands.h"
#include "libmcu/compiler.h"
#include "libmcu/shell.h"
#include "dht.h"
#include <stdio.h>

shell_cmd_error_t shell_cmd_dht(int argc, const char *argv[], const void *env)
{
    const shell_io_t *io = env;
    char *buffer;
    buffer = (char*)malloc(sizeof(char));

 	unused(argc);
	unused(argv);

    while(true) {
        if(io->read(buffer, 1)) return SHELL_CMD_SUCCESS;;
        dht_read();
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
 
	return SHELL_CMD_SUCCESS;
}
