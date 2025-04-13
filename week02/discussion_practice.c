#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

int my_pipe(){
    int pipefd[2];
    pid_t pid;
    char *message = "hello from parent";

    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid = fork();
    if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        // Child process

        close(pipefd[1]); // close write end

        char buffer[128];
        ssize_t n = read(pipefd[0], buffer, sizeof(buffer) - 1);
        if (n < 0) {
            perror("read");
            exit(EXIT_FAILURE);
        }

        buffer[n] = '\0'; // null-terminate

        // Convert to uppercase
        for (int i = 0; buffer[i]; i++) {
            buffer[i] = toupper(buffer[i]);
        }

        printf("Child received and uppercased: %s\n", buffer);

        close(pipefd[0]); // close read end
    } else {
        // Parent process

        close(pipefd[0]); // close read end

        write(pipefd[1], message, strlen(message));
        close(pipefd[1]); // signal EOF to child
    }

    return 0;
}

int my_execl(int argc, char* argv[]){
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <char> <filename>\n", argv[0]);
        return 1;
    }
    
    char ch = argv[1][0];      // First char of first argument
    char target[] = {ch,'\0'};
    char *filename = argv[2];

    pid_t val = fork();
    if (val == 0){
        // child process
        char *grep_path = "/bin/grep"; // or "/usr/bin/grep" depending on system
        char *option = "-o";

        // build a pattern: e.g., "x" becomes "[x]"
        char pattern[4] = "["; // opening bracket
        pattern[1] = argv[1][0]; // the char
        pattern[2] = ']'; // closing bracket
        pattern[3] = '\0';

        // execl(path, arg0, arg1, arg2, ..., NULL)
        execl(grep_path, "grep", target, argv[2], (char *)NULL);

        // If execl fails:
        perror("execl failed");
        return 1;
    }
    else{
        // parent process
        int status;
        pid_t finished_pid = waitpid(val, &status, 0); // 0 = block until child finishes
    }
    return 0;
}

int main(int argc, char* argv[]){
    my_pipe();
}