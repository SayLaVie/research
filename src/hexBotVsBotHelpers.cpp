 #include <hexBotVsBotHelpers.h>

void printUsage(int exitCode)
{
	cerr << "Usage: hexBotVsBot <option>" << endl;
	cerr << "Options:" << endl;
	cerr << "\t-h,--help\t\tShow this message" << endl;
	cerr << "\t-o,--output\t\tSpecify the relative or full path name of a file to print match results to (default: results/bot.match)" << endl;
	cerr << "\t-p,--players\t\tSpecify two relative or full path names of files that contain data for hexPlayers" << endl;

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

void playHexGame(hexGamePlayer botA, string nameBotA, hexGamePlayer botB, string nameBotB, ofstream &fout)
{
	int turnNumber, playerMove;
	Board board(BOARD_SIZE);
	player currentPlayer;

	turnNumber = 0;

	fout << "\tFirst match" << endl;
	fout << "Player A: " << nameBotA << endl;
	fout << "Player B: " << nameBotB << endl << endl;

	// BotA is playerA
	while (!board.isGameOver())
	{
		currentPlayer = static_cast<player>(turnNumber % 2);

		if (currentPlayer == playerA)
		{
			// Changed isEvolving value to true (last parameter) in order to use boardEvalLearning instead of minimax			
			playerMove = botA.play(board, playerA, true);
			board.makeMove(playerMove, playerA);
		}
		else
		{
			// Changed isEvolving value to true (last parameter) in order to use boardEvalLearning instead of minimax
			playerMove = botB.play(board, playerB, true);
			board.makeMove(playerMove, playerB);
		}

		turnNumber += 1;
	}

	// Print out results of this match
	board.printBoard(fout);

	fout << endl << endl << "Winner of this match is " << (currentPlayer == playerA ? nameBotA : nameBotB) << endl << endl << endl;

	fout << "\tSecond match" << endl;
	fout << "Player A: " << nameBotB << endl;
	fout << "Player B: " << nameBotA << endl << endl;

	// BotB is playerA
	board = Board(BOARD_SIZE);
	turnNumber = 0;

	while (!board.isGameOver())
	{
		currentPlayer = static_cast<player>(turnNumber % 2);

		if (currentPlayer == playerA)
		{
			// botB is playerA now
			// Changed isEvolving value to true (last parameter) in order to use boardEvalLearning instead of minimax			
			playerMove = botB.play(board, playerA, true);
			board.makeMove(playerMove, playerA);
		}
		else
		{
			// Changed isEvolving value to true (last parameter) in order to use boardEvalLearning instead of minimax			
			playerMove = botA.play(board, playerB, true);
			board.makeMove(playerMove, playerB);
		}

		turnNumber += 1;
	}

	// Print out results of match
	board.printBoard(fout);

	fout << endl << endl << "Winner of this match is " << (currentPlayer == playerA ? nameBotB : nameBotA) << endl;
}
