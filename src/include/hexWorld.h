/**********************
Hex Research Project

author: Michael McCarver
advisor: Dr. Rob LeGrand
**********************/

/**************************
This function creates and maintains a population of hexGamePlayers.
**************************/

#ifndef HEXWORLD_H
#define HEXWORLD_H

#include "hexGamePlayer.h"
#include <random>
#include <ctime>
#include <utility>

// static default_random_engine seedGenerator(time(NULL));
static mt19937_64 seedGenerator(time(NULL));

class hexWorld
{
private:
	int numPlayers;
	vector<hexGamePlayer> hexGamePlayers;
	vector<int> netShape;

	void geneticAlgorithm();
	void deletePopulation(){hexGamePlayers.clear();}

	int getNeighborLocation(int playerLocation, int columnOffset, int rowOffset);

	// Swap random weights between two players
	void swapWeightsBetweenPlayers(int playerALocation, int playerBLocation);
	// Swap weights within one player
	void swapWeights(vector<vector<vector<double> > > &net);
	// Copy value of one weight onto another weight within one player
	void copyWeight(vector<vector<vector<double> > > &net);
	// Specify the strategy for how often/how likely swaps will occur
	void swappingStrategy(vector<vector<vector<double> > > &net);

public:
	hexWorld(int numPlayers = 100, vector<int> netShape = {BOARD_SIZE * BOARD_SIZE, BOARD_SIZE, 1});

	hexWorld(vector<hexGamePlayer> hexGamePlayers);

	// Some sort of void function that creates the next generation of players.
	// If this is the first iteration (no existing players), create hexGamePlayers
	void nextGeneration();

	// Function to return a vector of all neighboring hexGamePlayers.
	// Takes an int location as a parameter (which hexGamePlayer we are
	// currently centered on).
	vector<int> getNeighbors(int hexPlayerLocation);

	int getNumPlayers(){return numPlayers;}

	// Return a hexGamePlayer from vector hexGamePlayers
	hexGamePlayer getHexGamePlayer(int playerLocation) {return hexGamePlayers[playerLocation];}
	vector<hexGamePlayer> getHexGamePlayers() {return hexGamePlayers;}

	// Return netshape
	vector<int> getNetShape() {return netShape;}

	// Add a win to a players stats
	void addPlayerWin(int playerLocation) {hexGamePlayers[playerLocation].addGameWon();}

	int getBreeder(mt19937_64 &seedGenerator, vector<int> probabilities);
	double generateWeight(mt19937_64 &seedGenerator, double mean);

	hexWorld& operator=(hexWorld rhs);
};

#endif // #ifndef HEXWORLD_H
