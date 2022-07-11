#ifndef _ERRORS_H
#define _ERRORS_H

void Error(char *fmt, ...);
void Expected(char *fmt, ...);
void Undefined(char *name);
void Duplicate(char *name);

#endif