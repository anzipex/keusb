
/*
 * windows/build.h -- макроопределения и параметры компиляции
 *
 * Разработал: Муковников М. Ю.
 * Версия:     1.0.0
 * Изменён:    16.04.2010
 *
 */

#ifndef BUILD_H
#define BUILD_H

#include <windows.h>

#define sleep(x) (Sleep((x)*1000),0)
#define kill(x, y) !TerminateProcess(OpenProcess(PROCESS_TERMINATE, 0, (x)), 0)

#define shift() (++argv, --argc)
#define shift_or_die() if (!shift()) return die("cmdline_parse: missing operand")
#define eq(x) !strcmp(argv[0], (x))
#define eq2(x, y) !strcmp((x), (y))
#define die_p(x, y) if(!(x)) die((y))
#define kexec(x) if(!(x)) return !die("keusb_main: failed")
 
#define VERSION_STR "1.0.0"
#define USAGE_COM   "com1: .. com9:"
#define CONFIG_NAME "keusbd.conf"
#define LOG_NAME    "keusbd.log"
#define PID_NAME    "keusbd.pid"

#define OS_SPECIFIC_OPTIONS                                 \
   "  -ay                                Set autorun on\n"	\
   "  -an                                Set autorun off\n"

int die(const char *str);
int logger(char level, const char *str);

#endif // BUILD_H

