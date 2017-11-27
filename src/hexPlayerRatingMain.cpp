/*
This main function rates a player against various levels of 'hardness'
	The code currently only supports the weakest hardness, what we call Talc
	Talc is a measurement of a player's performance against completely random players

Author: Michael McCarver
Advisor: Dr. Rob LeGrand
*/

#include <hexPlayerRatingHelpers.h>

int MAX_DEPTH = 0;

int main(int argc, char *argv[])
{
	ifstream fin;
	ofstream foutResults, foutSave;
	int arg;
	string argument, outputFile, playerFileName;

	// Default values
	outputFile = "results/playerHardness.rating";

	for (arg = 1; arg < argc; arg += 1)
	{
		argument = argv[arg];

		// Display command li	 options
		if (argument == "-h" || argument == "--help")
		{
			printUsage(0);
		}

		// Name of results file to output to
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

		else if (argument == "-p" || argument == "--output")
		{
			if (arg + 1 >= argc)
			{
				cerr << "Player option requires one argument" << endl;
				printUsage(1);
			}

			arg += 1;
			playerFileName = argv[arg];
		}

		else
		{
			cerr << "Invalid option." << endl;
			printUsage(1);
		}
	}

	/*
		Open output file for results
	*/
	foutResults.open(outputFile);

	if (!foutResults.is_open())
	{
		cerr << "Unable to open file " << outputFile << endl;
		printUsage(1);
	}

	/*
		Open player file name
	*/
	if (playerFileName.empty())
	{
		cerr << "The name of a player's file is required for execution" << endl;
		printUsage(1);
	}

	fin.open(playerFileName);

	if (!fin.is_open())
	{
		cerr << "Unable to open player's file" << endl;
		printUsage(1);
	}

	// Declare and initialize hexGamePlayer opponent from input file data
	// *NOTE* there is currently no input sanitation
	hexGamePlayer hexPlayer(singleNeuralNetFileParser(fin));

	foutResults << "Player " << playerFileName << endl;

	// Play all games against random players
	playHexGames(hexPlayer, foutResults);

	foutResults.close();
}
