#include "types.h"
#include "stat.h"
#include "user.h"

int main() {
    int result;

    printf(1, "\n--- Test 2: Edge Cases ---\n");

    // Test with invalid PID
    printf(1, "\nTesting with invalid PID (non-existent PID 999)\n");
    result = nice(999, 3);  // Test with non-existent PID
    if (result == -1) {
        printf(1, "PASS: Invalid PID handled correctly (PID 999)\n");
    } else {
        printf(1, "FAIL: Invalid PID handling failed (PID 999)\n");
    }

    // Test with out-of-range nice values
    printf(1, "\nTesting with out-of-range nice values\n");

    result = nice(getpid(), 6);  // High out-of-range value
    if (result == -1) {
        printf(1, "PASS: Out-of-range high nice value handled correctly (Value 6)\n");
    } else {
        printf(1, "FAIL: Out-of-range high nice value handling failed (Value 6)\n");
    }

    result = nice(getpid(), 0);  // Low out-of-range value
    if (result == -1) {
        printf(1, "PASS: Out-of-range low nice value handled correctly (Value 0)\n");
    } else {
        printf(1, "FAIL: Out-of-range low nice value handling failed (Value 0)\n");
    }

    printf(1, "Test 2 completed.\n");
    exit();
}
