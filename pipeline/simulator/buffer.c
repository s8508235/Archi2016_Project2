#include "buffer.h"
#include "string.h"
void init_buffer()
{
    IF2ID.pc = 0;
    memset(IF2ID.instrcution,0,sizeof(IF2ID.instrcution));

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

    EX2MEM.ALUout = 0;
    memset(EX2MEM.tmp_dmem,0,sizeof(EX2MEM.tmp_dmem));
    EX2MEM.addr = 0;
    EX2MEM.write_dest = 0;
    EX2MEM.opcode = 0;
    EX2MEM.tmp_rt = 0;

    MEM2WB.ALUout = 0;
    MEM2WB.jal_out = 0;
    MEM2WB.opcode = 0;
    MEM2WB.write_dest = 0;
    MEM2WB.addr = 0;
}
