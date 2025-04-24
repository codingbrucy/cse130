CLI = client.o
SRV = server.o
OBJS = 
CFLAGS = -g 

all: server client

client: $(CLI) $(OBJS)
	$(CC) $(CFLAGS) $(LFLAGS) -o $@ $^

server: $(SRV) $(OBJS)
	$(CC) $(CFLAGS) $(LFLAGS) -o $@ $^

clean:
	rm -rf *.o server client *~
