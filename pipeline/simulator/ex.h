#ifndef EX_H_INCLUDED
#define EX_H_INCLUDED
#include "buffer.h"
#include "defines.h"
#include "stdio.h"
void EX();
void implementR();
void implementJ();
void implementI();
void branch_forward_detect();
void err_processing(int errtype);
int masks[31];
#endif // EX_H_INCLUDED
