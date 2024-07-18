SRCS:=$(wildcard *.cc)
OUTS:=$(patsubst %.cc, %, $(SRCS))
CC:=g++

.PHONY: all clean rebuild

all: $(OUTS) 

%: %.cpp
	$(CC) $^ -o $@ -g -Wall 

clean:
	$(RM) $(OUTS)

rebuild: clean all
	

