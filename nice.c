#include "types.h"
#include "stat.h"
#include "user.h"

int
main(int argc, char *argv[])
{
    int pid, value;
    
    // Check argument count
    if(argc != 2 && argc != 3) {
        printf(2, "Usage: nice <pid> <value> or nice <value>\n");
        exit();
    }
    
    // Parse arguments based on count
    if(argc == 2) {
        // Single argument case: nice <value>
        value = atoi(argv[1]);
        pid = getpid();  // Get current process ID
    } else {
        // Two argument case: nice <pid> <value>
        pid = atoi(argv[1]);
        value = atoi(argv[2]);
    }
    
    // Validate nice value (1-5 levels of priority)
    if(value < 1 || value > 5) {
        printf(2, "Error: Nice value must be between 1 and 5\n");
        exit();
    }
    
    // Call nice system call and print result
    // Format: <pid> <oldnice>
    int oldnice = nice(pid, value);
    printf(1, "%d %d\n", pid, oldnice);
    
    exit();
}