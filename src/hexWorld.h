/**************************
This function creates and maintains a population of hexGamePlayers.
We may have an private evolution function within this class to 
handle the evolution of new generations (or simply create a new world
when needed). This class will probably need to maintain the neighbor
relationships (used to determine which hexGamePlayers will play against
each other).
**************************/

#include "hexGamePlayer.h"

class hexWorld
{
private:	
	int numPlayers;

public:
	hexWorld(int numPlayers){this->numPlayers = numPlayers;}
	
	// Some sort of void function that creates the next generation of players
	// may pass in array of current players. May overload for first generation.
	void nextGeneration(hexGamePlayer hexGamePlayers[]);

	// Function to return a vector of all neighboring hexGamePlayers.
	// Takes an int location as a parameter (which hexGamePlayer we are
	// currently centered on). To be used by the playGame function.
	vector<int> getNeighbors(int hexPlayerLocation);
};
