 #include "hexWorld.h"

#ifndef HEXHELPERFUNCTIONS_H
#define HEXHELPERFUNCTIONS_H

void printUsage(int exitCode);
bool isNumeric(string input);

void playHexGames(hexWorld &population, ofstream &fout);
player playHexGame(hexGamePlayer hexPlayerA, hexGamePlayer hexPlayerB);

vector<vector<vector<double> > > singleNeuralNetFileParser(ifstream &fin);
vector<hexGamePlayer> entirePopulationFileParser(ifstream &fin);
void printCurrentGenerationToFile(hexWorld population, ofstream &fout);

#endif // #ifndef HEX_H
