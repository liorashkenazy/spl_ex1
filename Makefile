# Generic target
all: studiomgmt

studiomgmt: bin/main.o
	g++ -o bin/studiomgmt bin/main.o

bin/main.o: src/main.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -I include -o bin/main.o src/main.cpp

clean:
	rm -f bin/*