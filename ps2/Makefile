CC=cc
CFLAGS=-g 
LFLAGS=

all: analyze wgethtml wgettext

analyze: main.o analyze.o
	$(CC) $(CFLAGS) $^ -o $@

wgethtml: wgethtml.o analyze.o
	$(CC) $(CFLAGS) $^ -o $@

wgettext : wgettext.o analyze.o
	$(CC) $(CFLAGS) $^ -o $@

%.o : $.c
	$(CC) $(CFLAGS) -c $<

prereqs:
	sudo apt install -y nginx html2text

clean: 
	rm -rf *.o wgettext wgethtml analyze *~
