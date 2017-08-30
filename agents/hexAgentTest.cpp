/*
   Test player for hex game
*/

#include "../src/hex.h"

int hexAgentTestMiniMax(Board board, player whichPlayer, int depth, int alpha, int beta);
int hexAgentTestHeuristic(Board board);

int maxDepth = 5;

int hexAgentTest(const Board &board, player whichPlayer)
{
   int bestMove, boardSize;

   boardSize = board.getSize();

   bestMove = hexAgentTestMiniMax(board, whichPlayer, 0, -10000, 10000);

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

int hexAgentTestMiniMax(Board board, player whichPlayer, int depth, int alpha, int beta)
{
   int boardSize, utilityValue, eval, location, tmpBestMove, heuristicValue;
   bool maximizer;
   Board copyBoard;

   maximizer = depth % 2 == 0;


   if (board.isGameOver())
   {
      return maximizer ? -500 + depth : 500 - depth; // Try to weigh in favor of shallow depth winning moves
   }

   if (depth > maxDepth)
   {
      heuristicValue = hexAgentTestHeuristic(board);

      return maximizer ? heuristicValue : -heuristicValue;
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

         eval = hexAgentTestMiniMax(copyBoard, whichPlayer == playerA ? playerB : playerA, depth + 1, alpha, beta);

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

// This heuristic function should return a value between -500 and 500, representing how good of a board state
// we think this board is. We should keep in mind that winning moves return (500 - currentDepth), and we will
// not want to return a value that beats a winning move. We should determine what our max depth will be, then
// figure out what the lowest value winning move is possible to be returned, then make sure that our heuristic
// will never return something better than that, causing our minimax function to choose a well evaluated board
// instead of a winning move.

// For now, let's assume that the maxDepth will be 5 (*we will create a global variable). This means that the
// best possible return value is 499, and the worst winning move is 495. We'll let the minimax function
// determine whether it's positive or negative.

int hexAgentTestHeuristic(Board board)
{
   return 0;
}
