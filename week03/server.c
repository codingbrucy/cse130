/* Accepting a TCP connect request and sending a packet to the client */ 

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <dirent.h>
#include<sys/wait.h>

void checkError(int status) {
    if(status < 0) {
        printf("Socket error (%d): [%s]\n", getpid(), strerror(errno));
        exit(-1);
    }
}

void handleNewConnection(int chatSocket); /* takes the fd of the socket created for a client */

int main(int argc, char* argv[]) {
    // Create a TCP socket
    int sid = socket(PF_INET, SOCK_STREAM, 0);  // creates a TCP socket for welcoming connect requests
    
    //Set up our address -- will listen on port 8080 --
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;      // uses the IPv4 address family
    addr.sin_port = htons(8080);    // converts unsigned short int from host byte order to network byte order.
    addr.sin_addr.s_addr = INADDR_ANY; //set any IP on this machine

    // Pair the newly created socket with the requested address
    int status = bind(sid, (struct sockaddr*)&addr, sizeof(addr));
    checkError(status); // if < 0, implies socket binding has failed

    // Listen on that socket for the 'let's talk message'. 
    status = listen(sid, 10); // no more than 10 pending requests at once
    checkError(status);

    // Continuously run and accept incoming requests. For each request, create a separate comm socket
    while(1) {
        struct sockaddr_in client; // to store the client addr. Useful to send a response
        socklen_t clientSize;
        
        int chatSocket = accept(sid, (struct sockaddr*)&client, &clientSize); // blocking call
        checkError(status);
        printf("We accepted a socket: %d\n", chatSocket);

        pid_t value = fork(); // create a child process to communicate with the client
        if(value == 0) { //child process
            handleNewConnection(chatSocket);
            close(chatSocket);
            return 1; /* report that child died voluntarily */
        } else if (value > 0) { // parent process
            printf("Created a child with pid: %d\n", value);
            close(chatSocket); // close the chatSocket in the parent
            int status = 0;
            pid_t deadChild;
            do {
                deadChild = waitpid(0, &status, WNOHANG); // non-blocking call by setting WNOHANG
                checkError(deadChild);
                if(deadChild > 0)
                    printf("Reaped %d\n", deadChild);
            } while(deadChild > 0);
        }
    }
    return 0;
}

void handleNewConnection(int chatSocket) {
    // This is the child process
    //int done = 0;
    char buf[256];
    char msg[] = "Howdy!";
    int status = recv(chatSocket, &buf, sizeof(buf), 0);
    checkError(status);
    printf("Client said: [%s]\n", buf);
    printf("I'm responding: [%s]\n", msg);
    status = send(chatSocket, msg, strlen(msg)+1, 0); /* can we send the entire payload in a single packet? */
    checkError(status);

    return;
}
