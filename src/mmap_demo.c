#define _POSIX_C_SOURCE 200809L

#include "mmap_demo.h"

#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

void run_mmap_demo(void)
{
    const char *filename = "mmap_demo.txt";

    const char initial_text[] =
        "PageSim memory-mapped file demonstration.\n";

    const char updated_text[] =
        "PageSim file content updated through mmap.\n";

    /* Allocate enough mapped space for the longer text. */
    size_t length = sizeof(updated_text);

    printf("\n====================================\n");
    printf(" MEMORY-MAPPED FILE DEMO\n");
    printf("====================================\n");

    int fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, 0644);

    if (fd == -1) {
        perror("open");
        return;
    }

    if (ftruncate(fd, (off_t)length) == -1) {
        perror("ftruncate");
        close(fd);
        unlink(filename);
        return;
    }

    void *mapping = mmap(NULL, length,
                         PROT_READ | PROT_WRITE,
                         MAP_SHARED, fd, 0);

    if (mapping == MAP_FAILED) {
        perror("mmap");
        close(fd);
        unlink(filename);
        return;
    }

    close(fd);

    char *mapped_text = mapping;

    memcpy(mapped_text, initial_text, sizeof(initial_text));

    printf("Mapped file: %s\n", filename);
    printf("Mapping address: %p\n", mapping);
    printf("Original content: %s", mapped_text);

    memcpy(mapped_text, updated_text, sizeof(updated_text));

    if (msync(mapping, length, MS_SYNC) == -1) {
        perror("msync");
        munmap(mapping, length);
        unlink(filename);
        return;
    }

    printf("Updated content: %s", mapped_text);

    if (munmap(mapping, length) == -1) {
        perror("munmap");
        unlink(filename);
        return;
    }

    printf("Memory mapping released successfully.\n");

    if (unlink(filename) == -1) {
        perror("unlink");
        return;
    }

    printf("Temporary file removed successfully.\n");
    printf("Memory-mapped file demo completed.\n");
}
