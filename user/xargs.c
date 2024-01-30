#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"

char **cpy(int argc, char *argv[])
{
    char **Argv = (char **)malloc((argc + 20) * sizeof(char *)); // 复制argv并留空
    int j = 0;
    for (j = 0; j < argc; j++)
    {
        Argv[j] = (char *)malloc((strlen(argv[j]) + 1) * sizeof(char));
        strcpy(Argv[j], argv[j]);
    }
    free(Argv[0]);
    return (++Argv);
}

void Free(int argc, char **Argv)
{
    for (int i = 0; i < argc; i++)
    {
        free(Argv[i]);
    }
    free(Argv);
}

int main(int argc, char *argv[])
{
    char *ch = (char *)malloc(sizeof(char)); // 一行一行读添加到Argv中，然后执行
    char buf[500];

    //printf("ok1\n");

    char *program_name = (char *)malloc(sizeof(char) * (strlen(argv[1]) + 1));
    strcpy(program_name, argv[1]);
    program_name[strlen(argv[1])] = '\0';

    for (int i = 0; i < 500 && read(0, ch, 1); ++i)
    {
        buf[i] = *ch;
    }
    int i = 0;
    int count = 0;
    int spacecount = 0;
    int existArgv = 1;

    char **Argv = cpy(argc, argv);

    //printf("ok2\n");

    // for (i = 0; buf[i] != '\0'; i++)
    // {
    //     printf("%c\n", buf[i]);
    // }

    //printf("value i:%d\n", i);

    i = 0;

    int Argc = argc;

    while (buf[i] != '\0')
    {
        //printf("in while loop\n");
        if (buf[i] != '\n')
        {
            //printf("ok4\n");
            if (buf[i] != ' ')
            {
                count++;
                spacecount = 0;
            }
            else if (spacecount == 0)
            {
                if (existArgv == 0)
                {
                    //printf("ok3\n");

                    char **Argv = cpy(argc, argv);
                    existArgv = 1;
                    Argv[argc - 1] = (char *)malloc((count + 1) * sizeof(char));
                    memcpy(Argv[argc - 1], &buf[i - count], count);
                    Argv[argc - 1][count] = '\0';
                    spacecount++;
                    argc++;
                    count = 0;
                }
                //printf("arrived\n");
                Argv[argc - 1] = (char *)malloc((count + 1) * sizeof(char));
                //printf("11\n");

                memcpy(Argv[argc - 1], &buf[i - count], count);

                // for (int j = 0; j < argc; j++)
                // {
                //     printf("Argv[%d]: %s\n", j, Argv[j]);
                // }
                //printf("22\n");
                Argv[argc - 1][count] = '\0';
                //printf("33\n");
                argc++;
                spacecount++;
                count = 0;
                //printf("ok what\n");
            }
            i++;
        }
        else
        {
            if (existArgv == 0)
            {
                char **Argv = cpy(argc, argv);
                existArgv = 1;
                Argv[argc - 1] = (char *)malloc((count + 1) * sizeof(char));
                memcpy(Argv[argc - 1], &buf[i - count], count);
                Argv[argc - 1][count] = '\0';
                argc++;
            }
            else
            {
                Argv[argc - 1] = (char *)malloc((count + 1) * sizeof(char));
                memcpy(Argv[argc - 1], &buf[i - count], count);
                Argv[argc - 1][count] = '\0';
                argc++;
            }

            // for (int j = 0; j < argc; j++)
            //     {
            //         printf("Argv[%d]: %s\n", j, Argv[j]);
            //     }

            //printf("ok\n");
            if (fork() == 0)
            {
                exec(program_name, Argv);
            }
            //printf("execfail?\n");
            wait(0);
            //printf("aa\n");
            //Free(Argc + 20 - 1, Argv);
            //printf("bb\n");
            existArgv = 0;
            argc = Argc;
            i++;
            //printf("cc\n");
        }
    }

    return 0;
}