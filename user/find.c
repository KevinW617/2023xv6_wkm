#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"

char *fmtname(char *name)
{
    static char buf[DIRSIZ+1];
    char *p;
    int space = 0;

    for (p = name + strlen(name) -1; p >= name && *p == ' '; p--)
        space++;

    memmove(buf, name, strlen(name)-space);
    buf[strlen(name)-space] = '\0';
    return buf;
}

void find(char *path, char *filename)
{
    char buf[512], *p;
    int fd;
    struct dirent de; // 目录项
    struct stat st;   // 状态

    if ((fd = open(path, O_RDONLY)) < 0)
    {
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }

    if (fstat(fd, &st) < 0)
    {
        fprintf(2, "find: cannnot stat %s\n", path);
        close(fd);
        return;
    }

    switch (st.type)
    {
    case T_DEVICE:
    case T_FILE:
        fprintf(2, "find: %s not a path\n", path);
        break;

    case T_DIR:
        if (strlen(path) + 1 + DIRSIZ + 1 > sizeof buf)
        {
            printf("find: path too long\n");
            break;
        }
        strcpy(buf, path);
        p = buf + strlen(buf);
        *p++ = '/';
        while (read(fd, &de, sizeof(de)) == sizeof(de))
        {
            if (de.inum == 0)
                    continue;
            if (strcmp(".", fmtname(de.name)) == 0 || strcmp("..", fmtname(de.name)) == 0)
                    continue;
            memmove(p, de.name, DIRSIZ);
            p[DIRSIZ] = '\0';
            if (stat(buf, &st) < 0)
            {
                printf("ls: cannot stat %s\n", buf);
                continue;
            }
            if (st.type == T_FILE || st.type == T_DEVICE)
            {
                if (strcmp(de.name, filename) == 0)
                {
                    printf("%s\n", buf);
                }
            }
            else
            {
                find(buf, filename);
            }
        }
        break;
    }
    close(fd);
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        fprintf(2, "expected 3 args\n");
        exit(1);
    }

    char *path = argv[1];
    char *filename = argv[2];

    find(path, filename);

    return 0;
}