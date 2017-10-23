#ifndef HEXPOPULATIONVSPOPULATIONHELPERS_H
#define HEXPOPULATIONVSPOPULATIONHELPERS_H

#include "hexWorld.h"

struct PopulationPair
{
	int numberOfTotalWinsA = 0;
	// int bestPlayerA;
	// int bestPlayerAWins;

	// Using a pair to keep track of players original location in the population
	// before sorting
	vector<pair<hexGamePlayer, int> > sortedPlayersA;

	int numberOfTotalWinsB = 0;
	// int bestPlayerB;
	// int bestPlayerBWins;
	vector<pair<hexGamePlayer, int> > sortedPlayersB;

	int totalWinsAsA = 0;
	int totalWinsAsB = 0;
};

extern struct Comparison
{
	bool operator() (pair<hexGamePlayer, int> a, pair<hexGamePlayer, int> b)
	{
		return a.first.getGamesWon() > b.first.getGamesWon();
	}
} compare;

void printUsage(int exitCode);

bool isNumeric(string input);

void playHexGames(hexWorld populationA, hexWorld populationB, PopulationPair &gameStats);

player playHexGame(hexGamePlayer botA, hexGamePlayer botB);

hexWorld entirePopulationFileParser(ifstream &fin);

#endif
