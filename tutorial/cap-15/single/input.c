#include "input.h"

#include <stdio.h>

char Look; /* Caractere "lookahead */

/* Lê próximo caractere da entrada */
void NextChar() { Look = getchar(); }

/* Inicializa o módulo de entrada */
void InitInput() { NextChar(); }
