#include "hexGamePlayer.h"
#include <random>

static mt19937_64 seedGenerator(time(NULL));

void printUsage(int exitCode);

int randomPlayerMove(Board board);

Player playHexGame(hexGamePlayer hexPlayer);

void playHexGames(hexGamePlayer hexPlayer, ofstream &fout);

vector<vector<vector<double> > > singleNeuralNetFileParser(ifstream &fin);
