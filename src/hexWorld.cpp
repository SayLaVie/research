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

#include <hexWorld.h>

hexWorld::hexWorld(int numPlayers, vector<int> netShape)
{
	this->numPlayers = numPlayers;
	this->netShape = netShape;
}

hexWorld::hexWorld(vector<hexGamePlayer> hexGamePlayers)
{
	vector<vector<vector<double> > > neuralNet;

	this->numPlayers = hexGamePlayers.size();
	this->hexGamePlayers = hexGamePlayers;

	neuralNet = hexGamePlayers[0].getNet();

	// Infer the shape of the network from the weights
	for (int layer = 0; layer < neuralNet.size(); layer += 1)
	{
		this->netShape.push_back(neuralNet[layer][0].size() - 1);
	}

	// Nets always end with one output node
	this->netShape.push_back(1);
};


// This function sets the weights for the neural net
void hexWorld::nextGeneration()
{
   /**
	- netShape keeps track of the shape of the neuralNet
   - neighbors is a vector of current neighbors
	- netWeights is the actual set of weights for each player
	- rowOriginationVector is a helper vector for creating netWeights
	- rowDestinationVector is also a helper vector for creating netWeights
   **/
	vector<int> neighbors, neighborsGamesWon;
	vector<vector<vector<double> > > netWeights;
	vector<vector<double> > rowDestinationVector;
	vector<double> rowOriginationVector;
	double currentWeight;
	int layer, rowDestination, rowOrigination, player, neighbor, gamesWon, breederChoice;
	vector<hexGamePlayer> newHexGamePlayers;

	// Bernoulli_distribution is effectively a coin toss. Returns true or false.
	bernoulli_distribution coinToss(0.5);


	// Check if this is the first generation (start with randomized weights)
	if (hexGamePlayers.size() == 0)
	{
		for (player = 0; player < numPlayers; player += 1)
		{
			netWeights.clear();

			// Create the vector of weights for the new players
			for (layer = 0; layer < netShape.size() - 1; layer += 1)
			{
				rowDestinationVector.clear();

				for (rowDestination = 0; rowDestination < netShape[layer + 1]; rowDestination += 1)
				{
					rowOriginationVector.clear();

               // Push_back bias weight
               currentWeight = generateWeight(seedGenerator, 0.0);
               rowOriginationVector.push_back(currentWeight);

               // Start at 1 because of the bias weight
					for (rowOrigination = 1; rowOrigination < netShape[layer] + 1; rowOrigination += 1)
					{
						// Generate random weight.
						currentWeight = generateWeight(seedGenerator, 0.0);

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

		for (player = 0; player < numPlayers; player += 1)
		{
			netWeights.clear();
			neighbors = getNeighbors(player);

			for (layer = 0; layer < netShape.size() - 1; layer += 1)
			{
				rowDestinationVector.clear();

				for (rowDestination = 0; rowDestination < netShape[layer + 1]; rowDestination += 1)
				{
					rowOriginationVector.clear();

					// This is where the breeding takes place. Loop conditional is netShape[layer] + 1
               // because there is a bais weight in every rowOriginationVector.
					for (rowOrigination = 0; rowOrigination < netShape[layer] + 1; rowOrigination += 1)
					{
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

						// Mutate currentWeight according to normal distribution centered on currentWeight
						currentWeight = generateWeight(seedGenerator, currentWeight);

						rowOriginationVector.push_back(currentWeight);
					}

					rowDestinationVector.push_back(rowOriginationVector);
				}

				netWeights.push_back(rowDestinationVector);
			}

			// Swap weights here?

			newHexGamePlayers.push_back(netWeights);
		}
		// Or swap weights here?

		hexGamePlayers = newHexGamePlayers;
	}
}

// return vector of neighboring hexPlayers. Will check left, left-up, right, right-down, up, down
vector<int> hexWorld::getNeighbors(int hexPlayerLocation)
{
	int neighborLocation;
   vector<int> neighbors;

   // Find left position, add to vector
   neighborLocation = getNeighborLocation(hexPlayerLocation, -1, 0);
   neighbors.push_back(neighborLocation);

   // Find left-up postion, add to vector
   neighborLocation = getNeighborLocation(hexPlayerLocation, -1, 1);
   neighbors.push_back(neighborLocation);

   // Find right position, add to vector
   neighborLocation = getNeighborLocation(hexPlayerLocation, 1, 0);
   neighbors.push_back(neighborLocation);

   // Find right-down position, add to vector
   neighborLocation = getNeighborLocation(hexPlayerLocation, 1, -1);
   neighbors.push_back(neighborLocation);

   // Find up position, add to vector
   neighborLocation = getNeighborLocation(hexPlayerLocation, 0, 1);
   neighbors.push_back(neighborLocation);

   // Find down position, add to vector
   neighborLocation = getNeighborLocation(hexPlayerLocation, 0, -1);
   neighbors.push_back(neighborLocation);

   return neighbors;
}

// Find the integer location for indicated player. Make adjustments to row/column
// if offsets move location off of the map.
int hexWorld::getNeighborLocation(int playerLocation, int columnOffset, int rowOffset)
{
	int neighborLocation, neighborColumn, neighborRow, playerColumn, playerRow, numPlayersDimension;

	numPlayersDimension = sqrt(numPlayers);
	playerColumn = playerLocation % numPlayersDimension;
	playerRow = playerLocation / numPlayersDimension;

	neighborColumn = playerColumn + columnOffset;
	neighborRow = playerRow + rowOffset;

	if (neighborColumn < 0)
	{
		neighborColumn += numPlayersDimension;
	}
	else if (neighborColumn >= numPlayersDimension)
	{
		neighborColumn -= numPlayersDimension;
	}

	if (neighborRow < 0)
	{
		neighborRow += numPlayersDimension;
	}
	else if (neighborRow >= numPlayersDimension)
	{
		neighborRow -= numPlayersDimension;
	}

	neighborLocation = numPlayersDimension * neighborRow + neighborColumn;

	return neighborLocation;
}

// This generator is seperated from the nextGeneration function because it needs a new
// set of probabilities with each use. The probabilities of the discrete_distribution
// are (as far as I could tell) immutable. So I create a new instance inside of this
// function whenever it is needed.
int hexWorld::getBreeder(mt19937_64 &seedGenerator, vector<int> probabilities)
{
	// discrete_distribution generator to choose neighbors probablistically based on the
	// number of games that they've won (our fitness function)
	discrete_distribution<int> breederGenerator(probabilities.begin(), probabilities.end());

	return breederGenerator(seedGenerator);
}

double hexWorld::generateWeight(mt19937_64 &seedGenerator, double mean)
{
	// Normal distribution to be used when:
	// 	- initalizing weights (distribution centered around 0)
	//		- mutating existing weights (distribution centered around original weight)
	// For now, we'll allow the standard deviation to default to 1.0
	normal_distribution<double> weightGenerator(mean);

	return weightGenerator(seedGenerator);
}

hexWorld& hexWorld::operator=(hexWorld rhs)
{
	this->hexGamePlayers.clear();
	this->hexGamePlayers = rhs.getHexGamePlayers();
	this->numPlayers = rhs.getNumPlayers();

	return *this;
}

void hexWorld::swapWeightsBetweenPlayers(hexGamePlayer &playerA, hexGamePlayer &playerB)
{
	// Sizes of particular vectors.
	int numLayers, numDestinationsA, numDestinationsB, numOriginationsA, numOriginationsB;
	// ints containing the randomly picked indexes for the neuralNetWeights of the players
	int randomLayerA, randomLayerB, randomDestinationA, randomDestinationB, randomOriginationA, randomOriginationB;
	// Variables holding each players neuralNetWeight
	vector<vector<vector<double> > > netPlayerA, netPlayerB;
	// Temp variable used during swap;
	double swapTemp;

	netPlayerA = playerA.getNet();
	netPlayerB = playerB.getNet();

	// The last layer of a neuralNetWeights vector will always be the output node, so that can be ignored.
	// The number of layers will be the same for both players
	numLayers = netPlayerA.size() - 1;

	// If there is only one layer, no need to randomly choose one
	if (numLayers == 1)
	{
		randomLayerA = 0;
		randomLayerB = 0;

		// Since there is only one layer, the number of destinations will be 1.
		// This variables not really needed in this case.
		numDestinationsA = 1;
		numDestinationsB = 1;

		// Number of originations will be the same as each other
		numOriginationsA = netPlayerA[0][0].size();
		numOriginationsB = numOriginationsA;

		// Set the Destination
		randomDestinationA = 0;
		randomDestinationB = 0;

		// uniform_int_distribution for choosing originations (same span for both players)
		uniform_int_distribution<int> randomOrigination(0, numOriginationsA - 1);

		randomOriginationA = randomOrigination(seedGenerator);
		randomOriginationB = randomOrigination(seedGenerator);
	}
	// Number of destinations may be different between players, if there is a multi-layer network involved
	else
	{
		// uniform_int_distribution for choosing which layer to swap for each
		uniform_int_distribution<int> randomLayer(0, numLayers - 1);
		randomLayerA = randomLayer(seedGenerator);
		randomLayerB = randomLayer(seedGenerator);

		// Since the neuralNets are multi layered, the sizes of rowDestination and rowOrigination may be 
		// different from each other 
		numDestinationsA = netPlayerA[randomLayerA].size();
		numDestinationsB = netPlayerB[randomLayerB].size();

		// uniform_int_distribution for choosing A and B's randomDestination
		uniform_int_distribution<int> randomDestinationGenA(0, numDestinationsA - 1);
		uniform_int_distribution<int> randomDestinationGenB(0, numDestinationsB - 1);

		// Choose random rowDestinations for A and B
		randomDestinationA = randomDestinationGenA(seedGenerator);
		randomDestinationB = randomDestinationGenB(seedGenerator);

		// Set sizes of rowOriginations
		numOriginationsA = netPlayerA[randomLayerA][randomDestinationA].size();
		numOriginationsB = netPlayerB[randomLayerB][randomDestinationB].size();

		// uniform_int_distributions for choosing A and B's randomOrigination
		uniform_int_distribution<int> randomOriginationGenA(0, numOriginationsA);
		uniform_int_distribution<int> randomOriginationGenB(0, numOriginationsB);

		// Choose random rowOriginations for A and B
		randomOriginationA = randomOriginationGenA(seedGenerator);
		randomOriginationB = randomOriginationGenB(seedGenerator);
	}

	// Swap the two weights
	
	swapTemp = playerA.getWeight(randomLayerA, randomDestinationA, randomOriginationA);
	
	playerA.setWeight(randomLayerA, randomDestinationA, randomOriginationA, 
		playerB.getWeight(randomLayerB, randomDestinationB, randomOriginationB));

	playerB.setWeight(randomLayerB, randomDestinationB, randomOriginationB, swapTemp);
}
