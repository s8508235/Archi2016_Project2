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
    iImageData = fopen("iimage.bin","rb");//102062104_01 102062111 102062202_01
    dImageData = fopen("dimage.bin","rb");//102070028_01
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
    WB();
    DM();
    EX();
    if(end_program ==1) return 0;
    ID();
    if(end_program ==1) return 0;
    IF();
    return 0;
}
