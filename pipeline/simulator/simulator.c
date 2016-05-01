#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "defines.h"
#include "buffer.h"
#include "wb.h"
#include "dm.h"
#include "ex.h"
#include "id.h"
#include "if.h"
FILE *iImageData,*dImageData;
int main()
{
    cycle = 0;
    iImageData = fopen("iimage.bin","rb");
    dImageData = fopen("dimage.bin","rb");
    int isize,dsize;
    fseek(iImageData,0,SEEK_END);
    isize = ftell(iImageData);
    fseek(iImageData,0,SEEK_SET);

    fseek(dImageData,0,SEEK_END);
    dsize = ftell(dImageData);
    fseek(dImageData,0,SEEK_SET);
    if(isize >=1032) isize = 1032;
    if(dsize >=1032) dsize = 1032;
    int i;
    char ibuffer[1032],dbuffer[1032];
    unsigned int iimage[isize],dimage[dsize];
    memset(ibuffer,0,sizeof(ibuffer));
    memset(iimage,0,sizeof(iimage));
    memset(dbuffer,0,sizeof(dbuffer));
    memset(dimage,0,sizeof(dimage));
    memset(reg,0,sizeof(reg));
    for(i=0;i<8;i++)
    {
       mask[i] = 1<<i;
    }
    fread(&dbuffer,1,dsize,dImageData);
    fread(&ibuffer,1,isize,iImageData);
    for(i=0;i<isize;i++)
    {
       iimage[i] =(unsigned char) ibuffer[i];
    }
    for(i=0;i<dsize;i++)
    {
        dimage[i] =(unsigned char) dbuffer[i]-'\0';
    }
    fclose(iImageData);
    fclose(dImageData);

    pc =0;
    unsigned int sp = 0;
    unsigned int num_I = 0,num_D = 0;
    for(i=0;i<4;i++)
    {
        pc = pc<<8;
        pc +=iimage[i];
        num_I = num_I <<8;
        num_I += iimage[i+4];

        sp = sp<<8;
        sp+=dimage[i];
        num_D = num_D<<8;
        num_D +=dimage[i+4];
    }
    for(i=0;i<8192;i++)
    {
        Imem[i] = 0;
        Dmem[i] = 0;
    }
    int i_cnt = 0,d_cnt = 0,k;
    for(i=8;i<8+4*num_I;i++)
    {
        for(k=0;k<8;k++,i_cnt++)
        {
            if(iimage[i] & mask[7-k])
            {
                iimage[i] -=mask[7-k]+'\0';
                Imem[i_cnt] = 1;
            }
        }
    }
    for(i=8;i<8+4*num_D;i++)
    {
        for(k=0;k<8;k++,d_cnt++)
        {
            if(dimage[i] & mask[7-k])
            {
                dimage[i] -=mask[7-k]+'\0';
                Dmem[d_cnt] = 1;
            }
        }
    }
    reg[29] = sp;
    out = fopen("snapshot.rpt","wb");
    err = fopen("error_dump.rpt","wb");
    currpc = 0;
    init_buffer();
    int wb_stop = 0;
    while(wb_stop == 0)
    {
        end_program = 0;
    //   int wb_data = MEM2WB.instruction_op;
        /*previous wb's state*/
        fprintf(out,"cycle %d\n",cycle++);
        char* wb_command = MEM2WB.command;
        int wb_isNOP = MEM2WB.isNop;
        wb_stop = MEM2WB.stop;
        currpc = IF2ID.pc;
        WB();
        DM();
        EX();
        if(end_program ==1) break;
        ID();
        IF();
        printf("%d %d\n",pc,currpc);
        for(i=0;i<32;i++)
        {
            if(i<10)
            fprintf(out,"$0%d: 0x",i);
            else
            fprintf(out,"$%d: 0x",i);
            fprintf(out,"%08X\n",reg[i]);
        }
        fprintf(out,"PC: 0x%08X\n",pc+currpc);
        fprintf(out,"IF: 0x%08X",IF2ID.instruction_op);
        if(ID2EX.isStall ==1) fprintf(out," to_be_stalled");
        fprintf(out,"\n");
        fprintf(out,"ID: %s",(/*(ID2EX.command =="SLL"  && ID2EX.instruction_op  ==0x00000000 )|| */ID2EX.isNop ==1)?"NOP":ID2EX.command);
        if(ID2EX.isStall ==1) fprintf(out," to_be_stalled");
        else if(ID2EX.need_forward ==1) fprintf(out," fwd_EX-DM_rs_$%d",ID2EX.rs);
        else if(ID2EX.need_forward ==2) fprintf(out," fwd_EX-DM_rt_$%d",ID2EX.rt);
        fprintf(out,"\n");
        fprintf(out,"EX: %s\n",(/*(EX2MEM.command =="SLL" && EX2MEM.instruction_op ==0x00000000 )||*/ EX2MEM.isNop ==1)?"NOP":EX2MEM.command);
        fprintf(out,"DM: %s\n",(/*(MEM2WB.command =="SLL" && MEM2WB.instruction_op ==0x00000000 )|| */MEM2WB.isNop ==1)?"NOP":MEM2WB.command);
        fprintf(out,"WB: %s",(/*(wb_command =="SLL" && wb_data ==0x00000000)||*/wb_isNOP ==1)?"NOP":wb_command);
        fprintf(out,"\n\n\n");
    }
    return 0;
}
