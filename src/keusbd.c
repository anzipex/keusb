
/*
 * keusbd.c -- анализатор доступности узлов сети
 *
 * Разработал: Муковников М. Ю.
 * Версия:     1.0.0
 * Изменён:    16.04.2010
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>

#include "build.h"
#include "config.h"
#include "daemon.h"

FILE* log_fd;

int
logger(char level, const char *str)
{
   char buffer[80];
   time_t rawtime;
   struct tm *timeinfo;
   const char *p = str;

   while (*p++);
   if (str == --p)
      return 0;

   time(&rawtime);
   timeinfo = localtime(&rawtime);
   strftime(buffer, 80, "%b %d %X", timeinfo);

   fprintf(log_fd, "(%c%c) %s > %s",
           level, level, buffer, str);

   if (*--p != '\n')
      fputc('\n', log_fd);

   fflush(log_fd);
   return 0;
}

void
finalize(int code)
{
   logger('I', "keusbd stopped");
   fclose(log_fd);
   exit(code);
}

int
die(const char *str)
{
   logger('E', str);
   finalize(1);
   return 0;
}

int
print_usage()
{
   puts("KeUSB daemon.\n"
        "Usage: keusbd [<args>]\n\n"
        "general:\n"
        "  --help                             Display this help\n"
        "  --version                          Display version information\n"
        "  --daemon                           Run daemon\n\n"
        "files:\n"
        "  -f <cfg>                           Config file\n"
        "  -l <log>                           Log file\n"
        "  -p <pid>                           Pid file\n\n"
        "command:\n"
        "  -c                                 Validate config and exit\n"
        "  -d                                 Same as --daemon\n"
        "  -k                                 Kill daemon\n"
        OS_SPECIFIC_OPTIONS
        "\nCopyright Mikhail Mukovnikov, 2010\n"
        "Email bugs to <mix_mix@pop3.ru>");
		  
   return 2;
}

int
main(int argc, char *argv[])
{
   FILE *pid_fd;
   char cfg_name[] = CONFIG_NAME;
   char log_name[] = LOG_NAME;
   char pid_name[] = PID_NAME;
   char *file_n[3] = {cfg_name, log_name, pid_name};

   signal(SIGINT, finalize);
   signal(SIGTERM, finalize);

   if (argc < 2)
      return print_usage();

   log_fd = stdout;
   while (shift())
   {
      if (eq("--help"))
         return print_usage();

      if (eq("--version"))
         return !puts("keusbd " VERSION_STR);

      if (eq("--daemon") || eq("-d"))
         continue;

      if (eq("-f"))
      {
         shift_or_die();
         file_n[0] = argv[0];
         continue;
      }

      if (eq("-l"))
      {
         shift_or_die();
         file_n[1] = argv[0];
         continue;
      }

      if (eq("-p"))
      {
         shift_or_die();
         file_n[2] = argv[0];
         continue;
      }

      if (eq("-c"))
      {
         config_parse(file_n[0]);
         rules_test();
         logger('I', "config was successfully parsed");
         return 0;
      }

      if (eq("-k"))
      {
         int size;
         pid_t pid;

         pid_fd = fopen(file_n[2], "r");
         die_p(pid_fd, "daemon_kill: it seems daemon isn't running");
         size = fscanf(pid_fd, "%u", &pid);
         fclose(pid_fd);

         die_p(!kill(pid, SIGTERM), "daemon_kill: can't kill process");
         return 0;
      }

      die_p(cmdline_parse_rest(argc, argv),
            "cmdline_parse: invalid argument");
   }

   daemonize();

   log_fd = fopen(file_n[1], "a");
   die_p(log_fd, "logger_init: can't open file");
   fputc('\n', log_fd);
   logger('I', "keusbd started");

   pid_fd = fopen(file_n[2], "w+");
   die_p(pid_fd, "daemon_pid: can't open pid file");
   fprintf(pid_fd, "%u\n", getpid());
   fclose(pid_fd);

   config_parse(file_n[0]);
   rules_test();

   while (1)
   {
      rules_exec();
   }

   finalize(0);
   return 0;
}

