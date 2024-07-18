SRCS:=$(wildcard *.c)
OBJS:=$(patsubst %.c,%.o,$(SRCS))
ELF:=client
CC:=gcc
CFLAGS:=-g -Wall 
$(ELF):$(OBJS)
	gcc -o $@ $^ -I /usr/include/mysql -lmysqlclient -pthread -lcrypt 
clean:
	rm -rf $(OBJS) $(ELF)
