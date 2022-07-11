#ifndef _SCANNER_H
#define _SCANNER_H

#define SYMBOLTABLE_SIZE 1000

typedef struct Symbol {
  char *name; /* Nome do símbolo */
  char type;  /* Tipo do símbolo */
} Symbol;

typedef struct SymbolTable {
  Symbol table[SYMBOLTABLE_SIZE]; /* Tabela de símbolos */
  int count; /* Número de entradas na tabela de símbolos */
} SymbolTable;

extern SymbolTable symbolTable;

#define KEYWORDLIST_SIZE 9

typedef struct Keyword {
  char *keyword; /* Lista de palavras-chave */
  char code;     /* Códigos de palavras-chave: a ordem deve obedecer a lista de
                    palavras-chave */
} Keyword;

extern Keyword keywordList[KEYWORDLIST_SIZE];

#define MAXTOKEN 16

typedef struct Token {
  char text[MAXTOKEN + 1]; /* Texto do token atual */
  char code;               /* Código do token atual */
} Token;

extern Token token;

void SkipComment();
void SkipWhite();
void Match(char c);
void CheckIdent();
void GetName();
void GetNum();
void GetOp();
void NextToken();
int LookupKeyword(char *s);
int LookupSymbol(char *s);
void Scan();
void MatchString(char *s);
int IsAddOp(char c);
int IsMulOp(char c);
int IsOrOp(char c);
int IsRelOp(char c);
int InSymbolTable(char *name);
int Locate(char *name);
void CheckTable(char *name);
void CheckDuplicate(char *name);

#endif