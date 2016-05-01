#include "buffer.h"
#include "string.h"
void init_buffer()
{
    IF2ID.pc = 0;
    memset(IF2ID.instrcution,0,sizeof(IF2ID.instrcution));
    IF2ID.opcode = 0;
    IF2ID.instruction_op = 0;

    ID2EX.rs = 0;
    ID2EX.rt = 0;
    ID2EX.rd = 0;
    ID2EX.sht = 0;
    ID2EX.func = 0;
    ID2EX.ctrl = 0;
    ID2EX.immediate = 0;
    ID2EX.addr = 0;
    ID2EX.tmp_rs = 0;
    ID2EX.tmp_rt = 0;
    ID2EX.instruction_op = 0;
    ID2EX.isNop = 1;
    ID2EX.isStall = 0;
    ID2EX.need_forward = 0;
    ID2EX.stop = 0;

    EX2MEM.ALUout = 0;
    memset(EX2MEM.tmp_dmem,0,sizeof(EX2MEM.tmp_dmem));
    EX2MEM.addr = 0;
    EX2MEM.write_dest = 0;
    EX2MEM.opcode = 0;
    EX2MEM.tmp_rt = 0;
    EX2MEM.instruction_op = 0;
    EX2MEM.isNop = 1;
    EX2MEM.isStall = 0;
    EX2MEM.need_forward = 0;
    EX2MEM.stop = 0;

    MEM2WB.ALUout = 0;
    MEM2WB.jal_out = 0;
    MEM2WB.opcode = 0;
    MEM2WB.write_dest = 0;
    MEM2WB.addr = 0;
    MEM2WB.instruction_op = 0;
    MEM2WB.isNop = 1;
    MEM2WB.isStall = 0;
    MEM2WB.stop = 0;

    tmp_wb.ALUout = 0;
    tmp_wb.jal_out = 0;
    tmp_wb.opcode = 0;
    tmp_wb.write_dest = 0;
    tmp_wb.addr = 0;
    tmp_wb.instruction_op = 0;
    tmp_wb.isNop = 1;
    tmp_wb.isStall = 0;
}
