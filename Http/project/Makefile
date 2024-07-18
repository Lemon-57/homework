all:test server
server:main.o hashValue.o
	g++ $^ -o $@ -lwfrest -lworkflow -lssl -lcrypto -lcrypt
main.o:main.cc
	g++ -c $^ -o $@ -g -Wall
test:test.o hashValue.o
	g++ $^ -o $@ -lwfrest -lworkflow -lssl -lcrypto
test.o:test.cc
	g++ -c $^ -o $@ -g -Wall
hashValue.o:hashValue.cc
	g++ -c $^ -o $@ -g -Wall
clean:
	$(RM) server main.o