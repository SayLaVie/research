/**********************
Hex Research Project

author: Michael McCarver
advisor: Dr. Rob LeGrand
**********************/

#include "hexWorld.h"

// This function sets the weights for the neural net
void hexWorld::nextGeneration()
{
	// This array sets the shape of the neural net
	// Each entry is the depth of each consecutive layer
	vector<int> netShape;
	// This vector of vectors of vectors keeps track of the weights
	vector<vector<vector<int> > > netWeights;
	int layer, row, player;

	// Push the depths of each layer into the netShape vector
	// Not sure yet what our first network shape will be,
	// setting it as <3, 2, 1> for now.
	netShape.push_back(3);
	netShape.push_back(2);
	netShape.push_back(1);

	// Check if this is the first generation (make randomized weights)
	if (hexGamePlayers.size() == 0)
	{
		// Create a skeleton vector for the shape of the neural net
		for (layer = 0; layer < netShape.size() - 1; ++layer)
		{
			for (row = 0; row < netShape[layer]; ++row)
			{

			}
		}

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
