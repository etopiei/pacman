LDFLAGS=-lncurses

all: pacman.o
	g++ -o main pacman.o main.cpp $(LDFLAGS)

pacman.o:
	g++ -c pacman.cpp -Wall -Wextra

clean:
	rm -f main
	rm -f *.o
