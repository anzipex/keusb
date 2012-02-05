
/*
 * config.h -- заголовочный файл модуля config.c
 *
 * Разработал: Муковников М. Ю.
 * Версия:     1.0.0
 * Изменён:    16.04.2010
 *
 */

#ifndef CONFIG_H
#define CONFIG_H

char *config_parse_line(char **str);
void config_parse(char *name);

void rules_test();
void rules_exec();
void rules_free();

#endif // CONFIG_H

