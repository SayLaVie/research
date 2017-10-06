/*
This main function creates a hexWorld and plays all players against one another

Author: Michael McCarver
Advisor: Dr. Rob LeGrand
*/

#include "hexWorld.h"
#include <cstdlib>

void printUsage(int exitCode);
bool isNumeric(string input);
void playHexGames(hexWorld &population, ofstream &fout);
player playHexGame(hexGamePlayer hexPlayerA, hexGamePlayer hexPlayerB);

int main(int argc, char *argv[])
{
	time_t now;
	ifstream fin;
	ofstream foutResults, foutSave;
	int saveAfterNIterations, arg, iteration, numberOfIterations;
	string argument, resumeFile, outputFile, currentDT, saveFileName;
	vector<hexGamePlayer> resumePlayers;
	hexWorld population;

	numberOfIterations = 10;
	saveAfterNIterations = 10;
	outputFile = "results/results.out";

	if (argc > 7)
	{
		printUsage(1);
	}

	for (arg = 1; arg < argc; arg += 1)
	{
		argument = argv[arg];

		if (argument == "-h" || argument == "--help")
		{
			printUsage(0);
		}

		if (argument == "-f" || argument == "--frequency")
		{
			if (arg + 1 >= argc)
			{
				cerr << "frequency option requires one argument" << endl;
				printUsage(1);
			}

			arg += 1;

			if (!isNumeric(argv[arg]) || atoi(argv[arg]) <= 0)
			{
				cerr << "frequency option takes integer values greater than 0 only" << endl;
				printUsage(1);
			}

			saveAfterNIterations = atoi(argv[arg]);
		}

		else if (argument == "-r" || argument == "--resume")
		{
			// Next argument should be the file name of hexWorld data to resume with.
			if (arg + 1 >= argc)
			{
				cerr << "resume option requires one argument" << endl;
				printUsage(1);
			}
			arg += 1;
			resumeFile = argv[arg];
		}
		else if (argument == "-i" || argument == "--iterations")
		{
			if (arg + 1 >= argc)
			{
				cerr << "iterations option requires one argument" << endl;
				printUsage(1);
			}

			arg += 1;

			if (!isNumeric(argv[arg]) || atoi(argv[arg]) <= 0)
			{
				cerr << "iteration option takes integer values greater than 0 only" << endl;
				printUsage(1);
			}

			numberOfIterations = atoi(argv[arg]);
		}
		else if (argument == "-o" || argument == "--output")
		{
			if (arg + 1 >= argc)
			{
				cerr << "ouput option requires one argument" << endl;
				printUsage(1);
			}
			arg += 1;
			outputFile = argv[arg];
		}
		else
		{
			cerr << "Invalid option." << endl;
			printUsage(1);
		}
	}

	if (!resumeFile.empty())
	{
		fin.open(resumeFile);

		if (!fin.is_open())
		{
			cerr << "Unable to open file " << resumeFile << endl;
			printUsage(1);
		}

		// Function call here to read input from resumeFile and store into resumePlayers vector
		// Verify that number of players found matches NUM_PLAYERS
		hexWorld population(resumePlayers);

		fin.close();
	}

	// Open file for results output
	foutResults.open(outputFile);

	if (!foutResults.is_open())
	{
		cerr << "Unable to open file " << outputFile << endl;
		printUsage(1);
	}

	// Run through all iterations
	for (iteration = 0; iteration < numberOfIterations; iteration += 1)
	{
		now = time(0);
		currentDT = ctime(&now);

		foutResults << "Iteration " << iteration << " beginning at " << currentDT << endl;

		population.nextGeneration();

		playHexGames(population, foutResults);

		if (iteration == saveAfterNIterations - 1)
		{
			now = time(0);
			saveFileName = ctime(&now) + iteration;
			foutSave.open(saveFileName);

			if (!foutSave.is_open())
			{
				cerr << "Could not save hexWorld data to file " << saveFileName << endl;
				exit(1);
			}

			// Function here to save hexWorld data
		}
	}

	foutResults.close();
}

void printUsage(int exitCode)
{
		cerr << "Usage: hexEvolution <option(s)" << endl;
		cerr << "Options:" << endl;
		cerr << "\t-f,--frequency\t\tSpecify how frequently hexWorld data should be saved (default: after every 10 iterations)" << endl;
		cerr << "\t-h,--help\t\tShow this message" << endl;
		cerr << "\t-i,--iterations\t\tSpecify number of iterations to run (default: 10)" << endl;
		cerr << "\t-o,--output\t\tSpecify full or relative path of the file name to output results to (default: results/results.out)" << endl;
		cerr << "\t-r,--resume\t\tSpecify the full or relative path of the file name containing hexWorld data" << endl;
		
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

void playHexGames(hexWorld &population, ofstream &fout)
{
	int playerLocation, currentNeighbor;
	player gameWinner;
	vector<int> neighboringPlayers;

	for (playerLocation = 0; playerLocation < population.getNumPlayers(); playerLocation += 1)
	{
		neighboringPlayers.clear();
		neighboringPlayers = population.getNeighbors(playerLocation);

		for (currentNeighbor = 0; currentNeighbor < neighboringPlayers.size(); currentNeighbor += 1)
		{
			gameWinner = playHexGame(population.getHexGamePlayer(playerLocation), population.getHexGamePlayer(neighboringPlayers[currentNeighbor]));
		}

		if (gameWinner == playerA)
		{
			population.addPlayerWin(playerLocation);
		}
		else
		{
			population.addPlayerWin(currentNeighbor);
		}
	}
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
			playerMove = hexPlayerA.play(board, playerA);
		}
		else
		{
			playerMove = hexPlayerB.play(board, playerB);
		}

		// Put an 'if not valid move' conditional here

		board.makeMove(playerMove, currentPlayer);

		numberOfTurns += 1;
	}

	// The most recent value of currentPlayer is the player who won
	return currentPlayer;
}
