#include "if.h"
void IF()
{
        int i,k;
        k = IF2ID.pc / 4;
        for(i=0;i<32;i++)
        {
            IF2ID.instrcution[i] = Imem[i+32*k];
        }
        if(ID2EX.isStall ==0)
            IF2ID.pc +=4;
        int op_num = 0;
        for(i=0;i<6;i++)
        {
            op_num  = op_num<< 1 ;
            op_num += IF2ID.instrcution[i];
        }
        IF2ID.opcode = op_num;
        int in_fetch = 0;
        for(i=0;i<32;i++)
        {
            in_fetch  = in_fetch<< 1 ;
            in_fetch += IF2ID.instrcution[i];
        }
        IF2ID.instruction_op = in_fetch;
}
