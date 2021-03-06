#ifndef HEXPOPULATIONVSPOPULATIONHELPERS_H
#define HEXPOPULATIONVSPOPULATIONHELPERS_H

#include "hexWorld.h"

struct PopulationPair
{
	int populationATotalWins = 0;
	// Using a pair to keep track of players original location in the population
	// before sorting
	vector<pair<hexGamePlayer, int> > sortedPlayersA;

	int populationBTotalWins = 0;
	vector<pair<hexGamePlayer, int> > sortedPlayersB;

	int totalWinsAsFirstPlayer = 0;
	int totalWinsAsSecondPlayer = 0;

	vector<pair<pair<hexGamePlayer, int>, int> > sortedPlayersAll;
};

extern struct Comparison
{
	bool operator() (pair<hexGamePlayer, int> a, pair<hexGamePlayer, int> b)
	{
		return a.first.getGamesWon() > b.first.getGamesWon();
	}
} compare;

extern struct ComparisonAll
{
	bool operator() (pair<pair<hexGamePlayer, int>, int> a, pair<pair<hexGamePlayer, int>, int> b)
	{
		return a.first.first.getGamesWon() > b.first.first.getGamesWon();
	}
} compareAll;

void printUsage(int exitCode);

bool isNumeric(string input);

void playHexGamesBipartite(hexWorld populationA, hexWorld populationB, PopulationPair &gameStats);

void playHexGamesMelee(hexWorld populationA, hexWorld populationB, PopulationPair &gameStats);

Player playHexGame(hexGamePlayer botA, hexGamePlayer botB);

hexWorld entirePopulationFileParser(ifstream &fin);

#endif
