#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void panic(const char *msg)
{
    fprintf(2, "%s\n", msg);
    exit(1);
}

char *
fmtname(char *path)
{
    char *p;
    for (p = path + strlen(path); p >= path && *p != '/'; p--)
        ;
    p++;
    return p;
}

void find(char *path, char *file)
{
    char buf[512], *p;
    int fd;
    if ((fd = open(path, 0)) < 0)
    {
        panic("error open");
    }
    struct stat st;
    if (fstat(fd, &st) < 0)
    {
        close(fd);
        panic("error fstat");
    }
    struct dirent de;
    switch (st.type)
    {
    case T_FILE:
        if (strcmp(fmtname(path), file) == 0)
        {
            fprintf(1, "%s\n", path);
        }
        break;
    case T_DIR:
        strcpy(buf, path);
        p = buf + strlen(buf);
        *p++ = '/';

        while (read(fd, &de, sizeof(de)) == sizeof(de))
        {
            if (de.inum == 0)
                continue;
            if (strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0)
            {
                continue;
            }
            memmove(p, de.name, DIRSIZ);
            p[DIRSIZ] = 0;
            find(buf, file);
        }
        break;
    }
    close(fd);
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        fprintf(1, "usage: find <path> <file>\n");
        exit(0);
    }

    find(argv[1], argv[2]);
    exit(0);
}
