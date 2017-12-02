#include "hexWorld.h"

void printUsage(int exitCode);

int randomPlayerMove(Board board);

Player playHexGame(hexGamePlayer hexPlayer);

void playHexGames(hexWorld hexPopulation, ofstream &fout);

hexWorld entirePopulationFileParser(ifstream &fin);

struct PlayerStatistics
{
   int playerNumber;
   int totalGamesWon;
   int numGamesWonAsA;
};

extern struct Comparison
{
   bool operator() (PlayerStatistics a, PlayerStatistics b)
   {
      return a.totalGamesWon > b.totalGamesWon;
   }
} compare;
