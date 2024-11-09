// test1.c - Test nice system call including edge cases
#include "types.h"
#include "stat.h"
#include "user.h"

int main() {
    printf(1, "\nTesting nice system call...\n");
    printf(1, "================================\n");

    int pid = getpid();

    // Test Case 1: Normal usage
    printf(1, "\nTest 1: Normal usage\n");
    printf(1, "Setting nice value 2 for current process\n");
    int oldnice = nice(pid, 2);
    printf(1, "%d %d\n", pid, oldnice);

    // Test Case 2: Edge case - value too low
    printf(1, "\nTest 2: Value too low (0)\n");
    oldnice = nice(pid, 0);
    if(oldnice < 0) {
        printf(1, "Error: Nice value must be between 1 and 5\n");
    }

    // Test Case 3: Edge case - value too high
    printf(1, "\nTest 3: Value too high (6)\n");
    oldnice = nice(pid, 6);
    if(oldnice < 0) {
        printf(1, "Error: Nice value must be between 1 and 5\n");
    }

    // Test Case 4: Invalid PID
    printf(1, "\nTest 4: Invalid PID\n");
    oldnice = nice(999, 3);
    if(oldnice < 0) {
        printf(1, "Error: Process not found\n");
    }

    // Test Case 5: Change back to default
    printf(1, "\nTest 5: Change back to default (3)\n");
    oldnice = nice(pid, 3);
    printf(1, "%d %d\n", pid, oldnice);

    printf(1, "\n================================\n");
    printf(1, "Test Complete\n");
    exit();
}