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

static void run_process(process_t *p, int quantum) {
    if (p->finished) {
        return;
    }

    p->task();
}

void scheduler_run(scheduler_t *scheduler) {
    for (int i = 0; i < scheduler->process_count; i++) {
        process_t *p = &scheduler->processes[i];
        run_process(p, scheduler->time_quantum);
    }

    puts("Scheduler has no more tasks, halting.", COLOR_WHITE);
}
