/* Number printer */

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
    pid_t childval = fork();
    if(childval> 0 ){
        // we in parent process!
        printf("hail from parent process, pid: %d \n",getpid());
        for(int i = 1; i<10; i+=2){
            printf("%d, ",i);
        }
        printf("\n");
    }
    else{
        printf("hail from child process, pid: %d \n",getpid());
        for(int i = 0; i<=10; i+=2){
            printf("%d, ",i);
        }
    }
    return 0;
}