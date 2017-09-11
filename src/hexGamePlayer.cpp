/**********************
Hex Research Project
   
author: Michael McCarver
advisor: Dr. Rob LeGrand
**********************/

#include "hexGamePlayer.h"

 // Functions that will be shared among the players
int hexGamePlayer::play(const Board &board, player whichPlayer)
{
   int moveToMake = 0;

   moveToMake = miniMax(board, whichPlayer, 0, -10000, 10000); // May change these infinities

   return moveToMake;
}

double hexGamePlayer::miniMax(Board board, player whichPlayer, int depth, int alpha, int beta)
{
   int boardSize, utilityValue, eval, location, tmpBestMove, heuristicValue;
   bool maximizer;
   Board copyBoard;

   maximizer = depth % 2 == 0;

   if (board.isGameOver())
      return maximizer ? -500 + depth : 500 - depth; // Will change these values to be less arbitrary

   if (depth > maxDepth)
   {
      heuristicValue = neuralNetHeuristic();

      return maximizer ? heuristicValue : -heuristicValue;
   }

   boardSize = board.getSize(); // With a set boardsize, we may not need this
   utilityValue = maximizer ? -10000 : 10000; // will change these infinities
   tmpBestMove = -1; // invalid default move

   for (location = 0; location < boardSize * boardSize; ++location) // May change the way we iterate through valid moves
   {
      if (board.isValidMove(location))
      {
         copyBoard = board;
         copyBoard.makeMove(location, whichPlayer);

         eval = miniMax(copyBoard, whichPlayer == playerA ? playerB : playerA, depth + 1, alpha, beta);

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


// Neural net takes in vector of vectors of weights, returns a double between 0 and 1
double hexGamePlayer::neuralNetHeuristic()
{
   //implement neural net here
}
