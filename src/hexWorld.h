/**********************
Hex Research Project

author: Michael McCarver
advisor: Dr. Rob LeGrand
**********************/

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
	vector<hexGamePlayer> hexGamePlayers;

	void geneticAlgorithm(); // Called by nextGeneration to determine weights for the new generation

public:
	hexWorld(int numPlayers){this->numPlayers = numPlayers;}

	// Some sort of void function that creates the next generation of players.
	// If this is the first iteration (no existing players), create hexGamePlayers
	void nextGeneration();

	// Function to return a vector of all neighboring hexGamePlayers.
	// Takes an int location as a parameter (which hexGamePlayer we are
	// currently centered on). To be used by the playGame function.
	vector<int> getNeighbors(int hexPlayerLocation);

	// Return a hexGamePlayer from vector hexGamePlayers
	hexGamePlayer getHexGamePlayer(int playerLocation) {return hexGamePlayers[playerLocation];}
};

#endif // #ifndef HEXWORLD_H
