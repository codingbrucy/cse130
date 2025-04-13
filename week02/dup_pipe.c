#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int main() {
    // Save the original stdout
    int stdout_copy = dup(STDOUT_FILENO);

    // Open file
    int fd = open("output.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        perror("open");
        return 1;
    }

    // Redirect stdout to file
    dup2(fd, STDOUT_FILENO);
    close(fd);  // fd no longer needed

    // This should now go to the file
    printf("This goes to the file.\n");
    fflush(stdout);  // flush buffer to the file!

    // Restore stdout
    dup2(stdout_copy, STDOUT_FILENO);
    close(stdout_copy);

    // This goes back to terminal
    printf("Larry returns to console.\n");

    return 0;
}
