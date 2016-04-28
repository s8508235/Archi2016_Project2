#ifndef BUFFER_H_INCLUDED
#define BUFFER_H_INCLUDED
#include "buffer.h"
typedef struct IF_ID
{
    unsigned pc;
    unsigned instrcution[32];

}IF_ID;
extern IF_ID IF2ID;
IF_ID IF2ID;
typedef struct ID_EX
{
    unsigned int opcode;
    unsigned int rs;
    unsigned int rt;
    unsigned int rd;
    unsigned int immediate;
    unsigned int addr;
    unsigned int tmp_rs;
    unsigned int tmp_rt;
    unsigned int sht;
    unsigned int func;
    int ctrl;
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
    unsigned int tmp_rt;
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
}MEM_WB;
extern MEM_WB MEM2WB;
MEM_WB MEM2WB;
void init_buffer();
#endif // BUFFER_H_INCLUDED
