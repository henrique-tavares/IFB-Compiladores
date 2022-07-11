#include "../headers/codegen.h"

#include <stdio.h>

#include "../headers/errors.h"
#include "../headers/output.h"

int LabelCount; /* Contador usado pelo gerador de rótulos */

/* Gera um novo rótulo único */
int NewLabel() { return LabelCount++; }

/* Emite um rótulo */
void PostLabel(int lbl) { printf("L%d:\n", lbl); }

/* Cabeçalho inicial para o montador */
void AsmHeader() {
  printf("org 100h\n");
  printf("section .data\n");
}

/* Emite código para o prólogo de um programa */
void AsmProlog() {
  printf("section .text\n");
  printf("_start:\n");
}

/* Emite código para o epílogo de um programa */
void AsmEpilog() {
  EmitLn("MOV AX, 4C00h");
  EmitLn("INT 21h");
  printf("\n%%include \"tinyrtl_dos.inc\"\n");
}

/* Zera o registrador primário */
void AsmClear() { EmitLn("XOR AX, AX"); }

/* Negativa o registrador primário */
void AsmNegate() { EmitLn("NEG AX"); }

/* Carrega uma constante numérica no registrador primário */
void AsmLoadConst(char *num) { EmitLn("MOV AX, %s", num); }

/* Carrega uma variável no registrador primário */
void AsmLoadVar(char *name) { EmitLn("MOV AX, [%s]", name); }

/* Armazena registrador primário em variável */
void AsmStore(char *name) { EmitLn("MOV [%s], AX", name); }

/* Coloca registrador primário na pilha */
void AsmPush() { EmitLn("PUSH AX"); }

/* Adiciona topo da pilha ao registrador primário */
void AsmPopAdd() {
  EmitLn("POP BX");
  EmitLn("ADD AX, BX");
}

/* Subtrai o registrador primário do topo da pilha */
void AsmPopSub() {
  EmitLn("POP BX");
  EmitLn("SUB AX, BX");
  EmitLn("NEG AX");
}

/* Multiplica o topo da pilha pelo registrador primário */
void AsmPopMul() {
  EmitLn("POP BX");
  EmitLn("IMUL BX");
}

/* Divide o topo da pilha pelo registrador primário */
void AsmPopDiv() {
  EmitLn("POP BX");
  EmitLn("XCHG AX, BX");
  EmitLn("CWD");
  EmitLn("IDIV BX");
}

/* Inverte registrador primário */
void AsmNot() { EmitLn("NOT AX"); }

/* Aplica "E" binário ao topo da pilha com registrador primário */
void AsmPopAnd() {
  EmitLn("POP BX");
  EmitLn("AND AX, BX");
}

/* Aplica "OU" binário ao topo da pilha com registrador primário */
void AsmPopOr() {
  EmitLn("POP BX");
  EmitLn("OR AX, BX");
}

/* Aplica "OU-exclusivo" binário ao topo da pilha com registrador primário */
void AsmPopXor() {
  EmitLn("POP BX");
  EmitLn("XOR AX, BX");
}

/* Compara topo da pilha com registrador primário */
void AsmPopCompare() {
  EmitLn("POP BX");
  EmitLn("CMP BX, AX");
}

/* Altera registrador primário (e flags, indiretamente) conforme a comparação */
void AsmRelOp(char op) {
  char *jump;
  int l1, l2;

  l1 = NewLabel();
  l2 = NewLabel();

  switch (op) {
    case '=': {
      jump = "JE";
      break;
    }
    case '#': {
      jump = "JNE";
      break;
    }
    case '<': {
      jump = "JL";
      break;
    }
    case '>': {
      jump = "JG";
      break;
    }
    case 'L': {
      jump = "JLE";
      break;
    }
    case 'G': {
      jump = "JGE";
      break;
    }
    default: {
      Error("Undefined Relational Operator!");
    }
  }

  EmitLn("%s L%d", jump, l1);
  EmitLn("XOR AX, AX");
  EmitLn("JMP L%d", l2);
  PostLabel(l1);
  EmitLn("MOV AX, -1");
  PostLabel(l2);
}

/* Desvio incondicional */
void AsmBranch(int label) { EmitLn("JMP L%d", label); }

/* Desvio se falso (0) */
void AsmBranchFalse(int label) { EmitLn("JZ L%d", label); }

/* Lê um valor a partir da entrada e armazena-o no registrador primário */
void AsmRead() { EmitLn("CALL READ"); }

/* Mostra valor do registrador primário */
void AsmWrite() { EmitLn("CALL WRITE"); }

/* Alocação de memória para uma variável global */
void AllocVar(char *name, int value) { printf("%s\tdw %d\n", name, value); }