#include "defines.h"

void err_processing(int errtype)
{
    if(errtype==Writeto0)
    {
        fprintf(err, "In cycle %d: Write $0 Error\n", cycle+1);
    }
    else if(errtype==NumOver)
    {
        fprintf(err,"In cycle %d: Number Overflow\n",cycle+1);
    }
    else if(errtype ==MemaddrOver)
    {
        fprintf(err, "In cycle %d: Address Overflow\n", cycle+1);
    }
    else if(errtype==DataMis)
    {
        fprintf(err, "In cycle %d: Misalignment Error\n", cycle+1);
    }
}
