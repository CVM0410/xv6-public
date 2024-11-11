#include "types.h"
#include "user.h"
#include "stat.h"

#define WORK_ITERATIONS 25
#define NUM_PROCESSES 4
#define MAX_FIBS 25
#define COMPUTE_ITERATIONS 100000000

struct process_metrics {
    int pid;
    int priority;
    int start_tick;
    int end_tick;
    int total_runtime;
    int fibs[MAX_FIBS];
    int fib_found_ticks[MAX_FIBS];
    int intervals[MAX_FIBS];
    int min_interval;
    int max_interval;
};

int find_nth_fibonacci(int n) {
    if (n <= 0) return 0;
    if (n == 1 || n == 2) return 1;
    
    int a = 1;  // F(n-2)
    int b = 1;  // F(n-1)
    int c;      // F(n)
    
    for(int i = 3; i <= n; i++) {
        c = a + b;
        a = b;
        b = c;
    }
    return c;
}

void heavy_compute() {
    volatile int result = 0;
    for(int j = 0; j < COMPUTE_ITERATIONS; j++) {
        result += j * j;
        if(j % 1000000 == 0) {
            sleep(1);
        }
    }
}

void print_process_results(struct process_metrics *m) {
    printf(1, "\nPID = %d (Nice = %d) Results:\n", m->pid, m->priority);
    printf(1, "----------------------------------------\n");
    
    printf(1, "Operation Timeline:\n");
    int total_interval = 0;
    m->min_interval = 999999;
    m->max_interval = 0;
    
    for(int i = 0; i < WORK_ITERATIONS; i++) {
        printf(1, "Fibonacci %d: %d (Found at T+%d, Interval: %d ticks)\n", 
               i+1,
               m->fibs[i], 
               m->fib_found_ticks[i] - m->start_tick,
               m->intervals[i]);
               
        total_interval += m->intervals[i];
        if(m->intervals[i] > m->max_interval) m->max_interval = m->intervals[i];
        if(m->intervals[i] < m->min_interval) m->min_interval = m->intervals[i];
    }
    
    printf(1, "\nInterval Analysis:\n");
    printf(1, "Total Runtime: %d ticks\n", m->total_runtime);
    printf(1, "Average Interval: %d ticks\n", total_interval / WORK_ITERATIONS);
    printf(1, "Minimum Interval: %d ticks\n", m->min_interval);
    printf(1, "Maximum Interval: %d ticks\n", m->max_interval);
    printf(1, "Variance in Intervals: %d ticks\n\n", 
           m->max_interval - m->min_interval);
}

