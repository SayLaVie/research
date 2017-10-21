#include "hexWorld.h"

struct PopulationPair
{
	int numberOfTotalWinsA = 0;
	int bestPlayerA;

	int numberOfTotalWinsB = 0;
	int bestPlayerB;
};

void printUsage(int exitCode);

PopulationPair playHexGames(hexWorld populationA, hexWorld populationB);

player playHexGame(hexGamePlayer botA, hexGamePlayer botB);

hexWorld entirePopulationFileParser(ifstream &fin);
