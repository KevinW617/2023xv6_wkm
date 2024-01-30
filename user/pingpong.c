#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"

int main(int argc , char * argv)
{
    if(argc != 1)
    {
        fprintf(2,"expected pingpong\n");
        exit(1);
    }

    int P2C[2],C2P[2];
    pipe(P2C);
    pipe(C2P);
    char * ch = malloc(sizeof(char));

    int pid = fork();

    if(pid == 0)
    {
        pid = getpid();
        close(P2C[1]);
        close(C2P[0]);
        if(read(P2C[0],ch,1)>0)
        {
            close(P2C[0]);
            fprintf(1,"%d: received ping\n",pid);
            write(C2P[1],"b",1);
            close(C2P[1]);
            exit(0);
        }
        else
        {
            close(P2C[0]);
            close(C2P[1]);
            fprintf(2,"child cannot receive ping\n");
            exit(1);
        }
    }
    
    else if(pid > 0)
    {
        pid = getpid();
        close(P2C[0]);
        close(C2P[1]);
        write(P2C[1],"a",1);
        
        // printf("%d\n",);
        // printf("%d\n",write(P2C[1],"a",1));
        // printf("%d\n",write(P2C[1],"a",1));
        // printf("%d\n",write(P2C[1],"a",1));   之所以后三个写入不进去，是因为第一次写完后会切换到子进程，读完之后两个读端都被关闭，因而无法写入。
        close(P2C[1]);
        if(read(C2P[0],ch,1))
        {
            close(C2P[0]);
            fprintf(1,"%d: received pong\n",pid);
            exit(0);
        }
        else
        {
            close(C2P[0]);
            fprintf(2,"parent cannot receive pong\n");
            exit(1);
        }

        wait(0);
    
        
    }
    else
    {
        fprintf(2,"fork error\n");
        exit(1);
    }

    
    exit(0);
}
    

    //single pipe version:
    // if(pid == 0)
    // {
    //     pid = getpid();
    //     read(p[0], ch , 1);
    //     printf("%c\n",*ch);
    //     fprintf(1,"%d: received ping\n",pid);
    //     write(p[1],"a" , 1);
    // }
    // else if(pid > 0)
    // {
    //     pid = getpid();
    //     //close(p[0]);
    //     write(p[1],"b",1);
    //     close(p[1]);
    //     wait(0);
    //     read(p[0],ch,1);
    //     printf("%cwhat\n",*ch);
    //     fprintf(1,"%d: received pong\n",pid);
    //     close(p[1]);
    // }
    // else
    // {
    //     fprintf(2,"fork error\n");
    //     exit(1);
    // }




