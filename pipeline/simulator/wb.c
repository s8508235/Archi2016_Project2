#include "wb.h"
void WB()
{
    if(MEM2WB.opcode == halt)
    {

    }
    else if(MEM2WB.write_dest !=0)
    {
        reg[tmp_wb.write_dest] = tmp_wb.ALUout;
        printf("%s write in wb %d\n",tmp_wb.command,tmp_wb.write_dest);
    }
    else if(MEM2WB.write_dest ==0/* && strcmp(tmp_wb.command,"BEQ")!=0 && strcmp(tmp_wb.command,"BNE")!=0 && strcmp(tmp_wb.command,"BGTZ")!=0*/)
    {
        if(tmp_wb.opcode !=bgtz && tmp_wb.opcode !=bne && tmp_wb.opcode !=beq && tmp_wb.isNop ==0)
        fprintf(err, "In cycle %d: Write $0 Error\n", cycle);
    }
    tmp_wb.opcode = EX2MEM.opcode;
    tmp_wb.addr = EX2MEM.addr + 4;
    tmp_wb.write_dest = MEM2WB.write_dest;
    tmp_wb.ALUout = MEM2WB.ALUout;
    tmp_wb.instruction_op = EX2MEM.instruction_op;
    tmp_wb.command = EX2MEM.command;
    tmp_wb.isNop = EX2MEM.isNop;
    tmp_wb.stop = EX2MEM.stop;
    tmp_wb.isStall = 0;
}
