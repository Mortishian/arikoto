#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#define MAX_PROCESSES 10

// Define a type for task functions.
typedef void (*task_func_t)(void);

// Process control block structure.
typedef struct {
    int id;             // Process identifier.
    task_func_t task;   // The function pointer representing the task.
    bool finished;      // Flag to track if the process has finished
} process_t;

// Scheduler structure.
typedef struct {
    process_t processes[MAX_PROCESSES];
    int process_count;
    int time_quantum;
} scheduler_t;

// Initialize the scheduler
void scheduler_init(scheduler_t *scheduler, int time_quantum);

// Add a process/task to the scheduler.
int scheduler_add_process(scheduler_t *scheduler, int id, task_func_t task);

// Run the scheduler
void scheduler_run(scheduler_t *scheduler);
