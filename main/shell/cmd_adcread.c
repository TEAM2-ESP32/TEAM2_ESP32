#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "commands.h"
#include "libmcu/shell.h"
#include "adcread.h"
#include <stdio.h>

shell_cmd_error_t shell_cmd_adcread(int argc, const char *argv[], const void *env)
{
	if (argc != 2) {
		return SHELL_CMD_INVALID_PARAM;
	}

	const shell_io_t *io = env;
    char *buffer;
    buffer = (char*)malloc(sizeof(char));

	int ch = argv[1][0] - '0';

	if (ch < 1 || ch > 3) {
		io->write("Invalid parameters\r\n", 20);
		return SHELL_CMD_INVALID_PARAM;
	}

	switch (ch) {
	case 1:
		io->write("adcread 1\r\n", 11);
		break;
	case 2:
		io->write("adcread 2\r\n", 11);
		break;
	case 3:
		io->write("adcread 3\r\n", 11);
		break;
	default:
		break;
	}

    while(true) {
        if(io->read(buffer, 1)) return SHELL_CMD_SUCCESS;;
        adc_read(ch);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }

	return SHELL_CMD_SUCCESS;
}
