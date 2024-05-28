#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
int main(int argc,char* argv[])
{
    if(argc < 3)
    {
          fprintf(2,"xargs <function> <args>\n");
          exit(1);
    }
    char* newargv[100]={0};
    int i;
    for(i=0;argv[i+1]!=0;i++)
    {
        newargv[i]=argv[i+1];
    }
    // printf("%s\n",newargv[0]);
    char buf[100];
    char c,*p=buf;
    while(read(0,&c,1)!=0)
    {
        if(c=='\n')
        {
            // printf("1\n");
            *p=0;
            newargv[i]=buf;
            // i++;
            if(fork()==0)
            {
                // printf("%s\n",newargv[0]);
                exec(newargv[0],newargv);
                exit(0);
            }
            wait(0);
            p=buf;
        }
        else
        {
            // printf("2");
            *p=c;
            p++;
        }
    }
    exit(0);
}