/*
   Simple player for hex game
*/

#include "../src/hex.h"

int hexAgentSimpleMiniMax(Board board, player whichPlayer, int depth, int alpha, int beta);

int hexAgentSimple(const Board &board, player whichPlayer)
{
   int bestMove, boardSize;

   boardSize = board.getSize();

   bestMove = hexAgentSimpleMiniMax(board, whichPlayer, 0, -10000, 10000);

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

int hexAgentSimpleMiniMax(Board board, player whichPlayer, int depth, int alpha, int beta)
{
   int boardSize, utilityValue, eval, location, tmpBestMove;
   bool maximizer;
   Board copyBoard;

   maximizer = depth % 2 == 0;


   if (board.isGameOver())
   {
      return maximizer ? -500 + depth : 500 - depth; // Try to weigh in favor of shallow depth winning moves
      // return maximizer ? -1 : 1;
   }

   boardSize = board.getSize();
   utilityValue = maximizer ? -10000 : 10000; // Set appropriate infinity
   tmpBestMove = -1;

   for (location = 0; location < boardSize * boardSize; ++location)
   {
      if (board.isValidMove(location))
      {
         copyBoard = board;
         copyBoard.makeMove(location, whichPlayer);

         eval = hexAgentSimpleMiniMax(copyBoard, whichPlayer == playerA ? playerB : playerA, depth + 1, alpha, beta);

         if (maximizer)
         {
            if (utilityValue < eval)
            {
               utilityValue = eval;

               tmpBestMove = location;
            }

            if (utilityValue >= beta)
            {
               if (depth == 0)
               {
                  // cout << endl << utilityValue;
                  return tmpBestMove;
               }
               return utilityValue;
            }

            if (alpha < utilityValue)
            {
               alpha = utilityValue;
            }
         }

         else
         {
            if (utilityValue > eval)
            {
               utilityValue = eval;
            }

            if (utilityValue <= alpha)
            {
               return utilityValue;
            }

            if (beta > utilityValue)
            {
               beta = utilityValue;
            }
         }
      }
   }

   if (depth == 0)
   {
      return tmpBestMove;
   }

   return utilityValue;
}
