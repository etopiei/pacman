#include "pacman.h"

Pacman::Pacman() {
	direction = 3;
	nextDirection = 3;
	x = 7;
	y = 8; 
}

void Pacman::changeCoordinates(int xChange, int yChange) {
	x = x + xChange;
	if(x == -1) {
		x = 16;
	} else if (x == 16) {
		x = -1;
	}
	y = y + yChange;
}

int Pacman::getNextDirection() {
	return nextDirection;
}

void Pacman::setDirection(int newDirection) {
	nextDirection = newDirection;
}

void Pacman::changeDirection() {
	direction = nextDirection;
}

int Pacman::getDirection() {
	return direction;	
}

int Pacman::getX() {
	return x;
}

int Pacman::getY() {
	return y;
}


