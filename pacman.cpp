#include "pacman.h"

Pacman::Pacman() {
	direction = 1;
	nextDirection = 1;
	x = 3;
	y = 4; 
}

void Pacman::changeCoordinates(int xChange, int yChange) {
	x = x + xChange;
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


