/**********************
Hex Research Project

author: Michael McCarver
advisor: Dr. Rob LeGrand
**********************/

#include "hexWorld.h"
#include <fstream>

void playHexGames(hexWorld &population);
player playHexGame(hexGamePlayer playerA, hexGamePlayer playerB);

int main(int argc, char *argv[])
{
   cout << "Taking a closer look at the particulars of what the functions are doing." << endl;
   cout << "\tPlayers: 16\t\tNetShape: 25/10/5/1\t\tDepth: 4" << endl << endl;

   // Create an instance of hexWorld
   hexWorld population(NUM_PLAYERS);

   // for (int iteration = 0; iteration < 5; iteration += 1)
   // {
      // cout << "Iteration: " << iteration << endl;

      // Find and set weights for the upcoming generation.
      population.nextGeneration();

      population.getHexGamePlayer(0).printWeights();

      cout << endl << endl;

      // Play all games amongst neighbors.
      // playHexGames(population);
      playHexGame(population.getHexGamePlayer(0), population.getHexGamePlayer(1));
      
   // }
}

void playHexGames(hexWorld &population)
{
   int playerLocation, currentNeighbor;
   player gameWinner;
   vector<int> neighboringPlayers;

  for (playerLocation = 0; playerLocation < NUM_PLAYERS; ++playerLocation)
   {
      neighboringPlayers.clear();
      neighboringPlayers = population.getNeighbors(playerLocation);

      for (currentNeighbor = 0; currentNeighbor < neighboringPlayers.size(); ++currentNeighbor)
      {
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
         board.printBoard();
         // cout << endl << "Player " << (numberOfTurns % 2 == 0 ? "A" : "B") << " board evaluation: ";
         // cout << (numberOfTurns % 2 == 0 ? hexPlayerA.neuralNetHeuristic(board, playerA) : hexPlayerB.neuralNetHeuristic(board, playerB));
         cout << endl << "\t----------------------------------\n\n";
      }

      numberOfTurns += 1;
   }

   // board.printBoard();
   cout << endl << "Winner: " << (numberOfTurns % 2 == 1 ? "PlayerA" : "PlayerB") << endl;

   // Last player who played has won the game.
   // If numberOfTurns is odd, playerA won the game
   return numberOfTurns % 2 == 1 ? playerA : playerB;
}
