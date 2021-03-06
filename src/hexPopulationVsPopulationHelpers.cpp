 #include <hexPopulationVsPopulationHelpers.h>

void printUsage(int exitCode)
{
	cerr << "Usage: hexPopulationVsPopulation <option(s)>" << endl;
	cerr << "Options:" << endl;
	cerr << "\t-b,--bipartite\t\tStrictly play agents from different populations (no intermingling)" << endl;
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

void playHexGamesBipartite(hexWorld populationA, hexWorld populationB, PopulationPair &gameStats)
{
	int numberWinsA, numberWinsB, numPlayers, hexPlayer, populationAPlayer, populationBPlayer;
	vector<pair<hexGamePlayer, int> > sortedPlayersA, sortedPlayersB;
	Player winner;
	hexGamePlayer hexPlayerA, hexPlayerB;

	numberWinsA = 0;
	numberWinsB = 0;

	// * Assuming that both populations are the same size *
	numPlayers = populationA.getNumPlayers();

	// Play two games with each player starting first
	for (populationAPlayer = 0; populationAPlayer < numPlayers; populationAPlayer += 1)
	{
		hexPlayerA = populationA.getHexGamePlayer(populationAPlayer);

		for (populationBPlayer = 0; populationBPlayer < numPlayers; populationBPlayer += 1)
		{
			hexPlayerB = populationB.getHexGamePlayer(populationBPlayer);

			winner = playHexGame(hexPlayerA, hexPlayerB);

			if (winner == PlayerA)
			{
				gameStats.populationATotalWins += 1;
				gameStats.totalWinsAsFirstPlayer += 1;
				populationA.addPlayerWin(populationAPlayer);
			}
			else
			{
				gameStats.populationBTotalWins += 1;
				gameStats.totalWinsAsSecondPlayer += 1;
				populationB.addPlayerWin(populationBPlayer);
			}

			winner = playHexGame(hexPlayerB, hexPlayerA);

			if (winner == PlayerA)
			{
				gameStats.populationBTotalWins += 1;
				gameStats.totalWinsAsFirstPlayer += 1;
				populationB.addPlayerWin(populationBPlayer);
			}
			else
			{
				gameStats.populationATotalWins += 1;
				gameStats.totalWinsAsSecondPlayer += 1;
				populationA.addPlayerWin(populationAPlayer);
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

void playHexGamesMelee(hexWorld populationA, hexWorld populationB, PopulationPair &gameStats)
{
	int numberWinsA, numberWinsB, numPlayers, hexPlayer, playerA, playerB;
	vector<pair<hexGamePlayer, int> > sortedPlayersA, sortedPlayersB;
	vector<pair<pair<hexGamePlayer, int>, int> > sortedPlayersAll;
	Player winner;
	hexGamePlayer hexPlayerA, hexPlayerB;

	numberWinsA = 0;
	numberWinsB = 0;

	// * Assuming that both populations are the same size *
	numPlayers = populationA.getNumPlayers();

	// Play populationA against itself
	for (playerA = 0; playerA < numPlayers; playerA += 1)
	{
		hexPlayerA = populationA.getHexGamePlayer(playerA);

		for (playerB = 0; playerB < numPlayers; playerB += 1)
		{
			if (playerA != playerB)
			{
				hexPlayerB = populationA.getHexGamePlayer(playerB);

				winner = playHexGame(hexPlayerA, hexPlayerB);

				if (winner == PlayerA)
				{
					gameStats.totalWinsAsFirstPlayer += 1;
					populationA.addPlayerWin(playerA);
				}
				else
				{
					gameStats.totalWinsAsSecondPlayer += 1;
					populationA.addPlayerWin(playerB);
				}
			}
		}
	}

		// Play populationB against itself
	for (playerA = 0; playerA < numPlayers; playerA += 1)
	{
		hexPlayerA = populationB.getHexGamePlayer(playerA);

		for (playerB = 0; playerB < numPlayers; playerB += 1)
		{
			if (playerA != playerB)
			{
				hexPlayerB = populationB.getHexGamePlayer(playerB);

				winner = playHexGame(hexPlayerA, hexPlayerB);

				if (winner == PlayerA)
				{
					gameStats.totalWinsAsFirstPlayer += 1;
					populationB.addPlayerWin(playerA);
				}
				else
				{
					gameStats.totalWinsAsSecondPlayer += 1;
					populationB.addPlayerWin(playerB);
				}
			}
		}
	}

	// Play populationA against populationB
	for (playerA = 0; playerA < numPlayers; playerA += 1)
	{
		hexPlayerA = populationA.getHexGamePlayer(playerA);

		for (playerB = 0; playerB < numPlayers; playerB += 1)
		{
			hexPlayerB = populationB.getHexGamePlayer(playerB);

			winner = playHexGame(hexPlayerA, hexPlayerB);

			if (winner == PlayerA)
			{
				gameStats.populationATotalWins += 1;
				gameStats.totalWinsAsFirstPlayer += 1;
				populationA.addPlayerWin(playerA);
			}
			else
			{
				gameStats.populationBTotalWins += 1;
				gameStats.totalWinsAsSecondPlayer += 1;
				populationB.addPlayerWin(playerB);
			}

			// Switch positions
			winner = playHexGame(hexPlayerB, hexPlayerA);


			if (winner == PlayerA)
			{
				gameStats.populationBTotalWins += 1;
				gameStats.totalWinsAsFirstPlayer += 1;
				populationB.addPlayerWin(playerB);
			}
			else
			{
				gameStats.populationATotalWins += 1;
				gameStats.totalWinsAsSecondPlayer += 1;
				populationA.addPlayerWin(playerA);
			}
		}
	}	

	// Currently not keeping track of individually sorted populations
	// Code is still here in case we change our minds in the future
/*
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
*/

	// Push all players into a single vector, keeping track of which population each belongs to
	// *Assumes both populations are same size*
	for (hexPlayer = 0; hexPlayer < numPlayers; hexPlayer += 1)
	{
		// The last int represents which population they belong to - 1 is pop1, 2 is pop2
		sortedPlayersAll.push_back(pair<pair<hexGamePlayer, int>, int>(pair<hexGamePlayer, int>(populationA.getHexGamePlayer(hexPlayer), hexPlayer), 1));
		sortedPlayersAll.push_back(pair<pair<hexGamePlayer, int>, int>(pair<hexGamePlayer, int>(populationB.getHexGamePlayer(hexPlayer), hexPlayer), 2));
	}

	sort(sortedPlayersAll.begin(), sortedPlayersAll.end(), compareAll);

	gameStats.sortedPlayersAll = sortedPlayersAll;
}

Player playHexGame(hexGamePlayer botA, hexGamePlayer botB)
{
	int turnNumber, playerMove;
	Board board(BOARD_SIZE);
	Player currentPlayer;

	turnNumber = 0;

	while (!board.isGameOver())
	{
		currentPlayer = static_cast<Player>(turnNumber % 2);

		if (currentPlayer == PlayerA)
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
