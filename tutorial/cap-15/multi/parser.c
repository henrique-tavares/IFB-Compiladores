#include "parser.h"

#include <ctype.h>

#include "codegen.h"
#include "errors.h"
#include "input.h"
#include "scanner.h"

/* Analisa e traduz um fator matemático */
void Factor() {
  char name[MAXNAME + 1], num[MAXNUM + 1];

  if (isdigit(Look)) {
    GetNum(num);
    AsmLoadConst(num);
  } else if (isalpha(Look)) {
    GetName(name);
    AsmLoadVar(name);
  } else {
    Error("Unrecognized character: '%c'", Look);
  }
}