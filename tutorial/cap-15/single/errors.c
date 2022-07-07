#include "errors.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

/* Exibe uma mensagem de erro formatada */
void Error(char *fmt, ...) {
  va_list args;

  fputs("Error: ", stderr);

  va_start(args, fmt);
  vfprintf(stderr, fmt, args);
  va_end(args);

  fputc('\n', stderr);

  exit(1);
}

/* Alerta sobre alguma entrada esperada */
void Expected(char *fmt, ...) {
  va_list args;

  fputs("Error: ", stderr);

  va_start(args, fmt);
  vfprintf(stderr, fmt, args);
  va_end(args);

  fputs(" expected!\n", stderr);

  exit(1);
}