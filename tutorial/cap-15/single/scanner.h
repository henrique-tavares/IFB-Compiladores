#ifndef _SCANNER_H
#define _SCANNER_H

int IsAddOp(char c);
int IsMulOp(char c);

void Match(char c);
char GetName();
char GetNum();

#endif