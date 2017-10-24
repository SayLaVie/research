 #include <hexPopulationVsPopulationHelpers.h>

void printUsage(int exitCode)
{
	cerr << "Usage: hexPopulationVsPopulation <option(s)>" << endl;
	cerr << "Options:" << endl;
	cerr << "\t-d,--depth\t\tSpecify a depth for the minimax search (default: depth=0 i.e. do not use minimax)" << endl;
	cerr << "\t-h,--help\t\tShow this message" << endl;
	cerr << "\t-o,--output\t\tSpecify the relative or full path name of a file to print data to (default: results/population.match" << endl;
	cerr << "\t-p,--populations\tSpecify two relative or full path names of files that contain population data" << endl;

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

void playHexGames(hexWorld populationA, hexWorld populationB, PopulationPair &gameStats)
{
	int numberWinsA, numberWinsB, numPlayers, hexPlayer, playerA, playerB;
	vector<pair<hexGamePlayer, int> > sortedPlayersA, sortedPlayersB;
	player winner;
	hexGamePlayer hexPlayerA, hexPlayerB;

	numberWinsA = 0;
	numberWinsB = 0;

	// * Assuming that both populations are the same size *
	numPlayers = populationA.getNumPlayers();

	// Play two games with each player starting first
	for (playerA = 0; playerA < numPlayers; playerA += 1)
	{
		for (playerB = 0; playerB < numPlayers; playerB += 1)
		{
			hexPlayerA = populationA.getHexGamePlayer(playerA);
			hexPlayerB = populationB.getHexGamePlayer(playerB);

			winner = playHexGame(hexPlayerA, hexPlayerB);

			if (winner == playerA)
			{
				gameStats.populationATotalWins += 1;
				gameStats.totalWinsAsFirstPlayer += 1;
				populationA.addHexGamePlayerWin(playerA);
			}
			else
			{
				gameStats.populationBTotalWins += 1;
				gameStats.totalWinsAsSecondPlayer += 1;
				populationB.addHexGamePlayerWin(playerB);
			}

			hexPlayerA = hexPlayerB;
			hexPlayerB = populationA.getHexGamePlayer(playerA);

			winner = playHexGame(hexPlayerA, hexPlayerB);

			if (winner == playerA)
			{
				gameStats.populationBTotalWins += 1;
				gameStats.totalWinsAsFirstPlayer += 1;
				populationB.addHexGamePlayerWin(playerB);
			}
			else
			{
				gameStats.populationATotalWins += 1;
				gameStats.totalWinsAsSecondPlayer += 1;
				populationA.addHexGamePlayerWin(playerA);
			}
		}
	}

	// Push players of both populations into vectors for sorting
	for (hexPlayer = 0; hexPlayer < numPlayers; hexPlayer += 1)
	{
		sortedPlayersA.push_back(pair<hexGamePlayer, int>(populationA.getHexGamePlayer(hexPlayer), hexPlayer));
		sortedPlayersB.push_back(pair<hexGamePlayer, int>(populationB.getHexGamePlayer(hexPlayer), hexPlayer));
	}

	sort(sortedPlayersA.begin(), sortedPlayersA.end(), compare);
	sort(sortedPlayersB.begin(), sortedPlayersB.end(), compare);

	gameStats.sortedPlayersA = sortedPlayersA;
	gameStats.sortedPlayersB = sortedPlayersB;
}

player playHexGame(hexGamePlayer botA, hexGamePlayer botB)
{
	int turnNumber, playerMove;
	Board board(BOARD_SIZE);
	player currentPlayer;

	turnNumber = 0;

	while (!board.isGameOver())
	{
		currentPlayer = static_cast<player>(turnNumber % 2);

		if (currentPlayer == playerA)
		{
			playerMove = botA.play(board, currentPlayer);
		}
		else
		{
			playerMove = botB.play(board, currentPlayer);
		}

		board.makeMove(playerMove, currentPlayer);

		turnNumber += 1;
	}

	// currentPlayer is the last player to move, thus the winner
	return currentPlayer;
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
