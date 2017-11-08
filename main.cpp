#include <iostream>
#include <ncurses.h>
#include <unistd.h>
#include <curses.h>
#include "pacman.h"

#define DELAY = 2000;

struct position {
	int x;
	int y; 
};

position convertBoardPosition(int x, int y) {
	int boardY = y + 2;
	int boardX = 3*x+4;
	return {boardX, boardY};
}

void setupMaze() {
	attron(COLOR_PAIR(6));
	mvprintw(0, 1, "Pacman - Press 'q' to quit");
	attron(COLOR_PAIR(1));
	for(int i = 2; i < 13; i++) {
		if(i != 7) {
			mvprintw(i, 1, " ");
			mvprintw(i, 2, " ");
			mvprintw(i, 3*19, " ");	
			mvprintw(i, 3*19+1, " ");				
		}
	}
	for(int i = 1; i < 19; i++) {
		mvprintw(1, 3*i, " ");
		mvprintw(1, (3*i+1), " ");
		mvprintw(1, (3*i+2), " ");
		mvprintw(13, (3*i), " ");
		mvprintw(13, (3*i+1), " ");
		mvprintw(13, (3*i+2), " ");
	}
	mvprintw(1, 1, " ");
	mvprintw(1, 2, " ");
	mvprintw(13, 1, " ");
	mvprintw(13, 2, " ");
	mvprintw(1, 3*19, " ");
	mvprintw(1, 3*19+1, " ");
	mvprintw(13, 3*19, " ");
	mvprintw(13, 3*19+1, " ");

	attron(COLOR_PAIR(2));

	for(int i = 2; i < 13; i++) {
		for(int j = 4; j < 3*19; j+=3) {
			mvaddch(i, j, ACS_BULLET);		
		}
	}
	
	refresh();
}

void updatePacman(Pacman *player) {

	int pacmanCharacter = 0;

	int direction = player->getDirection();
	int nextDirection = player->getNextDirection();
	int x = player->getX();
	int y = player->getY();

	//check here if the new direction is valid yet, if it isn't give it one more chance, else remove it

	//if it is valid switch direction
	//for now just allow switching direction whenever you want
	if(direction != nextDirection) {
		player->changeDirection();		
	}


	int xChange = 0;
	int yChange = 0;
	switch (direction) {
		case 1:
			pacmanCharacter = ACS_DARROW;
			xChange = 0;
			yChange = 1;
			break;
		case 2:
			pacmanCharacter = ACS_UARROW;
			xChange = 0;
			yChange = -1;
			break;
		case 3:
			pacmanCharacter = ACS_LARROW;
			xChange = -1;
			yChange = 0;
			break;
		case 4:
			pacmanCharacter = ACS_RARROW;
			xChange = 1;
			yChange = 0;
			break;
	}

	//now that we have a valid direction for pacman, pacman needs to move, and update it's classes position.
	player->changeCoordinates(xChange, yChange);

	//re-write square pacman was on to be blank (other checks, for ghosts etc. will go here later.)
	attron(COLOR_PAIR(2));
	position oldPosition = convertBoardPosition(x, y);
	mvprintw(oldPosition.y, oldPosition.x, " ");

	//print pacman in a position on the board.
	position pacmanPosition = convertBoardPosition(x+xChange, y+yChange);
	attron(COLOR_PAIR(6)); 
	mvaddch(pacmanPosition.y, pacmanPosition.x, pacmanCharacter);
	refresh();
	
}

bool checkForChangeMovement(int userInput, Pacman *player) {
	if(userInput == 'q') {
		return false;		
	} else if(userInput > 257 && userInput < 262) {
		player->setDirection(userInput-257);
	}
	return true;
}

int main() {

	initscr();
	
	start_color();
	init_pair(1, COLOR_BLACK, COLOR_BLUE);
	init_pair(2, COLOR_WHITE, COLOR_BLACK);
	init_pair(3, COLOR_BLACK, COLOR_GREEN);
	init_pair(4, COLOR_BLACK, COLOR_RED);
	init_pair(5, COLOR_BLACK, COLOR_MAGENTA);
	init_pair(6, COLOR_BLACK, COLOR_YELLOW);
	
	noecho();
	curs_set(FALSE);
	nodelay(stdscr, TRUE);
	keypad(stdscr, TRUE);
	
	bool playing = true;
	Pacman player = Pacman();
	setupMaze();
	int step = 0;
	while(playing) {
		int userInput = getch();
		if(userInput > 0) {
			playing = checkForChangeMovement(userInput, &player);
		}
		if(step%5000 == 0) {
			updatePacman(&player);		
		}
		sleep(0.1);
		step++;
	}

	endwin();
	return 0;	
}
