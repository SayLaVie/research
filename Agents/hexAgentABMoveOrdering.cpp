/*
   Michael McCarver

   This agent used an alpha-beta pruning minimax foundation, and tried
   to use a heuristic to order moves in an intelligent way.
*/

#include "../src/hex.h"
#include <queue>

struct hexAgentABMoveOrderingComparator // Comparator for priority_queue comparison evaluations
{
   bool operator()(pair<int, int> lhs, pair<int, int> rhs)
   {
      return lhs.second > rhs.second;
      // return (lhs->costToHere + lhs->heuristicCost) > (rhs->costToHere + rhs->heuristicCost);
   }
};

int hexAgentABMoveOrderingMax(Board board, player whichPlayer, int alpha, int beta, int depth); // Max part of minimax

int hexAgentABMoveOrderingMin(Board board, player whichPlayer, int alpha, int beta, int depth); // Min part of minimax

priority_queue<pair<int, int>, vector<pair<int, int> >, hexAgentABMoveOrderingComparator> hexAgentABMoveOrderingEval(Board board, player whichPlayer); // Global priority_queue used for move ordering

int hexAgentABMoveOrdering(const Board &board, player whichPlayer)
{
   int bestMove, boardSize;

   boardSize = board.getSize();

   bestMove = hexAgentABMoveOrderingMax(board, whichPlayer, -2, 2, 0);

   if (board.isValidMove(bestMove)) // If no best move was found, minimax returns location 0. This is not always valid
   {
      return bestMove;
   }

   for (bestMove = 0; bestMove < boardSize * boardSize; ++bestMove) // Find the first available location
   {
      if (board.isValidMove(bestMove))
      {
         return bestMove;
      }
   }

   return bestMove;
}


