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
   for (int iteration = 0; iteration < 100; ++iteration)
   {
      cout << "Entering iteration " << iteration << endl;

      // Find and set weights for the upcoming generation.
      population.nextGeneration();

      cout << "\tNext generation created" << endl;

      // Play all games amongst neighbors.
      playHexGames(population);

      cout << "Games played. Leaving iteration " << iteration << endl;
   }
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

      cout << "\t\tPlayer " << playerLocation << endl;

      for (currentNeighbor = 0; currentNeighbor < neighboringPlayers.size(); ++currentNeighbor)
      {
         cout << "\t\t\tPlayer " << playerLocation << " vs Player " << neighboringPlayers[currentNeighbor] << endl;
         // Play two games, so that each player can play as both A and B.
         gameWinner = playHexGame(population.getHexGamePlayer(playerLocation), population.getHexGamePlayer(neighboringPlayers[currentNeighbor]));

         // Add appropriate wins
         if (gameWinner == playerA)
         {
            population.addPlayerWin(playerLocation);
         }
         else
         {
            population.addPlayerWin(currentNeighbor);
         }
      }
   }

   cout << "\tLeaving playHexGames" << endl << endl;
}

// Return the player that won the game
// May change return value to a pair<player, int> if we want to keep track of
// length of game
player playHexGame(hexGamePlayer hexPlayerA, hexGamePlayer hexPlayerB)
{
   int playerMove, numberOfTurns;
   player currentPlayer;
   Board board(BOARD_SIZE);

   numberOfTurns = 0;

   cout << "Turns passed: ";

   while (!board.isGameOver())
   {
      if (numberOfTurns % 5 == 0)
      {
         cout << numberOfTurns << " ";
      }

      currentPlayer = static_cast<player>(numberOfTurns % 2);

      if (currentPlayer == playerA)
      {
         playerMove = hexPlayerA.play(board, playerA);
      }
      else
      {
         playerMove = hexPlayerB.play(board, playerB);
      }

      // Make sure move is valid
      if (!board.isValidMove(playerMove))
      {
         // Output some sort of message??
         // Or just keep track of how many bad moves?
      }
      else
      {
         board.makeMove(playerMove, currentPlayer);
      }

      numberOfTurns += 1;
   }

   cout << "\t\t\t\tLeaving playHexGame" << endl;
   board.printBoard();

   // Last player who played has won the game.
   // If numberOfTurns is odd, playerA won the game
   return numberOfTurns % 2 == 1 ? playerA : playerB;
}
