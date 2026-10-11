
#ifndef SHELL_H
#define SHELL_H

/*
 * PageShell - a small Linux command shell.
 *
 * Demonstrates:
 * - User-space command execution
 * - Process creation using fork()
 * - Program execution using execvp()
 * - Child-process synchronization using waitpid()
 */

void shell_run(void);

#endif

