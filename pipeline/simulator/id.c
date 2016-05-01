#include "stdio.h"
#include "id.h"
void ID()
{
    ID2EX.need_forward = 0;
    ID2EX.addr  =IF2ID.pc;
    op_num = 0;
    end_program = 0;
    int i;
    if(EX2MEM.need_forward > 0)
    {
        printf("forward!\n");
        if(EX2MEM.need_forward ==1)
        ID2EX.tmp_rs = EX2MEM.ALUout;
        else if(EX2MEM.need_forward ==2)
        ID2EX.tmp_rt = EX2MEM.ALUout;
        ID2EX.need_forward = EX2MEM.need_forward;
        EX2MEM.need_forward = 0;
        ID2EX.isStall= 0;
        return ;
    }
    if(ID2EX.isStall == 0)
    {
        ID2EX.instruction_op =IF2ID.instruction_op ;
        for(i=0;i<6;i++)
        {
            op_num  = op_num<< 1 ;
            op_num += IF2ID.instrcution[i];
        }
        rs = 0,rt = 0,imm = 0,rd = 0,sht = 0,func = 0;
        for(i=6;i<11;i++)
        {
            rs = rs <<1;
            rs +=IF2ID.instrcution[i];
        }
        for(i=11;i<16;i++)
        {
            rt = rt<<1;
            rt +=IF2ID.instrcution[i];
        }
        for(i=16;i<21;i++)
        {
            rd = rd<<1;
            rd +=IF2ID.instrcution[i];
        }
        for(i=21;i<26;i++)
        {
            sht = sht<<1;
            sht += IF2ID.instrcution[i];
        }
        for(i=26;i<32;i++)
        {
            func = func <<1;
            func+= IF2ID.instrcution[i];
        }
        for(i=16;i<32;i++)
        {
            imm = imm<<1;
            imm +=IF2ID.instrcution[i];
        }
        ID2EX.rs = rs;
        ID2EX.rt = rt;
        if(op_num == 0)
        {
            instruction_R();
        }
        else if(op_num==2 || op_num==3)
        {
            instruction_J();
        }
        else if(op_num ==63)
        {
            fclose(err);
            end_program = 1;
            ID2EX.command = "HALT";
        }
        else
        {
            instruction_I();
        }
    }
    ID2EX.isStall = 0;
    branch_stall_detect();
    stall_detect();
    printf("%s\n",ID2EX.command);
    ID2EX.stop = end_program;
}
void instruction_R()
{
    ID2EX.opcode = op_num;
    ID2EX.rs = rs;
    ID2EX.rt = rt;
    ID2EX.rd = rd;
    ID2EX.sht = sht;
    ID2EX.tmp_rs = reg[rs];
    ID2EX.tmp_rt = reg[rt];
    ID2EX.func = func;
    ID2EX.isNop = 0;
    if(func == add)
    {
        ID2EX.command = "ADD";
    }
    else if (func == addu)
    {
        ID2EX.command = "ADDU";
    }
    else if (func == sub)
    {
        ID2EX.command = "SUB";
    }
    else if (func == AND)
    {
        ID2EX.command = "AND";
    }
    else if (func == OR)
    {
        ID2EX.command = "OR";
    }
    else if (func == XOR)
    {
        ID2EX.command = "XOR";
    }
    else if (func == NOR)
    {
        ID2EX.command = "NOR";
    }
    else if (func == NAND)
    {
        ID2EX.command = "NAND";
    }
    else if (func == slt)
    {
        ID2EX.command = "SLT";
    }
    else if (func == sll)
    {
        ID2EX.command = "SLL";
        if(ID2EX.instruction_op == 0x00000000) ID2EX.isNop = 1;
    }
    else if (func == srl)
    {
        ID2EX.command = "SRL";
    }
    else if (func == sra)
    {
        ID2EX.command = "SRA";
    }
    else if (func == jr)
    {
        ID2EX.command = "JR";
    }
}
void instruction_I()
{
    ID2EX.opcode = op_num;
    ID2EX.rs = rs;
    ID2EX.rt = rt;
    ID2EX.isNop = 0;
    if(op_num == andi || op_num == ori || op_num == nori)
    {
        ID2EX.immediate = imm;
    }
    else
    {
        unsigned eximm;
        if(imm & 0x8000)
            eximm = imm | 0xffff0000;
        else
            eximm = imm | 0x00000000;
        ID2EX.immediate = eximm;
        ID2EX.tmp_rs = reg[rs];
        ID2EX.tmp_rt = reg[rt];
    }

    if(op_num == addi)
    {
        ID2EX.command = "ADDI";
    }
    else if(op_num == addiu)
    {
        ID2EX.command = "ADDU";
    }
    else if(op_num == lw)
    {
        ID2EX.command = "LW";
    }
    else if(op_num == lh)
    {
        ID2EX.command = "LH";
    }
    else if(op_num == lhu)
    {
      ID2EX.command = "LHU";
    }
    else if(op_num == lb)
    {
        ID2EX.command = "LB";
    }
    else if(op_num == lbu)
    {
        ID2EX.command = "LBU";
    }
    else if(op_num == sw)
    {
        ID2EX.command = "SW";
    }
    else if(op_num == sh)
    {
        ID2EX.command = "SH";
    }
    else if(op_num == sb)
    {
        ID2EX.command = "SB";
    }
    else if(op_num == lui)
    {
        ID2EX.command = "LUI";
    }
    else if(op_num == andi)
    {
        ID2EX.command = "ANDI";
    }
    else if(op_num == ori)
    {
        ID2EX.command = "ORI";
    }
    else if(op_num == nori)
    {
        ID2EX.command = "NORI";
    }
    else if(op_num == slti )
    {
        ID2EX.command = "SLTI";
    }
    else if(op_num == beq)
    {
        ID2EX.command = "BEQ";
    }
    else if(op_num == bne)
    {
        ID2EX.command = "BNE";
    }
    else if(op_num == bgtz)
    {
        ID2EX.command = "BGTZ";
    }
}
void instruction_J()
{
    ID2EX.opcode = op_num;
    ID2EX.isNop = 0;
    if(op_num == j)
    {
        ID2EX.addr = ((pc + IF2ID.pc + 4) & 0x80000000 )| (4 * imm);
        ID2EX.command = "J";
    }
    else if(op_num == jal)
    {
        ID2EX.addr = ((pc + IF2ID.pc + 4) & 0x80000000 )| (4 * imm);
        ID2EX.command = "JAL";
    }

}
void branch_stall_detect()
{
    if(EX2MEM.write_dest !=0)
    {
        if(op_num >0 && op_num <=37)
        {
            if(EX2MEM.write_dest == rs || EX2MEM.write_dest ==rt)
            {
                printf("branch stall\n");
                ID2EX.isStall = 1;
            }
        }
        else if(op_num ==0)
        {printf("%s %s ",ID2EX.command,EX2MEM.command);
            printf("%s\n",MEM2WB.command);
            if(EX2MEM.write_dest == rs || EX2MEM.write_dest == rt ||EX2MEM.write_dest == rd )
            {
                ID2EX.isStall = 1;
                printf("stall!!\n");
            }
        }
    }

}
void stall_detect()
{
    if(op_num ==0)
    {printf("%s: %d\n",MEM2WB.command,MEM2WB.write_dest);
        if(MEM2WB.write_dest !=0)
        if(MEM2WB.write_dest == rs || MEM2WB.write_dest == rt || MEM2WB.write_dest == rd)
        {
            ID2EX.isStall = 1;
            printf("stall!!???\n");
        }
    }
}
