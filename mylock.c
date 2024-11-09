#include "types.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "x86.h"
#include "proc.h"
#include "spinlock.h"
#include "mylock.h"

struct mylock_data mylocks[MAX_LOCKS];
struct spinlock mylock_table_lock;

void
init_mylocks(void)
{
    initlock(&mylock_table_lock, "mylocktable");
    for(int i = 0; i < MAX_LOCKS; i++) {
        mylocks[i].id = i;
        mylocks[i].held = 0;
        mylocks[i].owner = 0;
        mylocks[i].orig_priority = 0;
    }
}

int
acquire_mylock(int id)
{
    if(id < 0 || id >= MAX_LOCKS)
        return -1;

    acquire(&mylock_table_lock);
    struct proc *p = myproc();
    
    while(mylocks[id].held) {
        // Priority inheritance
        if(mylocks[id].owner && p->nice < mylocks[id].owner->nice) {
            mylocks[id].orig_priority = mylocks[id].owner->nice;
            mylocks[id].owner->nice = p->nice;
            cprintf("Priority inherited: Process %d nice changed from %d to %d\n",
                   mylocks[id].owner->pid, mylocks[id].orig_priority, p->nice);
        }
        sleep(&mylocks[id], &mylock_table_lock);
    }

    mylocks[id].held = 1;
    mylocks[id].owner = p;
    
    release(&mylock_table_lock);
    return 0;
}

int
release_mylock(int id)
{
    if(id < 0 || id >= MAX_LOCKS)
        return -1;

    acquire(&mylock_table_lock);
    struct proc *p = myproc();

    if(!mylocks[id].held || mylocks[id].owner != p) {
        release(&mylock_table_lock);
        return -1;
    }

    // Restore original priority if it was inherited
    if(mylocks[id].orig_priority) {
        p->nice = mylocks[id].orig_priority;
        cprintf("Priority restored: Process %d nice reset to %d\n",
               p->pid, p->nice);
        mylocks[id].orig_priority = 0;
    }

    mylocks[id].held = 0;
    mylocks[id].owner = 0;
    wakeup(&mylocks[id]);
    
    release(&mylock_table_lock);
    return 0;
}

// System call implementations
int
sys_lock(void)
{
    int id;
    if(argint(0, &id) < 0)
        return -1;
    return acquire_mylock(id);
}

int
sys_release(void)
{
    int id;
    if(argint(0, &id) < 0)
        return -1;
    return release_mylock(id);
}