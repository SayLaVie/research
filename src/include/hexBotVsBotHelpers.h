  #include "hexGamePlayer.h"

vector<vector<vector<double> > > singleNeuralNetFileParser(ifstream &fin);

void printUsage(int exitcode);

void playHexGame(hexGamePlayer botA, string nameBotA, hexGamePlayer botB, string nameBotB, ofstream &fout);

bool isNumeric(string input);
