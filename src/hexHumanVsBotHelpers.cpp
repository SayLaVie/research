#include <hexHumanVsBotHelpers.h>

void printUsage(int exitCode)
{
	cerr << "Usage: hexHumanVsBot <option>" << endl;
	cerr << "Options:" << endl;
	cerr << "\t-h,--help\t\tShow this message" << endl;
	cerr << "\t-p,--player\t\tSpecify the relative or full path name of a file that contains the data for a hexPlayer" << endl;

	exit(exitCode);
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

void playHexGame(hexGamePlayer bot)
{
	Board board(BOARD_SIZE);
	string answer, move;
	int row, column, playerMove, turnNumber;
	double botEval;
	bool answerQuestion, madeMove;
	player currentPlayer, humanPlayer;

	answerQuestion = false;

	while (!answerQuestion)
	{
		cout << "Would you like to play first (Y/N)? ";
		cin >> answer;

		if (answer.length() != 1 || (toupper(answer[0]) != 'Y' && toupper(answer[0]) != 'N'))
		{
			cout << "Invalid choice" << endl;
		}
		else
		{
			answerQuestion = true;
		}
	}

	if (toupper(answer[0]) == 'Y')
	{
		humanPlayer = playerA;
	}
	else
	{
		humanPlayer = playerB;
	}

	turnNumber = 0;

	cout << endl << "Try to connect the left and right sides!" << endl;
	cout << "Enter your move in the form <alpha><digit>" << endl;
	cout << "For example, A0" << endl << endl;

	while (!board.isGameOver())
	{
		currentPlayer = static_cast<player>(turnNumber % 2);

		if (currentPlayer == humanPlayer)
		{
			board.printBoard();
			cout << endl << "Move: ";
			cin >> move;

			if (move.length() > 2 || !isalpha(move[0]) || toupper(move[0]) - 64 > BOARD_SIZE || 
					!isdigit(move[1]) || (move[1] - 48) + 1 > BOARD_SIZE)
			{
				cout << "Make sure that your move is of the form <alpha><digit>" << endl;
				cout << "For example, A0" << endl;
			}
			else
			{
				madeMove = true;
			}

			column = toupper(move[0]) - 65;
			row = move[1] - 48;			
			playerMove = BOARD_SIZE * row + column;

			board.makeMove(playerMove, currentPlayer);			
		}
		else
		{
			playerMove = bot.play(board, currentPlayer, false);
			board.makeMove(playerMove, currentPlayer);
			botEval = bot.neuralNetHeuristic(board, currentPlayer);
			cout << endl << "Bot's evaluation: " << botEval << endl;
		}

		turnNumber += 1;
	}

	if (currentPlayer == humanPlayer)
	{
		cout << endl << "Congratulations! You are the Winner!" << endl;
	}
	else
	{
		// Print out board one last time, because bot has made a move since the last print out
		board.printBoard();
		cout << endl << "Better luck next time!" << endl;
	}
}
