#include <hexPopulationRatingHelpers.h>

void printUsage(int exitCode)
{
	cerr << "Usage: hexPlayerRating <option(s)>" << endl;
	cerr << "Options:" << endl;
	cerr << "\t-h,--help\t\tShow this message" << endl;
	cerr << "\t-o,--output\t\tSpecify full or relative path of the file name to output results to (default: results/playerHardness.rating)" << endl;
	cerr << "\t-p,--player\t\tSpecify the relative or full path name of a file that contains population data" << endl;

	exit(exitCode);
}

int randomPlayerMove(Board board)
{
	int location, nextMove;
	vector<int> availableMoves;

	availableMoves.clear();

	// Push back available moves onto vector
	for (location = 0; location < BOARD_SIZE * BOARD_SIZE; location += 1)
	{
		if (board.isValidMove(location))
		{
			availableMoves.push_back(location);
		}
	}

	// Uniform int distribution to randomly select a move from available moves
	uniform_int_distribution<> nextMoveGenerator(0, availableMoves.size() - 1);

	nextMove = nextMoveGenerator(seedGenerator);

	return availableMoves[nextMove];
}

Player playHexGame(hexGamePlayer hexPlayer, Player whichPlayer)
{
	int playerMove, numberOfTurns;
	Player currentPlayer;
	Board board(BOARD_SIZE);

	numberOfTurns = 0;

	while (!board.isGameOver())
	{
		currentPlayer = static_cast<Player>(numberOfTurns % 2);

		if (currentPlayer == whichPlayer)
		{
			playerMove = hexPlayer.play(board, currentPlayer);
		}
		else
		{
			playerMove = randomPlayerMove(board);
		}

		// Put an 'if not valid move' conditional here
		if (!board.isValidMove(playerMove))
		{
			cerr << whichPlayer << endl << currentPlayer << endl;;
			return None;
		}

		board.makeMove(playerMove, currentPlayer);

		numberOfTurns += 1;
	}

	// The most recent value of currentPlayer is the player who won
	return currentPlayer;
}

void playHexGames(hexWorld hexPopulation, ofstream &fout)
{
	int whichPlayer, whichGame, gamesToPlay;
	Player gameWinner;
	PlayerStatistics tmpStats;
	vector<PlayerStatistics> sortedStatsVector;

	gamesToPlay = 1000;

	for (whichPlayer = 0; whichPlayer < hexPopulation.getNumPlayers(); whichPlayer += 1)
	{
		tmpStats.playerNumber = whichPlayer;
		tmpStats.totalGamesWon = 0;
		tmpStats.numGamesWonAsA = 0;

		for (whichGame = 0; whichGame < gamesToPlay; whichGame += 1)
		{
			gameWinner = playHexGame(hexPopulation.getHexGamePlayer(whichPlayer), PlayerA);

			if (gameWinner == PlayerA)
			{
				tmpStats.totalGamesWon += 1;
				tmpStats.numGamesWonAsA += 1;
			}
			else if (gameWinner == None)
			{
				cerr << "Invalid move." << endl;
				exit(1);
			}

			gameWinner = playHexGame(hexPopulation.getHexGamePlayer(whichPlayer), PlayerB);

			if (gameWinner == PlayerB)
			{
				tmpStats.totalGamesWon += 1;
			}
			else if (gameWinner == None)
			{
				cerr << "Invalid move." << endl;
				exit(1);
			}
		}

		sortedStatsVector.push_back(tmpStats);
	}

	sort(sortedStatsVector.begin(), sortedStatsVector.end(), compare);

	// Print out all player stats to file
	fout << "All players of a population ranked according to their performance against completely random players" << endl << endl;

	fout << "PlayerNumber\tTotalGamesWon\tGamesWonAsA" << endl;

	for (whichPlayer = 0; whichPlayer < sortedStatsVector.size(); whichPlayer += 1)
	{
		tmpStats = sortedStatsVector[whichPlayer];

		fout << setw(12) << tmpStats.playerNumber << "\t";

		fout << setw(13) << tmpStats.totalGamesWon << "\t";
		fout << setw(11) << tmpStats.numGamesWonAsA << endl;
	}
}

hexWorld entirePopulationFileParser(ifstream &fin)
{
	// int numGamesWon;
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
			// fin >> numGamesWon;
			// Ignore numGamesWon
			getline(fin, line);
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
			// Ignore numGamesWon
			// hexGamePlayers.push_back(hexGamePlayer(vectorOfLayers, numGamesWon));
			hexGamePlayers.push_back(hexGamePlayer(vectorOfLayers));

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

	// No sanitation yet
	// hexPopulationSanitation(population);

	return population;
}
