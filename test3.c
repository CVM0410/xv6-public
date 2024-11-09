// test3.c - CPU intensive computation with different priorities
#include "types.h"
#include "user.h"

void matrix_multiply(int size) {
    int result = 0;
    for(int i = 0; i < size; i++) {
        for(int j = 0; j < size; j++) {
            result += (i * j) % 17;
        }
    }
}

int main() {
    printf(1, "\nCPU-Intensive Priority Test\n");
    printf(1, "==========================\n\n");
    
    int pid1 = fork();
    if(pid1 == 0) {
        sleep(100);  // Wait for parent to start
        nice(getpid(), 4);  // Lowest priority
        printf(1, "Low Priority    (PID=%d, Nice=4) starting...\n", getpid());
        for(int i = 0; i < 10; i++) {
            matrix_multiply(100);
            printf(1, "Low Priority    (PID=%d): Completed iteration %d\n", 
                   getpid(), i+1);
        }
        exit();
    }
    
    int pid2 = fork();
    if(pid2 == 0) {
        sleep(50);  // Wait for parent to start
        nice(getpid(), 2);  // Medium priority
        printf(1, "Medium Priority (PID=%d, Nice=2) starting...\n", getpid());
        for(int i = 0; i < 10; i++) {
            matrix_multiply(100);
            printf(1, "Medium Priority (PID=%d): Completed iteration %d\n", 
                   getpid(), i+1);
        }
        exit();
    }
    
    // Parent process
    nice(getpid(), 1);  // Highest priority
    printf(1, "High Priority  (PID=%d, Nice=1) starting...\n", getpid());
    for(int i = 0; i < 10; i++) {
        matrix_multiply(100);
        printf(1, "High Priority  (PID=%d): Completed iteration %d\n", 
               getpid(), i+1);
    }
    
    wait();
    wait();
    
    printf(1, "\n==========================\n");
    printf(1, "Test Complete\n\n");
    exit();
}