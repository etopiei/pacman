#include <iostream>
#include <ncurses.h>
#include <unistd.h>
#include <fstream>
#include <curses.h>
#include <vector>
#include <time.h>
#include <stdlib.h>

#include "ghost.h"
#include "pacman.h"

#define DELAY = 2000;

std::vector<std::vector<int> > walls;

struct position {
	int x;
	int y; 
};

position convertBoardPosition(int x, int y) {
	int boardY = y + 2;
	int boardX = 3*x+4;
	position boardPosition;
	boardPosition.x = boardX;
	boardPosition.y = boardY;
	return boardPosition;
}

std::vector<std::vector<bool> > setupMaze(std::vector<Ghost*> ghosts) {
	attron(COLOR_PAIR(6));
	mvprintw(0, 1, "Pacman - Press 'q' to quit");
	attron(COLOR_PAIR(1));
	for(int i = 2; i < 13; i++) {
		if(i != 7) {
			mvprintw(i, 1, " ");
			mvprintw(i, 2, " ");
			mvprintw(i, 3*17, " ");	
			mvprintw(i, 3*17+1, " ");				
		}
	}
	for(int i = 1; i < 17; i++) {
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
	mvprintw(1, 3*17, " ");
	mvprintw(1, 3*17+1, " ");
	mvprintw(13, 3*17, " ");
	mvprintw(13, 3*17+1, " ");

	std::vector<std::vector<bool> > board;
	for(int i = 0; i < 11; i++) {
		std::vector<bool> dotRow;
		for(int j = 0; j < 16; j++) {
			dotRow.push_back(true);
		}
		board.push_back(dotRow);
	}

	//read in walls here
	std::ifstream myFile;
	myFile.open("walls.txt");

	int wallX, wallY;

	int counter = 0;
	std::vector<int> currentWall;

	while(true) {
		attron(COLOR_PAIR(1));
		//take each wall coordinate and color that square blue
		myFile >> wallX >> wallY;
		//update walls vector
		if(wallY > counter) {
			walls.push_back(currentWall);
			counter++;
			std::vector<int>().swap(currentWall);		
		}
		currentWall.push_back(wallX);
		board[wallY][wallX] = false;
		//first translate the board coordinate to a screen coordinate
		position wallBlock = convertBoardPosition(wallX, wallY);
		mvprintw(wallBlock.y, wallBlock.x, " ");
		mvprintw(wallBlock.y, wallBlock.x-1, " ");
		mvprintw(wallBlock.y, wallBlock.x+1, " ");
		if(wallX == 12 && wallY == 10) {
			walls.push_back(currentWall);
			break;		
		}
	}

	
	
	board[4][6] = false;
	board[4][7] = false;
	board[4][8] = false;
	board[4][9] = false;
	board[3][6] = false;
	board[3][7] = false;
	board[3][8] = false;
	board[3][9] = false;	

	//now write dots to screen
	attron(COLOR_PAIR(2));
	for(unsigned int i = 0; i < board.size(); i++) {
		for(unsigned int j = 0; j < board[i].size(); j++) {
			if(board[i][j]) {
				position dotPosition = convertBoardPosition(j, i);
				mvaddch(dotPosition.y, dotPosition.x, ACS_BULLET);	
			}
		}
	}

	//now add ghosts
	for(unsigned int i = 0; i < ghosts.size(); i++) {
		attron(COLOR_PAIR(i+2));
		int ghostX = ghosts[i]->getGhostX();
		int ghostY = ghosts[i]->getGhostY();
		position ghostPosition = convertBoardPosition(ghostX, ghostY);
		mvprintw(ghostPosition.y, ghostPosition.x, "G");
	}
		
	refresh();
	return board;
}

bool checkForWall(int x, int y) {
	for(unsigned int i = 0; i < walls[y].size(); i++) {
		if(walls[y][i] == x) {
			return true;		
		}		
	}
	return false;
}

bool validBoard(int x, int y) {
	return (x >= 0 && y >= 0 && x < 16 && y < 11) || (y==5);
}

bool validGhostBoard(int x, int y) {
	return (x >= 0 && y >= 0 && x < 16 && y < 11);
}

bool validGhostDirection(int nextDirection, int x, int y) {
	int xChange = 0;
    int yChange = 0;
 	switch (nextDirection) {
 		case 1:
 			yChange = 1;
 			break;
 		case 2:
 			yChange = -1;
 			break;
 		case 3:
 			xChange = -1;
 			break;
 		case 4:
 			xChange = 1;
 			break; 
 	}
 	return (validGhostBoard(x + xChange, y + yChange) && !checkForWall(x + xChange, y + yChange)); 
}

bool validNextDirection(int nextDirection, int x, int y) {
	int xChange, yChange;
	switch (nextDirection) {
		case 1:
			xChange = 0;
			yChange = 1;
			break;
		case 2:
			xChange = 0;
			yChange = -1;
			break;
		case 3:
			xChange = -1;
			yChange = 0;
			break;
		case 4:
			xChange = 1;
			yChange = 0;
			break; 
	}
	return (validBoard(x + xChange, y + yChange) && !checkForWall(x + xChange, y + yChange));
}

void writeOldGhostPosition(int x, int y, std::vector<std::vector<bool> > *board) {
	attron(COLOR_PAIR(2));
	position dotPosition = convertBoardPosition(x, y);
	if((*board)[y][x]) {
		mvaddch(dotPosition.y, dotPosition.x, ACS_BULLET);
	} else {
		mvaddch(dotPosition.y, dotPosition.x, ' ');	
	}	
}

bool moveGhosts(std::vector<Ghost*> *ghosts, Pacman* player, std::vector<std::vector<bool> > *board) {
    int pacmanX = player->getX();
    int pacmanY = player->getY();
    bool gameOver = false;
	//loop through all ghosts and pick a direction to move in.
	for(unsigned int i = 0; i < ghosts->size(); i++) {

		int direction = (*ghosts)[i]->getGhostDirection();
		int x = (*ghosts)[i]->getGhostX();
		int y = (*ghosts)[i]->getGhostY();
        int nextDirection = direction;
        int changeInX = 0;
        int changeInY = 0;

        unsigned int notAllowed = 1;

        switch(direction) {
            case 1:
                notAllowed = 2;
                break;
            case 2:
                notAllowed = 1;
                break;
            case 3:
                notAllowed = 4;
                break;
            case 4:
                notAllowed = 3;
                break;
        }	

        std::vector<int> possible;
        for(unsigned int j = 1; j < 5; j++) {
            if(j != notAllowed && validGhostDirection(j, x, y)) {
                possible.push_back(j);
            }
        }

        if(possible.size() == 0) {
            //ghost has nowhere to go, allow them to turn around (maybe should redesign board)
            nextDirection = notAllowed;
        } else {
            nextDirection = possible[rand()%possible.size()];
        }

        switch(nextDirection) {
            case 1:
                changeInY = 1;
                break;
            case 2:
                changeInY = -1;
                break;
            case 3:
                changeInX = -1;
                break;
            case 4:
                changeInX = 1;
                break;
        }

        attron(COLOR_PAIR(i+3));
        writeOldGhostPosition(x, y, board);
        (*ghosts)[i]->changePosition(changeInX, changeInY);
        (*ghosts)[i]->changeGhostDirection(nextDirection);
        position ghostPosition = convertBoardPosition(x+changeInX, y+changeInY);
        mvprintw(ghostPosition.y, ghostPosition.x, "G");

        if(x+changeInX == pacmanX && y+changeInY == pacmanY) {
            gameOver = true;
        }
	
	}

    refresh();	
    return gameOver;
}

void updatePacman(Pacman *player, std::vector<std::vector<bool> > *board) {

	int pacmanCharacter = 0;
	int direction = player->getDirection();
	int nextDirection = player->getNextDirection();
	int x = player->getX();
	int y = player->getY();

	//update board so current position no longer has dot
    if(x > 0 && x < 16) {
    	(*board)[y][x] = false;
    }
	//if it is valid switch direction
	if(direction != nextDirection && validNextDirection(nextDirection, x, y)) {
		player->changeDirection();		
		direction = player->getDirection();
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

	//is the new x and y valid? if so continue, else keep old position
	if(validBoard(x + xChange, y + yChange) && !checkForWall(x + xChange, y + yChange)) {
		//now that we have a valid direction for pacman, pacman needs to move, and update it's classes position.
		player->changeCoordinates(xChange, yChange);

		//re-write square pacman was on to be blank (other checks, for ghosts etc. will go here later.)
		attron(COLOR_PAIR(2));
		position oldPosition = convertBoardPosition(x, y);
		mvprintw(oldPosition.y, oldPosition.x, " ");

		position pacmanPosition;
		//print pacman in a position on the board.
		if(x+xChange == -1) {
			pacmanPosition = convertBoardPosition(16, y+yChange);
		} else if(x+xChange == 16) {
			pacmanPosition = convertBoardPosition(-1, y+yChange);
		} else {
			pacmanPosition = convertBoardPosition(x+xChange, y+yChange);
		}
		attron(COLOR_PAIR(6)); 
		mvaddch(pacmanPosition.y, pacmanPosition.x, pacmanCharacter);
		refresh();
	}
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
    srand(time(NULL));
	
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
	Ghost ghost1 = Ghost(1);
    Ghost ghost2 = Ghost(2);
	Ghost ghost3 = Ghost(3);
	std::vector<Ghost*> ghosts;
	ghosts.push_back(&ghost1);
	ghosts.push_back(&ghost2);
	ghosts.push_back(&ghost3);
	std::vector<std::vector<bool> > board = setupMaze(ghosts);
	int step = 0;
	while(playing) {
		int userInput = getch();
		if(userInput > 0) {
			playing = checkForChangeMovement(userInput, &player);
		}
		if(step%9000 == 0) {
			updatePacman(&player, &board);
			playing = !moveGhosts(&ghosts, &player, &board);		
		}
		usleep(20);
		step++;
	}

	endwin();
	return 0;	
}
