#include "types.h"
#include "stat.h"
#include "user.h"

int
main(int argc, char *argv[])
{
    int pid, value;
    
    if(argc != 2 && argc != 3) {
        printf(1, "Usage: nice <pid> <value> or nice <value>\n");
        exit();
    }
    
    if(argc == 2) {
        // nice <value>
        value = atoi(argv[1]);
        pid = getpid();  // Get current process ID
    } else {
        // nice <pid> <value>
        pid = atoi(argv[1]);
        value = atoi(argv[2]);
    }
    
    // Validate nice value (1-5)
    if(value < 1 || value > 5) {
        printf(1, "Error: Nice value must be between 1 and 5\n");
        exit();
    }
    
    // Call nice system call
    int oldnice = nice(pid, value);
    
    // Check for invalid PID
    if(oldnice == -1) {
        printf(1, "Error: Invalid PID\n");
    } else {
        // Print pid and oldnice as specified
        printf(1, "%d %d\n", pid, oldnice);
    }
    
    exit();
}
