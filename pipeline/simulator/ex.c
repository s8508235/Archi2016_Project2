#include "ex.h"
int mask_31 = 0x80000000;
int mask_15 = 0x8000;
void EX()
{
    EX2MEM.addr  += 4 ;
    EX2MEM.opcode=ID2EX.opcode;
    EX2MEM.tmp_rt = ID2EX.tmp_rt;
    int i;
    for(i=0; i<32; i++)
    {
        masks[i] = 1 <<i;
    }
    end_program = 0;
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
}
void implementJ()
{
    if(ID2EX.opcode == j)
    {
        pc = ID2EX.addr;
    }
    else if(ID2EX.opcode == jal)
    {
        pc = ID2EX.addr;

    }
}
void implementR()
{
    unsigned sRs,sRt,sRd;
    if(ID2EX.func == add)
    {
        sRs = (ID2EX.tmp_rs & mask_31)? 1:0;
        sRt = (ID2EX.tmp_rt & mask_31)?1:0;
        EX2MEM.ALUout = ID2EX.tmp_rs + ID2EX.tmp_rt;
        EX2MEM.write_dest = ID2EX.rd;
        sRd = (EX2MEM.ALUout & mask_31)?1:0;
        if(sRs ==sRt && sRs !=sRd)
        {
            err_processing(NumOver);
        }
    }
    else if (ID2EX.func == addu)
    {
        EX2MEM.ALUout = ID2EX.tmp_rs + ID2EX.tmp_rt;
        EX2MEM.write_dest = ID2EX.rd;
    }
    else if (ID2EX.func == sub)
    {
        int flag = 0;
        sRs = (ID2EX.tmp_rs & mask_31)?1:0;
        sRt = (-(ID2EX.tmp_rt) & mask_31)?1:0;
        EX2MEM.ALUout = ID2EX.tmp_rs - ID2EX.tmp_rt;
        EX2MEM.write_dest = ID2EX.rd;
        sRd = (EX2MEM.ALUout & mask_31)?1:0;
        if(sRs ==sRt && sRs !=sRd)
        {
            err_processing(NumOver);
        }
        if(flag) EX2MEM.ALUout = 0;
    }
    else if (ID2EX.func == AND)
    {
        EX2MEM.ALUout = ID2EX.tmp_rs & ID2EX.tmp_rt;
        EX2MEM.write_dest = ID2EX.rd;
    }
    else if (ID2EX.func == OR)
    {
        EX2MEM.ALUout = ID2EX.tmp_rs | ID2EX.tmp_rt;
        EX2MEM.write_dest = ID2EX.rd;
    }
    else if (ID2EX.func == XOR)
    {
        EX2MEM.ALUout = ID2EX.tmp_rs ^ ID2EX.tmp_rt;
        EX2MEM.write_dest = ID2EX.rd;
    }
    else if (ID2EX.func == NOR)
    {
        EX2MEM.ALUout =~ (ID2EX.tmp_rs | ID2EX.tmp_rt);
        EX2MEM.write_dest = ID2EX.rd;
    }
    else if (ID2EX.func == NAND)
    {
        EX2MEM.ALUout =~ (ID2EX.tmp_rs & ID2EX.tmp_rt);
        EX2MEM.write_dest = ID2EX.rd;
    }
    else if (ID2EX.func == slt)
    {
        sRs = (ID2EX.tmp_rs & mask_31)?1:0;
        sRt = (ID2EX.tmp_rt& mask_31)?1:0;
        if(sRs < sRt)
            EX2MEM.ALUout = 0;
        else if(sRs == sRt)
        {
            EX2MEM.ALUout = ID2EX.tmp_rs < ID2EX.tmp_rt;
        }
        else
            EX2MEM.ALUout = 1;
        EX2MEM.write_dest = ID2EX.rd;
    }
    else if (ID2EX.func == sll)
    {
        EX2MEM.ALUout = ID2EX.tmp_rt << ID2EX.sht;
        EX2MEM.write_dest = ID2EX.rd;
    }
    else if (ID2EX.func == srl)
    {
        EX2MEM.ALUout = ID2EX.tmp_rt >> ID2EX.sht;
        EX2MEM.write_dest = ID2EX.rd;
    }
    else if (ID2EX.func == sra)
    {
        int tmp = ID2EX.tmp_rt;
        tmp = tmp >>ID2EX.sht;
        EX2MEM.ALUout =tmp;
        EX2MEM.write_dest = ID2EX.rd;
    }
    else if (ID2EX.func == jr)
    {
        EX2MEM.addr = ID2EX.rs - pc -4;
        //  *pc = reg[rs]-unmod_pc-4 ;
    }
}
void implementI()
{
    int sRs,sRt;
    unsigned sImm;
    if(ID2EX.opcode == addi)
    {
        sRs = ID2EX.tmp_rs & mask_31;
        EX2MEM.ALUout = ID2EX.tmp_rs + ID2EX.immediate;
        EX2MEM.write_dest = ID2EX.rd;
        sRt = (ID2EX.tmp_rt & mask_31)?1:0;
        sRt = ID2EX.tmp_rt & mask_31;
        if(sRs == (ID2EX.immediate & mask_31) && sRs !=sRt)
        {
            err_processing(NumOver);
        }
    }
    else if(ID2EX.opcode == addiu)
    {
        EX2MEM.ALUout = ID2EX.tmp_rs + ID2EX.immediate;
        EX2MEM.write_dest = ID2EX.rd;
    }
    else if(ID2EX.opcode == lw)
    {
        unsigned addr;//sign handle
        addr = ID2EX.addr + ID2EX.tmp_rs;
        unsigned sAddr;
        sRs = (ID2EX.tmp_rs & mask_31)?1:0;
        sImm = (ID2EX.immediate & mask_31)?1:0;
        addr = ID2EX.immediate + ID2EX.tmp_rs;
        sAddr =(addr & mask_31)?1:0;

        if(sRs == sImm && sAddr!=sRs)
            err_processing(NumOver);
        if(addr >=1021 || addr%4)
        {
            if(addr >=1021)
                err_processing(MemaddrOver);
            if(addr%4)
                err_processing(DataMis);
            end_program = 1;
            return ;
        }
        EX2MEM.ALUout = 0;
        EX2MEM.write_dest = ID2EX.rd;
        EX2MEM.addr = addr;
    }
    else if(ID2EX.opcode == lh)
    {
        unsigned addr;//sign handle
        unsigned sAddr;
        sRs =(ID2EX.tmp_rs & mask_31)?1:0;
        sImm =(ID2EX.immediate & mask_31)? 1:0;
        addr = ID2EX.immediate + ID2EX.tmp_rs;
        sAddr =(addr & mask_31)?1:0;
        if(sRs == sImm && sAddr!=sRs)
        {
            err_processing(NumOver);
        }
        if(addr >=1023 || addr%2)
        {
            if(addr >=1023)
                err_processing(MemaddrOver);
            if(addr%2)
                err_processing(DataMis);
            end_program= 1;
            return ;
        }
        EX2MEM.ALUout = 0;
        EX2MEM.write_dest = ID2EX.rd;
        EX2MEM.addr = addr;
    }
    else if(ID2EX.opcode == lhu)
    {
        unsigned addr;//sign handle
        unsigned sAddr;
        sRs =(ID2EX.tmp_rs & mask_31)?1:0;
        sImm =(ID2EX.immediate & mask_31)? 1:0;
        addr = ID2EX.immediate + ID2EX.tmp_rs;
        sAddr =(addr & mask_31)?1:0;
        if(sRs == sImm && sAddr!=sRs)
        {
            err_processing(NumOver);
        }
        if(addr >=1023 || addr%2)
        {
            if(addr >=1023)
                err_processing(MemaddrOver);
            if(addr%2)
                err_processing(DataMis);
            end_program = 1;
            return ;
        }
        EX2MEM.ALUout = 0;
        EX2MEM.write_dest = ID2EX.rd;
        EX2MEM.addr = addr;
    }
    else if(ID2EX.opcode == lb)
    {
        unsigned addr;//sign handle
        unsigned sAddr;
        sRs =(ID2EX.tmp_rs & mask_31)?1:0;
        sImm =(ID2EX.immediate & mask_31)? 1:0;
        addr = ID2EX.immediate + ID2EX.tmp_rs;
        sAddr =(addr & mask_31)?1:0;
        if(sRs == sImm && sAddr!=sRs)
        {
            err_processing(NumOver);
        }
        if(addr >=1024)
        {
            if(addr >=1024)
                err_processing(MemaddrOver);
            end_program = 1;
            return ;
        }
        addr = ID2EX.immediate + ID2EX.tmp_rs;
        EX2MEM.ALUout = 0;
        EX2MEM.write_dest = ID2EX.rd;
        EX2MEM.addr = addr;
    }
    else if(ID2EX.opcode == lbu)
    {
        unsigned addr;//sign handle
        unsigned sAddr;
        sRs =(ID2EX.tmp_rs & mask_31)?1:0;
        sImm =(ID2EX.immediate & mask_31)? 1:0;
        addr = ID2EX.immediate + ID2EX.tmp_rs;
        sAddr =(addr & mask_31)?1:0;
        if(sRs == sImm && sAddr!=sRs)
        {
            err_processing(NumOver);
        }
        if(addr >=1024)
        {
            err_processing(MemaddrOver);
            end_program = 1;
            return ;
        }
            EX2MEM.ALUout = 0;
            EX2MEM.write_dest = ID2EX.rd;
            EX2MEM.addr = addr;;
    }
    else if(ID2EX.opcode == sw)
    {
        unsigned addr;//sign handle
        addr = ID2EX.immediate + ID2EX.tmp_rs;
        unsigned sAddr;
        sRs =(ID2EX.tmp_rs & mask_31)?1:0;
        sImm =(ID2EX.immediate & mask_31)? 1:0;
        addr = ID2EX.immediate + ID2EX.tmp_rs;
        sAddr =(addr & mask_31)?1:0;
        if(sRs == sImm && sAddr!=sRs)
        {
            err_processing(NumOver);
        }
        if(addr >=1021 || addr%4)
        {
            if(addr >=1021)
                err_processing(MemaddrOver);
            if(addr%4)
                err_processing(DataMis);
            end_program = 1;
            return ;
        }
        {
            int i;
            int tmp = ID2EX.tmp_rt;
            EX2MEM.addr = addr;
            EX2MEM.write_dest = ID2EX.rd;
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
        }
    }
    else if(ID2EX.opcode == sh)
    {
        unsigned addr;//sign handle
        unsigned sAddr;
        sRs =(ID2EX.tmp_rs & mask_31)?1:0;
        sImm =(ID2EX.immediate & mask_31)? 1:0;
        addr = ID2EX.immediate + ID2EX.tmp_rs;
        sAddr =(addr & mask_31)?1:0;
        if(sRs == sImm && sAddr!=sRs)
        {
            err_processing(NumOver);
        }
        addr = ID2EX.immediate + ID2EX.tmp_rs;
        if(addr >=1023 || addr%2)
        {
            if(addr >=1023)
                err_processing(MemaddrOver);
            if(addr%2)
                err_processing(DataMis);
            end_program = 1;
            return ;
        }
        {
            int i;
            EX2MEM.addr = addr;
            EX2MEM.write_dest = ID2EX.rd;
            int tmp = ID2EX.tmp_rt & 0x0000FFFF;
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
        sRs =(ID2EX.tmp_rs & mask_31)?1:0;
        sImm =(ID2EX.immediate & mask_31)? 1:0;
        addr = ID2EX.immediate + ID2EX.tmp_rs;
        sAddr =(addr & mask_31)?1:0;
        if(sRs == sImm && sAddr!=sRs)
        {
            err_processing(NumOver);
        }
        if(addr >=1024)
        {
            if(addr >=1024)
                err_processing(MemaddrOver);
            end_program = 1;
            return ;
        }
        addr = ID2EX.immediate + ID2EX.tmp_rs;
        {
            EX2MEM.addr = addr;
            EX2MEM.write_dest = ID2EX.rd;
            int i;
            int tmp = ID2EX.tmp_rt & 0x000000FF;
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
        EX2MEM.write_dest = ID2EX.rd;
    }
    else if(ID2EX.opcode == andi)
    {
        EX2MEM.ALUout = ID2EX.tmp_rs & ID2EX.immediate;
        EX2MEM.write_dest = ID2EX.rd;
    }
    else if(ID2EX.opcode == ori)
    {

        EX2MEM.ALUout = ID2EX.tmp_rs | ID2EX.immediate;
        EX2MEM.write_dest = ID2EX.rd;
    }
    else if(ID2EX.opcode == nori)
    {
        EX2MEM.ALUout = ~(ID2EX.tmp_rs | ID2EX.immediate);
        EX2MEM.write_dest = ID2EX.rd;
    }
    else if(ID2EX.opcode == slti )
    {
        int intImm = ID2EX.immediate;
        int sRs = ID2EX.tmp_rs;
        EX2MEM.ALUout = sRs < intImm;
    }
    else if(ID2EX.opcode == beq)
    {
        if(ID2EX.tmp_rs==ID2EX.tmp_rt)
        {
            int x = ID2EX.immediate;
            x = x<<2 ;
            EX2MEM.addr = pc + x;
            //      *pc +=x;
        }
        if(ID2EX.addr >=1020)
        {
            err_processing(NumOver);
            err_processing(MemaddrOver);
            err_processing(DataMis);
            end_program  = 1;
        }
    }
    else if(ID2EX.opcode == bne)
    {
        if(ID2EX.tmp_rs!=ID2EX.tmp_rt)
        {
            int x = ID2EX.immediate;
            x = x<<2 ;
            EX2MEM.addr = ID2EX.addr + x;
            //     *pc =*pc + x;
        }
        if(ID2EX.addr >=1020)
        {
            err_processing(NumOver);
            err_processing(MemaddrOver);
            err_processing(DataMis);
            end_program  = 1;
        }
    }
    else if(ID2EX.opcode == bgtz)
    {
        sRs = (ID2EX.tmp_rs & mask_31) ? 1:0;
        if(ID2EX.tmp_rs>0 && sRs==0)
        {
            int x = ID2EX.immediate;
            x = x<<2 ;
            EX2MEM.addr = ID2EX.addr + x;
            //  *pc = *pc  +x;
        }
    }
}
void err_processing(int errtype)
{
    if(errtype==Writeto0)
    {
        fprintf(err, "In cycle %d: Write $0 Error\n", cycle);
    }
    else if(errtype==NumOver)
    {
        fprintf(err,"In cycle %d: Number Overflow\n",cycle);
    }
    else if(errtype ==MemaddrOver)
    {
        fprintf(err, "In cycle %d: Address Overflow\n", cycle);
    }
    else if(errtype==DataMis)
    {
        fprintf(err, "In cycle %d: Misalignment Error\n", cycle);
    }
}
