#include "../src/hex.h"

/*
   The idea behind this strategy is to break the board into all
   sub-boards that are size 3, then to find the best moves for each
   of those size-3 boards, and then to push those moves onto a 
   priority-queue (or maybe just a regular vector, since there will
   only be one best move in the end). Then, the board is broken down 
   into all sub-boards that are size 4, the best moves of the size-3 
   boards are used to determine the best moves for the size-4 boards, 
   which replace the size-3 best moves. This continues, with the 
   sub-boards growing in size, until the sub-board is the same size as 
   the real board. Hopefully, the best moves of each sub-board will be 
   the best move for any larger board.


      ***NOTES***
   Will have to come up with a default movelist to use for the initial
   3x3's, or possibly tell the code to perform a true minimax if the
   movelist is empty.
*/

int hexAgentSubGraphMinMax(Board board, player whichPlayer, int depth, vector<int> moveList);
int hexAgentSubGraphMinMax(Board board, player whichPlayer, int depth);

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

            // Use vector of tiles to maintain states of subgraph tiles
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
            //    Will probably need to do some move sanitation here
            moveToAdd = hexAgentSubGraphMinMax(*subGraph, whichPlayer, 0, moveList);

            // Next, we should convert the moveToAdd to a real position on the full-size
            // board. Then, when sending a moveList to a sub-board for evaluation, we
            // can check to make sure that those positions are within the sub-board.
            // ??Should I create a helper function for converting to/from??

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


               // The following section looks wrong to me. The moves should be converted
               // to be appropriate for the true board size, then converted back if the
               // next board needs them.
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


// This function is used to search for best moves on 3x3 size boards
int hexAgentSubGraphMinMax(Board board, player whichPlayer, int depth, vector<int> moveList)
{
   int location, boardSize, minMax;
   vector<int> emptyVector;
   Board copyBoard;
   
   boardSize = board.getSize();
   emptyVector.clear();

   if (board.isGameOver()) {
      return depth % 2 == 0 ? -1 : 1;
   }

   if (boardSize == 3) {
      // Iterate through all possible moves. There are quicker searches which
      // choose moves in a different order, but this will serve for now.
      for (location = 0; location < boardSize * boardSize; ++location)
      {
         if (board.isValidMove(location)) {
            copyBoard = board;
            copyBoard.makeMove(location, whichPlayer);

            // find the value of the current board
            minMax = hexAgentSubGraphMinMax(copyBoard, whichPlayer == playerA ? playerB : playerA, depth + 1);

            // If we are on the initial level of recursion, and we get a 1 back, this is a best move
            if (depth == 0 && minMax == 1)
               return location;

            // If depth % 2 == 0, we are looking for a maximum value
            if (depth % 2 == 0 && minMax == 1)
               return 1;

            if (depth % 2 == 1 && minMax == -1)
               return -1;
         }
      }
   }

   else if (!moveList.empty()) {
      while (!moveList.empty()) {
         location = moveList.back();
         moveList.pop_back();

         copyBoard = board;
         copyBoard.makeMove(location, whichPlayer);

         // Find value of current board
         minMax = hexAgentSubGraphMinMax(copyBoard, whichPlayer == playerA ? playerB : playerA, depth + 1, emptyVector);
      }
   }

   else {
      minMax = 0;
   }

   // If we made it all the way through the loop without finding a best move, return
   // the value for the last recursion search we performed. Since the value wasn't
   // picked as a best move value, it will necessarily be a bad move value, which
   // is appropriate.
   return minMax;
}
