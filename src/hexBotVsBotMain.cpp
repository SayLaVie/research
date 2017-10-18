 
/*
This main function allows a Human to play against a hexGamePlayer
created by previous executions of hexEvolutions

Author: Michael McCarver
Advisor: Dr. Rob LeGrand
*/

#include <hexBotVsBotHelpers.h>
// #include <cstdlib>
// #include <sys/stat.h>
// #include <cctype>

// Define extern variable netShape
vector<int> netShape;

void printUsage(int exitCode);

int main(int argc, char *argv[])
{
	int arg;
	string argument, playerAFileName, playerBFileName, resultsFileName, answer;
	ifstream fin;
	ofstream fout;

	resultsFileName = "results/match.out";

	for (arg = 1; arg < argc; arg += 1)
	{
		argument = argv[arg];

		if (argument == "-h" || argument == "--help")
		{
			printUsage(0);
		}

		if (argument == "-o" || argument == "--output")
		{
			if (arg + 1 > argc)
			{
				cerr << "ouput option requires one argument" << endl;
				printUsage(1);
			}

			arg += 1;
			resultsFileName = argv[arg];
		}

		if (argument == "-p" || argument == "--players")
		{
			if (arg + 2 > argc)
			{
				cerr << "player option requires two arguments" << endl;
				printUsage(1);
			}
			
			arg += 1;
			playerAFileName = argv[arg];
			arg += 1;
			playerBFileName = argv[arg];
		}

		else
		{
			cerr << "Invalid option" << endl;
			printUsage(1);
		}
	}

	if (playerAFileName.empty() || playerBFileName.empty())
	{
		cerr << "The name of two players' files are required for execution" << endl;
		printUsage(1);
	}

	fin.open(playerAFileName);

	if (!fin.is_open())
	{
		cerr << "Unable to open player's file" << endl;
		printUsage(1);
	}

	// Declare and initialize hexGamePlayer opponent from input file data
	// *NOTE* there is currently no input sanitation
	hexGamePlayer botA(singleNeuralNetFileParser(fin));

	fin.close();

	// Open and read in second player's file
	fin.open(playerBFileName);

	if (!fin.is_open())
	{
		cerr << "The name of two players' files are required for execution" << endl;
		printUsage(1);
	}

	hexGamePlayer botB(singleNeuralNetFileParser(fin));

	fin.close();

	// Open and check resultsFileName
	fout.open(resultsFileName);

	if (!fout.is_open())
	{
		cerr << "Unable to open file " << resultsFileName << endl;
		printUsage(1);
	}

	playHexGame(botA, playerAFileName, botB, playerBFileName, fout);

	fout.close();
}
