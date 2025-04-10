#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
void printNums(){
    int p2c[2];  // Pipe: parent --> child
    int c2p[2];  // Pipe: child --> parent
    char buf;

    // Create the pipes
    pipe(p2c);
    pid_t pid = fork();

    if (pid > 0) { 
        // Parent process
        close(p2c[0]); // Close parent's read side of p2c
        close(c2p[1]); // Close parent's write side of c2p

        // Parent prints 1, 3, 5
        for (int i = 1; i <= 5; i += 2) {
            // Print odd number
            printf("%d", i);
            fflush(stdout);

            // Signal child to print an even number
            write(p2c[1], "x", 1);

            read(c2p[0], &buf, 1);
        }
    } else {
        // Child process
        close(p2c[1]); // Close child's write side of p2c
        close(c2p[0]); // Close child's read side of c2p

        // Child prints 2, 4, 6
        for (int i = 2; i <= 6; i += 2) {
            // Wait for parent's signal
            read(p2c[0], &buf, 1);

            // Print even number
            printf("%d", i);
            fflush(stdout);
            // Signal back to parent
            write(c2p[1], "x", 1);
        }
    }
    return;
}

int main() {
    int fds[2];  // fds[0] is for reading, fds[1] is for writing.
    char buffer[100];
    char *msg = "Hello from child";

    // Create a pipe
    pipe(fds);

    pid_t pid = fork();
    if (pid == 0) {
        // Child process
        close(fds[0]); // Close the read end; child doesn't need it.
        // Write the message to the write end of the pipe.
        write(fds[1], msg, strlen(msg) + 1);
        printf("within child, descriptor is in fds[1](and child writes): %d\n", fds[0]);
        close(fds[1]); // Close the write end after sending the message.
    } else {
        // Parent process
        close(fds[1]); // Close the write end; parent doesn't need it.
        // Read the message from the read end of the pipe.
        read(fds[0], buffer, sizeof(buffer));
        printf("within parent, descriptor is in fds[0]: %d\n", fds[0]);
        close(fds[0]); // Close the read end.
        printf("Parent received: %s\n", buffer);
    }

    return 0;
}
