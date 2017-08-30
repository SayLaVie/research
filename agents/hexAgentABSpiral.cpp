/*
   Michael McCarver

   This agent used an alpha-beta pruning minimax, and evaluated moves
   in a sort of spiral pattern.
*/

#include "../src/hex.h"

// Max portion of minimax
int hexAgentABSpiralMax(Board board, player whichPlayer, int alpha, int beta, int depth);

// Min portion of minimax
int hexAgentABSpiralMin(Board board, player whichPlayer, int alpha, int beta, int depth);


// Find a best move with minimax functions. If no best move is found, 0 will be returned,
// which is not always a valid move. If that is the case, play the first available square
int hexAgentABSpiral(const Board &board, player whichPlayer)
{
   int bestMove, boardSize;

   boardSize = board.getSize();

   bestMove = hexAgentABSpiralMax(board, whichPlayer, -2, 2, 0);

   if (board.isValidMove(bestMove))
   {
      return bestMove;
   }

   for (bestMove = 0; bestMove < boardSize * boardSize; ++bestMove)
   {
      if (board.isValidMove(bestMove))
      {
         return bestMove;
      }
   }

   return bestMove;
}


int hexAgentABSpiralMax(Board board, player whichPlayer, int alpha, int beta, int depth)
{
   int utilityValue, min, location, size, tmpBestMove, column, row, jump;
   Board copyBoard;

   // The base case
   if (board.isGameOver())
   {
      return -1;
   }


   size = board.getSize();
   location = 0;
   tmpBestMove = 0;

   // Set to negative infinity
   utilityValue = -2;

   if (depth > 5)
   {
      return 0;
   }

   // Begin looking for moves in middle of board, in a side to side fashion
   if (whichPlayer == playerA)
   {
      column = 0;
      row = size / 2 - ((size + 1) % 2);
      jump = 1;

      // Move left to right accross board
      while (row >= 0 && row < size)
      {
         while (column < size)
         {
            location = size * row + column;

            if (board.isValidMove(location))
            {
               copyBoard = board;
               copyBoard.makeMove(location, whichPlayer);

               min = hexAgentABSpiralMin(copyBoard, whichPlayer == playerA ? playerB : playerA, alpha, beta, depth + 1);

               // utilityValue equals the maximization of utilityValue and min
               if (utilityValue < min)
               {
                  utilityValue = min;

                  // Assign the best move so far
                  tmpBestMove = location;
               }

               // If utilityValue is >= our Beta, prune
               if (utilityValue >= beta || utilityValue == 1)
               {
                  return depth == 0 ? tmpBestMove : utilityValue;
               }

               // Alpha equals the maximization of alpha and utilityValue
               if (alpha < utilityValue)
               {
                  alpha = utilityValue;
               }
            }

            ++column;
         }

         // Jump to a row above and fix column value
         row += jump++;
         --column;

         if (row >= 0 && row < size)
         {
            // Move right to left across board
            while (column >= 0)
            {
               location = size * row + column;

               if (board.isValidMove(location))
               {
                  copyBoard = board;
                  copyBoard.makeMove(location, whichPlayer);

                  min = hexAgentABSpiralMin(copyBoard, whichPlayer == playerA ? playerB : playerA, alpha, beta, depth + 1);

                  // utilityValue equals the maximization of utilityValue and min
                  if (utilityValue < min)
                  {
                     utilityValue = min;

                     // Assign the best move so far
                     tmpBestMove = location;
                  }

                  // If utilityValue is >= our Beta, prune
                  if (utilityValue >= beta || utilityValue == 1)
                  {
                     return depth == 0 ? tmpBestMove : utilityValue;
                  }

                  // Alpha equals the maximization of alpha and utilityValue
                  if (alpha < utilityValue)
                  {
                     alpha = utilityValue;
                  }
               }

               --column;
            }
         }

         // Jump to a row below and fix column value
         row -= jump++;
         ++column;
      }
   }
   // Start in middle of board connnecting from bottom to top
   else // We are playerB
   {
      column = size / 2 - ((size + 1) % 2);
      row = 0;
      jump = 1;

      // Move bottom to top across board
      while (column >= 0 && column < size)
      {
         while (row < size)
         {
            location = size * row + column;

            if (board.isValidMove(location))
            {
               copyBoard = board;
               copyBoard.makeMove(location, whichPlayer);

               min = hexAgentABSpiralMin(copyBoard, whichPlayer == playerA ? playerB : playerA, alpha, beta, depth + 1);

               // utilityValue equals the maximization of utilityValue and min
               if (utilityValue < min)
               {
                  utilityValue = min;

                  // Assign the best move so far
                  tmpBestMove = location;
               }

               // If utilityValue is >= our Beta, prune
               if (utilityValue >= beta || utilityValue == 1)
               {
                  return depth == 0 ? tmpBestMove : utilityValue;
               }

               // Alpha equals the maximization of alpha and utilityValue
               if (alpha < utilityValue)
               {
                  alpha = utilityValue;
               }
            }

            ++row;
         }

         // Jump to a column above and fix column value
         column += jump++;
         --row;

         if (column >= 0 && column < size)
         {
            // Move top to bottom across board
            while (row >= 0)
               {
               location = size * row + column;

               if (board.isValidMove(location))
               {
                  copyBoard = board;
                  copyBoard.makeMove(location, whichPlayer);

                  min = hexAgentABSpiralMin(copyBoard, whichPlayer == playerA ? playerB : playerA, alpha, beta, depth + 1);

                  // utilityValue equals the maximization of utilityValue and min
                  if (utilityValue < min)
                  {
                     utilityValue = min;

                     // Assign the best move so far
                     tmpBestMove = location;
                  }

                  // If utilityValue is >= our Beta, prune
                  if (utilityValue >= beta || utilityValue == 1)
                  {
                     return depth == 0 ? tmpBestMove : utilityValue;
                  }

                  // Alpha equals the maximization of alpha and utilityValue
                  if (alpha < utilityValue)
                  {
                     alpha = utilityValue;
                  }
               }

            --row;
            }
         }

         // This commented out section should have been in the code, but was not
         // column -= jump++;
         // ++row;
      }
   }

   if (depth == 0)
   {
      return tmpBestMove;
   }

   return utilityValue;
}

