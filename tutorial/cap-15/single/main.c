#include <stdio.h>

#include "input.h"
#include "scanner.h"

/* Programa principal */
int main() {
  InitInput();
  printf("Name: %c\n", GetName());
  Match('=');
  printf("Num: %c\n", GetNum());
  Match('+');
  printf("Name: %c\n", GetName());

  return 0;
}