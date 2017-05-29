/*
   Research: The Game of Hex
   Student: Michael McCarver
   Professor: Dr. Rob LeGrand

   This research project is centered around The Game of Hex.
*/

#include "hex.h"

extern const int numAgents;
extern int (*agentFunc[])(const Board &, player);
extern string agentStr[];

// plays the game and returns a char representing the result
char playHexGame(int (*agentA)(const Board&, player whichPlayer), int (*agentB)(const Board&, player whichPlayer), Board& hexGame);

// main function
int main()
{
   char result;
   int  i, j, boardSize, numWinsA[numAgents], numWinsB[numAgents],
       numLossesA[numAgents], numLossesB[numAgents], numMoves[numAgents], order[numAgents], numInvalid[numAgents], temp;

   // the hexBoard that will be played on
   Board *hexGrid;

   cout << "The Game of Hex\n"
        << "Results\n\n";

   // intializes the arrays that will be used to sort for the top player
   for (i = 0; i < numAgents; i += 1)
   {
      numWinsA[i] = 0;
      numWinsB[i] = 0;
      numLossesA[i] = 0;
      numLossesB[i] = 0;
      numMoves[i] = 0;
      numInvalid[i] = 0;
   }

   // plays the game with all of the available agents and prints the board of moves made by the agents
   for (boardSize = 3; boardSize <= 11; boardSize += 1)
   {
      // loops throug the agent functions
      for (i = 0; i < numAgents; i += 1)
      {
         for (j = 0; j < numAgents; j += 1)
         {
            if (i != j)
            {
               // outputs the current board size
               cout << "Current board size is " << boardSize << "x" << boardSize << "\n";
               // sets hexGrid equal to a new board of the given size
               hexGrid = new Board(boardSize);
               // outputs the agents playing against each other
               cout << "player A = " << agentStr[i] << ", player B = " << agentStr[j];
               // plays the agents against each other and saves the result
               result = playHexGame(agentFunc[i], agentFunc[j], *hexGrid);
               // updates the number of moves made by each agent
               numMoves[i] += hexGrid->getATurn();
               numMoves[j] += hexGrid->getBTurn();
               // outputs the final board state
               cout << "\nFinal Board State: \n";
               hexGrid->printBoard();
               cout << "\n";
               // outputs the result of the game, who is the winner and if an agent played an
               // invalid move as well as updates number of wins and losses as A and B
               if (result == 'A')
               {
                  cout << "A";
                  numWinsA[i] += 1;
                  numLossesB[j] += 1;
               }
               else if (result == 'B')
               {
                  cout << "B";
                  numLossesA[i] += 1;
                  numWinsB[j] += 1;
               }
               else if (result == 'C')
               {
                  cout << "B played an invalid move! A";
                  numWinsA[i] += 1;
                  numLossesB[j] +=1;
                  numInvalid[j] += 1;
               }
               else
               {
                  cout << "A played an invalid move! B";
                  numLossesA[i] += 1;
                  numWinsB[j] += 1;
                  numInvalid[i] += 1;
               }

               cout << " is the winner!\n\n";
               // deletes the board
               delete hexGrid;
            }
         }
      }
   }


   //  sort the agents based on total number of wins, and then the total number of moves made
   for (i = 0; i < numAgents; i += 1)
   {
      order[i] = i;
   }

   for (i = 0; i < numAgents - 1; i += 1)
   {
      for (j = i + 1; j < numAgents; j += 1)
      {
         if (numWinsA[order[i]] + numWinsB[order[i]] < numWinsA[order[j]] + numWinsB[order[j]] || numWinsA[order[i]] + numWinsB[order[i]] == numWinsA[order[j]] + numWinsB[order[j]] &&
             (numMoves[order[i]] > numMoves[order[j]] || numMoves[order[i]] == numMoves[order[j]]  &&
              agentStr[order[i]] >= agentStr[order[j]])) // sort the agents from best to worst
         {
            temp = order[i];
            order[i] = order[j];
            order[j] = temp;
         }
      }
   }

   // output the overall results of the game
      cout << "\n"
        << "Overall standings:       all         as A         as B     Total #     Invalid     W / L\n"
        << "                       W    L       W    L       W    L      moves      moves      Ratio\n";
   for (i = 0; i < numAgents; i += 1)
   {
      double ratio;
      if (numLossesA[order[i]] + numLossesB[order[i]] == 0)
      {
         ratio = 1;
      }
      else
      {
         ratio = (static_cast<double>(numWinsA[order[i]]) + static_cast<double>(numWinsB[order[i]]))/(static_cast<double>(numLossesA[order[i]]) + static_cast<double>(numLossesB[order[i]]));
      }
      cout << setw(20) << left << agentStr[order[i]]
           << " " << setw(4) << right << numWinsA[order[i]] + numWinsB[order[i]]
           << " " << setw(4) << right << numLossesA[order[i]] + numLossesB[order[i]]
           << " " << setw(7) << right << numWinsA[order[i]]
           << " " << setw(4) << right << numLossesA[order[i]]
           << " " << setw(7) << right << numWinsB[order[i]]
           << " " << setw(4) << right << numLossesB[order[i]]
           << " " << setw(8) << right << numMoves[order[i]]
           << " " << setw(8) << right << numInvalid[order[i]]
           << " " << setprecision(5) << setw(11) << right << ratio << "\n";
   }

   return 0;
}

// plays the hex game
char playHexGame(int (*agentA)(const Board&, player whichPlayer), int (*agentB)(const Board&, player whichPlayer), Board& hexGrid)
{
   char result = 'A'; // compiler complains without an initialization

   // repeatedly call playerA's and playerB's function until someone wins or makes an invalid move
   while(!hexGrid.isGameOver())
   {
      // player A goes first
      int playerA_move = (*agentA)(hexGrid, playerA);

      //if it is an invalid move, B automatically wins
      if(!hexGrid.isValidMove(playerA_move))
      {
         // Let's let 'D' represent that player B won because of A's invalid move
         result = 'D';

         return result;
      }
      // Update the board for player A
      else
      {
         hexGrid.makeMove(playerA_move, playerA);
      }

      // if playerA made a move that won them the game then return result
      if(hexGrid.isGameOver())
      {
         result = 'A';

         return result;
      }

      int playerB_move = (*agentB)(hexGrid, playerB);

      // if it is an invalid move, A automatically wins
      if(!hexGrid.isValidMove(playerB_move))
      {
         // Let's let 'C' represent that A won because of B's invalid move
         result = 'C';

         return result;
      }
      // Update the board for player B
      else
      {
         hexGrid.makeMove(playerB_move, playerB);
      }

      // if playerB made a move that won them the game then return result
      if(hexGrid.isGameOver())
      {
         result = 'B';

         return result;
      }

   }

   return result;
}
