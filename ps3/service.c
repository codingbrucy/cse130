#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <errno.h>
#include <assert.h>

/* prints the error that is encountered and terminate the program */
void checkError(int status,int line) {
   if (status < 0) {
      printf("socket error(%d)-%d: [%s]\n",getpid(),line,strerror(errno));
      exit(-1);
   }
}

int main(int argc,char* argv[]) {
   if (argc < 3) {
      printf("usage is: service <hostname> <port>\n");
      return 1;
   }

   char* hostname = argv[1];
   int port       = atoi(argv[2]);

   int sid = socket(PF_INET, SOCK_STREAM, 0);
   checkError(sid, __LINE__);

   struct hostent *server = gethostbyname(hostname);
   assert(server);

   struct sockaddr_in srv;
   memset(&srv, 0, sizeof(srv));
   srv.sin_family = AF_INET;
   srv.sin_port   = htons(port);
   memcpy(&srv.sin_addr.s_addr, server->h_addr_list[0], server->h_length);

   int status = connect(sid, (struct sockaddr*)&srv, sizeof(srv));
   checkError(status, __LINE__);

   // send the "$die!" command to tell the server to terminate
   const char *cmd = "$die!";
   ssize_t n = write(sid, cmd, strlen(cmd));
   checkError(n < 0 ? -1 : n, __LINE__);

   close(sid);
   return 0;
}