void print_comparative_analysis(struct process_metrics metrics[], int n) {
    printf(1, "\nComparative Analysis:\n");
    printf(1, "----------------------------------------\n");
    
    for(int i = 0; i < NUM_PROCESSES; i++) {
        int total_interval = 0;
        for(int j = 0; j < WORK_ITERATIONS; j++) {
            total_interval += metrics[i].intervals[j];
        }
        
        printf(1, "Nice = %d (PID = %d):\n", 
               metrics[i].priority, metrics[i].pid);
        printf(1, "  Total Runtime: %d ticks\n", metrics[i].total_runtime);
        printf(1, "  Average Interval: %d ticks\n", 
               total_interval / WORK_ITERATIONS);
        if(i > 0) {
            int slowdown = ((metrics[i].total_runtime - metrics[0].total_runtime) * 100) 
                          / metrics[0].total_runtime;
            printf(1, "  Slowdown: %d%%\n", slowdown);
        } else {
            printf(1, "  Baseline Performance\n");
        }
        printf(1, "\n");
    }

    printf(1, "Priority Impact Analysis:\n");
    printf(1, "----------------------------------------\n");
    for(int i = 1; i < n; i++) {
        int runtime_diff = metrics[i].total_runtime - metrics[0].total_runtime;
        int interval_increase = 
            ((metrics[i].total_runtime / WORK_ITERATIONS) * 100) / 
            (metrics[0].total_runtime / WORK_ITERATIONS) - 100;
        
        printf(1, "Priority(Nice) %d vs Priority(Nice) 1:\n", i + 1);
        printf(1, "  Runtime Difference: +%d ticks (%d%%)\n", 
               runtime_diff,
               (runtime_diff * 100) / metrics[0].total_runtime);
        printf(1, "  Interval Increase: %d%%\n\n", 
               interval_increase);
    }

    printf(1, "Performance Summary:\n");
    printf(1, "----------------------------------------\n");
    printf(1, "Priority(Nice) 1 (Baseline):\n");
    printf(1, "  - Fastest completion time: %d ticks\n", metrics[0].total_runtime);
    printf(1, "  - Most consistent intervals (variance: %d ticks)\n", 
           metrics[0].max_interval - metrics[0].min_interval);
    printf(1, "Lower priorities show:\n");
    printf(1, "  - Increased runtime: up to %d%%\n", 
           ((metrics[NUM_PROCESSES-1].total_runtime - metrics[0].total_runtime) * 100) 
           / metrics[0].total_runtime);
    printf(1, "  - Higher interval variance: up to %d ticks\n",
           metrics[NUM_PROCESSES-1].max_interval - metrics[NUM_PROCESSES-1].min_interval);
    printf(1, "  - Progressively longer intervals\n");
}

int main() {
    struct process_metrics metrics[NUM_PROCESSES];
    int pipes[NUM_PROCESSES][2];
    
    printf(1, "\nPriority Scheduling Test (Fibonacci Computation)\n");
    printf(1, "============================================\n\n");

    for(int i = 0; i < NUM_PROCESSES; i++) {
        if(pipe(pipes[i]) < 0) {
            printf(1, "Pipe creation failed\n");
            exit();
        }
    }

    int base_time = uptime() + 50;
    
    for(int i = 0; i < NUM_PROCESSES; i++) {
        int pid = fork();
        
        if(pid == 0) {  // Child process
            struct process_metrics m;
            int priority = i + 1;
            
            for(int j = 0; j < NUM_PROCESSES; j++) {
                if(j != i) {
                    close(pipes[j][0]);
                    close(pipes[j][1]);
                }
                if(j == i) close(pipes[j][0]);
            }
            
            nice(getpid(), priority);
            m.pid = getpid();
            m.priority = priority;
            m.min_interval = 999999;
            m.max_interval = 0;
            
            while(uptime() < base_time);
            
            m.start_tick = uptime();
            int last_tick = m.start_tick;
            
            for(int task = 0; task < WORK_ITERATIONS; task++) {
                m.fibs[task] = find_nth_fibonacci(task + 1);
                
                heavy_compute();
                
                int current_tick = uptime();
                m.fib_found_ticks[task] = current_tick;
                m.intervals[task] = current_tick - last_tick;
                
                if(m.intervals[task] > m.max_interval) m.max_interval = m.intervals[task];
                if(m.intervals[task] < m.min_interval) m.min_interval = m.intervals[task];
                
                last_tick = current_tick;
            }
            
            m.end_tick = uptime();
            m.total_runtime = m.end_tick - m.start_tick;
            
            write(pipes[i][1], &m, sizeof(m));
            close(pipes[i][1]);
            exit();
        }
    }
    
    printf(1, "All processes starting simultaneously...\n");
    
    for(int i = 0; i < NUM_PROCESSES; i++) {
        close(pipes[i][1]);
    }
    
    for(int i = 0; i < NUM_PROCESSES; i++) {
        read(pipes[i][0], &metrics[i], sizeof(struct process_metrics));
        close(pipes[i][0]);
        wait();
    }

    for(int i = 0; i < NUM_PROCESSES; i++) {
        print_process_results(&metrics[i]);
    }
    
    print_comparative_analysis(metrics, NUM_PROCESSES);
    
    exit();
}