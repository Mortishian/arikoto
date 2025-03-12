#include <scheduler.h>
#include <print.h>
#include <stddef.h>
#include <kernel.h>

void scheduler_init(scheduler_t *scheduler, int time_quantum) {
    scheduler->process_count = 0;
    scheduler->time_quantum = time_quantum;
}

int scheduler_add_process(scheduler_t *scheduler, int id, task_func_t task) {
    if (scheduler->process_count >= MAX_PROCESSES) {
        return -1;
    }

    process_t *p = &scheduler->processes[scheduler->process_count++];
    p->id = id;
    p->task = task;
    p->finished = false;
    return 0;
}

static void run_process(process_t *p) {
    if (p->finished) {
        return;
    }

    p->task();
}

void scheduler_run(scheduler_t *scheduler) {
    if (!scheduler || scheduler->process_count == 0) {
        puts("Error: Invalid scheduler state", COLOR_RED);
        return;
    }

    bool tasks_remaining = true;
    while (tasks_remaining) {
        tasks_remaining = false;
        for (int i = 0; i < scheduler->process_count; i++) {
            process_t *p = &scheduler->processes[i];
            if (!p->finished) {
                tasks_remaining = true;
                run_process(p);
                p->finished = true; // Mark as done after running
            }
        }
    }
    panic("PANIC: Scheduler ran out of tasks, panicking for safety.\n\n");
}
