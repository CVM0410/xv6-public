// test2.c - Modified version for clearer output
#include "types.h"
#include "user.h"

int is_prime(int n) {
    if (n <= 1) return 0;
    for (int i = 2; i * i <= n; i++) {
        if (n % i == 0) return 0;
    }
    return 1;
}

int main() {
    printf(1, "\nMulti-Process Priority Test (4 Processes)\n");
    printf(1, "=======================================\n\n");
    
    // Create 4 children with different priorities
    for(int i = 1; i <= 4; i++) {
        int pid = fork();
        if(pid == 0) {
            sleep(100 * i);  // Stagger the starts
            nice(getpid(), i);  // Priorities 1,2,3,4
            printf(1, "\nProcess %d (PID=%d, Nice=%d) starting...\n", 
                   i, getpid(), i);
            
            int count = 0;
            for(int n = 1; n < 100000 && count < 20; n++) {
                if(is_prime(n)) {
                    printf(1, "Process %d (PID=%d, Nice=%d): prime=%d\n", 
                           i, getpid(), i, n);
                    count++;
                    
                    // Lighter computation
                    int result = 0;
                    for(int j = 0; j < 1000; j++) {
                        result += j * j;
                    }
                }
            }
            printf(1, "\nProcess %d (PID=%d, Nice=%d) finished\n", 
                   i, getpid(), i);
            exit();
        }
    }
    
    // Parent waits for all children
    for(int i = 0; i < 4; i++) {
        wait();
    }
    
    printf(1, "\n=======================================\n");
    printf(1, "Test Complete\n\n");
    exit();
}