#ifndef _CODEGEN_H
#define _CODEGEN_H

extern int LabelCount;

int NewLabel();
void PostLabel(int lbl);
void AsmHeader();
void AsmProlog();
void AsmEpilog();
void AsmClear();
void AsmNegate();
void AsmLoadConst(char *num);
void AsmLoadVar(char *name);
void AsmStore(char *name);
void AsmPush();
void AsmPopAdd();
void AsmPopSub();
void AsmPopMul();
void AsmPopDiv();
void AsmNot();
void AsmPopAnd();
void AsmPopOr();
void AsmPopXor();
void AsmPopCompare();
void AsmRelOp(char op);
void AsmBranch(int label);
void AsmBranchFalse(int label);
void AsmRead();
void AsmWrite();
void AllocVar(char *name, int value);

#endif