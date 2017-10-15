/*
This main function creates a hexWorld and plays all players against one another

Author: Michael McCarver
Advisor: Dr. Rob LeGrand
*/

#include <hexEvolutionHelpers.h>
#include <cstdlib>
#include <sys/stat.h>

// Helper functions specific to hexEvolution
void printUsage(int exitCode);
void playHexGames(hexWorld &population, ofstream &fout);
player playHexGame(hexGamePlayer hexPlayerA, hexGamePlayer hexPlayerB);

int main(int argc, char *argv[])
{
	time_t now;
	struct tm * currentDT;
	ifstream fin;
	ofstream foutResults, foutSave;
	int player, saveAfterNIterations, arg, iteration, numberOfIterations;
	string argument, resumeFile, outputFile, experimentName, iterationDirectory, saveFile;
	char experimentTime[40], resultsTime[30];
	vector<hexGamePlayer> resumePlayers;
	hexWorld population;

	// Default values
	numberOfIterations = 10;
	saveAfterNIterations = 10;
	outputFile = "results/results.out";

	for (arg = 1; arg < argc; arg += 1)
	{
		argument = argv[arg];

		// Display command line options
		if (argument == "-h" || argument == "--help")
		{
			printUsage(0);
		}

		// Frequency with which to save generation states
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

		// Existing file of hexWorld data to begin evolution with
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

		// Total number of iterations to run.
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

		else
		{
			cerr << "Invalid option." << endl;
			printUsage(1);
		}
	}


	/*
		Read input from resume file
	*/
	if (!resumeFile.empty())
	{
		fin.open(resumeFile);

		if (!fin.is_open())
		{
			cerr << "Unable to open file " << resumeFile << endl;
			printUsage(1);
		}

		hexWorld population = entirePopulationFileParser(fin);

		fin.close();
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
		Create directories for saving experiment data
	*/
	now = time(0);
	currentDT = localtime(&now);
	strftime(experimentTime, 40, "data/experiment_%a_%b%d_%G_%T", currentDT);
	experimentName += experimentTime;
	mkdir(experimentName.c_str(), ACCESSPERMS);


	/*
		Run through all iterations
	*/
	for (iteration = 0; iteration < numberOfIterations; iteration += 1)
	{
		now = time(0);
		currentDT = localtime(&now);
		strftime(resultsTime, 30, "%a_%b%d_%G_%T", currentDT);

		population.nextGeneration();

		playHexGames(population, foutResults);

		if ((iteration + 1) % saveAfterNIterations == 0)
		{
			// Only print out results as often as iterations are printed out
			foutResults << endl << "Stats for iteration" << iteration << endl;

			for (player = 0; player < population.getNumPlayers(); player += 1)
			{
				foutResults << "\tPlayer" << player << " games won: " << population.getHexGamePlayer(player).getGamesWon() << endl;
			}
			foutResults << endl;


			// Make directory for current iteration
			iterationDirectory = experimentName + "/iteration" + to_string(iteration);
			mkdir(iterationDirectory.c_str(), ACCESSPERMS);

			// Save file for current hexWorld data
			saveFile = iterationDirectory + "/population.data";

			foutSave.open(saveFile);

			if (!foutSave.is_open())
			{
				cerr << "Could not save hexWorld data to file " << saveFile << endl;
				exit(1);
			}

			// Function here to save hexWorld data
			printCurrentGenerationToFile(population, foutSave);

			foutSave.close();

			// Save populationRepresentative
			saveFile = iterationDirectory + "/populationRepresentative.player";

			foutSave.open(saveFile);
			
			if (!foutSave.is_open())
			{
				cerr << "Could not save populationRepresentative data to file " << saveFile << endl;
				exit(1);
			}

			// Print a weighted average of all players
			printPopulationRepresentative(population, foutSave);

			foutSave.close();
		}
	}

	foutResults.close();
}
