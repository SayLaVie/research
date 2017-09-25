/**********************
Hex Research Project

author: Michael McCarver
advisor: Dr. Rob LeGrand
**********************/

#include "hexWorld.h"

void playHexGames(hexWorld &population);
player playHexGame(hexGamePlayer playerA, hexGamePlayer playerB);

int main(int argc, char *argv[])
{
   // Some sort of command-line argument sanitation will go here

   // Create an instance of hexWorld
   hexWorld population(NUM_PLAYERS);

   // While we are playing (for now, while true)
   // for (int iteration = 0; iteration < 100; ++iteration)
   // {
   //    cout << "Entering iteration " << iteration << endl;

   //    // Find and set weights for the upcoming generation.
   //    population.nextGeneration();

   //    cout << "\tNext generation created" << endl;

   //    // Play all games amongst neighbors.
   //    playHexGames(population);

   //    cout << "Games played. Leaving iteration " << iteration << endl;
   // }
   population.nextGeneration();
}

void playHexGames(hexWorld &population)
{
   int playerLocation, currentNeighbor;
   player gameWinner;
   vector<int> neighboringPlayers;

   cout << endl << "\tEntering playHexGames" << endl;

   for (playerLocation = 0; playerLocation < BOARD_SIZE * BOARD_SIZE; ++playerLocation)
   {
      neighboringPlayers.clear();
      neighboringPlayers = population.getNeighbors(playerLocation);

      cout << "\t\tPlayer " << playerLocation << ": ";

      for (currentNeighbor = 0; currentNeighbor < neighboringPlayers.size(); ++currentNeighbor)
      {
         // cout << "\t\t\tPlayer " << playerLocation << " vs Player " << neighboringPlayers[currentNeighbor] << endl;

         // Play two games, so that each player can play as both A and B.
         gameWinner = playHexGame(population.getHexGamePlayer(playerLocation), population.getHexGamePlayer(neighboringPlayers[currentNeighbor]));

         // Add appropriate wins
         if (gameWinner == playerA)
         {
            population.addPlayerWin(playerLocation);
            cout << "+ ";
         }
         else
         {
            population.addPlayerWin(currentNeighbor);
            cout << "- ";
         }
      }

      cout << endl;
   }

   for (playerLocation = 0; playerLocation < BOARD_SIZE * BOARD_SIZE; playerLocation += 1)
   {
      cout << "Player " << playerLocation << " won " << population.getHexGamePlayer(playerLocation).getGamesWon()
            << " total games." << endl;
   }

   cout << "\tLeaving playHexGames" << endl << endl;
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
