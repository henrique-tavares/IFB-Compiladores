#include "../headers/scanner.h"

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "../headers/errors.h"
#include "../headers/input.h"
#include "../headers/output.h"

SymbolTable symbolTable = {.count = 0};

Keyword keywordList[KEYWORDLIST_SIZE] = {
    {.keyword = "IF", .code = 'i'},       {.keyword = "ELSE", .code = 'l'},
    {.keyword = "ENDIF", .code = 'e'},    {.keyword = "WHILE", .code = 'w'},
    {.keyword = "ENDWHILE", .code = 'e'}, {.keyword = "READ", .code = 'R'},
    {.keyword = "WRITE", .code = 'W'},    {.keyword = "VAR", .code = 'v'},
    {.keyword = "END", .code = 'e'},
};

Token token;

/* Pula um campo de comentário */
void SkipComment() {
  while (Look != '}') {
    NextChar();
    if (Look == '{') /* Trata comentários aninhados */
    {
      SkipComment();
    }
  }
  NextChar();
}

/* Pula caracteres em branco */
void SkipWhite() {
  while (isspace(Look) || Look == '{') {
    if (Look == '{') {
      SkipComment();
    } else
      NextChar();
  }
}

/* Verifica se caractere combina com o esperado */
void Match(char c) {
  if (Look != c) {
    Expected("'%c'", c);
  }

  NextChar();
  SkipWhite();
}

/* Reporta um erro se Token NÃO for um identificador */
void CheckIdent() {
  if (token.code != 'x') {
    Expected("Identifier");
  }
}

/* Recebe o nome de um identificador ou palavra-chave */
void GetName() {
  int i;

  SkipWhite();
  if (!isalpha(Look)) {
    Expected("Identifier or Keyword");
  }
  for (i = 0; isalnum(Look) && i < MAXTOKEN; i++) {
    token.text[i] = toupper(Look);
    NextChar();
  }
  token.text[i] = '\0';
  token.code = 'x';
}

/* Recebe um número inteiro */
void GetNum() {
  int i;

  SkipWhite();
  if (!isdigit(Look)) {
    Expected("Integer");
  }
  for (i = 0; isdigit(Look) && i < MAXTOKEN; i++) {
    token.text[i] = Look;
    NextChar();
  }
  token.text[i] = '\0';
  token.code = '#';
}

/* Analisa e traduz um operador */
void GetOp() {
  SkipWhite();
  token.code = Look;
  token.text[0] = Look;
  token.text[1] = '\0';
  NextChar();
}

/* Pega o próximo Token de entrada */
void NextToken() {
  SkipWhite();
  if (isalpha(Look)) {
    GetName();
  } else if (isdigit(Look)) {
    GetNum();
  } else {
    GetOp();
  }
}

/* Se a string de entrada estiver na tabela de Keywords, devolve a posição ou -1
 * se não estiver */
int LookupKeyword(char *s) {
  for (int i = 0; i < KEYWORDLIST_SIZE; i++) {
    if (strcmp(keywordList[i].keyword, s) == 0) {
      return i;
    }
  }

  return -1;
}

/* Se a string de entrada estiver na tabela de Symbols, devolve a posição ou -1
 * se não estiver */
int LookupSymbol(char *s) {
  int i;

  for (i = 0; i < symbolTable.count; i++) {
    if (strcmp(symbolTable.table[i].name, s) == 0) {
      return i;
    }
  }

  return -1;
}

/* Analisador léxico */
void Scan() {
  int kw;

  if (token.code == 'x') {
    kw = LookupKeyword(token.text);
    if (kw >= 0) {
      token.code = keywordList[kw].code;
    }
  }
}

/* Verifica se a string combina com o esperado */
void MatchString(char *s) {
  if (strcmp(token.text, s) != 0) {
    Expected(s);
  }
  NextToken();
}

/* Testa operadores de adição */
int IsAddOp(char c) { return (c == '+' || c == '-'); }

/* Testa operadores de multiplicação */
int IsMulOp(char c) { return (c == '*' || c == '/'); }

/* Testa operadores OU */
int IsOrOp(char c) { return (c == '|' || c == '~'); }

/* Testa operadores relacionais */
int IsRelOp(char c) { return (c == '=' || c == '#' || c == '<' || c == '>'); }

/* Verifica se símbolo está na tabela */
int InSymbolTable(char *name) { return (LookupSymbol(name) >= 0); }

/* Retorna o endereço do identificador na tabela de símbolos */
int Locate(char *name) { return LookupSymbol(name); }

/* Reporta um erro se identificador NÃO constar na tabela de símbolos */
void CheckTable(char *name) {
  if (!InSymbolTable(name)) {
    Undefined(name);
  }
}

/* Reporta um erro se identificador JÁ constar na tabela de símbolos */
void CheckDuplicate(char *name) {
  if (InSymbolTable(name)) {
    Duplicate(name);
  }
}
