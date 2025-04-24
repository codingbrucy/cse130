#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<dirent.h>
#include<sys/wait.h>

void checkError(int status) {
	if(status < 0) {
		printf("socket error: [%s]\n", strerror(errno));
		exit(-1);
	}
}

int handleNewConnection(int chatSocket);

int main(int argc, char* argv[]) {
	// Create a socket
	int sid = socket(PF_INET, SOCK_STREAM, 0);

	//Setup our address - will listen on port 8090
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(8090);
	addr.sin_addr.s_addr = INADDR_ANY;

	//Pairs the newly created socket with the requested address
	int status = bind(sid, (struct sockaddr*)&addr, sizeof(addr));
	checkError(status);

	//Listen on that socket for let's talk message i.e. TCP connection requests
	status = listen(sid, 10);
	checkError(status);

    int stayingAlive = 1;
	while(stayingAlive) {                      /* can we terminate the server neatly? */
		struct sockaddr_in client;
		socklen_t clientSize;

		//Accept a TCP connection request
		int chatSocket = accept(sid, (struct sockaddr*)&client, &clientSize);
		checkError(chatSocket);
		printf("We accepted a socket: %d\n", chatSocket);

		//Create a new process to handle the new client
		pid_t child = fork();
		if(child==0) {
			int shouldIDie = handleNewConnection(chatSocket);
            printf("Should I die? : %d\n", shouldIDie);
			close(chatSocket);
			return shouldIDie? 2 : 1; //Report that the child died voluntarily
		} else if(child > 0) {
			printf("Created a child: %d\n", child);
			close(chatSocket);
			int status = 0;
			pid_t deadChild; //Reap the dead children (as long as we find some)
			do {
				deadChild = waitpid(0, &status, WNOHANG);
				checkError(deadChild);
                int childStatus = WEXITSTATUS(status);
				stayingAlive = childStatus != 2;
                if(deadChild>0) 
					printf("Reaped %d with status %d\n", deadChild, childStatus);
			}while(deadChild>0);
		}
	}
    close(sid);
	return 0;
}

int handleNewConnection(int chatSocket) {
	// This is a child process of the server
	int done = 0;
	char buf[256];
	char msg[] = "Howdy!";
	int status = recv(chatSocket, &buf, sizeof(buf), 0);
	checkError(status);
	printf("Client said: [%s]\n", buf);
	printf("I'm responding: [%s]\n", msg);
	int rem = strlen(msg)+1, sent=0;
	while(rem!=0) {
		status = send(chatSocket, msg+sent, rem, 0);
		rem -= status;
		sent +=status;
	}
    return strcmp(buf, "die!") == 0;
}
