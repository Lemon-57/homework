SRCS:=$(wildcard *.c)
OUTS:=$(patsubst %.c, %, $(SRCS))
CC:=gcc


.PHONY: all clean rebuild

all: server client 

server:	
	$(CC) server.c tcp.c -o $@ -lpthread
#%: %.c
#	$(CC) $^ -o $@ -lpthread
client:
	$(CC) client.c tcp.c -o $@ -lpthread
clean:
	$(RM) $(OUTS)

rebuild: clean all
	

