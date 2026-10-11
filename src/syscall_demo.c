
#define _POSIX_C_SOURCE 200809L

#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int main(void)
{
    const char *filename = "syscall_demo.txt";
    const char message[] =
        "PageSim: Linux system-call demonstration.\n";

    printf("====================================\n");
    printf("    PAGESIM SYSTEM CALL DEMO\n");
    printf("====================================\n");

    printf("Process ID: %ld\n", (long)getpid());

    /*
     * open() requests that the kernel open or create a file.
     */
    int fd = open(
        filename,
        O_WRONLY | O_CREAT | O_TRUNC,
        0644
    );

    if (fd == -1) {
        perror("open");
        return 1;
    }

    printf("open(): file descriptor = %d\n", fd);

    /*
     * write() requests that the kernel write bytes to the file.
     */
    ssize_t written = write(fd, message, sizeof(message) - 1);

    if (written == -1) {
        perror("write");
        close(fd);
        return 1;
    }

    printf("write(): wrote %ld bytes\n", (long)written);

    /*
     * close() releases this file descriptor.
     */
    if (close(fd) == -1) {
        perror("close");
        return 1;
    }

    printf("close(): file descriptor released\n");

    /*
     * getpid() returns the current process ID.
     */
    printf("System-call demonstration completed.\n");

    return 0;
}
