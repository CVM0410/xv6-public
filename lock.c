#include "types.h"
#include "stat.h"
#include "user.h"

void test_basic_lock() {
    printf(1, "\nTest Case 1: Basic Lock Functionality\n");
    printf(1, "=====================================\n");
    
    int pid = fork();
    if(pid == 0) {
        sleep(100);  // Wait for parent to get lock
        printf(1, "Child  (PID=%d): Attempting to acquire lock 1\n", getpid());
        if(lock(1) < 0) {
            printf(1, "Child: Failed to acquire lock\n");
            exit();
        }
        printf(1, "Child  (PID=%d): Acquired lock 1\n", getpid());
        sleep(50);
        printf(1, "Child  (PID=%d): Releasing lock 1\n", getpid());
        release(1);
        exit();
    } else {
        printf(1, "Parent (PID=%d): Attempting to acquire lock 1\n", getpid());
        if(lock(1) < 0) {
            printf(1, "Parent: Failed to acquire lock\n");
            exit();
        }
        printf(1, "Parent (PID=%d): Acquired lock 1\n", getpid());
        sleep(200);
        printf(1, "Parent (PID=%d): Releasing lock 1\n", getpid());
        release(1);
        wait();
    }
}

void test_priority_inversion() {
    printf(1, "\nTest Case 2: Priority Inversion\n");
    printf(1, "============================\n");
    
    int pid = fork();
    if(pid == 0) {
        // Low priority process
        nice(getpid(), 5);
        printf(1, "Time 0: Low Priority Process (PID=%d, Nice=5) starting\n", getpid());
        if(lock(2) >= 0) {
            printf(1, "Time 1: Low Priority Process: Acquired lock 2\n");
            printf(1, "Time 1: Low Priority Process: Computing...\n");
            // Hold lock and do significant work
            volatile int result = 0;
            for(int i = 0; i < 100000000; i++) {
                result += i * i;
            }
            printf(1, "Time 8: Low Priority Process: Computation complete\n");
            printf(1, "Time 8: Low Priority Process: Releasing lock 2\n");
            release(2);
        }
        exit();
    } else {
        sleep(50);  // Ensure low priority process gets lock first
        int high_pid = fork();
        if(high_pid == 0) {
            // High priority process
            nice(getpid(), 1);
            printf(1, "Time 3: High Priority Process (PID=%d, Nice=1) starting\n", getpid());
            printf(1, "Time 3: High Priority Process waiting for lock 2\n");
            if(lock(2) >= 0) {
                printf(1, "Time 8: High Priority Process: Acquired lock 2\n");
                printf(1, "Time 8: High Priority Process: Releasing lock 2\n");
                release(2);
            }
            exit();
        }
        wait();
        wait();
    }
}

void test_priority_inheritance() {
    printf(1, "\nTest Case 3: Priority Inheritance\n");
    printf(1, "============================\n");
    
    int pid = fork();
    if(pid == 0) {
        // Low priority process
        nice(getpid(), 5);
        printf(1, "Time 0: Low Priority Process (PID=%d, Nice=5) starting\n", getpid());
        if(lock(3) >= 0) {
            printf(1, "Time 1: Low Priority Process: Acquired lock 3\n");
            printf(1, "Time 1: Low Priority Process: Computing...\n");
            // Hold lock and do work
            volatile int result = 0;
            for(int i = 0; i < 100000000; i++) {
                result += i * i;
            }
            printf(1, "Time 6: Low Priority Process: Computation complete\n");
            printf(1, "Time 6: Low Priority Process: Releasing lock 3\n");
            release(3);
        }
        exit();
    } else {
        sleep(50);
        int high_pid = fork();
        if(high_pid == 0) {
            // High priority process
            nice(getpid(), 1);
            printf(1, "Time 3: High Priority Process (PID=%d, Nice=1) starting\n", getpid());
            printf(1, "Time 3: High Priority Process waiting for lock 3\n");
            if(lock(3) >= 0) {
                printf(1, "Time 6: High Priority Process: Acquired lock 3\n");
                printf(1, "Time 6: High Priority Process: Releasing lock 3\n");
                release(3);
            }
            exit();
        }
        wait();
        wait();
    }
}

int main() {
    printf(1, "Starting Priority Inheritance Tests\n");
    printf(1, "==================================\n");
    
    test_basic_lock();
    test_priority_inversion();
    test_priority_inheritance();
    
    printf(1, "\nAll tests completed\n");
    exit();
}