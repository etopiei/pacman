class Ghost {

	public:
		Ghost(int type);
		int getGhostX();
		int getGhostY();
		int getGhostDirection();
		void changeGhostDirection(int newDirection);
        void changePosition(int changeX, int changeY);
		

	private:
		int x, y, direction;
		bool active, edible;

};
