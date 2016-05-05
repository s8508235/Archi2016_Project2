#include "wb.h"
void WB()
{/*
    if(MEM2WB.jal_out == 1)
    {
        reg[31] = MEM2WB.ALUout;
    }*//*
printf("------------------------cycle:%d-------------------- currpc:%d\n",cycle,currpc);
printf("%s :%d %d %08X\n",tmp_wb.command,tmp_wb.RegWrite ,tmp_wb.write_dest,tmp_wb.ALUout);
printf("%s :%d %d %08X\n",MEM2WB.command,MEM2WB.RegWrite ,MEM2WB.write_dest,MEM2WB.ALUout);*/
    if(tmp_wb.RegWrite ==1)
    {
        if(tmp_wb.opcode == halt)
        {
            tmp_wb.stop = 1;
        }
        else if(tmp_wb.write_dest !=0)
        {
            reg[tmp_wb.write_dest] = tmp_wb.ALUout;
        }
    }
    if(MEM2WB.write_dest ==0 && MEM2WB.isNop == 0 && MEM2WB.RegWrite == 1 && strcmp(MEM2WB.command,"HALT")!=0)
    {
        err_processing(Writeto0);
    }/*
    written_in_this_cycle.opcode = tmp_wb.opcode;
    written_in_this_cycle.addr = tmp_wb.addr;
    written_in_this_cycle.write_dest = tmp_wb.write_dest;
    written_in_this_cycle.ALUout = tmp_wb.ALUout;
    written_in_this_cycle.instruction_op = tmp_wb.instruction_op;
    written_in_this_cycle.command = tmp_wb.command;
    written_in_this_cycle.isNop = tmp_wb.isNop;
    written_in_this_cycle.stop = tmp_wb.stop ;
    written_in_this_cycle.num_error = tmp_wb.num_error;
    written_in_this_cycle.mem_error = tmp_wb.mem_error;
    written_in_this_cycle.data_miss = tmp_wb.data_miss;
    written_in_this_cycle.mem_read = tmp_wb.mem_read;
    written_in_this_cycle.mem_write = tmp_wb.mem_write;
    written_in_this_cycle.RegWrite = tmp_wb.RegWrite;
    written_in_this_cycle.mem_addr = tmp_wb.mem_addr;
    written_in_this_cycle.mem_data = tmp_wb.mem_data;
*/
    tmp_wb.opcode = MEM2WB.opcode;
    tmp_wb.addr = MEM2WB.addr;
    tmp_wb.write_dest = MEM2WB.write_dest;
    tmp_wb.ALUout = MEM2WB.ALUout;
    tmp_wb.instruction_op = MEM2WB.instruction_op;
    tmp_wb.command = MEM2WB.command;
    tmp_wb.isNop = MEM2WB.isNop;
    tmp_wb.stop = MEM2WB.stop ;
    tmp_wb.num_error = MEM2WB.num_error;
    tmp_wb.mem_error = MEM2WB.mem_error;
    tmp_wb.data_miss = MEM2WB.data_miss;
    tmp_wb.mem_read = MEM2WB.mem_read;
    tmp_wb.mem_write = MEM2WB.mem_write;
    tmp_wb.RegWrite = MEM2WB.RegWrite;
    tmp_wb.mem_addr = MEM2WB.mem_addr;
    tmp_wb.mem_data = MEM2WB.mem_data;
}
