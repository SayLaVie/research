/**********************
Hex Research Project

author: Michael McCarver
advisor: Dr. Rob LeGrand
**********************/

#include "hexWorld.h"

// This function sets the weights for the neural net
void hexWorld::nextGeneration()
{
	// Check if this is the first generation (make randomized weights)
	if (hexGamePlayers.size() == 0)
	{
		for (int player = 0; player < NUM_PLAYERS; ++player)
		{
			hexGamePlayers.push_back(hexGamePlayer(/*random weights*/));
		}
	}
	// Else, we use a genetic algorithm to determine new weights
	else
	{

	}
}

// return vector of neighboring hexPlayers
vector<int> hexWorld::getNeighbors(int hexPlayerLocation)
{

}
