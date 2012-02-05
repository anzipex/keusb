
/*
 * config.c -- модуль обработки конфигурационного файла
 *
 * Разработал: Муковников М. Ю.
 * Версия:     1.0.0
 * Изменён:    16.04.2010
 *
 */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "build.h"
#include "daemon.h"

typedef struct ring_t
{
   char *str;
   struct ring_t *expr;
   struct ring_t *next;
   struct ring_t *prev;
} ring_t;

static int conf_wait_time = 5*60;
static int conf_tries_num = 10;
static char *config_file;
static ring_t rules = {0, 0, &rules, &rules};

static void *
xalloc(size_t size)
{
   void *ptr = malloc(size);
   die_p(ptr, "xalloc: can't allocate memory");
   return ptr;
}

void
rules_add(ring_t *rule, char *str)
{
   die_p(rule, "rules_add: missing action");

   while (1)
   {
      ring_t *t = (ring_t *) xalloc(sizeof(ring_t));

      t->str = str;
      t->expr = 0;
      t->next = rule;
      t->prev = rule->prev;
      rule->prev->next = t;
      rule->prev = t;
      
      while (*str && *str != ' ') str++;
      if (!*str) break;
      *str++ = '\0';
   }
}

int
rules_test_expr(ring_t **expr)
{
   char *str;

   *expr = (*expr)->next;
   str = (*expr)->str;

   die_p(str, "rules_test_expr: missing argument");

   if (eq2(str, "and")
       || eq2(str, "or")
       || eq2(str, "xor"))
      return rules_test_expr(expr)
         + rules_test_expr(expr) + 1;

   if (eq2(str, "not"))
      return rules_test_expr(expr) + 1;

   return 1;
}
	 
int
rules_eval_expr(ring_t **expr)
{
   char *str;

   *expr = (*expr)->next;
   str = (*expr)->str;

   if (eq2(str, "and"))
   {
      int a, b;
      a = rules_eval_expr(expr);
      b = a ? rules_eval_expr(expr) : rules_test_expr(expr);
      return a && b;
   }

   if (eq2(str, "or"))
   {
      int a, b;
      a = rules_eval_expr(expr);
      b = a ? rules_test_expr(expr) : rules_eval_expr(expr);
      return a || b;
   }

   if (eq2(str, "not"))
      return !rules_eval_expr(expr);

   if (eq2(str, "xor"))
      return rules_eval_expr(expr)
         ^ rules_eval_expr(expr);

   return ping(str, conf_tries_num);
}

void
rules_test()
{
   ring_t *k;

   for (k = rules.next; k != &rules; k = k->next)
   {
      ring_t *t  = k->expr;
      rules_test_expr(&t);
      die_p(t->next == k->expr, "rules_exec: unlinked expression");
   }
}

void
rules_exec()
{
   ring_t *k;

   for (k = rules.next; k != &rules; k = k->next)
   {
      ring_t *t  = k->expr;
      int result = rules_eval_expr(&t);
		  
      if (result)
      {
         FILE *pipe;
         char buf[85];

         logger('=', k->str);
         strncpy(buf, k->str, 80);
         strcat(buf, " 2>&1");
         pipe = popen(buf, "r");

         if (!pipe)
         {
            logger('W', "rules_exec: can't open pipe");
            continue;
         }

         while(fgets(buf, 85, pipe))
            logger('-', buf);

         pclose(pipe);
      }
   }

   sleep(conf_wait_time);
}

void
rules_free()
{
   ring_t *k, *r, *t;

   k = rules.next;
   while (k != &rules)
   {
      r = k->expr->next;
      while (r != k->expr)
      {
         t = r;
         r = r->next;
         free(t);
      }

      t = k;
      k = k->next;
      free(t);
   }
	
   free(config_file);
}

void
config_init(const char *name)
{
   int size;
   FILE *fp = fopen(name, "r");
	 
   die_p(fp, "config_init: can't open config");

   fseek(fp, 0L, SEEK_END);
   size = ftell(fp);
   rewind(fp);

   config_file = xalloc(size + 2);
   memset(config_file, 0, size + 2);

   size = fread(config_file, size, sizeof(char), fp);
   fclose(fp);
   atexit(rules_free);
}

char *
config_parse_line(char **str)
{
   char *p, *q, *k;

   while (1)
   {
      int fl = 0;

      p = *str;
      while (*p && isspace(*p)) p++;
      if (!*p) return 0;

      q = p;
      while (*q && *q != '\n' && *q != '#') q++;
      if (p == q) q--;
      while (q > p && isspace(*--q));

      k = q;
      while (*++k && *k != '\n');
      if (*k) k++;

      *++q = '\0';
      *str = q = p--;
      while (*++p)
      {
         int t = isspace(*p);

         if (t && fl) continue;
         fl = t ? 1 : 0;
         *q++ = t ? ' ' : *p;
      }

      *q = '\0';
      if (q - *str) return k;

      *str = k;
   }

   return 0;
}

void
config_parse(const char *name)
{
   char *p, *q;

   config_init(name);

   p = config_file;
   while ((q = p, p = config_parse_line(&q)))
   {
      if (q[0] == '!')
      {
         char *c = (*++q = ' ') ? ++q : q;

         while (*q && *q != ' ') q++;
         die_p(*q, "config_parse: argument not found");
         *q++ = '\0';

         if (eq2(c, "tries"))
         {
            conf_tries_num = atoi(q);
            die_p(conf_tries_num > 0 && conf_tries_num < 100,
                  "config_parse: invalid argument");
            continue;
         }

         if (eq2(c, "wait"))
         {
            char *postfix;

            conf_wait_time = strtol(q, &postfix, 10);
            die_p(conf_wait_time > 0, "config_parse: invalid argument");

            switch (*postfix)
            {
               case 's': case '\0': break;
               case 'm': conf_wait_time *= 60; break;
               case 'h': conf_wait_time *= 3600; break;
               default: die("config_parse: invalid postfix"); break;
            }

            continue;
         }

         die("config_parse: invalid option");
      }

      if (q[0] == '%')
      {
         ring_t *t = (ring_t *) xalloc(sizeof(ring_t));

         t->str = (*++q == ' ') ? q + 1 : q;
         t->expr = (ring_t *) xalloc(sizeof(ring_t));
         t->expr->str = 0;
         t->expr->expr = 0;
         t->expr->next = t->expr;
         t->expr->prev = t->expr;
         t->next = &rules;
         t->prev = rules.prev;
         rules.prev->next = t;
         rules.prev = t;
         continue;
      }

      rules_add(rules.prev->expr, q);
   }
}

