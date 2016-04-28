#include "if.h"
#include "string.h"
void IF()
{
    int i,k;
    k = IF2ID.pc / 4;
    for(i=0;i<32;i++)
    {
        IF2ID.instrcution[i] = Imem[i+32*k];
    }
    IF2ID.pc +=4;
}
