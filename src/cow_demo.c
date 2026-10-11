#include "cow_demo.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void run_cow_demo(void)
{
    int shared_value = 100;

    printf("\n====================================\n");
    printf(" COPY-ON-WRITE DEMONSTRATION\n");
    printf("====================================\n");

    printf("Parent: initial value = %d\n", shared_value);
    fflush(stdout);

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork failed");
        return;
    }

    if (pid == 0) {
        printf("Child: value before modification = %d\n",
               shared_value);

        shared_value = 200;

        printf("Child: value after modification = %d\n",
               shared_value);

        _exit(EXIT_SUCCESS);
    }

    int status;
    if (waitpid(pid, &status, 0) < 0) {
        perror("waitpid failed");
        return;
    }

    printf("Parent: value after child modification = %d\n",
           shared_value);

    if (WIFEXITED(status) && WEXITSTATUS(status) == EXIT_SUCCESS) {
        printf("Copy-on-write demonstration completed.\n");
    } else {
        printf("Child process did not exit successfully.\n");
    }
}
