#include "ghost.h"

Ghost::Ghost(int type) {
	y = 4;
	x = 5+type;
	direction = 4;
	if(type != 1) {
		active = false;
	} else {
		active = true;
	}
}

int Ghost::getGhostX() {
	return x;
}

int Ghost::getGhostY() {
	return y;
}

int Ghost::getGhostDirection() {
	return direction;
}

void Ghost::changeGhostDirection(int newDirection) {
	direction = newDirection;
}

void Ghost::changePosition(int changeX, int changeY) {
    x = x + changeX;
    y = y + changeY;
}
