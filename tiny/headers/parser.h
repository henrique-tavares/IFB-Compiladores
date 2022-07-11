#ifndef _PARSER_H
#define _PARSER_H

void AddEntry(char *name, char type);
void Declaration();
void Semicolon();
void TopDeclarations();
void Add();
void Subtract();
void Multiply();
void Divide();
void BoolOr();
void BoolXor();
void Factor();
void Term();
void Expression();
void Relation();
void NotFactor();
void BoolTerm();
void BoolExpression();
void Assignment();
void DoIf();
void DoWhile();
void DoRead();
void DoWrite();
void Statement();
void Block();

#endif