/* Sending a TCP connect request to the server and sending a single packet */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

void checkError(int status) {
    if(status < 0) {
        printf("Socket error (%d): [%s]\n", getpid(), strerror(errno));
        exit(-1);
    }
}

int main(int argc, char* argv[]) {
    // Create a TCP socket
    int sid = socket(PF_INET, SOCK_STREAM, 0);  // creates a TCP socket for communication

    // Set the address of the server - address family, IP address and port number
    struct sockaddr_in srv;
    struct hostent *server = gethostbyname("localhost"); // mapping hostname to IP address using DNS
    srv.sin_family = AF_INET;
    srv.sin_port = htons(8080); // because we have set the port # in server as 8080
    memcpy(&srv.sin_addr.s_addr, server->h_addr_list, server->h_length);

    // Send the connect request
    int status = connect(sid, (struct sockaddr*)&srv, sizeof(srv));
    checkError(status);

    // Send a message to the server
    char buf[] = "Hello server!";
    status = send(sid, buf, sizeof(buf), 0); /* can we send the entire payload in a single packet? */
    checkError(status);

    // Receive a message from the server
    char asw[256];
    status = recv(sid, asw, sizeof(asw), 0);
    checkError(status);
    printf("Response was: [%s]\n", asw);

    return 0;
}
