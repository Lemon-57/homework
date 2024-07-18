srcs:=$(wildcard *.cc)
exes:=$(srcs:%.cc=%)
all:$(exes)
%:%.cc
	g++ $^ -o $@ -g -Wall -lworkflow
clean:
	$(RM) $(exes)
rebuild:clean all
