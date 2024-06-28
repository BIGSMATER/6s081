#include "types.h"
#include "riscv.h"
#include "param.h"
#include "defs.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"
#include "sysinfo.h"
uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  argaddr(0, &p);
  return wait(p);
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int n;

  argint(0, &n);
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;


  argint(0, &n);
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(killed(myproc())){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}


#ifdef LAB_PGTBL
int
sys_pgaccess(void)
{
  // lab pgtbl: your code here.
  uint64 va,dst;
  int n;
  argint(1, &n);
  argaddr(0, &va);
  argaddr(2, &dst);
  if( n< 0 || va< 0 || dst< 0||n>32)
  {
    return -1;
  }
  if(va>=MAXVA)
  {
    return -1;
  }
  
  struct proc *p = myproc();
  pte_t *pte;
  uint32 bitmask=0;
  for(int i=0;i<n;i++)
  {
    pte=walk(p->pagetable,va,0);
    if(pte)
    {
      if(*pte&PTE_A)
      {
        bitmask|=(1<<i);
      }
      *pte&=~PTE_A;
    }
    va+=PGSIZE;
  }
  if(copyout(p->pagetable,dst,(char *)&bitmask,sizeof(bitmask)) < 0)
    return -1;
  return 0;
}
#endif

uint64
sys_kill(void)
{
  int pid;

  argint(0, &pid);
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}
uint64 sys_trace(void)
{
  int n;
  argint(0,&n);
  if(n<0)
    return -1;
  myproc()->tracemask=n;
  return 0;
}
uint64 sys_sysinfo(void)
{
  struct sysinfo st;
  struct proc* p=myproc();
  uint64 addr;
  argaddr(0,&addr);
  if(addr<0)
  {
		return -1;
	}

  st.freemem=getfreememnum();
  st.nproc=getprocnum();
	if(copyout(p->pagetable, addr, (char *)&st, sizeof(st))<0){
		return -1;
	}
	return 0;
}