#include "hexWorld.h"

struct PopulationPair
{
	int numberOfTotalWinsA = 0;
	int bestPlayerA;
	int bestPlayerAWins;

	int numberOfTotalWinsB = 0;
	int bestPlayerB;
	int bestPlayerBWins;

	int totalWinsAsA = 0;
	int totalWinsAsB = 0;
};

void printUsage(int exitCode);

bool isNumeric(string input);

void playHexGames(hexWorld populationA, hexWorld populationB, PopulationPair &gameStats);

player playHexGame(hexGamePlayer botA, hexGamePlayer botB);

hexWorld entirePopulationFileParser(ifstream &fin);
