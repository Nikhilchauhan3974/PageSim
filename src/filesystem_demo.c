#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>

int main(void)
{
    const char *filename = "pagesim_demo.txt";
    const char *message = "Hello from PageSim File System Demo!\n";
    char buffer[128];

    printf("\n=== FILE SYSTEM DEMO ===\n");

    /* Create or open the file */
    int fd = open(filename, O_CREAT | O_RDWR | O_TRUNC, 0644);
    if (fd == -1) {
        perror("open");
        return 1;
    }
    printf("1. File opened successfully.\n");

    /* Write data */
    ssize_t written = write(fd, message, strlen(message));
    if (written == -1 || (size_t)written != strlen(message)) {
        perror("write");
        close(fd);
        return 1;
    }
    printf("2. Data written successfully.\n");

    /* Move file position back to the beginning */
    if (lseek(fd, 0, SEEK_SET) == -1) {
        perror("lseek");
        close(fd);
        return 1;
    }

    /* Read data */
    ssize_t bytes = read(fd, buffer, sizeof(buffer) - 1);
    if (bytes == -1) {
        perror("read");
        close(fd);
        return 1;
    }
    buffer[bytes] = '\0';
    printf("3. Data read: %s", buffer);

    /* Display file metadata */
    struct stat info;
    if (fstat(fd, &info) == -1) {
        perror("fstat");
        close(fd);
        return 1;
    }
    printf("4. File size: %lld bytes\n", (long long)info.st_size);
    printf("5. File permissions: %03o\n", (unsigned int)(info.st_mode & 0777));

    /* Close file */
    if (close(fd) == -1) {
        perror("close");
        return 1;
    }

    printf("6. File closed successfully.\n");
    printf("Demo completed.\n");

    return 0;
}
