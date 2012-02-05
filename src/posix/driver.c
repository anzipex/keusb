
/*
 * posix/driver.c -- модуль подключения к устройству Ke-USB24R
 *
 * Разработал: Муковников М. Ю.
 * Версия:     1.0.0
 * Изменён:    16.04.2010
 *
 */

#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#define READ_TIMEOUT 100

static char device_name[] = "/dev/ttyACM?";
static int  device_fd = -1;

char *
device_gen_name()
{
   static int try_num = 0;
   static int mod_num = 0;

   if (try_num > 9)
   {
      if (!mod_num++)
      {
         try_num         =  0;
         device_name[8]  = 'U';
         device_name[9]  = 'S';
         device_name[10] = 'B';
      }
      else
         return 0;
   }

   device_name[11] = '0' + try_num++;
   return device_name;
}

int
device_open(const char *name)
{
   struct termios tty;

   device_fd = open(name, O_RDWR | O_NDELAY | O_NOCTTY);
	 
   if (device_fd == -1)
      return 0;

   ioctl(device_fd, TCGETA, &tty);
   tty.c_lflag     = 0;
   tty.c_iflag     = BRKINT;
   tty.c_oflag     = 0;
   tty.c_cflag     = B9600 | CS8 | CREAD | CLOCAL | PARENB;
   tty.c_cflag    &= ~PARODD;
   tty.c_cc[VMIN]  = 0;
   tty.c_cc[VTIME] = 1;
   ioctl(device_fd, TCSETA, &tty);

   return 1;
}

int
device_write(const void *buf, size_t count)
{
   return write(device_fd, buf, count) + 1;
}

int
device_read(void *buf, size_t count)
{
   usleep(READ_TIMEOUT * 1000);
   return read(device_fd, buf, count) + 1;
}

void
device_close()
{
   close(device_fd);
}

