SRCS:=$(wildcard *.cpp)
OUTS:=$(patsubst %.cpp, %, $(SRCS))
CC:=g++

.PHONY: all clean rebuild

all: $(OUTS) 

%: %.cpp
	$(CC) $^ -o $@ -g -Wall -lpthread 

clean:
	$(RM) $(OUTS)

rebuild: clean all
	

