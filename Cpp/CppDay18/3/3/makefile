SRCS:=$(wildcard *.cpp)
OUTS:=$(patsubst %.cpp, %, $(SRCS))
CC:=g++

.PHONY: all clean rebuild

all: $(OUTS) 

%: %.cpp
	$(CC) $^ -o $@ -g -Wall -llog4cpp -lpthread

clean:
	$(RM) $(OUTS)

rebuild: clean all
	

