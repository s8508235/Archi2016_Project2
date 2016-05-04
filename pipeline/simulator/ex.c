#include "ex.h"
int mask_31 = 0x80000000;
int mask_15 = 0x8000;
void EX()
{
    int x;
    for(x=0; x<32; x++)
    {
        masks[x] = 1 <<x;
    }
    EX2MEM.forward_mess = 0;
    EX2MEM.tmp_rs = ID2EX.tmp_rs;
    EX2MEM.tmp_rt = ID2EX.tmp_rt;
   // printf("%d %d\nchange is not :%d %d\n",ID2EX.rs,ID2EX.rt,EX2MEM.tmp_rs,EX2MEM.tmp_rt);
    if(tmp_wb.write_dest == ID2EX.rs && ID2EX.rs!=0 && tmp_wb.RegWrite ==1)
        EX2MEM.tmp_rs = tmp_wb.ALUout;
  //  else if(written_in_this_cycle.write_dest == ID2EX.rs && ID2EX.rs!=0 && written_in_this_cycle.RegWrite ==1)
   //     EX2MEM.tmp_rs = written_in_this_cycle.ALUout;

    if(tmp_wb.write_dest == ID2EX.rt&& ID2EX.rt!=0 && tmp_wb.RegWrite ==1)
        EX2MEM.tmp_rt = tmp_wb.ALUout;
 //   else if(written_in_this_cycle.write_dest ==ID2EX.rt && ID2EX.rt!=0 && written_in_this_cycle.RegWrite ==1)
 //       EX2MEM.tmp_rt = written_in_this_cycle.ALUout;
 //   printf("change is not :%d %d\n",EX2MEM.tmp_rs,EX2MEM.tmp_rt);
    if(EX2MEM.go_forward ==1)
    {
        printf("~~~~~~~go 1\n");
        EX2MEM.forward_mess = 1;
        EX2MEM.tmp_rs = MEM2WB.ALUout;
        printf("%08x\n",EX2MEM.tmp_rs);
        printf("forward : %08X \n",MEM2WB.ALUout);
        ID2EX.isStall = 0;
    }
    else if(EX2MEM.go_forward ==2)
    {
        EX2MEM.forward_mess = 2;
        printf("~~~~~~~~~~go 2\n");
        EX2MEM.tmp_rt = MEM2WB.ALUout;
        printf("%08x\n",EX2MEM.tmp_rt);
        printf("forward : %08X \n",MEM2WB.ALUout);
        ID2EX.isStall = 0;
    }
    else if(EX2MEM.go_forward == 3)
    {
        EX2MEM.forward_mess = 3;
        printf("~~~~~~~~~~go 3\n");
        EX2MEM.tmp_rs = MEM2WB.ALUout;
        EX2MEM.tmp_rt = MEM2WB.ALUout;
        printf("%08X %08x\n",EX2MEM.tmp_rs,EX2MEM.tmp_rt);
        printf("forward : %08X \n",MEM2WB.ALUout);
        ID2EX.isStall = 0;
    }

    if(ID2EX.isStall ==1)
    {
        printf("~~~~~~~~insert\n");
        EX2MEM.isNop = 1;
        EX2MEM.instruction_op = 0;
        EX2MEM.opcode = 0;
        EX2MEM.command = "!!!!!!!!!";
        EX2MEM.write_dest = 0;
        EX2MEM.tmp_rs = 0;
        EX2MEM.tmp_rt = 0;
        return ;
    }
    EX2MEM.ALUout = 0;
    EX2MEM.addr  = ID2EX.addr ;
    EX2MEM.opcode=ID2EX.opcode;
    EX2MEM.func = ID2EX.func;
    EX2MEM.instruction_op = ID2EX.instruction_op;
    EX2MEM.command = ID2EX.command;
    EX2MEM.isNop = ID2EX.isNop;
    EX2MEM.immediate = ID2EX.immediate;
    EX2MEM.go_forward = 0;
    EX2MEM.num_error = 0;
    EX2MEM.mem_error = 0;
    EX2MEM.data_miss = 0;
    EX2MEM.mem_addr = 0;
    EX2MEM.mem_read = ID2EX.mem_read;
    EX2MEM.mem_write = ID2EX.mem_write;
    EX2MEM.RegWrite = ID2EX.RegWrite;
    if(ID2EX.opcode == 0)
    {
        implementR();
    }
    else if(ID2EX.opcode==2 || ID2EX.opcode==3)
    {
        implementJ();
    }
    else
    {
        implementI();
    }
    EX2MEM.stop = ID2EX.stop;
 //   printf("%08X \n",currpc + pc);
  //  printf("**********************ex write:%d\n",EX2MEM.write_dest);
}
void implementJ()
{
    int x= ID2EX.C;
    x = x<<2;
    if(ID2EX.opcode == j)
    {
     //   printf("JJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJ!!!\n");
        currpc = (((ID2EX.addr) & 0xf0000000) | (x )) - pc;
     //   printf("%d %d %d\n",currpc,pc ,currpc+pc);
    }
    else if(ID2EX.opcode == jal)
    {
   //     printf("JAL!!!\n");
        currpc = (((ID2EX.addr+pc) & 0xf0000000) | (x)) - pc;
        EX2MEM.ALUout = ID2EX.jal_tmp;
        EX2MEM.write_dest = 31;
        //pc = ID2EX.addr;
    }
}
void implementR()
{
    unsigned sRs,sRt,sRd;
    if(ID2EX.func == add)
    {
        sRs = (EX2MEM.tmp_rs & mask_31)? 1:0;
        sRt = (EX2MEM.tmp_rt & mask_31)?1:0;
        EX2MEM.ALUout = EX2MEM.tmp_rs + EX2MEM.tmp_rt;
     //   printf("#################%08X %08X %08X\n",EX2MEM.ALUout,EX2MEM.tmp_rs,EX2MEM.tmp_rt);
        EX2MEM.write_dest = ID2EX.rd;
        sRd = (EX2MEM.ALUout & mask_31)?1:0;
        if(sRs ==sRt && sRs !=sRd)
        {
            EX2MEM.num_error = 1;
            err_processing(NumOver);
        }
    }
    else if (ID2EX.func == addu)
    {
        EX2MEM.ALUout = EX2MEM.tmp_rs + EX2MEM.tmp_rt;
        EX2MEM.write_dest = ID2EX.rd;
    }
    else if (ID2EX.func == sub)
    {
        sRs = (EX2MEM.tmp_rs & mask_31)?1:0;
        sRt = (-(EX2MEM.tmp_rt) & mask_31)?1:0;
        EX2MEM.ALUout = EX2MEM.tmp_rs - EX2MEM.tmp_rt;
        EX2MEM.write_dest = ID2EX.rd;
        sRd = (EX2MEM.ALUout & mask_31)?1:0;
   //     printf("SSSSSSSSSSSSSSSSSSSSS:%d %d %d\n",EX2MEM.tmp_rs,EX2MEM.tmp_rt,EX2MEM.ALUout);
        if(sRs ==sRt && sRs !=sRd)
        {
            EX2MEM.num_error = 1;
            err_processing(NumOver);
        }
    }
    else if (ID2EX.func == AND)
    {
        EX2MEM.ALUout = EX2MEM.tmp_rs & EX2MEM.tmp_rt;
        EX2MEM.write_dest = ID2EX.rd;
    }
    else if (ID2EX.func == OR)
    {
        EX2MEM.ALUout = EX2MEM.tmp_rs | EX2MEM.tmp_rt;
        EX2MEM.write_dest = ID2EX.rd;
    }
    else if (ID2EX.func == XOR)
    {
        EX2MEM.ALUout = EX2MEM.tmp_rs ^ EX2MEM.tmp_rt;
        EX2MEM.write_dest = ID2EX.rd;
    }
    else if (ID2EX.func == NOR)
    {
        EX2MEM.ALUout =~ (EX2MEM.tmp_rs | EX2MEM.tmp_rt);
        EX2MEM.write_dest = ID2EX.rd;
    }
    else if (ID2EX.func == NAND)
    {
        EX2MEM.ALUout =~ (EX2MEM.tmp_rs & EX2MEM.tmp_rt);
        EX2MEM.write_dest = ID2EX.rd;
    }
    else if (ID2EX.func == slt)
    {
        sRs = (EX2MEM.tmp_rs & mask_31)?1:0;
        sRt = (EX2MEM.tmp_rt& mask_31)?1:0;
        if(sRs < sRt)
            EX2MEM.ALUout = 0;
        else if(sRs == sRt)
        {
            EX2MEM.ALUout = EX2MEM.tmp_rs < EX2MEM.tmp_rt;
        }
        else
            EX2MEM.ALUout = 1;
        EX2MEM.write_dest = ID2EX.rd;
    }
    else if (ID2EX.func == sll)
    {
        EX2MEM.ALUout = EX2MEM.tmp_rt << ID2EX.sht;
        EX2MEM.write_dest = ID2EX.rd;
    }
    else if (ID2EX.func == srl)
    {
        EX2MEM.ALUout = EX2MEM.tmp_rt >> ID2EX.sht;
        EX2MEM.write_dest = ID2EX.rd;
    //    printf("SRRRRRRRRRRRRRRRRRRRRRRRRRRLLLLLLLLLLLLLLL:%d\n",EX2MEM.ALUout);
    }
    else if (ID2EX.func == sra)
    {
        int tmp = EX2MEM.tmp_rt;
        tmp = tmp >>ID2EX.sht;
        EX2MEM.ALUout =tmp;
        EX2MEM.write_dest = ID2EX.rd;
    }
    else if (ID2EX.func == jr)
    {
        currpc = EX2MEM.tmp_rs - pc;
        //  *pc = reg[rs]-unmod_pc-4 ;
    }
}
void implementI()
{
    int sRs,sRt;
    unsigned sImm;
    if(ID2EX.opcode == addi)
    {
        sRs = (EX2MEM.tmp_rs & mask_31)?1:0;
        EX2MEM.ALUout = EX2MEM.tmp_rs + ID2EX.immediate;
        EX2MEM.write_dest = ID2EX.rt;
        sRt = (EX2MEM.ALUout & mask_31)?1:0;
   //     printf("@@@@@@@@@@@@@@@@%08X %08X %08X\n",EX2MEM.ALUout,EX2MEM.tmp_rs,ID2EX.immediate);
        if(sRs == (ID2EX.immediate & mask_31) && sRs !=sRt)
        {
            EX2MEM.num_error = 1;
            err_processing(NumOver);
      //      printf("WRYYYYYYYYYYYYYY\n");
        }
    }
    else if(ID2EX.opcode == addiu)
    {
        EX2MEM.ALUout = EX2MEM.tmp_rs + ID2EX.immediate;
        EX2MEM.write_dest = ID2EX.rt;
    }
    else if(ID2EX.opcode == lw)
    {
        unsigned addr;//sign handle
        unsigned sAddr;
        sRs = (EX2MEM.tmp_rs & mask_31)?1:0;
        sImm = (ID2EX.immediate & mask_31)?1:0;
        addr = ID2EX.immediate + EX2MEM.tmp_rs;
        sAddr =(addr & mask_31)?1:0;
     //   printf("my op: %08X\n",EX2MEM.instruction_op);
//          printf("LWWWWWWWWWWWWWWWWW:%08X %08X %08X\n",ID2EX.immediate,EX2MEM.tmp_rs,addr);
      //  printf("%d\n",ID2EX.rt);
        if(sRs == sImm && sAddr!=sRs)
        {
            EX2MEM.num_error = 1;
            err_processing(NumOver);
        }
        if(addr >=1021 || addr%4)
        {
            if(addr >=1021)
            {
                EX2MEM.mem_error = 1;
            }
            if(addr%4)
            {
                EX2MEM.data_miss = 1;
            }
            end_program = 1;
            return ;
        }
        EX2MEM.ALUout = 0;
        EX2MEM.write_dest = ID2EX.rt;
        EX2MEM.mem_addr = addr;
    }
    else if(ID2EX.opcode == lh)
    {
        unsigned addr;//sign handle
        unsigned sAddr;
        sRs =(EX2MEM.tmp_rs & mask_31)?1:0;
        sImm =(ID2EX.immediate & mask_31)? 1:0;
        addr = ID2EX.immediate + EX2MEM.tmp_rs;
        sAddr =(addr & mask_31)?1:0;
        if(sRs == sImm && sAddr!=sRs)
        {
            EX2MEM.num_error = 1;
            err_processing(NumOver);
        }
        if(addr >=1023 || addr%2)
        {
            if(addr >=1023)
            {
                EX2MEM.mem_error = 1;
               // err_processing(MemaddrOver);
            }
            if(addr%2)
            {
                EX2MEM.data_miss = 1;
              //  err_processing(DataMis);
            }
            end_program= 1;
            return ;
        }
        EX2MEM.ALUout = 0;
        EX2MEM.write_dest = ID2EX.rt;
        EX2MEM.mem_addr = addr;
    }
    else if(ID2EX.opcode == lhu)
    {
        unsigned addr;//sign handle
        unsigned sAddr;
        sRs =(EX2MEM.tmp_rs & mask_31)?1:0;
        sImm =(ID2EX.immediate & mask_31)? 1:0;
        addr = ID2EX.immediate + EX2MEM.tmp_rs;
        sAddr =(addr & mask_31)?1:0;
        if(sRs == sImm && sAddr!=sRs)
        {
            EX2MEM.num_error = 1;
            err_processing(NumOver);
        }
        if(addr >=1023 || addr%2)
        {
            if(addr >=1023)
            {
                EX2MEM.mem_error = 1;
              //  err_processing(MemaddrOver);
            }
            if(addr%2)
            {
                EX2MEM.data_miss = 1;
             //   err_processing(DataMis);
            }
            end_program = 1;
            return ;
        }
        EX2MEM.ALUout = 0;
        EX2MEM.write_dest = ID2EX.rt;
        EX2MEM.mem_addr = addr;
    }
    else if(ID2EX.opcode == lb)
    {
        unsigned addr;//sign handle
        unsigned sAddr;
        sRs =(EX2MEM.tmp_rs & mask_31)?1:0;
        sImm =(ID2EX.immediate & mask_31)? 1:0;
        addr = ID2EX.immediate + EX2MEM.tmp_rs;
        sAddr =(addr & mask_31)?1:0;
        if(sRs == sImm && sAddr!=sRs)
        {
            EX2MEM.num_error = 1;
            err_processing(NumOver);
        }
        if(addr >=1024)
        {
            EX2MEM.mem_error = 1;
//            err_processing(MemaddrOver);
            end_program = 1;
            return ;
        }
        EX2MEM.ALUout = 0;
        EX2MEM.write_dest = ID2EX.rt;
        EX2MEM.mem_addr = addr;
    }
    else if(ID2EX.opcode == lbu)
    {
        unsigned addr;//sign handle
        unsigned sAddr;
        sRs =(EX2MEM.tmp_rs & mask_31)?1:0;
        sImm =(ID2EX.immediate & mask_31)? 1:0;
        addr = ID2EX.immediate + EX2MEM.tmp_rs;
        sAddr =(addr & mask_31)?1:0;
        if(sRs == sImm && sAddr!=sRs)
        {
            EX2MEM.num_error = 1;
            err_processing(NumOver);
        }
        if(addr >=1024)
        {
            EX2MEM.mem_error = 1;
//            err_processing(MemaddrOver);
            end_program = 1;
            return ;
        }
        EX2MEM.ALUout = 0;
        EX2MEM.write_dest = ID2EX.rt;
        EX2MEM.mem_addr = addr;
    }
    else if(ID2EX.opcode == sw)
    {
        unsigned addr;//sign handle
        addr = ID2EX.immediate + EX2MEM.tmp_rs;
        unsigned sAddr;
        sRs =(EX2MEM.tmp_rs & mask_31)?1:0;
        sImm =(ID2EX.immediate & mask_31)? 1:0;
        addr = ID2EX.immediate + EX2MEM.tmp_rs;
        sAddr =(addr & mask_31)?1:0;
        if(sRs == sImm && sAddr!=sRs)
        {
            EX2MEM.num_error = 1;
            err_processing(NumOver);
        }
        if(addr >=1021 || addr%4)
        {
            if(addr >=1021)
            {
                EX2MEM.mem_error = 1;
//                err_processing(MemaddrOver);
            }
            if(addr%4)
            {
//                err_processing(DataMis);
                EX2MEM.data_miss = 1;
            }
            end_program = 1;
        }
        {
            EX2MEM.mem_addr = addr;
            EX2MEM.write_dest = ID2EX.rt;
            int tmp = EX2MEM.tmp_rt;
            int i;
            for(i=0; i<32; i++)
            {
                if(tmp & masks[31-i])
                {
                    tmp -=masks[31-i];
                    EX2MEM.tmp_dmem[i] = 1;
                }
                else
                {
                    EX2MEM.tmp_dmem[i] = 0;
                }
            }
            EX2MEM.mem_data = EX2MEM.tmp_rt;
        }
    }
    else if(ID2EX.opcode == sh)
    {
        unsigned addr;//sign handle
        unsigned sAddr;
        sRs =(EX2MEM.tmp_rs & mask_31)?1:0;
        sImm =(ID2EX.immediate & mask_31)? 1:0;
        addr = ID2EX.immediate + EX2MEM.tmp_rs;
        sAddr =(addr & mask_31)?1:0;
        if(sRs == sImm && sAddr!=sRs)
        {
            EX2MEM.num_error = 1;
            err_processing(NumOver);
        }
        if(addr >=1023 || addr%2)
        {
            if(addr >=1023)
            {
                EX2MEM.mem_error = 1;
//                err_processing(MemaddrOver);
            }
            if(addr%2)
            {
                EX2MEM.data_miss = 1;
//                err_processing(DataMis);
            }
            end_program = 1;
            return ;
        }
        {
            EX2MEM.mem_addr = addr;
            EX2MEM.write_dest = ID2EX.rt;
            int tmp = EX2MEM.tmp_rt & 0x0000FFFF;
            int i ;
            for(i=0; i<16; i++)
            {
                if(tmp & masks[15-i])
                {
                    tmp -=masks[15-i];
                    EX2MEM.tmp_dmem[i] = 1;
                }
                else
                {
                    EX2MEM.tmp_dmem[i] = 0;
                }
            }
        }
    }
    else if(ID2EX.opcode == sb)
    {
        unsigned addr;//sign handle
        unsigned sAddr;
        sRs =(EX2MEM.tmp_rs & mask_31)?1:0;
        sImm =(ID2EX.immediate & mask_31)? 1:0;
        addr = ID2EX.immediate + EX2MEM.tmp_rs;
        sAddr =(addr & mask_31)?1:0;
        if(sRs == sImm && sAddr!=sRs)
        {
            EX2MEM.num_error = 1;
            err_processing(NumOver);
        }
        if(addr >=1024)
        {
            EX2MEM.mem_error = 1;
//            err_processing(MemaddrOver);
            end_program = 1;
            return ;
        }
        {
            EX2MEM.mem_addr = addr;
            EX2MEM.write_dest = ID2EX.rt;

            int tmp = EX2MEM.tmp_rt & 0x000000FF;
            int i ;
            for(i=0; i<8; i++)
            {
                if(tmp & masks[7-i])
                {
                    tmp -=masks[7-i];
                    EX2MEM.tmp_dmem[i] = 1;
                }
                else
                {
                    EX2MEM.tmp_dmem[i] = 0;
                }
            }
        }
    }
    else if(ID2EX.opcode == lui)
    {
        EX2MEM.ALUout = ID2EX.immediate <<16;
        EX2MEM.write_dest = ID2EX.rt;
    }
    else if(ID2EX.opcode == andi)
    {
        EX2MEM.ALUout = EX2MEM.tmp_rs & ID2EX.immediate;
        EX2MEM.write_dest = ID2EX.rt;
    }
    else if(ID2EX.opcode == ori)
    {
        EX2MEM.ALUout = EX2MEM.tmp_rs | ID2EX.immediate;
        EX2MEM.write_dest = ID2EX.rt;
    }
    else if(ID2EX.opcode == nori)
    {
        EX2MEM.ALUout = ~(EX2MEM.tmp_rs | ID2EX.immediate);
        EX2MEM.write_dest = ID2EX.rt;
    }
    else if(ID2EX.opcode == slti )
    {
        int intImm = ID2EX.immediate;
        int sRs = EX2MEM.tmp_rs;
        EX2MEM.ALUout = sRs < intImm;
        EX2MEM.write_dest = ID2EX.rt;
    }
    else if(ID2EX.opcode == beq)
    {
        EX2MEM.write_dest = 0;
        if(EX2MEM.tmp_rs==EX2MEM.tmp_rt)
        {
            int x = ID2EX.immediate;
            x = x<<2 ;
            currpc = ID2EX.addr + x;
            //      *pc +=x;
        }
        if(currpc >=1020)
        {
            EX2MEM.num_error = 1;
            EX2MEM.mem_error = 1;
            EX2MEM.data_miss = 1;
            err_processing(NumOver);
//            err_processing(MemaddrOver);
//            err_processing(DataMis);
            end_program  = 1;
        }
    }
    else if(ID2EX.opcode == bne)
    {
        EX2MEM.write_dest = 0;
        if(EX2MEM.tmp_rs!=EX2MEM.tmp_rt)
        {
            int x = ID2EX.immediate;
            x = x<<2 ;
            currpc = ID2EX.addr + x;
            //     *pc =*pc + x;
        }
        if(currpc >=1020)
        {
            EX2MEM.num_error = 1;
            EX2MEM.mem_error = 1;
            EX2MEM.data_miss = 1;
            err_processing(NumOver);
//            err_processing(MemaddrOver);
//            err_processing(DataMis);
            end_program  = 1;
        }
    }
    else if(ID2EX.opcode == bgtz)
    {
        EX2MEM.write_dest = 0;
        sRs = (EX2MEM.tmp_rs & mask_31) ? 1:0;
        if(EX2MEM.tmp_rs>0 && sRs==0)
        {
            int x = ID2EX.immediate;
            x = x<<2 ;
            currpc = ID2EX.addr + x - pc;
            //  *pc = *pc  +x;
        }
    }
}
