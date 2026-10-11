
#include "scheduler.h"

#include <stdio.h>

int main(void)
{
    int choice;
    int time_quantum;

    while (1) {
        printf("\n====================================\n");
        printf("     PAGESIM CPU SCHEDULER\n");
        printf("====================================\n");
        printf("1. First Come, First Served (FCFS)\n");
        printf("2. Round Robin (RR)\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a number.\n");
            return 1;
        }

        switch (choice) {
            case 1:
                run_fcfs();
                break;

            case 2:
                printf("Enter time quantum: ");

                if (scanf("%d", &time_quantum) != 1) {
                    printf("Invalid time quantum.\n");
                    return 1;
                }

                if (time_quantum <= 0) {
                    printf("Time quantum must be positive.\n");
                    break;
                }

                run_round_robin(time_quantum);
                break;

            case 3:
                printf("Exiting CPU scheduler.\n");
                return 0;

            default:
                printf("Invalid choice. Select 1, 2, or 3.\n");
        }
    }
}
