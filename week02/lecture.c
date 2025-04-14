#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main() {
    int fd;
    char *text = "Hello, File I/O!\n";
    char buffer[100];
    ssize_t bytes_read;

    // Open file for writing (create if doesn't exist)
    fd = open("example.txt", O_CREAT | O_WRONLY, 0644);
    if (fd == -1) {
        perror("Error opening file for writing");
        exit(1);
    }

    // Write to file
    if (write(fd, text, strlen(text)) == -1) {
        perror("Error writing to file");
        close(fd);
        exit(1);
    }

    close(fd);

    // Open file for reading
    fd = open("example.txt", O_RDONLY);
    if (fd == -1) {
        perror("Error opening file for reading");
        exit(1);
    }

    // Read from file
    bytes_read = read(fd, buffer, sizeof(buffer)-1);
    if (bytes_read == -1) {
        perror("Error reading from file");
        close(fd);
        exit(1);
    }

    buffer[bytes_read] = '\0';
    printf("Read from file: %s", buffer);

    close(fd);
    return 0;
}