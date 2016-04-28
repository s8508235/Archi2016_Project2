#include "stdio.h"
#include "id.h"
void ID()
{
    op_num = 0;
    end_program = 0;
    int i;
    for(i=0;i<6;i++)
    {
        op_num  = op_num<< 1 ;
        op_num += IF2ID.instrcution[i];
    }
    printf("op:%d\n",op_num);
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
    }
    else
    {
        instruction_I();
    }

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
    ID2EX.addr  +=4 ;
}
void instruction_I()
{
    ID2EX.opcode = op_num;
    ID2EX.rs = rs;
    ID2EX.rt = rt;
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
    ID2EX.addr += 4;
}
void instruction_J()
{
    ID2EX.opcode = op_num;
    if(op_num == j)
    {
        ID2EX.addr = ((pc + IF2ID.pc + 4) & 0x80000000 )| (4 * imm);
    }
    else if(op_num == jal)
    {
        ID2EX.addr = ((pc + IF2ID.pc + 4) & 0x80000000 )| (4 * imm);
    }
}

