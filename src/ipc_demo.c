
#define _POSIX_C_SOURCE 200809L

#include "ipc_demo.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void run_pipe_demo(void)
{
    int pipe_fd[2];
    pid_t pid;

    char message[] = "Hello from the parent process!";
    char buffer[100];

    printf("\n====================================\n");
    printf("     PAGESIM IPC PIPE DEMO\n");
    printf("====================================\n");

    /*
     * pipe_fd[0] is the read end.
     * pipe_fd[1] is the write end.
     */
    if (pipe(pipe_fd) == -1) {
        perror("pipe");
        return;
    }

    pid = fork();

    if (pid == -1) {
        perror("fork");
        close(pipe_fd[0]);
        close(pipe_fd[1]);
        return;
    }

    if (pid == 0) {
        /* Child: receive the message. */
        close(pipe_fd[1]);

        ssize_t bytes_read;
        size_t total_read = 0;

        while (total_read < sizeof(buffer) - 1) {
            bytes_read = read(
                pipe_fd[0],
                buffer + total_read,
                sizeof(buffer) - 1 - total_read
            );

            if (bytes_read == 0) {
                break;
            }

            if (bytes_read < 0) {
                if (errno == EINTR) {
                    continue;
                }

                perror("read");
                close(pipe_fd[0]);
                _exit(EXIT_FAILURE);
            }

            total_read += (size_t)bytes_read;
        }

        buffer[total_read] = '\0';

        printf("[Child] PID: %ld\n", (long)getpid());
        printf("[Child] Received: %s\n", buffer);
        fflush(stdout);

        close(pipe_fd[0]);
        _exit(EXIT_SUCCESS);
    }

    /* Parent: send the message. */
    close(pipe_fd[0]);

    printf("[Parent] PID: %ld\n", (long)getpid());
    printf("[Parent] Sending message to child...\n");

    size_t total_written = 0;
    size_t message_length = strlen(message);

    while (total_written < message_length) {
        ssize_t bytes_written = write(
            pipe_fd[1],
            message + total_written,
            message_length - total_written
        );

        if (bytes_written < 0) {
            if (errno == EINTR) {
                continue;
            }

            perror("write");
            close(pipe_fd[1]);

            int status;
            while (waitpid(pid, &status, 0) < 0 && errno == EINTR) {
            }

            return;
        }

        total_written += (size_t)bytes_written;
    }

    close(pipe_fd[1]);

    int status;
    pid_t result;

    do {
        result = waitpid(pid, &status, 0);
    } while (result < 0 && errno == EINTR);

    if (result == -1) {
        perror("waitpid");
        return;
    }

    if (WIFEXITED(status) && WEXITSTATUS(status) == EXIT_SUCCESS) {
        printf("[Parent] Child received the message successfully.\n");
    } else {
        printf("[Parent] Child did not finish successfully.\n");
    }
}
