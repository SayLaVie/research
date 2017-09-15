/**************************
This function creates and maintains a population of hexGamePlayers.
We may have a private evolution function within this class to
handle the evolution of new generations (or simply create a new world
when needed). This class will probably need to maintain the neighbor
relationships (used to determine which hexGamePlayers will play against
each other).
**************************/

#ifndef HEXWORLD_H
#define HEXWORLD_H

#include "hexGamePlayer.h"

class hexWorld
{
private:
	int numPlayers;

	void geneticAlgorithm(); // Called by nextGeneration to determine weights for the new generation

public:
	vector<hexGamePlayer> hexGamePlayers;

	hexWorld(int numPlayers){this->numPlayers = numPlayers;}

	// Some sort of void function that creates the next generation of players
	void nextGeneration();

	// Function to return a vector of all neighboring hexGamePlayers.
	// Takes an int location as a parameter (which hexGamePlayer we are
	// currently centered on). To be used by the playGame function.
	vector<int> getNeighbors(int hexPlayerLocation);
};

#endif // #ifndef HEXWORLD_H
