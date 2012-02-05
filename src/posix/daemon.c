
/*
 * posix/daemon.c -- модуль связи с операционной системой
 *
 * Разработал: Муковников М. Ю.
 * Версия:     1.0.0
 * Изменён:    16.04.2010
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

#include "build.h"

void
daemonize()
{
   pid_t pid = fork();

   if (pid < 0) exit(1);
   if (pid > 0) exit(0);

   umask(0);
   setsid();
	 
   close(STDIN_FILENO);
   close(STDOUT_FILENO);
   close(STDERR_FILENO);
}

int
ping(const char *host, int tries)
{
   int status;
   char buf[80];

   snprintf(buf, 80, "ping -c %d %s > /dev/null 2>&1", tries, host);
   status = system(buf);

   return (status != -1) ? WEXITSTATUS(status) : 2;
}

int
cmdline_parse_rest(int argc, char *argv[])
{
   return 0;
}

