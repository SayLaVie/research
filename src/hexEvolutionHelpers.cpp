#include <hexEvolutionHelpers.h>

void printUsage(int exitCode)
{
	cerr << "Usage: hexEvolution <option(s)>" << endl;
	cerr << "Options:" << endl;
	cerr << "\t-f,--frequency\t\tSpecify how frequently hexWorld data should be saved (default: after every 10 iterations)" << endl;
	cerr << "\t-h,--help\t\tShow this message" << endl;
	cerr << "\t-i,--iterations\t\tSpecify number of iterations to run (default: 10)" << endl;
	cerr << "\t-o,--output\t\tSpecify full or relative path of the file name to output results to (default: results/results.out)" << endl;
	cerr << "\t-r,--resume\t\tSpecify the full or relative path of the file name containing hexWorld data (usually found in data/ subdirectories)" << endl;
	
	exit(exitCode);
}

bool isNumeric(string input)
{
	for (int character = 0; character < input.length(); character += 1)
	{
		if (!isdigit(input[character]))
		{
			return false;
		}
	}

	return true;
}

player playHexGame(hexGamePlayer hexPlayerA, hexGamePlayer hexPlayerB)
{
	int playerMove, numberOfTurns;
	player currentPlayer;
	Board board(BOARD_SIZE);

	numberOfTurns = 0;

	while (!board.isGameOver())
	{
		currentPlayer = static_cast<player>(numberOfTurns % 2);

		if (currentPlayer == playerA)
		{
			playerMove = hexPlayerA.play(board, playerA, true);
		}
		else
		{
			playerMove = hexPlayerB.play(board, playerB, true);
		}

		// Put an 'if not valid move' conditional here

		board.makeMove(playerMove, currentPlayer);

		numberOfTurns += 1;
	}

	// The most recent value of currentPlayer is the player who won
	return currentPlayer;
}

void playHexGames(hexWorld &population, ofstream &fout)
{
	int playerLocation, currentNeighbor;
	player gameWinner;
	vector<int> neighboringPlayers;

	for (playerLocation = 0; playerLocation < population.getNumPlayers(); playerLocation += 1)
	{
		neighboringPlayers.clear();
		neighboringPlayers = population.getNeighbors(playerLocation);

		/**
		Testing Area
		**/
		// fout << "\tPlayer" << playerLocation << " vs:" << endl;

		for (currentNeighbor = 0; currentNeighbor < neighboringPlayers.size(); currentNeighbor += 1)
		{
			// fout << "\t\t" << neighboringPlayers[currentNeighbor] << ": ";
			gameWinner = playHexGame(population.getHexGamePlayer(playerLocation), population.getHexGamePlayer(neighboringPlayers[currentNeighbor]));

			if (gameWinner == playerA)
			{
				// fout << "Win" << endl;
				population.addPlayerWin(playerLocation);
			}
			else
			{
				// fout << "Loss" << endl;
				population.addPlayerWin(neighboringPlayers[currentNeighbor]);
			}
		}
	}
}

hexWorld entirePopulationFileParser(ifstream &fin)
{
	int numGamesWon;
	double weight;
	string parserGuide, line, token, delim;
	size_t pos;
	vector<double> singleInputNodeVector;
	vector<vector<double> > vectorOfInputNodes;
	vector<vector<vector<double> > > vectorOfLayers;
	vector<hexGamePlayer> hexGamePlayers;
	hexWorld population;

	hexGamePlayers.clear();
	delim = ",";

	while (fin >> parserGuide)
	{
		// numGamesWon stat for hexGamePlayer
		if (parserGuide == "stat")
		{
			fin >> numGamesWon;
		}
		// Beginning of a new layer
		else if (parserGuide == "layer")
		{
			if (vectorOfInputNodes.size() == 0)
			{
				// This is the beginning of the file, and there's nothing to push back
				vectorOfLayers.clear();
				vectorOfInputNodes.clear();
			}
			else
			{
				vectorOfLayers.push_back(vectorOfInputNodes);
				vectorOfInputNodes.clear();
			}
		}
		// Following line content is a vector of weights (one single node)
		else if (parserGuide == "node")
		{
			// The rest of the line should be double values seperated by commas
			getline(fin, line);
			singleInputNodeVector.clear();

			while (line.length() > 0)
			{
				pos = line.find(delim);
				token = line.substr(0, pos);
				
				weight = atof(token.c_str());
				singleInputNodeVector.push_back(weight);

				line.erase(0, pos + 1);
			}

			vectorOfInputNodes.push_back(singleInputNodeVector);
		}
		// End of a single player's input
		else if (parserGuide == "endPlayer")
		{
			vectorOfLayers.push_back(vectorOfInputNodes);
			hexGamePlayers.push_back(hexGamePlayer(vectorOfLayers, numGamesWon));

			vectorOfInputNodes.clear();
			vectorOfLayers.clear();
		}
		// Comment symbol ignores the rest of the line
		else if (parserGuide == "//")
		{
			getline(fin, line);
		}
		// Invalid syntax in file
		else
		{
			cout << "Invalid symbol found in file: " << parserGuide << endl;
			exit(0);
		}
	}

	population = hexWorld(hexGamePlayers);

	hexPopulationSanitation(population);

	return population;
}

void hexPopulationSanitation(hexWorld population)
{
	// No sanitation yet
	;
}

void printCurrentGenerationToFile(hexWorld population, ofstream &fout)
{
	int player;

	for (player = 0; player < population.getNumPlayers(); player += 1)
	{
		fout << "// Player" << player << endl;

		population.getHexGamePlayer(player).printWeights(fout);
	}
}

// This function returns a weighted-average representative of the given population
//		 *NOTE* this function may need to be altered for networks more complicated than
// single layer.
void printPopulationRepresentative(hexWorld population, ofstream &fout)
{
	int layer, rowDestination, rowOrigination, player;
	double sum, weightedAverage;
	vector<vector<vector<double> > > sampleNet;

	// Grab the net of the first player to use for loop parameters
	sampleNet = population.getHexGamePlayer(0).getNet();

	fout << "// populationRepresentative is a weighted average of all hexGamePlayers in a hexWorld" << endl;

	for (layer = 0; layer < sampleNet.size(); layer += 1)
	{
		fout << "\tlayer" << endl;

		for (rowDestination = 0; rowDestination < sampleNet[layer].size(); rowDestination += 1)
		{
			fout << "\t\tnode ";

			for (rowOrigination = 0; rowOrigination < sampleNet[layer][rowDestination].size(); rowOrigination += 1)
			{
				sum = 0;

				for (player = 0; player < population.getNumPlayers(); player += 1)
				{
					sum += population.getHexGamePlayer(player).getWeight(layer, rowDestination, rowOrigination) * population.getHexGamePlayer(player).getGamesWon();
				}

				weightedAverage = sum / population.getNumPlayers();

				fout << weightedAverage << ',';
			}

			fout << endl;
		}
	}

	fout << "endPlayer" << endl;
}
