 
/*
This main function allows a Human to play against a hexGamePlayer
created by previous executions of hexEvolutions

Author: Michael McCarver
Advisor: Dr. Rob LeGrand
*/

#include <hexHumanVsBotHelpers.h>
// #include <cstdlib>
// #include <sys/stat.h>
// #include <cctype>

// Define extern variable netShape
vector<int> netShape;
int MAX_DEPTH;
void printUsage(int exitCode);

int main(int argc, char *argv[])
{
	int arg;
	string argument, playerFileName, answer;
	bool playAgain, answerQuestion;
	ifstream fin;
	MAX_DEPTH = 0;

	for (arg = 1; arg < argc; arg += 1)
	{
		argument = argv[arg];

		if (argument == "-d" || argument == "--depth")
		{
			if (arg + 1 >= argc || !isNumeric(argv[arg + 1]))
			{
				cerr << "Player option requires one positive integer" << endl;
				printUsage(1);
			}

			arg += 1;

			MAX_DEPTH = atoi(argv[arg]);
		}

		else if (argument == "-h" || argument == "--help")
		{
			printUsage(0);
		}

		else if (argument == "-p" || argument == "--player")
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
			cerr << "Invalid option" << endl;
			printUsage(1);
		}
	}

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
	hexGamePlayer bot(singleNeuralNetFileParser(fin));

	// Play Hex Games
	do
	{
		playHexGame(bot);

		answerQuestion = false;

		while (!answerQuestion)
		{
			cout << "Would you like to play again (Y/N)? ";
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

		playAgain = answer == "Y" || answer == "y";

	} while(playAgain);
}
