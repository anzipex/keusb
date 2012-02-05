
/*
 * device.c -- модуль связи с устройством Ke-USB24R
 *
 * Разработал: Муковников М. Ю.
 * Версия:     1.0.0
 * Изменён:    16.04.2010
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <unistd.h>
#include <ctype.h>

#include "build.h"
#include "device.h"
#include "driver.h"

#define REQUEST_SIZE 200
#define REPLY_PARTS  10

static char *airbag = "";
static char *device_name = "";
static int  reply_size = 0;
static char reply_buf[REQUEST_SIZE];
static char *reply_part[REPLY_PARTS];

int
keusb_request(const char *command, ...)
{
   va_list ap;
   int length;
   int tries = 3;
   char request[REQUEST_SIZE + 3];

   va_start(ap, command);
   vsnprintf(request, REQUEST_SIZE, command, ap);
   va_end(ap);

   length = strlen(request);
   request[length]     = '\r';
   request[length + 1] = '\n';
   request[length + 2] = '\0';

   while (tries--)
   {
      int parts = 0;
      char *p, *q;

      reply_buf[0] = '\0';
      device_write(request, length + 2);
      device_read(reply_buf, REQUEST_SIZE);

      if (!strncmp(reply_buf, "#ERR\r\n", 6) 
          || reply_buf[0] != '#')
         continue;

      for (p = q = reply_buf + 1; *p != '\r'; p++)
         if (*p == ',')
         {
            reply_part[parts++] = q;
            *p = '\0';
            q = p + 1;
         }

      *p = '\0';
      reply_size = parts + 1;
      reply_part[parts] = q;
      break;
   }

   return tries + 1;
}

char *
str_to_lower(char *str)
{
   char *p = str;

   while (*p && *p != ' ')
      *p = tolower(*p), p++;

   *p = '\0';
   return str;
}

char *
keusb_get_signature()
{
   if (!keusb_request("$KE,SER") || reply_size < 2)
      return airbag;

   return str_to_lower(reply_part[1]);
}

int
keusb_connect(int type, char *path)
{
   char *name;
   device_name = path ? path : airbag;

   if (type == CONNECT_FILE)
      return (device_open(path)
              && keusb_request("$KE"))
         || die("keusb_connect: can't open device");

   while ((name = device_gen_name()))
   {
      if (!device_open(name))
         continue;

      if (!keusb_request("$KE"))
      {
         device_close();
         continue;
      }

      if (type == CONNECT_SIG
          && strcmp(keusb_get_signature(), str_to_lower(path)))
      {
         device_close();
         continue;
      }

      device_name = name;
      atexit(device_close);
      return 1;
   }

   return die("keusb_connect: can't find device");
}

int
keusb_status(int r_num)
{
   if (!keusb_request("$KE,RDR,%d", r_num) || reply_size < 3)
      die("keusb_status: request failed");
	 
   return reply_part[2] ? atoi(reply_part[2]) : 2;
}

char *
keusb_status_all()
{
   int i;
   char buf[REQUEST_SIZE];

   if (!keusb_request("$KE,RDR,ALL"))
   {
      die("keusb_status_all: request failed");
      return airbag;
   }

   buf[0] = '\0';
   for (i = 2; i < reply_size; i++)
   {
      strcat(buf, reply_part[i]);
      strcat(buf, "\t");
   }

   return strcpy(reply_buf, buf);
}	 

int
keusb_turn_on_off(int r_num, int on_off)
{
   return keusb_request("$KE,REL,%d,%d", r_num, on_off)
      || die("keusb_turn_on_off: request failed");
}

int
keusb_toggle(int r_num)
{
   int status = keusb_status(r_num);
	 
   if (status == 2)
      return die("keusb_toggle: request failed");
   else
      return keusb_turn_on_off(r_num, !status)
         || die("keusb_toggle: turn_on_off failed");
}

int
keusb_reset(int r_num, int wait)
{
   int status = keusb_status(r_num);

   if (status == 2)
      return die("keusb_reset: status failed");
   else
      return (keusb_turn_on_off(r_num, !status)
              && !sleep(wait)
              && keusb_turn_on_off(r_num, status))
         || die("keusb_reset: turn_on_off failed");
}

char *
keusb_selftest()
{
   char info[REQUEST_SIZE];

   if (!keusb_request("$KE,USB,GET") || reply_size < 2)
   {
      die("keusb_selftest: request failed");
      return airbag;
   }
 
   strncpy(info, device_name, 50);
   strcat(info, " | ");
   strncat(info, reply_part[1], 50);
   strcat(info, " | ");
   strncat(info, keusb_get_signature(), 80);
   strcat(info, " | status: OK");
   strcpy(reply_buf, info);

   return reply_buf;
}

int
keusb_hard_reset()
{
   return keusb_request("$KE,RST")
      || die("keusb_hard_reset: request failed");
}

