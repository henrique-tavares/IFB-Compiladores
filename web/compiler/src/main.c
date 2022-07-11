#include <stdio.h>

#include "../headers/codegen.h"
#include "../headers/errors.h"
#include "../headers/input.h"
#include "../headers/output.h"
#include "../headers/parser.h"
#include "../headers/scanner.h"

/* Inicialização do compilador */
void Init() {
  NextChar();
  NextToken();
}

/* Programa principal */
int main() {
  Init();

  MatchString("PROGRAM");
  Semicolon();
  AsmHeader();
  TopDeclarations();
  MatchString("BEGIN");
  AsmProlog();
  Block();
  MatchString("END");
  AsmEpilog();

  return 0;
}