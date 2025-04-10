#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>

int my_exec(void); 
int main(void){

    pid_t forkval = fork();
    // spwans child!
    // if forkval == 0 : in child
    // if forval >0, in parent

    // pid_t wait(int *status) <- returns id of dead child
    // pid_t waitpid(pid_t pid, int *status, int options)
    printf("The value = %d\n", forkval);
}

int my_exec(void){
    //execl() replaces/ upgrades a child process to a different process
    
}