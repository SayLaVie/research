#include "hexWorld.h"

void printUsage(int exitCode);

bool isNumeric(string input);

player playHexGame(hexGamePlayer hexPlayerA, hexGamePlayer hexPlayerB);

void playHexGames(hexWorld &population, ofstream &fout);

hexWorld entirePopulationFileParser(ifstream &fin);

void hexPopulationSanitation(hexWorld population);

void printCurrentGenerationToFile(hexWorld population, ofstream &fout);
