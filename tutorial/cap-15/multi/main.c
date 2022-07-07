#include <stdio.h>

#include "input.h"
#include "scanner.h"

/* Programa principal */
int main() {
  char name[MAXNAME + 1], num[MAXNUM + 1];

  InitInput();

  GetName(name);
  printf("Name: %s\n", name);

  Match('=');

  GetNum(num);
  printf("Num: %s\n", num);

  Match('+');

  GetName(name);
  printf("Num: %s\n", name);

  return 0;
}