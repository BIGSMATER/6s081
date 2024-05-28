#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc,char* argv[])
{
    int p[2],pid;
    if(pipe(p) < 0)
    {
        printf("pipe error\n");
        exit(1);
    }
    if((pid = fork()) < 0)
    {
        printf("fork error\n");
        exit(1);
    }
    else if(pid == 0)
    {
        char buf;
        close(p[0]);
        read(p[1],&buf,1);
        printf("%d: received ping\n",getpid(),buf);
        write(p[1],&buf,1);
        close(p[1]);
        exit(0);
    }
    char buf = 'a';
    close(p[1]);
    write(p[0],&buf,1);
    read(p[0],&buf,1);
    printf("%d: received pong\n",getpid(),buf);
    close(p[0]);
    exit(0);

}