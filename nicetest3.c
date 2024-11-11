#include "types.h"
#include "stat.h"
#include "user.h"

int main() {
    int pid = getpid();

    printf(1, "\n--- Test 3: Boundary Conditions ---\n");

    // Set nice to minimum boundary value
    printf(1, "Setting nice value to minimum boundary (1) for current process (PID %d)\n", pid);
    int old_nice = nice(pid, 1);
    printf(1, "Previous Nice Value: %d | New Nice Value: 1\n", old_nice);

    // Set nice to maximum boundary value
    printf(1, "Setting nice value to maximum boundary (5) for current process (PID %d)\n", pid);
    old_nice = nice(pid, 5);
    printf(1, "Previous Nice Value: %d | New Nice Value: 5\n", old_nice);

    printf(1, "Test 3 completed successfully.\n");
    exit();
}
