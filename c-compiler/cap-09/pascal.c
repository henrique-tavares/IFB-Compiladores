#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

char Look; /* O caractere lido "antecipadamente" (lookahead) */

/* Protótipos */
void Init();
void NextChar();
void Error(char *fmt, ...);
void Abort(char *fmt, ...);
void Expected(char *fmt, ...);
void Match(char c);
char GetName();
char GetNum();
void EmitLn(char *fmt, ...);
void AsmProlog();
void AsmEpilog();
void Program();
void Block();
void Declarations();
void Labels();
void Constants();
void Types();
void Variables();
void Procedures();
void Functions();
void Statements();

/* Programa principal */
int main() {
  Init();
  Program();

  return 0;
}

/* Inicialização do compilador */
void Init() { NextChar(); }

/* Lê próximo caractere da entrada */
void NextChar() { Look = getchar(); }

/* Exibe uma mensagem de erro formatada */
void Error(char *fmt, ...) {
  va_list args;

  fputs("Error: ", stderr);

  va_start(args, fmt);
  vfprintf(stderr, fmt, args);
  va_end(args);

  fputc('\n', stderr);
}

/* Exibe uma mensagem de erro formatada e sai */
void Abort(char *fmt, ...) {
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

/* Verifica se entrada combina com o esperado */
void Match(char c) {
  if (Look != c) Expected("'%c'", c);
  NextChar();
}

/* Recebe o nome de um identificador */
char GetName() {
  char name;

  if (!isalpha(Look)) Expected("Name");
  name = toupper(Look);
  NextChar();

  return name;
}

/* Recebe um número inteiro */
char GetNum() {
  char num;

  if (!isdigit(Look)) Expected("Integer");
  num = Look;
  NextChar();

  return num;
}

/* Emite uma instrução seguida por uma nova linha */
void EmitLn(char *fmt, ...) {
  va_list args;

  putchar('\t');

  va_start(args, fmt);
  vprintf(fmt, args);
  va_end(args);

  putchar('\n');
}

/* Emite código para o prólogo de um programa */
void AsmProlog() {
  printf("org 100h\n");
  printf("section .data\n");
  printf("section .text\n");
  printf("_start:\n");
}

/* Emite código para o epílogo de um programa */
void AsmEpilog() {
  EmitLn("MOV AX, 4C00h");
  EmitLn("INT 21h");
}

void Program() {
  char name;

  Match('p');
  name = GetName();
  AsmProlog();
  Block();
  Match('.');
  AsmEpilog();
}

/* Analisa e traduz um bloco pascal */
void Block() {
  Declarations();
  Statements();
}

/* Analisa e traduz declarações pascal */
void Declarations() {
  int valid;

  do {
    valid = 1;
    switch (Look) {
      case 'l':
        Labels();
        break;
      case 'c':
        Constants();
        break;
      case 't':
        Types();
        break;
      case 'v':
        Variables();
        break;
      case 'p':
        Procedures();
        break;
      case 'f':
        Functions();
        break;
      default:
        valid = 0;
        break;
    }
  } while (valid);
}

/* Funções de declaração pascal */

void Labels() { Match('l'); }

void Constants() { Match('c'); }

void Types() { Match('t'); }

void Variables() { Match('v'); }

void Procedures() { Match('p'); }

void Functions() { Match('f'); }

/* Analisa e traduz um bloco de comandos pascal */
void Statements() {
  Match('b');
  while (Look != 'e') NextChar();
  Match('e');
}