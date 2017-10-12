 
/*
This main function allows a Human to play against a hexGamePlayer
created by previous executions of hexEvolutions

Author: Michael McCarver
Advisor: Dr. Rob LeGrand
*/

#include "hexHelperFunctions.h"
#include <cstdlib>
#include <sys/stat.h>

int main(int argc, char *argv[])
{
	int arg;
	string argument, playerFileName;
	ifstream fin;

	for (arg = 1; arg < argc; arg += 1)
	{
		argument = argv[arg];

		if (argument == "-h" || argument == "--help")
		{
			printUsage(0, "hexHumanVsBot");
		}

		if (argument == "-p" || argument == "--player")
		{
			if (arg + 1 > argc)
			{
				cerr << "Player option requires one argument" << endl;
				printUsage(1, "hexHumanVsBot");
			}
			
			arg += 1;
			playerFileName = argv[arg];
		}

		else
		{
			cerr << "Invalid option" << endl;
			printUsage(1, "hexHumanVsBot");
		}
	}

	if (playerFileName.empty())
	{
		cerr << "The name of a player's file is required for execution" << endl;
		printUsage(1, "hexHumanVsBot");
	}

	fin.open(playerFileName);

	if (!fin.is_open())
	{
		cerr << "Unable to open player's file" << endl;
		printUsage(1, "hexHumanVsBot");
	}

	// Parse file here (take into consideration extern netShape vector)
}
