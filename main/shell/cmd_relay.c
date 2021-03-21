#include "commands.h"
#include "libmcu/shell.h"
#include "relay.h"

shell_cmd_error_t shell_cmd_relay(int argc, const char *argv[], const void *env)
{
	if (argc != 3) {
		return SHELL_CMD_INVALID_PARAM;
	}

	const shell_io_t *io = env;

	int relay_num = argv[1][0] - '0';
	int level = argv[2][0] - '0';

	if (level < 0 || level > 1 || relay_num < 1 || relay_num > 2) {
		io->write("Invalid parameters\r\n", 20);
		return SHELL_CMD_INVALID_PARAM;
	}

	switch (relay_num) {
	case 1:
		io->write("Relay1\r\n", 8);
		relay1_set(level);
		break;
	case 2:
		io->write("Relay2\r\n", 8);
		relay2_set(level);
		break;
	default:
		break;
	}

	return SHELL_CMD_SUCCESS;
}
