#define _GNU_SOURCE

#include "memory_protection.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <signal.h>

void run_memory_protection_demo(void)
{
    long page_size = sysconf(_SC_PAGESIZE);

    if (page_size <= 0) {
        perror("sysconf");
        return;
    }

    printf("\n====================================\n");
    printf(" MEMORY PROTECTION DEMO\n");
    printf("====================================\n");

    char *memory = mmap(NULL, (size_t)page_size,
                        PROT_READ | PROT_WRITE,
                        MAP_PRIVATE | MAP_ANONYMOUS,
                        -1, 0);

    if (memory == MAP_FAILED) {
        perror("mmap");
        return;
    }

    strcpy(memory, "PageSim protected memory");

    printf("Page size: %ld bytes\n", page_size);
    printf("Allocated memory address: %p\n", (void *)memory);
    printf("Before protection: %s\n", memory);

    if (mprotect(memory, (size_t)page_size, PROT_READ) == -1) {
        perror("mprotect");
        munmap(memory, (size_t)page_size);
        return;
    }

    printf("Memory protection changed to READ-ONLY.\n");
    printf("Reading protected memory: %s\n", memory);

    pid_t pid = fork();

    if (pid == -1) {
        perror("fork");
        munmap(memory, (size_t)page_size);
        return;
    }

    if (pid == 0) {
        /* Child attempts a write to read-only memory. */
        memory[0] = 'X';
        _exit(0);
    }

    int status;

    if (waitpid(pid, &status, 0) == -1) {
        perror("waitpid");
        munmap(memory, (size_t)page_size);
        return;
    }

    if (WIFSIGNALED(status) && WTERMSIG(status) == SIGSEGV) {
        printf("Protection verified: child received SIGSEGV.\n");
    } else if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
        printf("Warning: prohibited write did not trigger SIGSEGV.\n");
    } else {
        printf("Child terminated unexpectedly; status: %d\n", status);
    }

    if (munmap(memory, (size_t)page_size) == -1) {
        perror("munmap");
        return;
    }

    printf("Memory mapping released successfully.\n");
    printf("Memory protection demo completed.\n");
}
