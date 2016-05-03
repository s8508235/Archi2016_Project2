#include "buffer.h"
#include "string.h"
void init_buffer()
{
    IF2ID.pc = 0;
    memset(IF2ID.instrcution,0,sizeof(IF2ID.instrcution));
    IF2ID.opcode = 0;
    IF2ID.instruction_op = 0;
    IF2ID.isFlush = 0;

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
    ID2EX.stop = 0;
    ID2EX.RegWrite = 0;
    ID2EX.mem_read = 0;
    ID2EX.mem_write = 0;
    ID2EX.go_forward = 0;
    ID2EX.forward_mess = 0;
    ID2EX.isFlush = 0;

    EX2MEM.ALUout = 0;
    memset(EX2MEM.tmp_dmem,0,sizeof(EX2MEM.tmp_dmem));
    EX2MEM.addr = 0;
    EX2MEM.write_dest = 0;
    EX2MEM.opcode = 0;
    EX2MEM.tmp_rt = 0;
    EX2MEM.instruction_op = 0;
    EX2MEM.isNop = 1;
    EX2MEM.can_forward = 0;
    EX2MEM.stop = 0;
    EX2MEM.num_error = 0;
    EX2MEM.data_miss = 0;
    EX2MEM.mem_error = 0;
    EX2MEM.tmp_rs = 0;
    EX2MEM.immediate = 0;
    EX2MEM.RegWrite = 0;
    EX2MEM.mem_read = 0;
    EX2MEM.mem_write = 0;
    EX2MEM.go_forward = 0;
    EX2MEM.forward_mess = 0;
    EX2MEM.isFlush = 0;

    MEM2WB.ALUout = 0;
    MEM2WB.jal_out = 0;
    MEM2WB.opcode = 0;
    MEM2WB.write_dest = 0;
    MEM2WB.addr = 0;
    MEM2WB.instruction_op = 0;
    MEM2WB.isNop = 1;
    MEM2WB.stop = 0;
    MEM2WB.num_error = 0;
    MEM2WB.data_miss = 0;
    MEM2WB.mem_error = 0;
    MEM2WB.RegWrite = 0;
    MEM2WB.mem_read = 0;
    MEM2WB.mem_write = 0;
    MEM2WB.can_forward = 0;
    MEM2WB.forward_mess = 0;

    tmp_wb.ALUout = 0;
    tmp_wb.jal_out = 0;
    tmp_wb.opcode = 0;
    tmp_wb.write_dest = 0;
    tmp_wb.addr = 0;
    tmp_wb.instruction_op = 0;
    tmp_wb.isNop = 1;
    tmp_wb.num_error = 0;
    tmp_wb.data_miss = 0;
    tmp_wb.mem_error = 0;
    tmp_wb.RegWrite = 0;
    tmp_wb.mem_read = 0;
    tmp_wb.mem_write = 0;
    tmp_wb.can_forward = 0;
    tmp_wb.forward_mess = 0;


    written_in_this_cycle.ALUout = 0;
    written_in_this_cycle.jal_out = 0;
    written_in_this_cycle.opcode = 0;
    written_in_this_cycle.write_dest = 0;
    written_in_this_cycle.addr = 0;
    written_in_this_cycle.instruction_op = 0;
    written_in_this_cycle.isNop = 1;
    written_in_this_cycle.num_error = 0;
    written_in_this_cycle.data_miss = 0;
    written_in_this_cycle.mem_error = 0;
    written_in_this_cycle.RegWrite = 0;
    written_in_this_cycle.mem_read = 0;
    written_in_this_cycle.mem_write = 0;
    written_in_this_cycle.can_forward = 0;
    written_in_this_cycle.forward_mess = 0;
}
