#include "stdio.h"
#include "id.h"
int prev_tmprs;
int prev_tmprt;
void ID()
{//printf("currpc : %08X pc :%08X PC :%08X\n",currpc,pc ,currpc + pc);
    ID2EX.forward_mess = 0;
    ID2EX.addr  =IF2ID.pc;
    op_num = 0;
    ID2EX.jal_tmp = 0;
    if(ID2EX.isFlush ==1 /*|| currpc < 0 || currpc > num_I *8*/ )
    {
        ID2EX.opcode = 0;
        ID2EX.isNop = 1;
        ID2EX.command = "NOP";
        ID2EX.instruction_op  = 0;
        ID2EX.mem_read = 0;
        ID2EX.RegWrite = 0;
        ID2EX.mem_write = 0;
        ID2EX.tmp_rs = 0;
        ID2EX.tmp_rt = 0;
        ID2EX.rs = 0;
        ID2EX.rt = 0;
        ID2EX.rd = 0;
        ID2EX.immediate = 0;
    }
    ID2EX.instruction_op =IF2ID.instruction_op ;
    ID2EX.isFlush = 0;
    end_program = 0;
    int i;
    if(ID2EX.isStall == 0)
    {
        for(i=0; i<6; i++)
        {
            op_num  = op_num<< 1 ;
            op_num += IF2ID.instrcution[i];
        }
        rs = 0,rt = 0,imm = 0,rd = 0,sht = 0,func = 0,C = 0;
        for(i=6; i<11; i++)
        {
            rs = rs <<1;
            rs +=IF2ID.instrcution[i];
        }
        for(i=11; i<16; i++)
        {
            rt = rt<<1;
            rt +=IF2ID.instrcution[i];
        }
        for(i=16; i<21; i++)
        {
            rd = rd<<1;
            rd +=IF2ID.instrcution[i];
        }
        for(i=21; i<26; i++)
        {
            sht = sht<<1;
            sht += IF2ID.instrcution[i];
        }
        for(i=26; i<32; i++)
        {
            func = func <<1;
            func+= IF2ID.instrcution[i];
        }
        for(i=16; i<32; i++)
        {
            imm = imm<<1;
            imm +=IF2ID.instrcution[i];
        }
        for(i=6;i<32;i++)
        {
            C = C<<1;
            C +=IF2ID.instrcution[i];
        }
        ID2EX.rs = rs;
        ID2EX.rt = rt;
        ID2EX.RegWrite = 1;
        ID2EX.tmp_rs = 0;
        ID2EX.tmp_rt = 0;
        ID2EX.jal_tmp = 0;
        ID2EX.immediate = 0;
        ID2EX.mem_write = 0;
        ID2EX.C = C;
     //   printf("init com\n");
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
            ID2EX.opcode = op_num;
            end_program = 1;
            ID2EX.command = "HALT";
            ID2EX.isNop = 0;
        }
        else
        {
            instruction_I();
        }
    }
    else
    {
        if(tmp_wb.write_dest == ID2EX.rs&& ID2EX.rs!=0 && tmp_wb.RegWrite ==1)
            ID2EX.tmp_rs = tmp_wb.ALUout;
        else //if(written_in_this_cycle.RegWrite ==1)
            ID2EX.tmp_rs = reg[rs];

        if(tmp_wb.write_dest == ID2EX.rt&& ID2EX.rt!=0 && tmp_wb.RegWrite ==1)
            ID2EX.tmp_rt = tmp_wb.ALUout;
        else //if(written_in_this_cycle.RegWrite ==1)
            ID2EX.tmp_rt = reg[rt];
   //     printf("%d %d\n",ID2EX.tmp_rs,ID2EX.tmp_rt);
   //     printf("no reading\n");
    }
    prediction();
    ID2EX.isStall = 0;
    stall_detect();
  //  printf("stall & forward:%d %d~~~~~~~~~~~~~\n",ID2EX.isStall,EX2MEM.go_forward);
    ID2EX.stop = end_program;
    //judge flush
