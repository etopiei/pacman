class Pacman {
	public:
		Pacman();
		int getDirection();
		int getX();
		int getY();
		void setDirection(int newDirection);
		void changeDirection();
		int getNextDirection();
		void changeCoordinates(int xChange, int yChange);
		
	private:
		int x;
		int y;
		int direction;
		int nextDirection;
};
