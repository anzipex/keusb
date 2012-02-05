
/*
 * device.h -- заголовочный файл для модуля device.c
 *
 * Разработал: Муковников М. Ю.
 * Версия:     1.0.0
 * Изменён:    16.04.2010
 *
 */

#ifndef DEVICE_H
#define DEVICE_H

#define CONNECT_FILE 0
#define CONNECT_SIG  1
#define CONNECT_ANY  2

int keusb_request(const char *command, ...);
int keusb_connect(int type, char *path);

int keusb_status(int r_num);
char *keusb_status_all();

int keusb_turn_on_off(int r_num, int on_off);
int keusb_toggle(int r_num);
int keusb_reset(int r_num, int wait);
int keusb_hard_reset();

char *keusb_get_signature();
char *keusb_selftest();

#endif // DEVICE_H

