#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
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
  if(n < 0)
    n = 0;
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


//makefile
//user/user.h
//user/usys.pl
//kernel/syscall.h
//pridat sys_trace/sys_info do kernel/sysproc.c


//uprav fork() v kernel/proc.c
//uprav syscall() kernel/syscall.c

// trace
uint64
sys_trace(void){
  int n;

  // Extract the integer argument from the system call using argint.
  // Argument 0 refers to the first argument passed to the system call.
  argint(0, &n);

  // Set the trace mask for the current process.
  // The trace mask is a way to enable or disable tracing for specific system calls.
  myproc()->mask = n;

  // Return 0 to indicate that the system call has completed successfully.
  return 0;
}



//cal_freemem v kernel/kalloc.c
//cal_nproc v kernel/proc.c
//pridat do defs.h

uint64
sys_sysinfo(void){
  // Declare a struct to store system information, and a variable to hold the user-provided pointer.
  struct sysinfo kinfo;
  uint64 info;

  // Extract the user-provided pointer (argument 0) from the system call.
  argaddr(0, &info);

  // Calculate and populate system information into the 'kinfo' structure.
  // 1. Calculate the amount of free memory in the system using 'cal_freemem'.
  kinfo.freemem = cal_freemem();
  
  // 2. Calculate the number of active processes using 'cal_nproc'.
  kinfo.nproc = cal_nproc();
  
  // Copy the 'kinfo' structure to the user-provided memory using 'copyout'.
  // 'copyout' ensures that the kernel can safely copy data to the user's address space.
  if (copyout(myproc()->pagetable, (uint64)info, (char*)&kinfo, sizeof(struct sysinfo)) < 0) {
    // If the copy operation fails, return an error code (-1).
    return -1;
  }

  // Return 0 to indicate that the system call has completed successfully.
  return 0;
}