int hexAgentABMoveOrderingMax(Board board, player whichPlayer, int alpha, int beta, int depth)
{
   int utilityValue, min, location, size, tmpBestMove;
   Board copyBoard;
   priority_queue<pair<int, int>, vector<pair<int, int> >, hexAgentABMoveOrderingComparator> moveOrder;

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

   if (depth > 1)
   {
      return 0;
   }

   moveOrder = hexAgentABMoveOrderingEval(board, whichPlayer); // Evaluate all valid moves, and order them

   if (whichPlayer == playerA)
   {
      while (moveOrder.size() > 0)
      {
         location = moveOrder.top().first;
         moveOrder.pop();

         if (board.isValidMove(location))
         {
            copyBoard = board;
            copyBoard.makeMove(location, whichPlayer);

            min = hexAgentABMoveOrderingMin(copyBoard, whichPlayer == playerA ? playerB : playerA, alpha, beta, depth + 1);

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
      }
   }

   else // We are playerB
   {
      while (moveOrder.size() > 0)
      {
         location = moveOrder.top().first;
         moveOrder.pop();

         if (board.isValidMove(location))
         {
            copyBoard = board;
            copyBoard.makeMove(location, whichPlayer);

            min = hexAgentABMoveOrderingMin(copyBoard, whichPlayer == playerA ? playerB : playerA, alpha, beta, depth + 1);

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
      }

   }


   if (depth == 0)
   {
      return tmpBestMove;
   }

   return utilityValue;
}

int hexAgentABMoveOrderingMin(Board board, player whichPlayer, int alpha, int beta, int depth)
{
   int utilityValue, max, location, size;
   Board copyBoard;
   priority_queue<pair<int, int>, vector<pair<int, int> >, hexAgentABMoveOrderingComparator> moveOrder;

   // The base case
   if (board.isGameOver())
   {
      return 1;
   }

   size = board.getSize();
   location = 0;

   // Set utilityValue to infinity
   utilityValue = 2;

   moveOrder = hexAgentABMoveOrderingEval(board, whichPlayer);

   if (whichPlayer == playerA)
   {
      while (moveOrder.size() > 0)
      {
         location = moveOrder.top().first;
         moveOrder.pop();

         if (board.isValidMove(location))
         {
            copyBoard = board;
            copyBoard.makeMove(location, whichPlayer);

            max = hexAgentABMoveOrderingMax(copyBoard, whichPlayer == playerA ? playerB : playerA, alpha, beta, depth + 1);

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
      }
   }
   else // We are playerB
   {
      while (moveOrder.size() > 0)
      {
         location = moveOrder.top().first;
         moveOrder.pop();

        if (board.isValidMove(location))
         {
            copyBoard = board;
            copyBoard.makeMove(location, whichPlayer);

            max = hexAgentABMoveOrderingMax(copyBoard, whichPlayer == playerA ? playerB : playerA, alpha, beta, depth + 1);

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
      }
   }

   return utilityValue;
}

priority_queue<pair<int, int>, vector<pair<int, int> >, hexAgentABMoveOrderingComparator> hexAgentABMoveOrderingEval(Board board, player whichPlayer)
{
   int row, column, location, boardSize, distanceFromBorders, nearbyNeighbors, evaluation;
   priority_queue<pair<int, int>, vector<pair<int, int> >, hexAgentABMoveOrderingComparator> moveOrder;
   pair<int, int> thisLocation;

   boardSize = board.getSize();
   nearbyNeighbors = 0;

   for (location = 0; location < boardSize * boardSize; ++location)
   {
      thisLocation.first = location;
      thisLocation.second = 0;
      row = location / boardSize;
      column = location % boardSize;

      if (whichPlayer == playerA)
      {
         // Find distance from borders
         distanceFromBorders = min(row - 0, boardSize - row);
         // Make sure that distance is never 0
         distanceFromBorders += 1;

         // Find nearby Neighbors
         if (column - 1 >= 0)
         {
            if (board.getBoard()[boardSize * row + (column - 1)].getOwner() == playerB)
            {
               ++nearbyNeighbors;
            }

            if (row + 1 < boardSize)
            {
               if (board.getBoard()[boardSize * (row + 1) + (column - 1)].getOwner() == playerB)
               {
                  ++nearbyNeighbors;
               }
            }
         }

         if (column + 1 < boardSize)
         {
            if (board.getBoard()[boardSize * row + (column + 1)].getOwner() == playerB)
            {
               ++nearbyNeighbors;
            }

            if (row - 1 >= boardSize)
            {
               if (board.getBoard()[boardSize * (row - 1) + (column + 1)].getOwner() == playerB)
               {
                  ++nearbyNeighbors;
               }
            }
         }

         if (row + 1 < boardSize)
         {
            if (board.getBoard()[boardSize * (row + 1) + column].getOwner() == playerB)
            {
               ++nearbyNeighbors;
            }
         }

         if (row - 1 >= boardSize)
         {
            if (board.getBoard()[boardSize * (row - 1) + column].getOwner() == playerB)
            {
               ++nearbyNeighbors;
            }
         }


      }
      else // We are playerB
      {
         // Find distance from borders
         distanceFromBorders = min(column - 0, boardSize - column);
         // Make sure not to divide by 0
         distanceFromBorders += 1;

         // Find nearby Neighbors
         if (column - 1 >= 0)
         {
            if (board.getBoard()[boardSize * row + (column - 1)].getOwner() == playerB)
            {
               ++nearbyNeighbors;
            }

            if (row + 1 < boardSize)
            {
               if (board.getBoard()[boardSize * (row + 1) + (column - 1)].getOwner() == playerB)
               {
                  ++nearbyNeighbors;
               }
            }
         }

         if (column + 1 < boardSize)
         {
            if (board.getBoard()[boardSize * row + (column + 1)].getOwner() == playerB)
            {
               ++nearbyNeighbors;
            }

            if (row - 1 >= boardSize)
            {
               if (board.getBoard()[boardSize * (row - 1) + (column + 1)].getOwner() == playerB)
               {
                  ++nearbyNeighbors;
               }
            }
         }

         if (row + 1 < boardSize)
         {
            if (board.getBoard()[boardSize * (row + 1) + column].getOwner() == playerB)
            {
               ++nearbyNeighbors;
            }
         }

         if (row - 1 >= boardSize)
         {
            if (board.getBoard()[boardSize * (row - 1) + column].getOwner() == playerB)
            {
               ++nearbyNeighbors;
            }
         }
      }

      // How should these values be weighed?
      evaluation = 500 / distanceFromBorders;
      evaluation += (100 * nearbyNeighbors);
      thisLocation.second += evaluation;

      moveOrder.push(thisLocation);
   }

   return moveOrder;
}
