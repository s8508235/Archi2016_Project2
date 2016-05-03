#ifndef ID_H_INCLUDED
#define ID_H_INCLUDED
#include "buffer.h"
#include "defines.h"
#include "strings.h"
void ID();
void instruction_R();
void instruction_I();
void instruction_J();
void stall_detect();
void prediction();
int rs,rt,imm,rd,op_num,sht,func,C;
#endif // ID_H_INCLUDED
