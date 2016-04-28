#include "wb.h"
void WB()
{
    if(MEM2WB.opcode == halt)
    {

    }
    else
    {
        reg[MEM2WB.write_dest] = MEM2WB.ALUout;
    }
}
