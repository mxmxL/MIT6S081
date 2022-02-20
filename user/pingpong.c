#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char **argv)
{
    int p[2];
    if (pipe(p) != 0)
    {
        fprintf(2, "error pipe\n");
        exit(1);
    }
    char *ping = "ping";
    char *pong = "pong";
    if (fork() == 0)
    { // child
        write(p[1], ping, strlen(ping));
        char *msg = malloc(10);
        if (read(p[0], msg, 10) > 0)
        {
            fprintf(1, "%d: received %s\n", getpid(), msg);
        }else{
            fprintf(2,"error read in child");
        }
        close(p[0]);
        close(p[1]);
    }
    else
    { // parent
        char *msg = malloc(10);
        if (read(p[0], msg, 10) > 0)
        {
            fprintf(1, "%d: received %s\n", getpid(), msg);
            if (strcmp(ping,msg) == 0)
            {
                write(p[1], pong, strlen(pong));
            }
        }
        close(p[0]);
        close(p[1]);
        wait(0);
    }
    exit(0);
}
