#include "commands.h"
#include "libmcu/compiler.h"
#include "libmcu/shell.h"
#include "dht.h"
#include <stdio.h>

shell_cmd_error_t shell_cmd_dht(int argc, const char *argv[], const void *env)
{
	unused(argc);
	unused(argv);
	unused(env);

    dht_read();

	return SHELL_CMD_SUCCESS;
}
