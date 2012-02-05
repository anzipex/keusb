
/*
 * windows/driver.h -- заголовочный файл модуля driver.c
 *
 * Разработал: Муковников М. Ю.
 * Версия:     1.0.0
 * Изменён:    16.04.2010
 *
 */

#ifndef DRIVER_H
#define DRIVER_H

char *device_gen_name();

int device_open(const char *name);
void device_close();

int device_write(const void *buf, size_t count);
int device_read(void *buf, size_t count);

#endif // DRIVER_H

