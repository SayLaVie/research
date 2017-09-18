/**********************
Hex Research Project

author: Michael McCarver
advisor: Dr. Rob LeGrand
**********************/

#include "hexWorld.h"

// Function to play hex games. May take whole hexWorld as parameter,
// or maybe just two players. If only two players, will return the
// result of one particular game. If an entire world, nothing is
// needed for return.
void playHexGames(hexWorld &population);
player playHexGame(hexGamePlayer playerA, hexGamePlayer playerB);

int main(int argc, char *argv[])
{
   // Some sort of command-line argument sanitation will go here


   // Create an instance of hexWorld
   hexWorld population(NUM_PLAYERS);

   // While we are playing (for now, while true)
   while (true)
   {
      // Find and set weights for the upcoming generation.
      population.nextGeneration();

      // Play all games amongst neighbors.
      playHexGames(population);
   }
}

void playHexGames(hexWorld &population)
{
   int playerLocation, currentNeighbor;
   player gameWinner;
   vector<int> neighboringPlayers;

   for (playerLocation = 0; playerLocation < BOARD_SIZE * BOARD_SIZE; ++playerLocation)
   {
      neighboringPlayers = population.getNeighbors(playerLocation);

      for (currentNeighbor = 0; currentNeighbor < neighboringPlayers.size(); ++currentNeighbor)
      {
         // Play two games, so that each player can play as both A and B.
         gameWinner = playHexGame(population.getHexGamePlayer(playerLocation), population.getHexGamePlayer(currentNeighbor));

         // Add appropriate wins
         if (gameWinner == playerA)
         {
            population.getHexGamePlayer(playerLocation).addGameWon();
         }
         else
         {
            population.getHexGamePlayer(currentNeighbor).addGameWon();
         }
      }
   }
}

// Return the player that won the game
// May change return value to a pair<player, int> if we want to keep track of
// length of game
player playHex(hexGamePlayer hexPlayerA, hexGamePlayer hexPlayerB)
{
   int playerMove, numberOfTurns;
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

   // Last player who played has won the game.
   // If numberOfTurns is odd, playerA won the game
   return numberOfTurns % 2 == 1 ? playerA : playerB;
}
