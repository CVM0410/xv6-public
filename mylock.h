// mylock.h
#ifndef MYLOCK_H
#define MYLOCK_H

#define MAX_LOCKS 8

struct mylock_data {
    int id;                // Lock ID (0-7)
    int held;             // Is lock held?
    struct proc *owner;    // Current owner process
    int orig_priority;     // Original priority before inheritance
};

extern struct mylock_data mylocks[MAX_LOCKS];
void init_mylocks(void);
int acquire_mylock(int);
int release_mylock(int);

#endif