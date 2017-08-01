#include "../src/hex.h"

int hexAgentSubGraphMinMax(Board board, player whichPlayer, int depth, vector<int> moveList);

int hexAgentSubGraph (const Board &board, player whichPlayer)
{
   int subGraphSize, boardSize, row, column, moveToAdd, tmpRow, tmpCol;
   Board *subGraph;
   vector<Tile> subTiles;
   vector<int> moveList;
   player mover;

   boardSize = board.getSize();
   subGraphSize = 3;

   do
   {
      // Break board into all subgraphs, evaluating each one and pushing
      // moves onto priority_queue based on heuristic
      for (row = 0; row <= boardSize - subGraphSize; ++row)
      {
         for (column = 0; column <= boardSize - subGraphSize; ++column)
         {
            subGraph = new Board(subGraphSize);

            // Two inner loops to push tiles onto vector of tiles
            for (int subRow = row; subRow < row + subGraphSize; ++subRow)
            {
               for (int subCol = column; subCol < column + subGraphSize; ++subCol)
               {
                  subTiles.push_back(board.getBoard()[boardSize * subRow + subCol]);
               }
            }

            // Use vector of tiles to create subGraph
            for (int i = 0; i < subGraphSize * subGraphSize; ++i)
            {
               mover = subTiles[i].getOwner();

               if (mover != none)
               {
                  subGraph->makeMove(i, mover);
               }

               subTiles.clear();
            }

            // We now have a subGraph. Evaluate it
            moveToAdd = hexAgentSubGraphMinMax(*subGraph, whichPlayer, 0, moveList);

            // Turn this location into a location for a board the next size up, if it was a good one
            if (moveToAdd != -1)
            {
               // Turn location into row and column for this size board
               tmpRow = moveToAdd / subGraphSize;
               tmpCol = moveToAdd % subGraphSize;

               // Turn these numbers into a location for a board 1 size bigger
               tmpRow += row;
               tmpCol += column;

               // cout << endl << "row " << tmpRow << " col " << tmpCol;

               // Push onto move vector
               // cout << endl << "size " << subGraphSize << " currentCol: " << column << " move " << moveToAdd << " " << (subGraphSize + (subGraphSize == boardSize ? 0 : 1)) * tmpRow + tmpCol;


               // What's this doing? (Maybe pushing the move onto the next size up subgraph?)
               if (subGraphSize < boardSize)
               {
                  moveList.push_back((subGraphSize + 1) * tmpRow + tmpCol);
               }
               else
               {
                  moveList.push_back(subGraphSize * tmpRow + tmpCol);
               }
            }

            delete subGraph;

         }
      }

      subGraphSize += 1;

   } while (subGraphSize <= boardSize);

   if (moveList.size() > 0)
    {//cout << endl << "play " << moveList.back() << endl;;
      return moveList.back();
   }

   return 0;
}

int hexAgentSubGraphMinMax(Board board, player whichPlayer, int depth, vector<int> moveList)
{
   int location, boardSize, /*value,*/ minMax, bestMoveSoFar;
   vector<int> copyList, notOnList;
   Board copyBoard;

   boardSize = board.getSize();

   if (board.isGameOver())
   {
      if (depth % 2 == 0)
      {
         return -1;
      }

      return 1;
   }

   // Try out the best moves from before
   if (boardSize != 3)
   {
      while (!copyList.empty())
      {
         bestMoveSoFar = copyList.back();
         copyList.pop_back();

         if (board.isValidMove(bestMoveSoFar))
         {
            copyBoard = board;
            copyBoard.makeMove(bestMoveSoFar, whichPlayer);

            minMax = hexAgentSubGraphMinMax(copyBoard, whichPlayer == playerA ? playerB : playerA, depth + 1, copyList);

            if (depth % 2 == 0)
            {
               if (minMax == 1)
               {
                  if (depth == 0)
                  {
                     return bestMoveSoFar;
                  }

                  return 1;
               }
            }
            else
            {
               if (minMax == -1)
               {
                  return -1;
               }
            }
         }
      }

      if (depth == 0)
      {
         return -1;//bestMoveSoFar;
      }

      return minMax;
   }

   else
   {
      for (location = 0; location < boardSize * boardSize; ++location)
      {
         if (board.isValidMove(location))
         {
            copyBoard = board;

            copyBoard.makeMove(location, whichPlayer);

            minMax = hexAgentSubGraphMinMax(copyBoard, whichPlayer == playerA ? playerB : playerA, depth + 1, moveList);

            if (depth % 2 == 0)
            {
               if (minMax == 1)
               {
                  if (depth == 0)
                  {
                     return location;
                  }

                  return 1;
               }
            }
            else
            {
               if (minMax == -1)
               {
                  return -1;
               }
            }
         }
      }

      if (depth == 0)
      {
         return -1;//bestMoveSoFar;
      }

      return minMax;
   }

   return -1;
}
