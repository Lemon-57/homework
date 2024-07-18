SRCS:=$(wildcard *.c)
OBJECTS:=$(patsubst %.c, %, $(SRCS))
CC:=gcc
LIBS:=-lpthread


all:server client

server:
	$(CC) server.c tcp.c -o $@

client:
	$(CC) client.c tcp.c -o $@
#%:%.c
#	$(CC) $^ -o $@ $(LIBS) -g
print:
	echo $(OBJECTS)
clean:
	rm -rf server client

rebuild:
	make clean && make
