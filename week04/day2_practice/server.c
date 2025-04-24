/* Implementing server-side of a chat room */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <dirent.h>

void checkError(int status) {
   if (status < 0) {
      printf("socket error(%d): [%s]\n",getpid(),strerror(errno));
      exit(-1);
   }
}

/* Reads input till end of line from the socket */
char* readResponse(int fd) {
   int sz = 8;
   char* buf = malloc(sz);
   int ttl = 0,at = 0;
   int recvd;
   do {
      recvd = read(fd,buf+at,1);
      ttl += recvd;
      at  += recvd;
      if (recvd > 0 && ttl == sz) {
         buf = realloc(buf,sz*2);
         sz *= 2;
      }
      buf[ttl] = 0;
   } while (buf[ttl-1] != '\n' && recvd > 0);
   return buf;
}

int main(int argc,char* argv[])  {
   /* Read a port number from the user */
   if (argc < 2) {
      printf("Usage is: server <port>\n");
      return 1;
   }

   int p = atoi(argv[1]);                       /* Port number */

   int sid = socket(PF_INET,SOCK_STREAM,0);    /* Create a server socket */

   /* Make the server-side socket non-blocking */
   int flags = fcntl(sid, F_GETFL, 0);
   checkError(flags);
   flags = flags | O_NONBLOCK;
   int status = fcntl(sid, F_SETFL, flags);
   checkError(status);
   int enable = 1;
   status = setsockopt(sid, SOL_SOCKET,SO_REUSEADDR, &enable, sizeof(int));
   checkError(status);
  
   /* Setup server address and bind to the socket */
   struct sockaddr_in addr;
   addr.sin_family = AF_INET;
   addr.sin_port   = htons(p);
   addr.sin_addr.s_addr = INADDR_ANY;
   status = bind(sid,(struct sockaddr*)&addr,sizeof(addr));
   checkError(status);

   /* Listen on that socket for let's connect messages. No more than 10 pending at once. */
   status = listen(sid,10);
   checkError(status);

   int nbc = 0;         /* used to track the number of clients connected at any time */
   int clients[10];     /* an array to store the socket ids (fd) for each client */

   /* TODO:
    * Given n client sockets and the server socket, the server simply needs to listen to all of them using the select API. 
    * Initially, there are no client sockets. Therefore, the server only listens to sid. 
    * If the server socket is ready, it implies a client is trying to join the chat room. Server accepts the request and expands the set of file descriptors to listen to.
    * If any of the client sockets are ready, it reads the message and sends it to all the other clients. 
        * If the message is "Bye!", the server terminates after sending the message to all other clients. 
    * Note: do not forget to free any memory that you allocate to store the message received and do not forget to close the sockets before exiting the program normally.
   */
   
   return 0;
}