int hexAgentABSpiralMin(Board board, player whichPlayer, int alpha, int beta, int depth)
{
   int utilityValue, max, location, size, column, row, jump;
   Board copyBoard;

   // The base case
   if (board.isGameOver())
   {
      return 1;
   }

   size = board.getSize();
   location = 0;

   // Set utilityValue to infinity
   utilityValue = 2;

   if (whichPlayer == playerA)
   {
      column = 0;
      row = size / 2 - ((size + 1) % 2);
      jump = 1;

      while (row >= 0 && row < size)
      {
         // Move left to right across board
         while (column < size)
         {
            location = size * row + column;

            if (board.isValidMove(location))
            {
               copyBoard = board;
               copyBoard.makeMove(location, whichPlayer);

               max = hexAgentABSpiralMax(copyBoard, whichPlayer == playerA ? playerB : playerA, alpha, beta, depth + 1);

               // utilityValue equals the minimization of utilityValue and max
               if (utilityValue > max)
               {
                  utilityValue = max;
               }

               // If utilityValue <= our Alpha, prune
               if (utilityValue <= alpha || utilityValue == -1)
               {
                  return utilityValue;
               }

               // Beta equals the minimization of beta and utilityValue
               if (beta > utilityValue)
               {
                  beta = utilityValue;
               }
            }

            ++column;
         }

         // Jump to a row above and fix column value;
         row += jump++;
         --column;

         if (row >= 0 && row < size)
         {
            // Move right to left across board
            while (column >= 0)
            {
            if (board.isValidMove(location))
               {
                  copyBoard = board;
                  copyBoard.makeMove(location, whichPlayer);

                  max = hexAgentABSpiralMax(copyBoard, whichPlayer == playerA ? playerB : playerA, alpha, beta, depth + 1);

                  // utilityValue equals the minimization of utilityValue and max
                  if (utilityValue > max)
                  {
                     utilityValue = max;
                  }

                  // If utilityValue <= our Alpha, prune
                  if (utilityValue <= alpha || utilityValue == -1)
                  {
                     return utilityValue;
                  }

                  // Beta equals the minimization of beta and utilityValue
                  if (beta > utilityValue)
                  {
                     beta = utilityValue;
                  }
               }

               --column;
            }
         }

         // Jump to a row below and fix column value
         row -= jump++;
         ++column;
      }
   }
   else // We are playerB
   {
      column = size / 2 - ((size + 1) % 2);
      row = 0;
      jump = 1;

      while (column >= 0 && column < size)
      {
         // Move bottom to top across board
         while (row < size)
         {
            location = size * row + column;

            if (board.isValidMove(location))
            {
               copyBoard = board;
               copyBoard.makeMove(location, whichPlayer);

               max = hexAgentABSpiralMax(copyBoard, whichPlayer == playerA ? playerB : playerA, alpha, beta, depth + 1);

               // utilityValue equals the minimization of utilityValue and max
               if (utilityValue > max)
               {
                  utilityValue = max;
               }

               // If utilityValue <= our Alpha, prune
               if (utilityValue <= alpha || utilityValue == -1)
               {
                  return utilityValue;
               }

               // Beta equals the minimization of beta and utilityValue
               if (beta > utilityValue)
               {
                  beta = utilityValue;
               }
            }

            ++row;
         }

         // Jump to another column and fix row value.
         column += jump++;
         --row;

         if (column >= 0 && column < size)
         {
            // Move top to bottom across board
            while (row >= 0)
            {
               if (board.isValidMove(location))
               {
                  copyBoard = board;
                  copyBoard.makeMove(location, whichPlayer);

                  max = hexAgentABSpiralMax(copyBoard, whichPlayer == playerA ? playerB : playerA, alpha, beta, depth + 1);

                  // utilityValue equals the minimization of utilityValue and max
                  if (utilityValue > max)
                  {
                     utilityValue = max;
                  }

                  // If utilityValue <= our Alpha, prune
                  if (utilityValue <= alpha || utilityValue == -1)
                  {
                     return utilityValue;
                  }

                  // Beta equals the minimization of beta and utilityValue
                  if (beta > utilityValue)
                  {
                     beta = utilityValue;
                  }
               }

               --row;
            }

            // Jump to another column and fix row value
            column -= jump++;
            ++row;
         }
      }
   }

   return utilityValue;
}
