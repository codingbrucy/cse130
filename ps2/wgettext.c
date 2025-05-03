#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <errno.h>
#include <sys/wait.h>
#include <assert.h>
#include "analyze.h"

void checkError(int status,int line) {
   if (status < 0) {
      fprintf(stderr, "socket error(%d)-%d: [%s]\n",
              getpid(), line, strerror(errno));
      exit(1);
   }
}

// from Part 2
char* readResponse(int sid)
{
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
      buf = realloc(buf,sz + 1); // very rare case
   buf[received] = 0;
   return buf;
}

int main(int argc,char* argv[]) {
   if (argc < 2) {
      fprintf(stderr, "Usage: wgettext <url>\n");
      return 1;
   }

   // Part 1: parse URL
   char host[512];
   int  port = 80;
   char doc[512];
   analyzeURL(argv[1], host, &port, doc);
   printf("[%s] [%d] [%s]\n", host, port, doc);

   // Part 2: fetch raw HTML
   int sockfd = socket(AF_INET, SOCK_STREAM, 0);
   checkError(sockfd, __LINE__);

   struct hostent *he = gethostbyname(host);
   if (!he) {
      fprintf(stderr, "gethostbyname failed for %s\n", host);
      close(sockfd);
      return 1;
   }

   struct sockaddr_in servaddr = {0};
   servaddr.sin_family = AF_INET;
   servaddr.sin_port   = htons(port);
   memcpy(&servaddr.sin_addr,
          he->h_addr_list[0],
          he->h_length);

   checkError(connect(sockfd,
                     (struct sockaddr*)&servaddr,
                     sizeof(servaddr)), __LINE__);

   char request[1024];
   snprintf(request, sizeof(request), "GET %s\n", doc);
   checkError(send(sockfd, request, strlen(request), 0), __LINE__);

   char *html = readResponse(sockfd);
   close(sockfd);

   // Part 3: pipe through html2text
   int pipefd[2];
   checkError(pipe(pipefd), __LINE__);

   pid_t pid = fork();
   checkError(pid, __LINE__);

   if (pid == 0) {
     // Child: read from pipe, exec html2text
     close(pipefd[1]);
     dup2(pipefd[0], STDIN_FILENO);
     close(pipefd[0]);
     execlp("html2text", "html2text", (char*)NULL);
     // If exec fails:
     perror("execlp html2text");
     exit(1);
   } else {
     // Parent: write HTML into pipe
     close(pipefd[0]);
     size_t len = strlen(html);
     ssize_t n;
     for (size_t sent = 0; sent < len; sent += n) {
       n = write(pipefd[1], html + sent, len - sent);
       if (n < 0) break;
     }
     free(html);
     close(pipefd[1]);
     waitpid(pid, NULL, 0);
   }

   return 0;
}
