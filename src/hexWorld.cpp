/**********************
Hex Research Project

author: Michael McCarver
advisor: Dr. Rob LeGrand
**********************/


/*************************************
   NOTES
After talking with Dr Legrand, the weights for the neural
net should be doubles between -1 and 1, close to 0.
I can use the <random> class, and maybe choose a distribution
that clumps values closer to 1.
	- exponential_distribution
	- geometric_distribution
	- lognormal_distribution
	- fisher_f_distribution
	- piecewise_linear_distribution (with our own set probabilities)
*************************************/

#include "hexWorld.h"
#include <ctime>
#include <random>

// This function sets the weights for the neural net
void hexWorld::nextGeneration()
{
	// netShape keeps track of the shape of the neuralNet.
	// netWeights is the actual set of weights
	// rowOriginationVector is a helper vector for creating netWeights
	// rowDestinationVector is also a helper vector
	vector<int> netShape;
	vector<vector<vector<double> > > netWeights;
	vector<double> rowOriginationVector;
	vector<vector<double> > rowDestinationVector;

	int layer, rowDestination, rowOrigination, player;

	// Default random engine to be used as a input for other generators
	default_random_engine seed(time(NULL));

	// Bernoulli_distribution is effectively a coin toss. Returns true or false
	bernoulli_distribution coinToss(0.5);


	// Push the depths of each layer into the netShape vector
	// Not sure yet what our first network shape will be,
	// setting it as <3, 2, 1> for now.
	netShape.push_back(3);
	netShape.push_back(2);
	netShape.push_back(1);

	// Check if this is the first generation (start with randomized weights)
	if (hexGamePlayers.size() == 0)
	{
		// Initiate exponential_distribution PRNG. As of now, output will not be
		// confined between 0 and 1. 3.5 is chosen (from cplusplus.com) as it seems
		// to have a good distribution for our use. Flip a coin, and make negative accordingly.
		exponential_distribution<double> weightGenerator(3.5);
		double currentWeight;

		for (int player = 0; player < NUM_PLAYERS; ++player)
		{
			netWeights.clear();

			// Create the vector of weights for the new players
			for (layer = 0; layer < netShape.size() - 1; ++layer)
			{
				rowDestinationVector.clear();

				for (rowDestination = 0; rowDestination < netShape[layer + 1]; ++rowDestination)
				{
					rowOriginationVector.clear();

					for (rowOrigination = 0; rowOrigination < netShape[layer]; ++rowOrigination)
					{
						// Generate random weight
						currentWeight = weightGenerator(seed);
						currentWeight *= (coinToss(seed) ? 1 : -1);

						// Add new weight to vector
						rowOriginationVector.push_back(currentWeight);
					}

					rowDestinationVector.push_back(rowOriginationVector);
				}

				netWeights.push_back(rowDestinationVector);
			}

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
