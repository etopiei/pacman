LDFLAGS=-lncurses

all: main
	g++ -o main main.cpp $(LDFLAGS)
clean:
	rm -f main
	rm -f *.o
