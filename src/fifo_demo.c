
#define _POSIX_C_SOURCE 200809L

#include "fifo_demo.h"

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define FIFO_PATH "/tmp/pagesim_fifo"
#define MESSAGE "Hello through a named FIFO!"

void run_fifo_demo(void)
{
    pid_t pid;
    int status;

    printf("\n====================================\n");
    printf("     PAGESIM NAMED FIFO DEMO\n");
    printf("====================================\n");

    /*
     * Remove an old FIFO left by a previous run.
     * Ignore the error if the path does not exist.
     */
    if (unlink(FIFO_PATH) == -1 && errno != ENOENT) {
        perror("unlink");
        return;
    }

    /*
     * Create a named pipe in the filesystem.
     */
    if (mkfifo(FIFO_PATH, 0600) == -1) {
        perror("mkfifo");
        return;
    }

    printf("FIFO created at: %s\n", FIFO_PATH);
    fflush(stdout);

    pid = fork();

    if (pid == -1) {
        perror("fork");
        unlink(FIFO_PATH);
        return;
    }

    if (pid == 0) {
        /* Child: open the FIFO for reading. */
        char buffer[100];
        size_t total_read = 0;
        int fd = open(FIFO_PATH, O_RDONLY);

        if (fd == -1) {
            perror("child open");
            _exit(1);
        }

        while (total_read < sizeof(buffer) - 1) {
            ssize_t n = read(
                fd,
                buffer + total_read,
                sizeof(buffer) - 1 - total_read
            );

            if (n == 0) {
                break;
            }

            if (n < 0) {
                if (errno == EINTR) {
                    continue;
                }

                perror("child read");
                close(fd);
                _exit(1);
            }

            total_read += (size_t)n;
        }

        buffer[total_read] = '\0';

        printf("[Child] Received: %s\n", buffer);
        fflush(stdout);

        close(fd);
        _exit(0);
    }

    /* Parent: open the FIFO for writing. */
    int fd = open(FIFO_PATH, O_WRONLY);

    if (fd == -1) {
        perror("parent open");
        kill(pid, SIGTERM);

        while (waitpid(pid, &status, 0) == -1 && errno == EINTR) {
        }

        unlink(FIFO_PATH);
        return;
    }

    printf("[Parent] Sending: %s\n", MESSAGE);

    size_t length = strlen(MESSAGE);
    size_t total_written = 0;

    while (total_written < length) {
        ssize_t n = write(
            fd,
            MESSAGE + total_written,
            length - total_written
        );

        if (n < 0) {
            if (errno == EINTR) {
                continue;
            }

            perror("parent write");
            close(fd);

            while (waitpid(pid, &status, 0) == -1 && errno == EINTR) {
            }

            unlink(FIFO_PATH);
            return;
        }

        total_written += (size_t)n;
    }

    close(fd);

    while (waitpid(pid, &status, 0) == -1) {
        if (errno != EINTR) {
            perror("waitpid");
            unlink(FIFO_PATH);
            return;
        }
    }

    if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
        printf("[Parent] FIFO communication completed successfully.\n");
    } else {
        printf("[Parent] Child did not finish successfully.\n");
    }

    /* Remove the named pipe after the demo. */
    if (unlink(FIFO_PATH) == -1) {
        perror("unlink");
    }
}
