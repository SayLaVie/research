/**********************
Hex Research Project

author: Michael McCarver
advisor: Dr. Rob LeGrand
**********************/

#include "hexWorld.h"
#include <cstdlib>

// This function sets the weights for the neural net
void hexWorld::nextGeneration()
{
	// This array sets the shape of the neural net
	// Each entry is the depth of each consecutive layer
	vector<int> netShape;
	// This vector of vectors of vectors keeps track of the weights
	vector<vector<vector<double> > > netWeights;
	// These vectors are used for creating the netWeights vector
	vector<double> rowOriginationVector;
	vector<vector<double> > rowDestinationVector;
	int layer, rowDestination, rowOrigination, player;

	// Push the depths of each layer into the netShape vector
	// Not sure yet what our first network shape will be,
	// setting it as <3, 2, 1> for now.
	netShape.push_back(3);
	netShape.push_back(2);
	netShape.push_back(1);

	// Check if this is the first generation (start with randomized weights)
	if (hexGamePlayers.size() == 0)
	{
		// Create a skeleton vector for the shape of the neural net
		for (layer = 0; layer < netShape.size() - 1; ++layer)
		{
			rowDestinationVector.clear();

			for (rowDestination = 0; rowDestination < netShape[layer + 1]; ++rowDestination)
			{
				rowOriginationVector.clear();

				for (rowOrigination = 0; rowOrigination < netShape[layer]; ++rowOrigination)
				{
					// Push random doubles into the vector (randomized weights)
					rowOriginationVector.push_back(randomDouble());
				}

				rowDestinationVector.push_back(rowOriginationVector);
			}

			netWeights.push_back(rowDestinationVector);
		}

		for (int player = 0; player < NUM_PLAYERS; ++player)
		{
			hexGamePlayers.push_back(hexGamePlayer(netWeights));
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

double hexWorld::randomDouble()
{
	return (random() / (double)RAND_MAX);
}
