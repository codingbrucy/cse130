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

int main(int argc,char* argv[]) {
   if (argc < 2) {
      printf("usage is: client <hostname>\n");
      return 1;
   }
   char* hostname = argv[1];
   int sid = socket(PF_INET,SOCK_STREAM,0);
   
   // Set server address
   struct sockaddr_in srv;
   struct hostent *server = gethostbyname(hostname);
   srv.sin_family = AF_INET;
   srv.sin_port   = htons(9000);
   memcpy(&srv.sin_addr.s_addr,server->h_addr,server->h_length);
   int status = connect(sid,(struct sockaddr*)&srv,sizeof(srv));
   checkError(status,__LINE__);

   // Using the select API to use multiple file descriptors concurrently
   fd_set afd; //set of file descriptors
   char buf;
   int done = 0;
   do {
	   FD_ZERO(&afd); //clear the set of file descriptors
	   FD_SET(STDIN_FILENO, &afd);
	   FD_SET(sid, &afd);
	   int nbReady = select(sid+1, &afd, NULL, NULL, NULL); //highest value of a file descriptor + 1, file descriptor for read, write, error, timeout to get out of select otherwise it blocks; returns the number of file descriptors that are ready
	   if(nbReady > 0) {
		   if(FD_ISSET(STDIN_FILENO, &afd)) { //input available to read from the keyboard
			   int rc = read(STDIN_FILENO, &buf, 1); //reading 1 byte at a time
			   assert(rc>0);
			   int wc = write(sid, &buf, 1); //send the input to the remote shell through the socket
			   assert(wc > 0);
		   } else if(FD_ISSET(sid, &afd)) { //input available to read from the socket
			   int rc = read(sid, &buf, 1); //reading 1 byte at a time
			   done = rc == 0; //we may receive 0 bytes if remote shell dies
			   if(!done) {
				   int wc = write(STDOUT_FILENO, &buf, 1); //send the output received from the socket to stdout
				   assert(rc>0);
			   }
		   }
	   } else done = 1; //remote shell dies	
   } while(!done);
   return 0;
}
