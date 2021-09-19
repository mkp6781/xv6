#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"
#include "syscall.h"

uint64
sys_exit(void)
{
  int n;
  if(argint(0, &n) < 0)
    return -1;
  exit(n);
  if(myproc()->tracer[SYS_exit])
   printf("exit: %d\n", n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  if(myproc()->tracer[SYS_getpid])
   printf("getpid: No Arguments.\n");
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  if(myproc()->tracer[SYS_fork])
   printf("fork: No Arguments.\n");
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  if(argaddr(0, &p) < 0)
    return -1;
  if(myproc()->tracer[SYS_wait])
   printf("wait: %p\n", p);
  return wait(p);
}

uint64
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  myproc()->sz+=n;
  if(n<0){
    uvmdealloc(myproc()->pagetable, addr, myproc()->sz);
  }
  //if(growproc(n) < 0)
  //  return -1;

  if(myproc()->tracer[SYS_sbrk])
   printf("sbrk: %d\n", n);
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  printf("<from kernel> Sleeping for %d seconds\n", n);
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  if(myproc()->tracer[SYS_sleep])
   printf("sleep: %d\n", n);
  return 0;
}

uint64
sys_echo_simple(void){
 char buffer[MAXPATH];
 
 if(argstr(0, buffer, MAXPATH) < 0)
  return -1;
 if(myproc()->tracer[SYS_echo_simple])
  printf("echo_simple: %p\n", buffer);
 printf("%s\n", buffer);
 return 0;
}

uint64
sys_echo_kernel(void){
 uint64 p;
 int n;
 int i;

 if(argint(0, &n) < 0)
  return -1;
 if(argaddr(1, &p) < 0)
  return -1;
 for(i=1;i<n;++i){
  uint64 buf;
  char buffer[MAXPATH];

  if(fetchaddr(p+(i*sizeof(p)), &buf) < 0)
   return -1;
  if(fetchstr(buf, buffer, MAXPATH) < 0)
   return -1;
  if(i==n-1)
   printf("%s\n", buffer);
  else
   printf("%s ", buffer);
 }
 if(myproc()->tracer[SYS_echo_kernel])
  printf("echo_kernel: %d, %p\n", n, p);
 return 0;
}

uint64
sys_trace(void){
 int i;
 int n;

 if(argint(0, &n) < 0)
  return -1;
 for(i=0;i<32;++i){
  if((1<<i) & n){
   myproc()->tracer[i] = 1;  
  }
 }
 if(myproc()->tracer[SYS_trace])
  printf("trace: %d\n", n);
 return 0;
}

uint64
sys_get_process_info(void){
 uint64 procinfo;
 if(argaddr(0, &procinfo) < 0){
  return -1;
 }
 printf("<from kernel> Getting process info..");
 return getprocessinfo(procinfo);
}

uint64
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  if(myproc()->tracer[SYS_kill])
   printf("kill: %d\n", pid);
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
  if(myproc()->tracer[SYS_uptime])
   printf("uptime: No Arguments.\n");
  return xticks;
}
