#ifndef LIBMCU_SHELL_CMDS_H
#define LIBMCU_SHELL_CMDS_H 1

#if defined(__cplusplus)
extern "C" {
#endif

#include "../src/shell_command.h"

shell_cmd_error_t shell_cmd_exit(int argc, const char *argv[], const void *env);
shell_cmd_error_t shell_cmd_help(int argc, const char *argv[], const void *env);
shell_cmd_error_t shell_cmd_memdump(int argc, const char *argv[], const void *env);
shell_cmd_error_t shell_cmd_relay(int argc, const char *argv[], const void *env);
shell_cmd_error_t shell_cmd_adcread(int argc, const char *argv[], const void *env);

#if defined(__cplusplus)
}
#endif

#endif /* LIBMCU_SHELL_CMDS_H */
