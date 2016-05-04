#include "if.h"
void IF()
{
        int i,k;
        IF2ID.pc = currpc;
        k = IF2ID.pc / 4;
        if(k < 0) {
            IF2ID.pc += 4;
            return ;
        }
        for(i=0;i<32;i++)
        {
            IF2ID.instrcution[i] = Imem[i+32*k];
        }
        if(ID2EX.isStall ==0 && ID2EX.isFlush ==0)
            IF2ID.pc +=4;
       // else printf("stop!!!!!\n");
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
        if(ID2EX.isFlush == 1)
        {
            for(i=0;i<32;i++)
                IF2ID.instrcution[i] = 0;
        }
        IF2ID.instruction_op = in_fetch;
}
