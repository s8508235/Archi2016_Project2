#ifndef BUFFER_H_INCLUDED
#define BUFFER_H_INCLUDED
#include "buffer.h"
typedef struct IF_ID
{
    unsigned pc;
    unsigned instrcution[32];
    unsigned int opcode;
    unsigned int instruction_op;
}IF_ID;
extern IF_ID IF2ID;
IF_ID IF2ID;
typedef struct ID_EX
{
    unsigned int opcode;
    unsigned int rs;
    unsigned int rt;
    unsigned int rd;
    int immediate;
    unsigned int addr;
    unsigned int tmp_rs;
    unsigned int tmp_rt;
    unsigned int sht;
    unsigned int func;
    unsigned int instruction_op;
    char* command;
    int isNop;
    int isStall;
    int stop;
    int RegWrite;
    int mem_write;
    int mem_read;
    int go_forward;
    int forward_mess;
    int isFlush;
    int jal_tmp;
    int C;
}ID_EX;
extern ID_EX ID2EX;
ID_EX ID2EX;
typedef struct EX_MEM
{
    int ALUout;
    unsigned int tmp_dmem[32];
    int addr;
    int write_dest;
    int opcode;
    unsigned int tmp_rs;
    unsigned int tmp_rt;
    unsigned int func;
    unsigned int instruction_op;
    char* command;
    int isNop;
    int can_forward;
    int stop;
    int num_error;
    int mem_error;
    int data_miss;
    int immediate;
    int RegWrite;
    int mem_write;
    int mem_read;
    int go_forward;
    int forward_mess;
    int isFlush;
    int forward_pos;
}EX_MEM;
extern EX_MEM EX2MEM;
EX_MEM EX2MEM;
typedef struct MEM_WB
{
    int ALUout;
    unsigned int opcode;
    int jal_out;
    int write_dest;
    int addr;
    unsigned int func;
    unsigned int instruction_op;
    char* command;
    int isNop;
    int stop;
    int num_error;
    int mem_error;
    int data_miss;
    int RegWrite;
    int mem_write;
    int mem_read;
    int can_forward;
    int forward_mess;
}MEM_WB;
extern MEM_WB MEM2WB;
MEM_WB MEM2WB;
MEM_WB tmp_wb;
MEM_WB written_in_this_cycle;
void init_buffer();
#endif // BUFFER_H_INCLUDED
