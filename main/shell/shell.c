#include "shell.h"
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include "libmcu/shell.h"
#include "driver/uart.h"

static size_t shell_read(void *buf, size_t bufsize)
{
	// need to yield for some time here since fread() runs in non-blocking
	vTaskDelay(1);
	return fread(buf, bufsize, 1, stdin);
}

static size_t shell_write(const void *data, size_t datasize)
{
	return fwrite(data, datasize, 1, stdout);
}

static void shell_task(void *param)
{
	const shell_io_t io = {
		.read = shell_read,
		.write = shell_write,
	};
	shell_run(&io);
	vTaskDelete(NULL);
}

void shell_start(void)
{
	BaseType_t result = xTaskCreate(shell_task,
			"shell",
			3072,
			NULL,
			5,
			NULL);
	assert(result == pdPASS);
}
