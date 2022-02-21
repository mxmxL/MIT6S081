#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"


int main(int argc, char **argv)
{
    if (argc < 2)
    {
        fprintf(2, "usage: trace <sys call mask> <prog> ...\n");
        exit(1);
    }
    int mask = atoi(argv[1]);
    int i;
    for (i = 0; i < argc - 2; i++)
    {
        argv[i] = argv[i + 2];
    }
    argv[i] = 0;
    trace(mask);
    exec(argv[0], argv);
    exit(0);
}
