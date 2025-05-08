2. Code description

1. “Why This Works" explanation: 
in server.c, we run a while loop, we use select on fd_set of sid and srv.
and then we handle each case with checking if one of the fd is set. when sid is set,
it accepts the connection from client server, and forks it self to handle the sql query, 
and reaps zombie in parent process. if srv is set, it checks if it should self-terminate.
in client.c it checks stdin for command/eof to decide if it should send sql query and read 
reply, or terminate itself.
in service.c, it sends input to server.c the string "$die" to terminate server. 