
#define _POSIX_C_SOURCE 200809L

#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "shell.h"

#define MAX_INPUT 1024
#define MAX_ARGS 64

static void print_help(void)
{
    printf("\nPageShell built-in commands\n");
    printf("---------------------------\n");
    printf("help       Show this help\n");
    printf("pwd        Show current directory\n");
    printf("cd DIR     Change current directory\n");
    printf("exit       Exit PageShell\n");
    printf("\nOther commands run as Linux programs.\n");
    printf("Example: ls -l\n");
    printf("Example: ps\n");
    printf("Example: uname -a\n\n");
}

static int parse_input(char *input, char *args[])
{
    int count = 0;
    char *saveptr = NULL;

    char *token = strtok_r(input, " \t\r\n", &saveptr);

    while (token != NULL && count < MAX_ARGS - 1) {
        args[count++] = token;
        token = strtok_r(NULL, " \t\r\n", &saveptr);
    }

    args[count] = NULL;
    return count;
}

static void execute_command(char *args[])
{
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        return;
    }

    if (pid == 0) {
        /*
         * Child process:
         * Replace this process with the requested program.
         */
        execvp(args[0], args);

        /*
         * Reached only if execvp fails.
         */
        perror(args[0]);
        _exit(127);
    }

    /*
     * Parent process:
     * Wait for the child to finish.
     */
    int status;
    pid_t result;

    do {
        result = waitpid(pid, &status, 0);
    } while (result < 0 && errno == EINTR);

    if (result < 0) {
        perror("waitpid");
    } else if (WIFEXITED(status)) {
        printf("[PageShell] Child %ld exited with status %d\n",
               (long)pid, WEXITSTATUS(status));
    } else if (WIFSIGNALED(status)) {
        printf("[PageShell] Child %ld terminated by signal %d\n",
               (long)pid, WTERMSIG(status));
    }
}

void shell_run(void)
{
    char input[MAX_INPUT];

    printf("\n====================================\n");
    printf("          PAGESHELL v1.0\n");
    printf("====================================\n");
    printf("A small Linux command shell.\n");
    printf("Type 'help' for commands.\n\n");

    while (1) {
        printf("pageshell$ ");
        fflush(stdout);

        if (fgets(input, sizeof(input), stdin) == NULL) {
            if (feof(stdin)) {
                printf("\n");
                break;
            }

            if (ferror(stdin) && errno == EINTR) {
                clearerr(stdin);
                continue;
            }

            perror("fgets");
            break;
        }

        /*
         * Detect an overlong input line instead of
         * accidentally executing a truncated command.
         */
        if (strchr(input, '\n') == NULL && !feof(stdin)) {
            int ch;

            while ((ch = getchar()) != '\n' && ch != EOF) {
                /* Discard the rest of this input line. */
            }

            fprintf(stderr, "Command is too long.\n");
            continue;
        }

        char *args[MAX_ARGS];
        int argc = parse_input(input, args);

        if (argc == 0) {
            continue;
        }

        if (strcmp(args[0], "exit") == 0) {
            break;
        }

        if (strcmp(args[0], "help") == 0) {
            print_help();
            continue;
        }

        if (strcmp(args[0], "pwd") == 0) {
            char cwd[4096];

            if (getcwd(cwd, sizeof(cwd)) != NULL) {
                printf("%s\n", cwd);
            } else {
                perror("getcwd");
            }

            continue;
        }

        if (strcmp(args[0], "cd") == 0) {
    const char *home = getenv("HOME");
    const char *directory = (argc >= 2) ? args[1] : home;

    if (directory != NULL && strcmp(directory, "~") == 0) {
        directory = home;
    }

    if (directory == NULL) {
        fprintf(stderr, "Home directory is not set.\n");
    } else if (chdir(directory) != 0) {
        perror("cd");
    }

    continue;
}

        execute_command(args);
    }

    printf("Exiting PageShell.\n");
}
