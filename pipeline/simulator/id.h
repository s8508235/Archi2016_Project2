#ifndef ID_H_INCLUDED
#define ID_H_INCLUDED
#include "buffer.h"
#include "defines.h"

void ID();
void instruction_R();
void instruction_I();
void instruction_J();
int rs,rt,imm,rd,op_num,sht,func;
#endif // ID_H_INCLUDED
