#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

int readline(int fd, char* buffer){
    char* p=buffer;
    while (read(fd,p,1)==1)
    {
        if(*p =='\n'){
            *p=0;
            return 0;
        }
        p++;
    }
    return 1;
}

int main(int argc, char *argv[])
{
    char buf[1024];
    char *p=buf; 
    while (readline(0, buf)==0)
    {
        if(fork()==0){
            char* _argv[MAXARG];
            int cnt=0;
            _argv[cnt++]=argv[1];
            for(int i=2;i<argc;i++){
                _argv[cnt++]=argv[i];
            }
            char* arg=0;
            int idx=0;
            while (*p)
            {
                if(*p == ' '){
                    arg[idx]=0;
                    _argv[cnt++]=arg;
                    arg=0;
                    idx=0;
                }
                if(!arg){
                    arg=malloc(sizeof(char)*64);
                }
                arg[idx++]=*p;
                p++;
            }
            if(idx>0){
                _argv[cnt++]=arg;
            }
            exec(argv[1], _argv);
        }else{
            wait(0);
        }
    }
    
    exit(0);
}
