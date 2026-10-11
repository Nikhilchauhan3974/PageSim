
#define _POSIX_C_SOURCE 200809L

#include "signal_demo.h"

#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

static volatile sig_atomic_t received_signal = 0;

static void handle_signal(int signal_number)
{
    received_signal = signal_number;
}

void run_signal_demo(void)
{
    struct sigaction action;

    memset(&action, 0, sizeof(action));
    action.sa_handler = handle_signal;
    sigemptyset(&action.sa_mask);

    if (sigaction(SIGINT, &action, NULL) == -1) {
        perror("sigaction");
        return;
    }

    if (sigaction(SIGTERM, &action, NULL) == -1) {
        perror("sigaction");
        return;
    }

    received_signal = 0;

    printf("\n====================================\n");
    printf("     PAGESIM SIGNAL DEMO\n");
    printf("====================================\n");
    printf("Process PID: %ld\n", (long)getpid());
    printf("Waiting for SIGINT or SIGTERM...\n");
    printf("Press Ctrl+C to send SIGINT.\n");
    printf("Or use another terminal: kill -TERM %ld\n",
           (long)getpid());
    fflush(stdout);

    while (received_signal == 0) {
        pause();
    }

    printf("\nSignal received: %d (%s)\n",
           (int)received_signal,
           received_signal == SIGINT ? "SIGINT" : "SIGTERM");
    printf("Signal demo completed.\n");
}
