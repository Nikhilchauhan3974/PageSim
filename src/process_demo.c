
#define _POSIX_C_SOURCE 200809L

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void)
{
    /*
     * Flush buffered output before fork so the
     * startup messages are not duplicated.
     */
    printf("====================================\n");
    printf("     PAGESIM PROCESS CONTROL DEMO\n");
    printf("====================================\n");
    printf("Parent PID: %ld\n", (long)getpid());
    fflush(stdout);

    /*
     * Create a child process.
     */
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        return EXIT_FAILURE;
    }

    if (pid == 0) {
        /*
         * Child process.
         */
        printf("\n[Child]\n");
        printf("Child PID: %ld\n", (long)getpid());
        printf("Parent PID: %ld\n", (long)getppid());
        printf("Child is doing its work...\n");
        fflush(stdout);

        /*
         * The child's exit status is collected
         * by the parent using waitpid().
         */
        _exit(0);
    }

    /*
     * Parent process.
     */
    printf("\n[Parent]\n");
    printf("Created child PID: %ld\n", (long)pid);
    printf("Waiting for child to finish...\n");

    int status;
    pid_t result;

    do {
        result = waitpid(pid, &status, 0);
    } while (result < 0 && errno == EINTR);

    if (result < 0) {
        perror("waitpid");
        return EXIT_FAILURE;
    }

    if (WIFEXITED(status)) {
        printf("Child exited with status: %d\n",
               WEXITSTATUS(status));
    } else if (WIFSIGNALED(status)) {
        printf("Child terminated by signal: %d\n",
               WTERMSIG(status));
    }

    printf("Parent process completed.\n");

    return EXIT_SUCCESS;
}
