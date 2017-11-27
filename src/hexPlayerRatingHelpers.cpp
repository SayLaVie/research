#include <hexPlayerRatingHelpers.h>

void printUsage(int exitCode)
{
	cerr << "Usage: hexPlayerRating <option(s)>" << endl;
	cerr << "Options:" << endl;
	cerr << "\t-h,--help\t\tShow this message" << endl;
	cerr << "\t-o,--output\t\tSpecify full or relative path of the file name to output results to (default: results/playerHardness.rating)" << endl;
	cerr << "\t-p,--player\t\tSpecify the relative or full path name of a file that contains the data for a hexPlayer" << endl;

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

void playHexGames(hexGamePlayer hexPlayer, ofstream &fout)
{
	int whichGame, gamesToPlay, totalGamesWon, gamesPlayerWonAsPlayerA, gamesPlayerWonAsPlayerB;
	Player gameWinner;

	gamesToPlay = 1000;
	totalGamesWon = 0;
	gamesPlayerWonAsPlayerA = 0;
	gamesPlayerWonAsPlayerB = 0;

	for (whichGame = 0; whichGame < gamesToPlay; whichGame += 1)
	{
		gameWinner = playHexGame(hexPlayer, PlayerA);

		if (gameWinner == PlayerA)
		{
			totalGamesWon += 1;
			gamesPlayerWonAsPlayerA += 1;
		}
		else if (gameWinner == None)
		{
			cerr << "Invalid move." << endl;
			exit(1);
		}

		gameWinner = playHexGame(hexPlayer, PlayerB);

		if (gameWinner == PlayerB)
		{
			totalGamesWon += 1;
			gamesPlayerWonAsPlayerB += 1;
		}
		else if (gameWinner == None)
		{
			cerr << "Invalid move." << endl;
			exit(1);
		}
	}

	fout << "Total Games played:\t" << gamesToPlay * 2 << endl;
	fout << "Total Games won by player:\t" << totalGamesWon << endl;
	fout << "Games won by player as first player:\t" << gamesPlayerWonAsPlayerA << endl;
	fout << "Games won by player as second player:\t" << gamesPlayerWonAsPlayerB << endl;
}


// Takes ifstream and parses nerualNetWeights 3-D vector from file.
vector<vector<vector<double> > > singleNeuralNetFileParser(ifstream &fin)
{
	double weight;
	string parserGuide, line, token, delim;
	size_t pos;
	vector<int> netShape;
	vector<double> singleInputNodeVector;
	vector<vector<double> > vectorOfInputNodes;
	vector<vector<vector<double> > > vectorOfLayers;

	delim = ",";

	while (fin >> parserGuide)
	{
		if (parserGuide == "layer" || parserGuide == "endPlayer")
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
		else if (parserGuide == "node")
		{
			// The rest of the line should be double values seperated by comas
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
		else if (parserGuide == "stat")
		{
			// Ignore numGamesWon (the next input)
			fin >> parserGuide;
		}
		else if (parserGuide == "//")
		{
			// Ignore the rest of the line
			getline(fin, line);
		}
		else
		{
			cout << "Invalid symbol found in file" << endl;
			exit(0);
		}
	}

	return vectorOfLayers;
}
