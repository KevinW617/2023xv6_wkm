#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"

void primes(int *nums , int size)
{
    fprintf(1,"prime %d\n",nums[0]);

    if(size == 1)
    {
        exit(0);
    }
    
    int p[2];
    pipe(p);
    int pid = fork();
   
    


    
    if(pid == 0)
    {
        sleep(2);
        int * newNums = malloc(size*sizeof(int));
        int newSize = 0;
        pid = getpid();
        printf("hello\n");
        close(p[1]);//关闭写端
        printf("1return value of read: %d\n",read(p[0] , newNums , sizeof(int)));
        printf("bye\n");
          

        int * tmp = newNums;

        printf("2return value of read: %d\n",read(p[0] , newNums , sizeof(int)));
        while(read(p[0] , tmp , sizeof(int)) == sizeof(int))
        {
            newSize++;
            tmp++;
        }
        //printf("%d get %d nums\n",pid , newSize);
        close(p[0]); //关闭读端
        primes(newNums , newSize);
        free(newNums);
        //printf("%d ready to exit\n",pid);
        exit(0);
    }
    else if(pid > 0)
    {
            int i = 1;
            

            close(p[0]);//读端关闭

            printf("1return value of write: %d\n",write(p[1] , &i , sizeof(int)));//return 4

            //printf("2return value of write: %d\n",write(p[1] , &i , sizeof(int)));//return -1
            //printf("p[1]: %d\n", p[1]);
            for(int i = 1 ; i < size ; i++)
            {
                if(nums[i] % nums[0] != 0)
                {
                    
                    printf("return value of write: %d\n",write(p[1] , &nums[i] , sizeof(int)));
                    printf("%d ok\n",nums[i]);
                }
            }

            close(p[1]);     //关闭写端

            wait(0);
            
    }
    else
    {
        fprintf(2,"fork error\n");
        exit(1);
    }

    exit(0);
    
}

int main(int argc,char * argv[])
{
    int nums[34];
    for(int i = 0 ; i < 34 ; ++i)
    {
        nums[i] = i+2;
    }
    primes(nums , 34);
    exit(0);
}