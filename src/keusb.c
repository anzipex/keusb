
/*
 * keusb.c -- интерфейс к устройству Ke-USB24R
 *
 * Разработал: Муковников М. Ю.
 * Версия:     1.0.0
 * Изменён:    16.04.2010
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "build.h"
#include "device.h"

#define RESET_SLEEP 5

int
die(const char *str)
{
   fprintf(stderr, "error: %s\n", str);
   exit(1);
   return 0;
}

int
print_usage()
{
   puts("KeUSB* module interface.\n"
        "Usage: keusb [<device>] <command> <#>\n\n"
        "device:\n"
        "  " USAGE_COM  "                     Use specified device\n"
        "  xxxx-xxxx-xxxx-xxxx                Use signature to find device\n\n"
        "command:\n"
        "  --help                             Display this help\n"
        "  --vesrion                          Display version information\n\n"
        "  turn_on <#>                        Turn relay # on\n"
        "  turn_off <#>                       Turn relay # off\n"
        "  toggle <#>                         Toggle relay #\n"
        "  reset <#> [<sec>]                  Reset relay # on 5 or <sec> seconds\n"
        "  status <#>                         Display status of relay #\n"
        "  status all                         Display status of all relays\n\n"
        "  status                             Display device information\n"
        "  reset -1                           Hard reset device\n\n"
        "Copyright Mikhail Mukovnikov, 2010\n"
        "Email bugs to <mix_mix@pop3.ru>");
		  
   return 2;
}


int
main(int argc, char *argv[])
{
   int stat;

   if (!shift())
      return print_usage();

   if (eq("--help"))
      return print_usage();

   if (eq("--version"))
      return !puts("keusb " VERSION_STR);

   if (!(eq("turn_on")
         || eq("turn_off")
         || eq("toggle")
         || eq("reset")
         || eq("status")))
   {
      if (strlen(argv[0]) == 19
          && strchr(argv[0], '-'))
         stat = keusb_connect(CONNECT_SIG, argv[0]);
      else
         stat = keusb_connect(CONNECT_FILE, argv[0]);

      if (!shift())
         return print_usage();
   }
   else
      stat = keusb_connect(CONNECT_ANY, 0);

   if (!stat)
      return !die("keusb_main: can't open device");

   if(eq("turn_on"))
   {
      shift_or_die();
      kexec(keusb_turn_on_off(atoi(argv[0]), 1));
      return 0;
   }

   if (eq("turn_off"))
   {
      shift_or_die();
      kexec(keusb_turn_on_off(atoi(argv[0]), 0));
      return 0;
   }

   if (eq("toggle"))
   {
      shift_or_die();
      kexec(keusb_toggle(atoi(argv[0])));
      return 0;
   }

   if (eq("reset"))
   {
      int op1, op2;

      shift_or_die();
      op1 = atoi(argv[0]);
      op2 = shift() ? atoi(argv[0]) : RESET_SLEEP;

      if (op1 == -1)
      { kexec(keusb_hard_reset()); }
      else
      { kexec(keusb_reset(op1, op2)); }

      return 0;
   }

   if (eq("status"))
   {
      if (shift())
      {
         if(eq("all"))
         {
            puts(keusb_status_all());
            return 0;
         }

         stat = keusb_status(atoi(argv[0]));

         if (stat == 2)
            return die("keusb_main: status failed") - 1;

         printf("%d\n", stat);
         return stat;
      }
      else
         puts(keusb_selftest());

      return 0;
   }

   return 1;
}

