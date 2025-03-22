#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#define MAX_PROCESSES 10

/* Define a type for task functions */
typedef void (*task_func_t)(void);

/* Process control block structure */
typedef struct {
    int id;
    task_func_t task;
    bool finished;
} process_t;

/* Scheduler structure */
typedef struct {
    process_t processes[MAX_PROCESSES];
    int process_count;
    int time_quantum;
} scheduler_t;

void scheduler_init(scheduler_t *scheduler, int time_quantum);
int scheduler_add_process(scheduler_t *scheduler, int id, task_func_t task);
void scheduler_run(scheduler_t *scheduler);
