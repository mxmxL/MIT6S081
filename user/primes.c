#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int writeint(int fd, int value)
{
    char buffer[4];
    buffer[0] = 0xff & value;
    buffer[1] = 0xff & (value >> 8);
    buffer[2] = 0xff & (value >> 16);
    buffer[3] = 0xff & (value >> 24);
    return !(write(fd, buffer, 4) == 4); // 0 for sucess
}

int readint(int fd, int *value)
{
    char buffer[4];
    if (read(fd, buffer, 4) != 4)
    {
        return 1;
    }

    *value = 0;
    *value |= buffer[0];
    *value |= (((int)(buffer[1])) << 8);
    *value |= (((int)(buffer[2])) << 16);
    *value |= (((int)(buffer[3])) << 24);

    return 0;
}

int isprimes(int value)
{
    if (value == 0 || value == 1)
    {
        return 1;
    }
    for (int i = 2; i <= (value >> 1); i++)
    {
        if (value % i == 0)
        {
            return 0;
        }
    }
    return 1;
}

int nextprime(int cur)
{
    while (!isprimes(++cur))
        ;

    return cur;
}

void panic(const char *msg)
{
    fprintf(2, "%s\n", msg);
    exit(1);
}

int main(int argc, char **argv)
{
    int i = 2;

    while (1)
    {
        if (i > 35)
        {
            exit(0);
        }
        fprintf(1, "prime %d\n", i);
        int p[2];
        if (pipe(p) != 0)
        {
            panic("error pipe");
        }
        int pid;
        if ((pid = fork()) == 0)
        { // child
            if (readint(p[0], &i))
            {
                close(p[0]);
                close(p[1]);
                panic("error readint");
            }
            close(p[0]);
            close(p[1]);
        }
        else if (pid < 0)
        {
            panic("error fork");
        }
        else
        {
            if (writeint(p[1], nextprime(i)))
            {
                close(p[0]);
                close(p[1]);
                panic("error writeint");
            }
            close(p[0]);
            close(p[1]);
            wait(0);
            exit(0);
        }
    }

    exit(0);
}
