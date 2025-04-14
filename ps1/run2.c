#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char ** argv)
{
    /* There should at least be 3 arguments in addition to run2: 2 for the first command, and the rest for the second command */
    if (argc < 4) {
        fprintf(stderr, "Usage: %s cmd1 cmd1_arg cmd2 [cmd2_args ..]\n", argv[0]);
        return 1;
    }
    
    int status;
    pid_t pid1, pid2;
    
    // Fork the first child to run the first command with execlp
    pid1 = fork();
    if (pid1 < 0) { 
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid1 == 0) {
        // First child process.
        // Use execlp: first argument is the command name, second is usually the same,
        // followed by its argument and ending with (char *)NULL.
        execlp(argv[1], argv[1], argv[2], (char *)NULL);
        // If execlp returns, there was an error.
        perror("execlp failed");
        exit(EXIT_FAILURE);
    }
    
    // Fork the second child to run the second command with execvp
    pid2 = fork();
    if (pid2 < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid2 == 0) {
        // Second child process.
        // For execvp, we pass the command name (argv[3]) and a NULL-terminated 
        // array of arguments starting at argv[3].
        // Since argv was passed to main with a terminating NULL at argv[argc],
        // &argv[3] serves as our vector.
        execvp(argv[3], &argv[3]);
        // If execvp returns, there was an error.
        perror("execvp failed");
        exit(EXIT_FAILURE);
    }
    
    // Parent process: wait for both children and print their exit statuses.
    // Wait for first child
    if (waitpid(pid1, &status, 0) == -1) {
        perror("waitpid");
        exit(EXIT_FAILURE);
    }
    printf("exited=%d exitstatus=%d\n", WIFEXITED(status), WEXITSTATUS(status));
    
    // Wait for second child
    if (waitpid(pid2, &status, 0) == -1) {
        perror("waitpid");
        exit(EXIT_FAILURE);
    }
    printf("exited=%d exitstatus=%d\n", WIFEXITED(status), WEXITSTATUS(status));
    
    return 0;
}
