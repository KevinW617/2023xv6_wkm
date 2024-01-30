

//sleep.c

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"



int main (int argc , char * argv)
{
    if(argc!=2)
    {
        fprintf(2,"expected: sleep ticks\n");
        exit(1);
    }

    int ticks = atoi(&(argv[1]));
    if(ticks < 0)
    {
        fprintf(2,"ticks is invalid\n");
        exit(1);
    }
    
    if (sleep(ticks) != 0) {
        fprintf(2, "Error in sleep sys_call!\n");
        exit(1);
    }
    exit(0);
}