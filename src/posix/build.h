
/*
 * posix/build.h -- макроопределения и параметры компиляции
 *
 * Разработал: Муковников М. Ю.
 * Версия:     1.0.0
 * Изменён:    16.04.2010
 *
 */

#ifndef BUILD_H
#define BUILD_H

#define shift() (++argv, --argc)
#define shift_or_die() if (!shift()) return die("cmdline_parse: missing operand")
#define eq(x) !strcmp(argv[0], (x))
#define eq2(x, y) !strcmp((x), (y))
#define die_p(x, y) if(!(x)) die((y))
#define kexec(x) if(!(x)) return !die("keusb_main: failed")

#define VERSION_STR "1.0.0"
#define USAGE_COM   "/dev/ttyXX    "
#define CONFIG_NAME "/etc/conf.d/keusb"
#define LOG_NAME    "/var/log/keusbd.log"
#define PID_NAME    "/var/run/keusbd.pid"
#define OS_SPECIFIC_OPTIONS

int die(const char *str);
int logger(char level, const char *str);

#endif // BUILD_H

