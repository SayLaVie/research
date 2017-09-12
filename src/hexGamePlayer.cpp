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
double hexGamePlayer::neuralNetHeuristic(Board board, player whichPlayer)
{
	int location, column, rowOrigination, rowDestination;
	vector<int> boardState;
	vector<double> inputVector;
	double summation;
	player thisOwner;

	// First, extract the state of the current board
	// We are using: -1 for opponent owned, 0 for empty, 1 for ours
	for (location = 0; location < boardSize * boardSize; ++location)
	{
		thisOwner = board.getBoard()[location].getOwner();

		if (thisOwner == none)
			boardState.push_back(0);
		else if (thisOwner == whichPlayer)
			boardState.push_back(1);
		else
			boardState.push_back(-1);
	}

/****************************
This next section needs more thought
****************************/
	// Now, feed the inputs from the board into our Neural Net
	for (column = 0; column < neuralNetWeights.size(); ++column)
	{
		for (rowDestination = 0; rowDestination < neuralNetWeights[column].size(); ++rowDestination)
		{
			summation = 0;

			// The relationship between board state inputs and the first layer of the neural net should
			// be consistent.
			for (rowOrigination = 0; rowOrigination < neuralNetWeights[column][rowDestination].size(); ++rowOrigination)
			{
				// If column is 0, then we are feeding in the board state inputs
				if (column == 0)
				{
					summation += (boardState[rowOrigination] * neuralNetWeights[column][rowDestination][rowOrigination]);
				}

				// Otherwise, we are feeding in the inputs from previous summations
				else
				{
					summation += (inputVector[rowOrigination] * neuralNetWeights[column][rowDestination][rowOrigination]);
				}
			}

			// The summation becomes the input for the next column
			inputVector.push_back(summation);
		}
	}
}
