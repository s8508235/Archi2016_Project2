#include "dm.h"

void DM()
{
    MEM2WB.jal_out = 0;
    MEM2WB.opcode = EX2MEM.opcode;
    MEM2WB.addr = EX2MEM.addr;
    MEM2WB.mem_addr = EX2MEM.mem_addr;
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
    if(MEM2WB.mem_addr <0 ||MEM2WB.mem_addr >=1024)
    {
            err_processing(MemaddrOver);
            if(MEM2WB.data_miss == 1)
	    err_processing(DataMis);
            tmp_wb.stop = 1;
            return ;
    }
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
	if(MEM2WB.data_miss ==1 || MEM2WB.mem_error ==1) return ;
 
    if(MEM2WB.RegWrite ==1 )
    {
        MEM2WB.ALUout = EX2MEM.ALUout;
    }
    if(MEM2WB.mem_read == 1 && (MEM2WB.opcode==0x23 || MEM2WB.opcode==0x21 || MEM2WB.opcode==0x25 || MEM2WB.opcode==0x20 || MEM2WB.opcode==0x24))
    {
        if(MEM2WB.opcode == lw)
        {
            MEM2WB.ALUout = 0;
            int i;
            for(i=0; i<32; i++)
            {
                MEM2WB.ALUout =  MEM2WB.ALUout <<1;
                MEM2WB.ALUout += Dmem[MEM2WB.mem_addr*8+i];
            }
          //  printf("%d+++++++++++++determine lw's value: %08X\n",MEM2WB.write_dest,MEM2WB.ALUout);
        }
        else if(MEM2WB.opcode == lh)
        {
            MEM2WB.ALUout = 0;
            int i;
            for(i=0; i<16; i++)
            {
                MEM2WB.ALUout =  MEM2WB.ALUout <<1;
                MEM2WB.ALUout += Dmem[MEM2WB.mem_addr*8+i];
            }
            if(MEM2WB.ALUout & 0x8000)MEM2WB.ALUout = MEM2WB.ALUout | 0xFFFF0000;
        }
        else if(MEM2WB.opcode == lhu)
        {
            MEM2WB.ALUout = 0;
            int i;
            for(i=0; i<16; i++)
            {
                MEM2WB.ALUout =  MEM2WB.ALUout <<1;
                MEM2WB.ALUout += Dmem[MEM2WB.mem_addr*8+i];
            }
        }
        else if(MEM2WB.opcode == lb)
        {
            MEM2WB.ALUout = 0;
            int i;
            for(i=0; i<8; i++)
            {
                MEM2WB.ALUout =  MEM2WB.ALUout <<1;
                MEM2WB.ALUout += Dmem[MEM2WB.mem_addr*8+i];
            }
            if(MEM2WB.ALUout & 0x80)MEM2WB.ALUout = MEM2WB.ALUout | 0xFFFFFF00;
        }
        else if(MEM2WB.opcode == lbu)
        {
            MEM2WB.ALUout = 0;
            int i;
            for(i=0; i<8; i++)
            {
                MEM2WB.ALUout =  MEM2WB.ALUout <<1;
                MEM2WB.ALUout += Dmem[MEM2WB.mem_addr*8+i];
            }
        }
    }
    else if(MEM2WB.mem_write == 1 && (MEM2WB.opcode==0x2B || MEM2WB.opcode==0x29 || MEM2WB.opcode==0x28))
    {
        if(MEM2WB.opcode ==sw)
        {
            int i;
            int tmp = 0;
            for(i=0;i<32;i++)
            {
                Dmem[MEM2WB.mem_addr*8 + i] = EX2MEM.tmp_dmem[i];
                tmp = tmp<<1;
                tmp +=EX2MEM.tmp_dmem[i];
            }//printf("<<<<<<<<<<<<<<<<<<<<change in :%d %08X>>>>>>>>>>>>>>>>>\n",MEM2WB.mem_addr,tmp);
        }
        else if(MEM2WB.opcode == sh)
        {
            int i;
            for(i=0; i<16; i++)
            {
                Dmem[MEM2WB.mem_addr*8 + i] = EX2MEM.tmp_dmem[i];
            }
        }
        else if(MEM2WB.opcode == sb)
        {
            int i;
            for(i=0; i<8; i++)
            {
                Dmem[MEM2WB.mem_addr * 8 + i] = EX2MEM.tmp_dmem[i];
            }
        }
    }
    else if(MEM2WB.opcode == jal /*&& MEM2WB.RegWrite ==1*/)
    {
//        printf("go jal %08X\n",EX2MEM.ALUout);
        MEM2WB.jal_out = 1;
	
        MEM2WB.ALUout = EX2MEM.ALUout;
    }
}
