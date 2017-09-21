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
that clumps values closer to 0.
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
   /**
	- netShape keeps track of the shape of the neuralNet
   - neighbors is a vector of current neighbors
	- netWeights is the actual set of weights
	- rowOriginationVector is a helper vector for creating netWeights
	- rowDestinationVector is also a helper vector
   **/
	vector<int> netShape, neighbors, neighborsGamesWon;
	vector<vector<vector<double> > > netWeights;
	vector<double> rowOriginationVector;
	vector<vector<double> > rowDestinationVector;
	int layer, rowDestination, rowOrigination, player, neighbor, gamesWon, breederChoice;
	double currentWeight;
	vector<hexGamePlayer> newHexGamePlayers;

/*****************************************************************************************
	PRNG's
*****************************************************************************************/
	default_random_engine seedGenerator(time(NULL));

	// Bernoulli_distribution is effectively a coin toss. Returns true or false.
	bernoulli_distribution coinToss(0.5);

	// Initiate exponential_distribution PRNG. As of now, output will not be
	// confined between 0 and 1. 3.5 is chosen (from cplusplus.com) as it seems
	// to have a good distribution for our use.
	exponential_distribution<double> weightGenerator(3.5);
/*********************************************************************************************/

   /**
	Push the depths of each layer into the netShape vector
	Not sure yet what our first network shape will be,
	setting it as <BOARD_SIZE, 2, 1> for now.
   **/
	netShape.push_back(BOARD_SIZE);
	netShape.push_back(2);
	netShape.push_back(1);

	// Check if this is the first generation (start with randomized weights)
	if (hexGamePlayers.size() == 0)
	{
		for (player = 0; player < NUM_PLAYERS; ++player)
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
						// Generate random weight. Flip a coin to see if it needs to be negative
						currentWeight = weightGenerator(seedGenerator);
						currentWeight *= (coinToss(seedGenerator) ? 1 : -1);

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
		newHexGamePlayers.clear();

		for (player = 0; player < NUM_PLAYERS; ++player)
		{
			netWeights.clear();
			neighbors = getNeighbors(player);

			for (layer = 0; layer < netShape.size() - 1; ++layer)
			{
				rowDestinationVector.clear();

				for (rowDestination = 0; rowDestination < netShape[layer + 1]; ++rowDestination)
				{
					rowOriginationVector.clear();

					for (rowOrigination = 0; rowOrigination < netShape[layer]; ++rowOrigination)
					{
						// This is where the fitness decisions take place

						// If coin toss says to keep weight
						if (coinToss(seedGenerator))
						{
							currentWeight = getHexGamePlayer(player).getWeight(layer, rowDestination, rowOrigination);
						}

						// Else, choose current weight based on fitness function
						else
						{
							neighborsGamesWon.clear();
							breederChoice = 0;

							// Make a vector with each neighbor's numGamesWon
							for (neighbor = 0; neighbor < neighbors.size(); ++neighbor)
							{
								gamesWon = getHexGamePlayer(neighbor).getGamesWon();
								neighborsGamesWon.push_back(gamesWon);
							}

							// Use the getBreeder function to determine whose genes to use
							breederChoice = getBreeder(seedGenerator, neighborsGamesWon);

							currentWeight = getHexGamePlayer(neighbors[breederChoice]).getWeight(layer, rowDestination, rowOrigination);
						}

						rowOriginationVector.push_back(currentWeight);
					}

					rowDestinationVector.push_back(rowOriginationVector);
				}

				netWeights.push_back(rowDestinationVector);
			}

			newHexGamePlayers.push_back(netWeights);
		}

		hexGamePlayers = newHexGamePlayers;
	}
}

// return vector of neighboring hexPlayers
vector<int> hexWorld::getNeighbors(int hexPlayerLocation)
{

}

int hexWorld::getBreeder(default_random_engine &seedGenerator, vector<int> probabilities)
{
	// discrete_distribution generator to choose neighbors probablistically based on the
	// number of games that they've won (our fitness function)
	discrete_distribution<int> breederGenerator(probabilities.begin(), probabilities.end());

	return breederGenerator(seedGenerator);
}
