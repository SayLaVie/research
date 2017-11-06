#include <hexPopulationVsPopulationHelpers.h>

int MAX_DEPTH;

int main (int argc, char *argv[])
{
	int arg, player;
	string outputFile, populationAFileName, populationBFileName, argument;
	bool playBipartite;
	ofstream fout;
	ifstream fin;
	PopulationPair gameStats;

	outputFile = "results/population.match";
	MAX_DEPTH = 0;

	// Default is playing every player against every player
	playBipartite = false;

	// Parse through options
	for (arg = 1; arg < argc; arg += 1)
	{
		argument = argv[arg];

		if (argument == "-b" || argument == "--bipartite")
		{
			playBipartite = true;
		}

		else if (argument == "-d" || argument == "--depth")
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
	hexWorld populationA = hexWorld(entirePopulationFileParser(fin));

	fin.close();

	// Open second population
	fin.open(populationBFileName);

	if (!fin.is_open())
	{
		cerr << "Unable to open file " << populationBFileName << endl;
		printUsage(1);
	}

	hexWorld populationB = hexWorld(entirePopulationFileParser(fin));

	fin.close();

	// Open output file
	fout.open(outputFile);

	if (!fout.is_open())
	{
		cerr << "Unable to open file " << outputFile << endl;
		printUsage(1);
	}

	if (playBipartite)
	{
		playHexGamesBipartite(populationA, populationB, gameStats);		
	}
	else
	{
		playHexGamesMelee(populationA, populationB, gameStats);
	}


	// Print out game stats to output file
	fout << "Population matchup between:" << endl;
	fout << "\t" << populationAFileName << endl;
	fout << "\t" << populationBFileName << endl;
	fout << endl;
	fout << "Total games won:" << endl;
	fout << "\t" << populationAFileName << ": " << gameStats.populationATotalWins << endl;
	fout << "\t" << populationBFileName << ": " << gameStats.populationBTotalWins << endl;
	fout << endl;
	fout << "Total number of games won as:" << endl;
	fout << "\tPlayerA: " << gameStats.totalWinsAsFirstPlayer << endl;
	fout << "\tPlayerB: " << gameStats.totalWinsAsSecondPlayer << endl;
	fout << endl;

	// Print out sorted players for both players
	// Uses pairs defined in ...helpers.h
	fout << "Players Sorted by number of wins" << endl << endl;

	fout << setw(20) << "PopulationA" << setw(25) << "PopulationB" << endl << endl;

	// Let's assume both populations are the same size, to make printing easier
	for (player = 0; player < gameStats.sortedPlayersA.size(); player += 1)
	{
		fout << setw(14) << "Player" << gameStats.sortedPlayersA[player].second << ": "
			<< gameStats.sortedPlayersA[player].first.getGamesWon() <<  " A";

		fout << setw(16) << right << "Player" << gameStats.sortedPlayersB[player].second << ": "
			<< gameStats.sortedPlayersB[player].first.getGamesWon() << " B" << endl;
	}

	fout << endl;
/*
	fout << populationBFileName << ":" << endl;

	for (player = 0; player < gameStats.sortedPlayersB.size(); player += 1)
	{
		fout << "\tPlayer" << gameStats.sortedPlayersB[player].second << ": "
			<< gameStats.sortedPlayersB[player].first.getGamesWon() << " B" << endl;
	}
*/
	fout.close();
}
