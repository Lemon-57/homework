# srcs:=$(wildcard *.cc)
# exes:=$(srcs:%.cc=%)
# all:$(exes)
# %:%.cc
server:main.0
	g++ $^ -o $@ -lwfrest -lworkflow
main.o:main.cc
	g++ -c $^ -o $@ -g -Wall
clean:
	$(RM) server main.o
