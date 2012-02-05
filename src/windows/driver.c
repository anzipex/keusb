
/*
 * windows/driver.c -- модуль подключения к устройству Ke-USB24R
 *
 * Разработал: Муковников М. Ю.
 * Версия:     1.0.0
 * Изменён:    16.04.2010
 *
 */

#include <windows.h>
#include <stdio.h>

#define READ_TIMEOUT 100

static char device_name[] = "\\\\.\\COM?";
static HANDLE device_fd = 0;

char *
device_gen_name()
{
   static int try_num = 1;

   if (try_num > 9)
      return 0;

   device_name[7] = '0' + try_num++;
   return device_name;
}

int
device_open(const char *name)
{
   DCB dcb;
   COMMTIMEOUTS CommTimeOuts;

   device_fd = CreateFile(name, GENERIC_READ | GENERIC_WRITE,
                          0, NULL, OPEN_EXISTING, 0, NULL);
	
   if (device_fd == INVALID_HANDLE_VALUE)
      return 0;

   GetCommState(device_fd, &dcb);

   dcb.BaudRate = CBR_9600;
   dcb.ByteSize = 8;
   dcb.Parity   = NOPARITY;
   dcb.StopBits = ONESTOPBIT;
		  
   CommTimeOuts.ReadIntervalTimeout         = MAXDWORD;
   CommTimeOuts.ReadTotalTimeoutMultiplier  = 0;
   CommTimeOuts.ReadTotalTimeoutConstant    = 0;
   CommTimeOuts.WriteTotalTimeoutMultiplier = 0;
   CommTimeOuts.WriteTotalTimeoutConstant   = 1000;

   SetCommTimeouts(device_fd, &CommTimeOuts);
   SetCommState(device_fd, &dcb);

   return 1;
}

int
device_write(const void *buf, size_t count)
{
   DWORD bytes_written;

   WriteFile(device_fd, buf, count, &bytes_written, NULL);
   return bytes_written;
}

int
device_read(void *buf, size_t count)
{
   DWORD bytes_read;

   Sleep(READ_TIMEOUT);
   ReadFile(device_fd, buf, count, &bytes_read, NULL);
   return bytes_read;
}

void
device_close()
{
   CloseHandle(device_fd);
}

