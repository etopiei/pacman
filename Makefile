LDFLAGS=-lncurses
WARN=-Wall -Wextra

all: pacman.o ghost.o
	g++ -o main pacman.o ghost.o main.cpp $(LDFLAGS) $(WARN)

pacman.o:
	g++ -c pacman.cpp $(WARN)

ghost.o:
	g++ -c ghost.cpp $(WARN)

run: all
	./main

clean:
	rm -f main
	rm -f *.o
