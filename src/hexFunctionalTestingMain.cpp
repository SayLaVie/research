/**********************
Hex Research Project

author: Michael McCarver
advisor: Dr. Rob LeGrand
**********************/

#include "hexWorld.h"
#include <fstream>

void playHexGames(hexWorld &population, ofstream &fout);
player playHexGame(hexGamePlayer playerA, hexGamePlayer playerB);

int main(int argc, char *argv[])
{
   clock_t t;
   ofstream fout;

   fout.open("results.out");

   fout << "Timing to see what it looks like to run 5 iterations" << endl;
   fout << "\tPlayers: 16\t\tNetShape: 25/10/5/1\t\tDepth: 4" << endl << endl;

   // Create an instance of hexWorld
   hexWorld population(NUM_PLAYERS);

   // While we are playing (for now, while true)

   for (int iteration = 0; iteration < 5; iteration += 1)
   {
      fout << "Iteration: " << iteration << endl;

      // Find and set weights for the upcoming generation.
      population.nextGeneration();

      t = clock();
      // Play all games amongst neighbors.
      playHexGames(population, fout);

      t = clock() - t;
      fout << endl << "\tIt took " << ((float)t / CLOCKS_PER_SEC / 60) << " minutes for this iteration" << endl << endl;   

      for (int playerLocation = 0; playerLocation < NUM_PLAYERS; playerLocation += 1)
      {
         fout << "Player " << playerLocation << " won " << population.getHexGamePlayer(playerLocation).getGamesWon()
               << " total games." << endl;
      }   
      
      fout << "~~~~~~~~~~~~~~~~~~~~~~~~~" << endl << endl;

      population.nextGeneration();
   }

   fout.close();      
}

void playHexGames(hexWorld &population, ofstream &fout)
{
   int playerLocation, currentNeighbor;
   player gameWinner;
   vector<int> neighboringPlayers;

  for (playerLocation = 0; playerLocation < BOARD_SIZE * BOARD_SIZE; ++playerLocation)
   {
      neighboringPlayers.clear();
      neighboringPlayers = population.getNeighbors(playerLocation);

      fout << "\tPlayer " << playerLocation << ": ";

      for (currentNeighbor = 0; currentNeighbor < neighboringPlayers.size(); ++currentNeighbor)
      {
         // cout << "\t\t\tPlayer " << playerLocation << " vs Player " << neighboringPlayers[currentNeighbor] << endl;

         // t = clock();
         // Play two games, so that each player can play as both A and B.
         gameWinner = playHexGame(population.getHexGamePlayer(playerLocation), population.getHexGamePlayer(neighboringPlayers[currentNeighbor]));
         // cout << "It took " << ((float)(clock() - t) / CLOCKS_PER_SEC) << " seconds to play this game" << endl;
         // Add appropriate wins
         if (gameWinner == playerA)
         {
            population.addPlayerWin(playerLocation);
            fout << "+ ";
         }
         else
         {
            population.addPlayerWin(currentNeighbor);
            fout << "- ";
         }
      }
   }

   // cout << "\tLeaving playHexGames" << endl << endl;
}

// Return the player that won the game
// May change return value to a pair<player, int> if we want to keep track of
// length of game
player playHexGame(hexGamePlayer hexPlayerA, hexGamePlayer hexPlayerB)
{
   int playerMove, numberOfTurns, tmp;
   player currentPlayer;
   Board board(BOARD_SIZE);

   numberOfTurns = 0;

   while (!board.isGameOver())
   {
      currentPlayer = static_cast<player>(numberOfTurns % 2);

      if (currentPlayer == playerA)
      {
         playerMove = hexPlayerA.play(board, playerA);
      }
      else
      {
         playerMove = hexPlayerB.play(board, playerB);
      }

      if (!board.isValidMove(playerMove))
      {
         cout << "INVALID MOVE: " << playerMove << endl;
         board.printBoard();
         while (true)
         {
            cin >> tmp;
            if (tmp == -1)
            {
               board.printBoard();
            }
            else if (tmp == -2)
            {
               return playerB;
            }
            else
            {
               cout << "Parent: " << board.getBoard()[tmp].getParent() << " Flag: " << board.getBoard()[tmp].getFlag()<< endl;
            }
         }

      }
      else
      {
         board.makeMove(playerMove, currentPlayer);
      }

      numberOfTurns += 1;
   }

   // Last player who played has won the game.
   // If numberOfTurns is odd, playerA won the game
   return numberOfTurns % 2 == 1 ? playerA : playerB;
}