if(ID2EX.isStall == 0 && ID2EX.go_forward == 0 && EX2MEM.go_forward == 0)
{
    //printf("ready to flush!!!!!!!!!!!!!!!!!! \n");
    if(strcmp(ID2EX.command,"BGTZ") ==0)
    {
        int sRG = ID2EX.tmp_rs;
        if(sRG >  0)
        {
            ID2EX.isFlush = 1;
        }
    }
    else if(strcmp(ID2EX.command,"BEQ") ==0)
    {//printf("compare:%d %d\n",ID2EX.tmp_rs,ID2EX.tmp_rt);
        if(ID2EX.tmp_rs == ID2EX.tmp_rt)
        {
            ID2EX.isFlush = 1;
        }
    }
    else if(strcmp(ID2EX.command,"BNE") ==0)
    {
        if(ID2EX.tmp_rs != ID2EX.tmp_rt)
        {
            ID2EX.isFlush = 1;
        }
    }
    else if(strcmp(ID2EX.command,"JR") ==0)
    {
        ID2EX.isFlush = 1;
    }
    else if(strcmp(ID2EX.command,"JAL") ==0)
    {//printf("go jal\n");
        ID2EX.isFlush = 1;
    }
    else if(strcmp(ID2EX.command,"J") ==0)
    {
        ID2EX.isFlush = 1;
    }

}
  //  printf("************get %s numbers : %d %d %d %08X %08X %08X\n",ID2EX.command,rs,rt,rd,ID2EX.tmp_rs,ID2EX.tmp_rt,ID2EX.immediate);
  //  printf("%08X\n",ID2EX.instruction_op);
}
void instruction_R()
{
    ID2EX.opcode = op_num;
    ID2EX.rs = rs;
    ID2EX.rt = rt;
    ID2EX.rd = rd;
    ID2EX.sht = sht;
    if(tmp_wb.write_dest == rs && rs !=0 && tmp_wb.RegWrite == 1)
        ID2EX.tmp_rs = tmp_wb.ALUout;
    else ID2EX.tmp_rs = reg[rs];
    if(tmp_wb.write_dest ==rt && rt!=0 && tmp_wb.RegWrite == 1)
        ID2EX.tmp_rt = tmp_wb.ALUout;
    else ID2EX.tmp_rt = reg[rt];
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
        int x,flag = 0;
        for(x= 11;x<32;x++)
        {
           if(IF2ID.instrcution[x] ==1)
           {
                flag = 1;
                break;
           }
        }
        if(flag ==0)
        {
            ID2EX.isNop = 1;
            ID2EX.command ="NOP";
        }
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
        ID2EX.RegWrite = 0;
        //ID2EX.addr = ID2EX.rs - pc -4;
    }
}
void instruction_I()
{
    ID2EX.opcode = op_num;
    ID2EX.rs = rs;
    ID2EX.rt = rt;
    ID2EX.isNop = 0;
    if(tmp_wb.write_dest == rs && rs!=0 && tmp_wb.RegWrite == 1)
        ID2EX.tmp_rs = tmp_wb.ALUout;
    else ID2EX.tmp_rs = reg[rs];
    if(tmp_wb.write_dest ==rt && rt!=0 && tmp_wb.RegWrite == 1)
    {
        ID2EX.tmp_rt = tmp_wb.ALUout;
    }
    else ID2EX.tmp_rt = reg[rt];

    if(op_num == andi || op_num == ori || op_num == nori)
    {
        ID2EX.immediate = imm;
    }
    else
    {//printf("exxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxe\n");
        unsigned eximm;
        if(imm & 0x8000)
            eximm = imm | 0xffff0000;
        else
            eximm = imm | 0x00000000;
        ID2EX.immediate = eximm;
    }

    if(op_num == addi)
    {
        ID2EX.command = "ADDI";
    }
    else if(op_num == addiu)
    {
        ID2EX.command = "ADDIU";
    }
    else if(op_num == lw)
    {
        ID2EX.command = "LW";
        ID2EX.mem_read = 1;
    }
    else if(op_num == lh)
    {
        ID2EX.command = "LH";
        ID2EX.mem_read = 1;
    }
    else if(op_num == lhu)
    {
        ID2EX.command = "LHU";
        ID2EX.mem_read = 1;
    }
    else if(op_num == lb)
    {
        ID2EX.command = "LB";
        ID2EX.mem_read = 1;
    }
    else if(op_num == lbu)
    {
        ID2EX.command = "LBU";
        ID2EX.mem_read = 1;
    }
    else if(op_num == sw)
    {
        ID2EX.command = "SW";
        ID2EX.RegWrite = 0;
        ID2EX.mem_write = 1;
    }
    else if(op_num == sh)
    {
        ID2EX.command = "SH";
        ID2EX.RegWrite = 0;
        ID2EX.mem_write = 1;
    }
    else if(op_num == sb)
    {
        ID2EX.command = "SB";
        ID2EX.RegWrite = 0;
        ID2EX.mem_write = 1;
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
        ID2EX.RegWrite = 0;
    }
    else if(op_num == bne)
    {
        ID2EX.command = "BNE";
        ID2EX.RegWrite = 0;
    }
    else if(op_num == bgtz)
    {
        ID2EX.command = "BGTZ";
        ID2EX.RegWrite = 0;
    }
}
void instruction_J()
{
    ID2EX.opcode = op_num;
    ID2EX.isNop = 0;
    if(op_num == j)
    {
       // ID2EX.addr = ((pc + IF2ID.pc + 4) & 0x80000000 )| (4 * imm);
        ID2EX.command = "J";
  //      printf("JOOOOOOOOOOOOOOOOOO\n");
        ID2EX.RegWrite = 0;
    }
    else if(op_num == jal)
    {
       // ID2EX.addr = ((pc + IF2ID.pc + 4) & 0x80000000 )| (4 * imm);
        ID2EX.command = "JAL";
        ID2EX.jal_tmp = pc + IF2ID.pc;
//	printf("ID2EX:%08X\n",ID2EX.jal_tmp);
    }

}/*
int rs_func[10] = {add,sub,AND,OR,XOR,NOR,NAND,slt,jr};
int rt_func[12] = {add,sub,AND,OR,XOR,NOR,NAND,slt,sll,srl,sra};
int rs_opcode[16] = {addi,lw,lh,lhu,lb,lbu,sw,sh,sb,andi,ori,nori,slti,beq,bne};
int rt_opcode[5] = {sw,sh,sb,beq,bne};*/
int flag_rs_exmem;
int flag_rt_exmem;
int flag_rs_memwb;
int flag_rt_memwb;
void prediction()
{
    flag_rs_exmem = 0;
    flag_rt_exmem = 0;
    flag_rs_memwb = 0;
    flag_rt_memwb = 0;
    EX2MEM.can_forward = 0;
    if(EX2MEM.RegWrite ==1)
    if(EX2MEM.write_dest !=0)
    {
        if(EX2MEM.write_dest == rs)
        {
            if(EX2MEM.opcode == 0)
            {
                if(EX2MEM.func !=8)
                {
                    flag_rs_exmem = 1;/*
                    if(rs == rt && (ID2EX.func ==0 || ID2EX.func ==2 || ID2EX.func ==3) && ID2EX.isNop ==0)
                    {
                        flag_rs_exmem = 0;
                        flag_rt_exmem = 1;
                    }*/
                }
            }
            else
            {
                if(EX2MEM.opcode >=8 && EX2MEM.opcode <=37)
                {
                    flag_rs_exmem = 1;
                }
                else if(EX2MEM.opcode ==3)
                {
                    if(EX2MEM.write_dest == 31)
                        flag_rs_exmem = 1;
                }
            }
        }
        else if(EX2MEM.write_dest == rt)
        {
            if(EX2MEM.opcode == 0)
            {
                if(EX2MEM.func !=jr)
                {
                    flag_rt_exmem = 1;
                }
            }
            else
            {
                if(EX2MEM.opcode >=8 && EX2MEM.opcode <=37)
                {
                    flag_rt_exmem = 1;
                }
                else if(EX2MEM.opcode ==jal)
                {
                    if(EX2MEM.write_dest == 31)
                        flag_rt_exmem = 1;
                }
            }
        }
        if(((EX2MEM.opcode == jal)||(EX2MEM.opcode == R && EX2MEM.func != 8) || (EX2MEM.opcode >= 8 && EX2MEM.opcode <= 15)))
            EX2MEM.can_forward = 1;
    }
    if(func == sll || func == sra  || func == srl)
    {if(ID2EX.opcode == 0)
        if(rs == rt && ID2EX.isNop ==0 && rs !=0)
        {
            if(EX2MEM.write_dest == rs )
            {//printf("%d:change!!! %s \n",cycle,ID2EX.command);
                if(flag_rs_exmem == 1){
		flag_rs_exmem = 0;
                flag_rt_exmem = 1;}
            }
        }
    }//if(rs==rt) flag_rt_exmem = flag_rs_exmem;
    MEM2WB.can_forward = 0;
    if(MEM2WB.RegWrite ==1)
    if(MEM2WB.write_dest !=0)
    {
        if(MEM2WB.write_dest == rs)
        {
            if(MEM2WB.opcode == 0)
            {
                if(MEM2WB.func !=jr)
                {
                    flag_rs_memwb = 1;
                }
            }
            else
            {
                if(MEM2WB.opcode >=8 && MEM2WB.opcode <=37)
                {
                    flag_rs_memwb = 1;
                }
                else if(MEM2WB.opcode ==3)
                {
                    if(MEM2WB.write_dest == 31)
                        flag_rs_memwb = 1;
                }
            }
        }
        else if(MEM2WB.write_dest == rt)
        {
            if(MEM2WB.opcode == 0)
            {
                if(MEM2WB.func !=jr)
                {
                    flag_rt_memwb = 1;
                }
            }
            else
            {
                if(MEM2WB.opcode >=8 && MEM2WB.opcode <=37)
                {
                    flag_rt_memwb = 1;
                }
                else if(MEM2WB.opcode ==jal)
                {
                    if(MEM2WB.write_dest == 31)
                        flag_rt_memwb = 1;
                }
            }
        }
        if((MEM2WB.opcode == R && MEM2WB.func != 8) || (MEM2WB.opcode >= 8 && MEM2WB.opcode <= 15) || (MEM2WB.opcode == jal))
            MEM2WB.can_forward = 1;
            
if(func == sll || func == sra  || func == srl)
    {if(ID2EX.opcode == 0)
        if(rs == rt && ID2EX.isNop ==0 && rs !=0)
        {
            if(MEM2WB.write_dest == rs )
            {//printf("%d:change!!! %s \n",cycle,ID2EX.command);
                if(flag_rs_memwb == 1){
	             	flag_rs_memwb = 0;
                flag_rt_memwb = 1;}
            }
        }
    }
    }
            if(MEM2WB.mem_error ==1 || MEM2WB.data_miss == 1)
            {
                flag_rs_memwb = 0;
                flag_rt_memwb = 0;
            }
}
void stall_detect()
{   if(EX2MEM.isNop ==1 && MEM2WB.isNop ==1) return ;/*
printf("curr name:%s\n",ID2EX.command);
printf("flags:%d %d %d %d\n",flag_rs_exmem,flag_rt_exmem,flag_rs_memwb,flag_rt_memwb);
printf("go in exmem: %s memwb: %s\n",EX2MEM.command,MEM2WB.command);
printf("goto %d %d\n",EX2MEM.write_dest ,MEM2WB.write_dest);
printf("can forward :%d %d\n",EX2MEM.can_forward,MEM2WB.can_forward);
printf("curr %d %d\n",ID2EX.rs,ID2EX.rt);*/
    ID2EX.go_forward = 0;/* 1 for rs 2 for rt*/
    EX2MEM.go_forward = 0;
    if((ID2EX.opcode>=7 && ID2EX.opcode <=37 && ID2EX.opcode !=lui )|| (ID2EX.opcode==0 &&ID2EX.func == 8))
    {//printf("I type or jr\n");
        if(flag_rs_exmem ==1 && flag_rt_exmem ==0)
        {//printf("rs exmem\n");
            if(EX2MEM.can_forward == 1 && ID2EX.opcode !=7 && ID2EX.opcode !=0)
            {//printf("exmem can forward(exclude bgtz)\n");
                EX2MEM.go_forward = 1;
                EX2MEM.forward_pos = ID2EX.rs;
                ID2EX.isStall = 0;
            }
            else
            {
                ID2EX.isStall = 1;
            }
        }
        else if(flag_rs_memwb == 1)
        {//printf("rs memwb\n");
            if(MEM2WB.can_forward ==1 && (ID2EX.opcode ==bgtz|| (ID2EX.opcode ==0 && ID2EX.func ==8)))
            {/*do forward now and here*/
                ID2EX.forward_mess = 1;
                ID2EX.tmp_rs = MEM2WB.ALUout;
		if(cycle == 54)
                ID2EX.isStall = 0;
            }
            else ID2EX.isStall = 1;
        }
        else ID2EX.isStall = 0;
    }
    else if(ID2EX.opcode ==0 && ID2EX.func >=0 && ID2EX.func <=3 && ID2EX.func !=1)
    {//printf("R srl sll slt\n");
        if(flag_rt_exmem ==1)
        {//printf("exmem rt\n");
            if(EX2MEM.can_forward == 1)
            {//printf("exmem can forward\n");
                EX2MEM.go_forward = 2;
                EX2MEM.forward_pos = ID2EX.rt;
                ID2EX.isStall = 0;
            }
            else
                ID2EX.isStall = 1;
        }
        else if(flag_rt_memwb == 1)
        {//printf("memwb rt\n");
            ID2EX.isStall = 1;
        }
        else ID2EX.isStall = 0;
    }
    else if(ID2EX.opcode != j && ID2EX.opcode != jal && ID2EX.opcode != halt)
    {	if(ID2EX.opcode == lui){ return ;}
        if(ID2EX.rs == ID2EX.rt)
        {//printf("rs == rt\n");
            if(flag_rs_exmem ==1 )
            {//printf("rs exmem\n");
                if(EX2MEM.can_forward ==1 && ID2EX.opcode != bne && ID2EX.opcode != beq)
                {//printf("exmem can forward\n");
                    EX2MEM.go_forward = 3;
                    EX2MEM.forward_pos = EX2MEM.write_dest;
                    ID2EX.isStall = 0;
                }
                else
                {
                    ID2EX.isStall = 1;
                }
            }
            else if (flag_rs_memwb)
            {//printf("rs memwb\n");
                if(MEM2WB.can_forward ==1 && (ID2EX.opcode == bne || ID2EX.opcode == beq))
                {/*do forward now and here*///printf("memwb can forward (bne/beq)&here\n");
                    ID2EX.forward_mess = 3;
                    ID2EX.tmp_rs = MEM2WB.ALUout;
                    ID2EX.tmp_rt = MEM2WB.ALUout;
                    ID2EX.isStall = 0;
                }
                else
                {
                    ID2EX.isStall = 1;
                }
            }
            else
            {
                ID2EX.isStall = 0;
            }
        }
        else
        {//printf("rs != rt\n");
            if((flag_rs_exmem && flag_rt_memwb) || (flag_rs_memwb && flag_rt_exmem))
            {// double data hazard
                ID2EX.isStall = 1;
            }
            else if ((flag_rs_exmem || flag_rt_exmem)&& EX2MEM.can_forward ==1 )
            {//printf("exmem has flag\n");
                if( ID2EX.opcode != bne && ID2EX.opcode != beq)
                {//printf("exmem can forward not bne/beq\n");
                    if(flag_rs_exmem)
                    {//printf("rs exmem\n");
                        EX2MEM.go_forward = 1;
                        EX2MEM.forward_pos = ID2EX.rs;
                    }
                    else
                    {//printf("rt exmem\n");
                        EX2MEM.go_forward = 2;
                        EX2MEM.forward_pos = ID2EX.rt;
                    }
                    ID2EX.isStall = 0;
                }
                else
                {
                    ID2EX.isStall = 1;
                }
            }
            else if ((flag_rs_memwb || flag_rt_memwb)&&MEM2WB.can_forward ==1)
            {//printf("memwb has flag\n");
                if( (ID2EX.opcode == bne || ID2EX.opcode == beq))
                {//printf("memwb can forward (bne/beq)from %s forward to %s\n",MEM2WB.command,ID2EX.command);
                    if(flag_rs_memwb)
                    {//printf("rs forward\n");
                        ID2EX.forward_mess = 1;
                        ID2EX.tmp_rs = MEM2WB.ALUout;
                    }
                    else
                    {//printf("rt forward\n");
                        ID2EX.forward_mess = 2;
                        ID2EX.tmp_rt = MEM2WB.ALUout;
                    }
                    ID2EX.isStall = 0;
                }
                else
                {
                    ID2EX.isStall = 1;
                }
            }
	    else if((flag_rs_exmem || flag_rt_exmem) && EX2MEM.can_forward==0) ID2EX.isStall = 1;
	    else if((flag_rs_memwb || flag_rt_memwb) && MEM2WB.can_forward==0) ID2EX.isStall = 1;
            else
            {
                ID2EX.isStall = 0; //no stall
            }
        }
    }
}
