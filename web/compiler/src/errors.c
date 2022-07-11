#include "../headers/errors.h"

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

/* Avisa a respeito de um identificador desconhecido */
void Undefined(char *name) { Error("Undefined identifier %s\n", name); }

/* Avisa a respeito de um identificador duplicado */
void Duplicate(char *name) { Error("Duplicated identifier %s\n", name); }