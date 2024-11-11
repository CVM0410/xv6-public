#include "types.h"
#include "stat.h"
#include "user.h"

int main() {
    int pid = getpid();  // Get the current process ID
    int old_nice;

    printf(1, "\n--- Test 1: Sequential Nice Value Changes ---\n");

    printf(1, "Setting nice value of 3 for current process (PID %d)\n", pid);
    old_nice = nice(pid, 3);  // Set nice to 3
    printf(1, "Previous Nice Value: %d | New Nice Value: 3\n", old_nice);

    printf(1, "Changing nice value to 5 for current process (PID %d)\n", pid);
    old_nice = nice(pid, 5);  // Set nice to 5
    printf(1, "Previous Nice Value: %d | New Nice Value: 5\n", old_nice);

    printf(1, "Changing nice value to 2 for current process (PID %d)\n", pid);
    old_nice = nice(pid, 2);  // Set nice to 2
    printf(1, "Previous Nice Value: %d | New Nice Value: 2\n", old_nice);

    printf(1, "Test 1 completed successfully.\n");
    exit();
}
