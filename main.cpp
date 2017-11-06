#include <iostream>
#include <ncurses.h>
#include <unistd.h>

#define DELAY = 2000;

struct position {
	int x;
	int y; 
};

position convertBoardPosition(int x, int y) {
	int boardX = x + 2;
	int boardY = 3*y+1;
	return {boardX, boardY};
}

void setupMaze() {
	attron(COLOR_PAIR(6));
	mvprintw(0, 1, "Pacman");
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

	setupMaze();
	sleep(6);
	endwin();
	
}