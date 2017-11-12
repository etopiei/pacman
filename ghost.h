class Ghost {

	public:
		Ghost(int type);
		int getGhostX();
		int getGhostY();
		int getGhostDirection();
		void changeGhostDirection(int newDirection);
		

	private:
		int x, y, direction;
		bool active, edible;

};
