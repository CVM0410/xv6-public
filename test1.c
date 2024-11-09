#include "types.h"
#include "user.h"

int is_prime(int n);

int main() {
    printf(1, "Starting Priority Scheduler Test (100 Primes)\n");
    printf(1, "==========================================\n\n");
    
    int pid = fork();
    
    if (pid == 0) {
        // Child process waits to ensure parent starts first
        sleep(200);
        nice(getpid(), 3);  // Lower priority
        printf(1, "Child  (PID=%d, Nice=3) starting...\n", getpid());
        
        int count = 0;
        for (int i = 1; i < 200000 && count < 100; i++) {
            if (is_prime(i)) {
                // Print each prime as we find it
                printf(1, "Child  (PID=%d): prime = %d\n", getpid(), i);
                count++;
                
                // Very light computation to avoid slowing down too much
                int result = 0;
                for(int j = 0; j < 5000; j++) {
                    result += j * j;
                }
            }
        }
        printf(1, "Child  (PID=%d) finished\n", getpid());
        exit();
    } else {
        nice(getpid(), 1);  // Higher priority
        printf(1, "Parent (PID=%d, Nice=1) starting...\n", getpid());
        
        int count = 0;
        for (int i = 1; i < 200000 && count < 100; i++) {
            if (is_prime(i)) {
                // Print each prime as we find it
                printf(1, "Parent (PID=%d): prime = %d\n", getpid(), i);
                count++;
                
                // Very light computation to avoid slowing down too much
                int result = 0;
                for(int j = 0; j < 5000; j++) {
                    result += j * j;
                }
            }
        }
        printf(1, "Parent (PID=%d) finished\n", getpid());
        wait();
    }
    
    printf(1, "\n==========================================\n");
    printf(1, "Test Complete\n");
    exit();
}

int is_prime(int n) {
    if (n <= 1) return 0;
    for (int i = 2; i * i <= n; i++) {
        if (n % i == 0) return 0;
    }
    return 1;
}