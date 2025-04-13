#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>

int my_exec(void); 
int main(void){
    printf("A");

    fork();
    printf("B");
    
}

int my_exec(void){
    //execl() replaces/ upgrades a child process to a different process
    
}