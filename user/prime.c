#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
void func(int *p1)
{
    int prime,num;
    close(p1[1]);
    if(read(p1[0],&prime,4)!=0)
    { 
        int p2[2];
        pipe(p2);
        if(fork()>0)
        {
            printf("prime %d\n",prime);
            close(p2[0]);
            while (read(p1[0],&num,4)!=0)
            {
                if(num%prime!=0)
                {
                    write(p2[1],&num,4);
                }
            }
            close(p2[1]);
            wait(0);
        }
        else
        {
            func(p2);
        }
    }
    else
    {
        exit(0);    
    }
    exit(0);
}
int main()
{
    int p1[2];
    pipe(p1);

    if(fork()>0)
    {
        for(int i=2;i<=35;i++)
        {
            write(p1[1],&i,4);
        }
        close(p1[1]);
        wait(0);
    }
    else
    {
        func(p1);
    }
    exit(0);
}