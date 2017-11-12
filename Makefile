LDFLAGS=-lncurses

all: pacman.o ghost.o
	g++ -o main pacman.o ghost.o main.cpp $(LDFLAGS)

pacman.o:
	g++ -c pacman.cpp -Wall -Wextra

ghost.o:
	g++ -c ghost.cpp -Wall -Wextra

run: all
	./main

clean:
	rm -f main
	rm -f *.o
