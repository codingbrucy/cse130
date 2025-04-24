/* client.c — Implementing client-side of a chat room */

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

void checkError(int status,int line) {
   if (status < 0) {
      printf("socket error(%d)-%d: [%s]\n",getpid(),line,strerror(errno));
      exit(-1);
   }
}

/* Reads input till end of line from the socket */
char* readResponse(int fd) {
   int sz = 8;
   char* buf = malloc(sz);
   int ttl = 0, at = 0;
   int recvd;
   do {
      recvd = read(fd, buf+at, 1);
      ttl  += recvd;
      at   += recvd;
      if (recvd > 0 && ttl == sz) {
         sz *= 2;
         buf = realloc(buf, sz);
      }
      buf[ttl] = 0;
   } while (recvd > 0 && buf[ttl-1] != '\n');
   return buf;
}

int main(int argc,char* argv[]) {
   if (argc < 3) {
      printf("Usage is: client <hostname> <port>\n");
      return 1;
   }

   int  p      = atoi(argv[2]);
   int  sid    = socket(PF_INET, SOCK_STREAM, 0);
   int  status;
   struct sockaddr_in srv;
   struct hostent *server = gethostbyname(argv[1]);
   if (!server) {
      printf("Couldn't find host: %s\n", argv[1]);
      return 2;
   }

   srv.sin_family = AF_INET;
   srv.sin_port   = htons(p);
   memcpy(&srv.sin_addr, server->h_addr_list[0], server->h_length);

   status = connect(sid, (struct sockaddr*)&srv, sizeof(srv));
   checkError(status, __LINE__);

   printf("Connected to %s:%d — type messages, 'Bye!' to quit.\n", argv[1], p);

   fd_set rfds;
   int    maxfd = sid > STDIN_FILENO ? sid : STDIN_FILENO;
   char  *line   = NULL;
   size_t linecap = 0;

   while (1) {
      FD_ZERO(&rfds);
      FD_SET(STDIN_FILENO, &rfds);
      FD_SET(sid,           &rfds);

      status = select(maxfd+1, &rfds, NULL, NULL, NULL);
      checkError(status, __LINE__);

      // 1) Keyboard input → send to server
      if (FD_ISSET(STDIN_FILENO, &rfds)) {
         ssize_t nread = getline(&line, &linecap, stdin);
         if (nread <= 0) break;               // EOF or error
         send(sid, line, nread, 0);
         if (strncmp(line, "Bye!", 4) == 0)   // user wants to quit
            break;
      }

      // 2) Server message → print to stdout
      if (FD_ISSET(sid, &rfds)) {
         char *resp = readResponse(sid);
         if (resp[0] == '\0') { free(resp); break; }  // server closed
         fputs(resp, stdout);
         if (strncmp(resp, "Bye!", 4) == 0) {          // server says bye
            free(resp);
            break;
         }
         free(resp);
      }
   }

   free(line);
   close(sid);
   return 0;
}
