#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/select.h>
#include <assert.h>
#include "analyze.h"

void checkError(int status,int line) {
   if (status < 0) {
      printf("socket error(%d)-%d: [%s]\n",getpid(),line,strerror(errno));
      exit(-1);
   }
}

/* given helper function to read the output stream sent by the server */
char* readResponse(int sid) {
   size_t sz = 8;
   char* buf = malloc(sizeof(char)* sz);
   int done = 0,received = 0;  
   while (!done) {
      int nbb = read(sid,buf + received,sz - received);
      if (nbb > 0)
         received += nbb;
      else if (nbb == 0)
         done = 1;
      if (received == sz) {
         buf = realloc(buf,sz * 2);
         sz  *= 2;
      }
   }
   if (received == sz)
      buf = realloc(buf,sz + 1);
   buf[received] = 0;
   return buf;
}

int main(int argc,char* argv[]) {
   if (argc < 2) {
      printf("Usage is: wgettext <url>\n");
      return 1;
   }
   char host[512];
   int  port = 80;
   char url[512];
   analyzeURL(argv[1],host,&port,url);
   printf("[%s] [%d] [%s]\n",host,port,url);
   
   // TODO Q2: Create a socket and communicate with the server!
   /* Start by creating a TCP socket and establishing a connection with the server. Use AF_INET for the address family of the socket.
    * The port number and hostname is obtained above via a call to analyzeURL. Remember to convert hostname to IP address.
    * Once the connection is established, send the GET request and receive the response.
    * A helper function is given to readResponse.
    * Print the response on stdout.
   */
   
   return 0;
}


