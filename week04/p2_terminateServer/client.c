#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>

void checkError(int status) {
	if(status < 0) {
		printf("socket error: [%s]\n", strerror(errno));
		exit(-1);
	}
}

int main(int argc, char* argv[]) {
	// Create a socket
	int sid = socket(PF_INET, SOCK_STREAM, 0);

	//Specify the address of the server
	struct sockaddr_in srv;
	struct hostent *server = gethostbyname("localhost");
	srv.sin_family = AF_INET;
	srv.sin_port = htons(8090);
	memcpy(&srv.sin_addr.s_addr, server->h_addr, server->h_length);

	//Request to connect with the server
	int status = connect(sid, (struct sockaddr*)&srv, sizeof(srv));
	checkError(status);

	//Send a packet to the server
	char buf[] = "Hello server!";
    char die[] = "die!";
    if(argc > 1) {
        int rem = strlen(die)+1, sent=0;
        while(rem!=0) {
            status = send(sid, die+sent, rem,0);
            checkError(status);
            sent += status;
            rem -= status;
        }
    } else {
        int rem = strlen(buf)+1, sent=0;
        while(rem!=0) {
            status = send(sid, buf+sent, rem,0);
            checkError(status);
            sent += status;
            rem -= status;
        }
    }
	char asw[256];
	status = recv(sid, asw, sizeof(asw),0); /* Do we not need something similar for receive? - emphasizes the need for a protocol */
	printf("Response was: [%s]\n", asw);
	return 0;
}
