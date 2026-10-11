
#include "scheduler.h"

#include <stdio.h>
#include <stdlib.h>

static void print_results(Process processes[], int count)
{
    double total_waiting = 0.0;
    double total_turnaround = 0.0;

    printf("\nPID\tArrival\tBurst\tCompletion\tTurnaround\tWaiting\n");

    for (int i = 0; i < count; i++) {
        printf("P%d\t%d\t%d\t%d\t\t%d\t\t%d\n",
               processes[i].pid,
               processes[i].arrival_time,
               processes[i].burst_time,
               processes[i].completion_time,
               processes[i].turnaround_time,
               processes[i].waiting_time);

        total_waiting += processes[i].waiting_time;
        total_turnaround += processes[i].turnaround_time;
    }

    printf("\nAverage waiting time: %.2f\n", total_waiting / count);
    printf("Average turnaround time: %.2f\n",
           total_turnaround / count);
}

static int read_processes(Process processes[])
{
    int count;

    printf("Enter number of processes (1-%d): ", MAX_PROCESSES);

    if (scanf("%d", &count) != 1 ||
        count < 1 || count > MAX_PROCESSES) {
        printf("Invalid number of processes.\n");
        return 0;
    }

    for (int i = 0; i < count; i++) {
        processes[i].pid = i + 1;

        printf("Enter arrival time and burst time for P%d: ", i + 1);

        if (scanf("%d %d",
                  &processes[i].arrival_time,
                  &processes[i].burst_time) != 2 ||
            processes[i].arrival_time < 0 ||
            processes[i].burst_time <= 0) {
            printf("Invalid input. Arrival must be >= 0 and burst must be > 0.\n");
            return 0;
        }

        processes[i].remaining_time = processes[i].burst_time;
        processes[i].completion_time = 0;
        processes[i].turnaround_time = 0;
        processes[i].waiting_time = 0;
    }

    return count;
}

void run_fcfs(void)
{
    Process processes[MAX_PROCESSES];
    int count = read_processes(processes);

    if (count == 0) {
        return;
    }

    /* Sort processes by arrival time. */
    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            if (processes[j].arrival_time < processes[i].arrival_time) {
                Process temp = processes[i];
                processes[i] = processes[j];
                processes[j] = temp;
            }
        }
    }

    int current_time = 0;

    for (int i = 0; i < count; i++) {
        if (current_time < processes[i].arrival_time) {
            current_time = processes[i].arrival_time;
        }

        current_time += processes[i].burst_time;
        processes[i].completion_time = current_time;

        processes[i].turnaround_time =
            processes[i].completion_time - processes[i].arrival_time;

        processes[i].waiting_time =
            processes[i].turnaround_time - processes[i].burst_time;
    }

    printf("\n=== FCFS Scheduling Results ===\n");
    print_results(processes, count);
}

void run_round_robin(int time_quantum)
{
    Process processes[MAX_PROCESSES];
    int count = read_processes(processes);

    if (count == 0) {
        return;
    }

    if (time_quantum <= 0) {
        printf("Time quantum must be positive.\n");
        return;
    }

    int current_time = 0;
    int completed = 0;
    int queue[MAX_PROCESSES * 100];
    int front = 0;
    int rear = 0;
    int in_queue[MAX_PROCESSES] = {0};

    /* Start at the earliest arrival time. */
    int earliest = processes[0].arrival_time;

    for (int i = 1; i < count; i++) {
        if (processes[i].arrival_time < earliest) {
            earliest = processes[i].arrival_time;
        }
    }

    current_time = earliest;

    /* Add processes that have already arrived. */
    for (int i = 0; i < count; i++) {
        if (processes[i].arrival_time <= current_time) {
            queue[rear++] = i;
            in_queue[i] = 1;
        }
    }

    while (completed < count) {
        if (front == rear) {
            /* CPU is idle until the next process arrives. */
            int next_arrival = -1;

            for (int i = 0; i < count; i++) {
                if (processes[i].remaining_time > 0 &&
                    (next_arrival == -1 ||
                     processes[i].arrival_time <
                     processes[next_arrival].arrival_time)) {
                    next_arrival = i;
                }
            }

            if (next_arrival == -1) {
                break;
            }

            current_time = processes[next_arrival].arrival_time;

            for (int i = 0; i < count; i++) {
                if (!in_queue[i] &&
                    processes[i].remaining_time > 0 &&
                    processes[i].arrival_time <= current_time) {
                    queue[rear++] = i;
                    in_queue[i] = 1;
                }
            }

            continue;
        }

        int index = queue[front++];

        int run_time = processes[index].remaining_time;

        if (run_time > time_quantum) {
            run_time = time_quantum;
        }

        current_time += run_time;
        processes[index].remaining_time -= run_time;

        /* Add newly arrived processes to the ready queue. */
        for (int i = 0; i < count; i++) {
            if (!in_queue[i] &&
                processes[i].remaining_time > 0 &&
                processes[i].arrival_time <= current_time) {
                queue[rear++] = i;
                in_queue[i] = 1;
            }
        }

        if (processes[index].remaining_time > 0) {
            queue[rear++] = index;
        } else {
            processes[index].completion_time = current_time;

            processes[index].turnaround_time =
                processes[index].completion_time -
                processes[index].arrival_time;

            processes[index].waiting_time =
                processes[index].turnaround_time -
                processes[index].burst_time;

            completed++;
        }
    }

    printf("\n=== Round Robin Scheduling Results ===\n");
    printf("Time quantum: %d\n", time_quantum);
    print_results(processes, count);
}
