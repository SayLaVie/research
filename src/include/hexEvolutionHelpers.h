#include "hexWorld.h"

void printUsage(int exitCode);

bool isNumeric(string input);

Player playHexGame(hexGamePlayer hexPlayerA, hexGamePlayer hexPlayerB);

void playHexGames(hexWorld &population, ofstream &fout);

hexWorld entirePopulationFileParser(ifstream &fin);

void hexPopulationSanitation(hexWorld population);

void printCurrentGenerationToFile(hexWorld population, ofstream &fout);

// Find a weighted average for each weight throughout the population, and create a hexGamePlayer
// with these weights.
void printPopulationRepresentative(hexWorld population, ofstream &fout);

// Print a data file for each player in the iteration
void printEachPopulationPlayer(hexWorld population, string fileNamePrefix);
