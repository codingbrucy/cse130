#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <dirent.h>
#include <sys/wait.h>

void checkError(int status)
{
   if (status < 0) {
      printf("socket error(%d): [%s]\n",getpid(),strerror(errno));
      exit(-1);
   }
}

int main(int argc,char* argv[]) {
   // Create a socket
   int sid = socket(PF_INET,SOCK_STREAM,0);
   // setup our address -- will listen on 9000 --
   struct sockaddr_in addr;
   addr.sin_family = AF_INET;
   addr.sin_port   = htons(9000);
   addr.sin_addr.s_addr = INADDR_ANY;
   int status = bind(sid,(struct sockaddr*)&addr,sizeof(addr));
   checkError(status);
   status = listen(sid,10);
   checkError(status);

   while(1) {
      struct sockaddr_in client;
      socklen_t clientSize = sizeof(client);
      int rshSocket = accept(sid,(struct sockaddr*)&client,&clientSize);
      checkError(rshSocket);
      printf("We accepted a socket: %d\n",rshSocket);
      
      // Create a child process to execute the remote shell
      pid_t child = fork();
      if (child == 0) {
         dup2(rshSocket,0);
	      dup2(rshSocket,1);
	      dup2(rshSocket,2);
	      close(rshSocket);
         int status = execlp("/bin/bash","/bin/bash","-l",NULL);
         checkError(status);
         return -1;         
      } else if (child > 0) {
         close(rshSocket);
         pid_t deadChild;
         int status = 0;
         do {
            deadChild = waitpid(0,&status,WNOHANG);
            checkError(deadChild);
            if (deadChild > 0)
               printf("Reaped: %d\n",deadChild);
         } while (deadChild > 0);
      }
   }
   return 0;
}
