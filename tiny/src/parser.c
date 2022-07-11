#include "../headers/parser.h"

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "../headers/codegen.h"
#include "../headers/errors.h"
#include "../headers/input.h"
#include "../headers/scanner.h"

/* Adiciona uma nova entrada à tabela de símbolos */
void AddEntry(char *name, char type) {
  char *newSymbol;

  CheckDuplicate(name);

  if (symbolTable.count >= SYMBOLTABLE_SIZE) {
    Error("Symbol table full!");
  }

  newSymbol = (char *)malloc(sizeof(char) * (strlen(name) + 1));
  if (newSymbol == NULL) {
    Error("Out of memory!");
  }

  strcpy(newSymbol, name);

  symbolTable.table[symbolTable.count].name = newSymbol;
  symbolTable.table[symbolTable.count].type = type;
  symbolTable.count++;
}

/* Analisa uma lista de declaração de variáveis */
void Declaration() {
  NextToken();
  CheckIdent();
  CheckDuplicate(token.text);
  AddEntry(token.text, 'v');
  AllocVar(token.text, 0);
  NextToken();
}

/* Reconhece um ponto-e-vírgula opcional */
void Semicolon() {
  if (token.code == ';') {
    MatchString(";");
  }
}

/* Analisa e traduz declarações globais */
void TopDeclarations() {
  Scan();
  while (token.code == 'v') {
    do {
      Declaration();
    } while (token.code == ',');
    Semicolon();
    Scan();
  }
}

/* Reconhece e traduz uma adição */
void Add() {
  NextToken();
  Term();
  AsmPopAdd();
}

/* Reconhece e traduz uma subtração*/
void Subtract() {
  NextToken();
  Term();
  AsmPopSub();
}

/* Reconhece e traduz uma multiplicação */
void Multiply() {
  NextToken();
  Factor();
  AsmPopMul();
}

/* Reconhece e traduz uma divisão */
void Divide() {
  NextToken();
  Factor();
  AsmPopDiv();
}

/* Reconhece e traduz um operador OR */
void BoolOr() {
  NextToken();
  BoolTerm();
  AsmPopOr();
}

/* Reconhece e traduz um operador XOR */
void BoolXor() {
  NextToken();
  BoolTerm();
  AsmPopXor();
}

/* Analisa e traduz um fator matemático */
void Factor() {
  if (token.code == '(') {
    NextToken();
    BoolExpression();
    MatchString(")");
  } else {
    if (token.code == 'x') {
      CheckTable(token.text);
      AsmLoadVar(token.text);
    } else if (token.code == '#') {
      AsmLoadConst(token.text);
    } else {
      Expected("Math Factor");
    }
    NextToken();
  }
}

/* Analisa e traduz um termo matemático */
void Term() {
  Factor();
  while (IsMulOp(token.code)) {
    AsmPush();
    switch (token.code) {
      case '*': {
        Multiply();
        break;
      }
      case '/': {
        Divide();
        break;
      }
    }
  }
}

/* Analisa e traduz uma expressão matemática */
void Expression() {
  if (IsAddOp(token.code))
    AsmClear();
  else
    Term();
  while (IsAddOp(token.code)) {
    AsmPush();
    switch (token.code) {
      case '+': {
        Add();
        break;
      }
      case '-': {
        Subtract();
        break;
      }
    }
  }
}

/* Analisa e traduz uma relação */
void Relation() {
  char op;

  Expression();
  if (IsRelOp(token.code)) {
    op = token.code;
    NextToken(); /* Só para remover o operador do caminho */
    if (op == '<') {
      if (token.code == '>') { /* Trata operador <> */
        NextToken();
        op = '#';
      } else if (token.code == '=') { /* Trata operador <= */
        NextToken();
        op = 'L';
      }
    } else if (op == '>' && token.code == '=') { /* Trata operador >= */
      NextToken();
      op = 'G';
    }
    AsmPush();
    Expression();
    AsmPopCompare();
    AsmRelOp(op);
  }
}

/* Analisa e traduz um fator booleano com NOT inicial */
void NotFactor() {
  if (token.code == '!') {
    NextToken();
    Relation();
    AsmNot();
  } else {
    Relation();
  }
}

/* Analisa e traduz um termo booleano */
void BoolTerm() {
  NotFactor();
  while (token.code == '&') {
    NextToken();
    AsmPush();
    NotFactor();
    AsmPopAnd();
  }
}

/* Analisa e traduz uma expressão booleana */
void BoolExpression() {
  BoolTerm();
  while (IsOrOp(token.code)) {
    AsmPush();
    switch (token.code) {
      case '|': {
        BoolOr();
        break;
      }
      case '~': {
        BoolXor();
        break;
      }
    }
  }
}

/* Analisa e traduz um comando de atribuição */
void Assignment() {
  char name[MAXTOKEN + 1];

  strcpy(name, token.text);
  CheckTable(name);
  NextToken();
  MatchString("=");
  BoolExpression();
  AsmStore(name);
}

/* Analisa e traduz um comando IF */
void DoIf() {
  int l1, l2;

  NextToken();
  BoolExpression();
  l1 = NewLabel();
  l2 = l1;
  AsmBranchFalse(l1);
  Block();
  if (token.code == 'l') {
    NextToken();
    l2 = NewLabel();
    AsmBranch(l2);
    PostLabel(l1);
    Block();
  }
  PostLabel(l2);
  MatchString("ENDIF");
}

/* Analisa e traduz um comando WHILE */
void DoWhile() {
  int l1, l2;

  NextToken();
  l1 = NewLabel();
  l2 = NewLabel();
  PostLabel(l1);
  BoolExpression();
  AsmBranchFalse(l2);
  Block();
  MatchString("ENDWHILE");
  AsmBranch(l1);
  PostLabel(l2);
}

/* Processa um comando READ */
void DoRead() {
  NextToken();
  MatchString("(");
  for (;;) {
    CheckIdent();
    CheckTable(token.text);
    AsmRead();
    AsmStore(token.text);
    NextToken();
    if (token.code != ',') break;
    NextToken();
  }
  MatchString(")");
}

/* Processa um comando WRITE */
void DoWrite() {
  NextToken();
  MatchString("(");
  for (;;) {
    Expression();
    AsmWrite();
    if (token.code != ',') break;
    NextToken();
  }
  MatchString(")");
}

/* Analisa e traduz um bloco de comandos estilo "C/Ada" */
void Block() {
  int follow = 0;

  do {
    Scan();
    switch (token.code) {
      case 'i':
        DoIf();
        break;
      case 'w':
        DoWhile();
        break;
      case 'R':
        DoRead();
        break;
      case 'W':
        DoWrite();
        break;
      case 'x':
        Assignment();
        break;
      case 'e':
      case 'l':
        follow = 1;
        break;
    }
    if (!follow) Semicolon();
  } while (!follow);
}
