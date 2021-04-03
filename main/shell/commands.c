#include "commands.h"
#include <stddef.h>

static const shell_cmd_t commands[] = {
	{"exit", shell_cmd_exit, "Exit the shell" },
	{"help", shell_cmd_help, "List available commands" },
	{"md", shell_cmd_memdump, "md <addr> <len>" },
	{"relay", shell_cmd_relay, "relay <1|2> <0|1>" },
	{"adcread", shell_cmd_adcread, "adcread <1|2>"},
	{ NULL, NULL, NULL },
};

const shell_cmd_t *shell_get_command_list(void)
{
	return commands;
}
