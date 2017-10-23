#include <hexPopulationVsPopulationHelpers.h>

int MAX_DEPTH;

int main (int argc, char *argv[])
{
	int arg, player;
	string outputFile, populationAFileName, populationBFileName, argument;
	ofstream fout;
	ifstream fin;
	PopulationPair gameStats;

	outputFile = "results/population.match";
	MAX_DEPTH = 0;

	// Parse through options
	for (arg = 1; arg < argc; arg += 1)
	{
		argument = argv[arg];

		if (argument == "-d" || argument == "--depth")
		{
			if (arg + 1 >= argc || !isNumeric(argv[arg + 1]))
			{
				cerr << "Depth option requires one positive integer argument" << endl;
				printUsage(1);
			}

			arg += 1;

			MAX_DEPTH = atoi(argv[arg]);
		}

		else if (argument == "-h" || argument == "--help")
		{
			printUsage(0);
		}
		else if (argument == "-o" || argument == "--output")
		{
			if (arg + 1 >= argc)
			{
				cerr << "Output option requires one argument" << endl;
				printUsage(1);
			}

			arg += 1;
			outputFile = argv[arg];
		}
		else if (argument == "-p" || argument == "--populations")
		{
			if (arg + 2 >= argc)
			{
				cerr << "populations option requires two arguments" << endl;
				printUsage(1);
			}

			arg += 1;
			populationAFileName = argv[arg];
			arg += 1;
			populationBFileName = argv[arg];
		}
		else
		{
			cerr << "Invalid option" << endl;
			printUsage(1);
		}
	}

	if (populationAFileName.empty() || populationBFileName.empty())
	{
		cerr << "The filenames of two populations are required" << endl;
		printUsage(1);
	}

	// Open first population
	fin.open(populationAFileName);

	if (!fin.is_open())
	{
		cerr << "Unable to open file " << populationAFileName << endl;
		printUsage(1);
	}

	// Declare and initialize populationA
	hexWorld populationA = entirePopulationFileParser(fin);

	fin.close();

	// Open second population
	fin.open(populationBFileName);

	if (!fin.is_open())
	{
		cerr << "Unable to open file " << populationBFileName << endl;
		printUsage(1);
	}

	hexWorld populationB = entirePopulationFileParser(fin);

	fin.close();

	// Open output file
	fout.open(outputFile);

	if (!fout.is_open())
	{
		cerr << "Unable to open file " << outputFile << endl;
		printUsage(1);
	}

	playHexGames(populationA, populationB, gameStats);

	// Print out game stats to output file
	fout << "Population matchup between:" << endl;
	fout << "\t" << populationAFileName << endl;
	fout << "\t" << populationBFileName << endl;
	fout << endl;
	fout << "Total games won:" << endl;
	fout << "\t" << populationAFileName << ": " << gameStats.numberOfTotalWinsA << endl;
	fout << "\t" << populationBFileName << ": " << gameStats.numberOfTotalWinsB << endl;
	fout << endl;
	fout << "Total number of games won as:" << endl;
	fout << "\tPlayerA: " << gameStats.totalWinsAsA << endl;
	fout << "\tPlayerB: " << gameStats.totalWinsAsB << endl;
	fout << endl;
	// fout << "Winningest players:" << endl;
	// fout << "\t" << populationAFileName << ": " << gameStats.bestPlayerA << " with " 
	// 	<< gameStats.bestPlayerAWins << " games won" << endl;
	// fout << "\t" << populationBFileName << ": " << gameStats.bestPlayerB << " with "
	// 	<< gameStats.bestPlayerBWins << " games won" << endl;

	// Print out sorted players for both players
	// Uses pairs defined in ...helpers.h
	fout << "Players Sorted by number of wins" << endl;

	fout << populationAFileName << ":" << endl;
	
	for (player = 0; player < gameStats.sortedPlayersA.size(); player += 1)
	{
		fout << "\tPlayer" << gameStats.sortedPlayersA[player].second << ": " 
			<< gameStats.sortedPlayersA[player].first.getGamesWon() << endl;
	}

	fout << endl;
	fout << populationBFileName << ":" << endl;
	for (player = 0; player < gameStats.sortedPlayersB.size(); player += 1)
	{
		fout << "\tPlayer" << gameStats.sortedPlayersB[player].second << ": "
			<< gameStats.sortedPlayersB[player].first.getGamesWon() << endl;
	}

	fout.close();
}
