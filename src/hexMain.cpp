/*
   Research: The Game of Hex
   Student: Michael McCarver
   Professor: Dr. Rob LeGrand

   This research project is centered around The Game of Hex.
*/

#include "hexWorld.h"

// Function to play hex games. May take whole hexWorld as parameter,
// or maybe just two players. If only two players, will return the
// result of one particular game. If an entire world, nothing is
// needed for return.
void beginIteration(hexWorld &population);
void playHex(hexGamePlayer &playerA, hexGamePlayer &playerB);

int main(int argc, char *argv[])
{
   // Some sort of command-line argument sanitation will go here


   // Create an instance of hexWorld
   hexWorld population(NUM_PLAYERS);

   // While we are playing (for now, while true)
   while (1)
   {
      // Find and set weights for the upcoming generation.
      population.nextGeneration();

      // Play all games amongst neighbors.
      beginIteration(population);
   }
}

void beginIteration(hexWorld &population)
{
   int playerLocation, currentNeighbor;
   vector<int> neighboringPlayers;

   for (playerLocation = 0; playerLocation < BOARD_SIZE * BOARD_SIZE; ++playerLocation)
   {
      neighboringPlayers = population.getNeighbors(playerLocation);

      for (currentNeighbor = 0; currentNeighbor < neighboringPlayers.size(); ++currentNeighbor)
      {
         // Play two games, so that each player can play as both A and B.
         playHex(population.hexGamePlayers[playerLocation], population.hexGamePlayers[currentNeighbor]);
         playHex(population.hexGamePlayers[currentNeighbor], population.hexGamePlayers[playerLocation]);
      }
   }
}

void playHex(hexGamePlayer &hexPlayerA, hexGamePlayer &hexPlayerB)
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
   }

   // Last player who played has won the game.
   // We can keep track of data here.
}
