
/*
 * posix/daemon.h -- заголовочный файл модуля daemon.c
 *
 * Разработал: Муковников М. Ю.
 * Версия:     1.0.0
 * Изменён:    16.04.2010
 *
 */

#ifndef DAEMON_H
#define DAEMON_H

void daemonize();
int ping(const char *host, int tries);
int cmdline_parse_rest(int argc, char *argv[]);

#endif // DAEMON_H

