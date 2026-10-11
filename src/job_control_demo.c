
#define _POSIX_C_SOURCE 200809L

#include "job_control_demo.h"

#include <stdio.h>
#include <unistd.h>

void run_job_control_demo(void)
{
    printf("\n====================================\n");
    printf("     PAGESIM JOB CONTROL DEMO\n");
    printf("====================================\n");

    printf("Process ID (PID):        %ld\n", (long)getpid());
    printf("Parent Process ID (PPID): %ld\n", (long)getppid());
    printf("Process Group ID (PGID):  %ld\n", (long)getpgrp());
    printf("Session ID (SID):         %ld\n", (long)getsid(0));

    printf("\nJob-control information displayed successfully.\n");
}
