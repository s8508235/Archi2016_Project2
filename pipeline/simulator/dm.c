#include "dm.h"

void DM()
{
    MEM2WB.jal_out = 0;
    MEM2WB.opcode = EX2MEM.opcode;
    MEM2WB.addr = EX2MEM.addr;
    MEM2WB.func = EX2MEM.func;
    MEM2WB.instruction_op = EX2MEM.instruction_op;
    MEM2WB.command = EX2MEM.command;
    MEM2WB.isNop = EX2MEM.isNop;
    MEM2WB.write_dest = EX2MEM.write_dest;
    MEM2WB.stop = EX2MEM.stop;
    MEM2WB.num_error = EX2MEM.num_error;
    MEM2WB.mem_error = EX2MEM.mem_error;
    MEM2WB.data_miss = EX2MEM.data_miss;
    MEM2WB.mem_read = EX2MEM.mem_read;
    MEM2WB.mem_write = EX2MEM.mem_write;
    MEM2WB.RegWrite = EX2MEM.RegWrite;
        if(MEM2WB.mem_error ==1)
        {
            err_processing(MemaddrOver);
            tmp_wb.stop = 1;
        }
        if(MEM2WB.data_miss ==1)
        {
            err_processing(DataMis);
            tmp_wb.stop = 1;
        }
        if(EX2MEM.RegWrite ==1)
        {
            MEM2WB.ALUout = EX2MEM.ALUout;
        }
    if(EX2MEM.mem_read == 1)
    {
        if(EX2MEM.opcode == lw)
        {
            MEM2WB.opcode = EX2MEM.opcode;
            MEM2WB.ALUout = 0;
            int i;
            for(i=0; i<32; i++)
            {
                MEM2WB.ALUout =  MEM2WB.ALUout <<1;
                MEM2WB.ALUout += Dmem[EX2MEM.addr*8+i];
            }
        }
        else if(EX2MEM.opcode == lh)
        {
            MEM2WB.opcode = EX2MEM.opcode;
            MEM2WB.ALUout = 0;
            int i;
            for(i=0; i<16; i++)
            {
                MEM2WB.ALUout =  MEM2WB.ALUout <<1;
                MEM2WB.ALUout += Dmem[EX2MEM.addr*8+i];
            }
            if(MEM2WB.ALUout & 0x8000)MEM2WB.ALUout = MEM2WB.ALUout | 0xFFFF0000;
        }
        else if(EX2MEM.opcode == lhu)
        {
            MEM2WB.opcode = EX2MEM.opcode;
            MEM2WB.ALUout = 0;
            int i;
            for(i=0; i<16; i++)
            {
                MEM2WB.ALUout =  MEM2WB.ALUout <<1;
                MEM2WB.ALUout += Dmem[EX2MEM.addr*8+i];
            }
        }
        else if(EX2MEM.opcode == lb)
        {
            MEM2WB.opcode = EX2MEM.opcode;
            MEM2WB.ALUout = 0;
            int i;
            for(i=0; i<8; i++)
            {
                MEM2WB.ALUout =  MEM2WB.ALUout <<1;
                MEM2WB.ALUout += Dmem[EX2MEM.addr*8+i];
            }
            if(MEM2WB.ALUout & 0x80)MEM2WB.ALUout = MEM2WB.ALUout | 0xFFFFFF00;
        }
        else if(EX2MEM.opcode == lbu)
        {
            MEM2WB.opcode = EX2MEM.opcode;
            MEM2WB.ALUout = 0;
            int i;
            for(i=0; i<8; i++)
            {
                MEM2WB.ALUout =  MEM2WB.ALUout <<1;
                MEM2WB.ALUout += Dmem[EX2MEM.addr*8+i];
            }
        }
    }
    else if(EX2MEM.mem_write == 1)
    {
        if(EX2MEM.opcode ==sw)
        {
            int i;
            for(i=0; i<32; i++)
            {
                Dmem[EX2MEM.addr*8 + i] = EX2MEM.tmp_dmem[i];
            }
        }
        else if(EX2MEM.opcode == sh)
        {
            int i;
            MEM2WB.ALUout = 0;
            for(i=0; i<16; i++)
            {
                Dmem[EX2MEM.addr*8 + i] = EX2MEM.tmp_dmem[i];
            }
        }
        else if(EX2MEM.opcode == sb)
        {
            int i;
            for(i=0; i<8; i++)
            {
                Dmem[EX2MEM.addr * 8 + i] = EX2MEM.tmp_dmem[i];
            }
        }
    }
    else if(EX2MEM.opcode == jal)
    {
        MEM2WB.jal_out = 1;
        MEM2WB.ALUout = EX2MEM.addr;
    }
}
