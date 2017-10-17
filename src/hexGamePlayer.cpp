/**********************
Hex Research Project

author: Michael McCarver
advisor: Dr. Rob LeGrand
**********************/

#include <hexGamePlayer.h>
#include <cmath>

// Constructor takes a set of weights for the neural net heuristic
hexGamePlayer::hexGamePlayer(vector<vector<vector<double> > > neuralNetWeights)
{
	this->neuralNetWeights = neuralNetWeights;
	this->gamesWon = 0;
	this->movesMade = 0;
}

hexGamePlayer::hexGamePlayer(vector<vector<vector<double> > > neuralNetWeights, int numGamesWon)
{
   this->neuralNetWeights = neuralNetWeights;
   this->gamesWon = numGamesWon;
   this->movesMade = 0;
}

// This function starts off the minimax function with default values, and returns the integer location
// of which move to play next
int hexGamePlayer::play(const Board &board, player whichPlayer)
{
   int moveToMake, location;
   Board copyBoard;

   moveToMake = miniMax(board, whichPlayer, 0, -MAX_DEPTH + 1, MAX_DEPTH + 1);

   if (board.isValidMove(moveToMake))
   {
      return moveToMake;
   }

   // If no good move was found, simply choose the next available move
   for (location = 0; location < BOARD_SIZE * BOARD_SIZE; location += 1)
   {
      if (board.isValidMove(location))
      {
         return location;
      }
   }

   return moveToMake;
}

// This is the recursive minimax function, with a depth limit (maxDepth set in the hex.h file).
// If it finishes at level 0, will return the location of the best move found.
double hexGamePlayer::miniMax(Board board, player whichPlayer, int depth, double alpha, double beta)
{
   int location, tmpBestMove, column, row, jump;
   double utilityValue, eval, heuristicValue;
   bool maximizer;
   Board copyBoard;

   maximizer = depth % 2 == 0;

   if (board.isGameOver())
   {
      return maximizer ? -1 - (double)(MAX_DEPTH / depth) : 1 + (double)(MAX_DEPTH / depth);
   }

   if (depth >= MAX_DEPTH)
   {
      heuristicValue = neuralNetHeuristic(board, whichPlayer);

      return maximizer ? heuristicValue : -heuristicValue;
   }

   utilityValue = maximizer ? -MAX_DEPTH - 1 : MAX_DEPTH + 1;
   tmpBestMove = -1; // invalid default move


   // Begin looking for moves in middle of board, going side to side
   if (whichPlayer == playerA)
   {
      column = 0;
      row = BOARD_SIZE / 2 - ((BOARD_SIZE + 1) % 2);
      jump = 1;

      // Move left to right across board
      while (row >= 0 && row < BOARD_SIZE)
      {
         while (column < BOARD_SIZE)
         {
            location = BOARD_SIZE * row + column;

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
                        // cout << "Player" << (whichPlayer == playerA ? "A" : "B") << " Evaluation: " << utilityValue << endl;
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

            column += 1;
         }

         // Jump to a row above and fix column value
         row += jump;
         jump += 1;
         column -= 1;

         if (row >= 0 && row < BOARD_SIZE)
         {
            // Move right to left across board
            while (column >= 0)
            {
               location = BOARD_SIZE * row + column;

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
                           // cout << "Player" << (whichPlayer == playerA ? "A" : "B") << " Evaluation: " << utilityValue << endl;
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

               column -= 1;
            }
         }

         // Jump to a row below and fix column value
         row -= jump;
         jump += 1;
         column += 1;
      }
   }
   // Start in middle of board connecting top to bottom
   else
   {
      column = BOARD_SIZE / 2 - ((BOARD_SIZE + 1) % 2);
      row = 0;
      jump = 1;

      // Move bottom to top across board
      while (column >= 0 && column < BOARD_SIZE)
      {
         while (row < BOARD_SIZE)
         {
            location = BOARD_SIZE * row + column;

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
                        // cout << "Player" << (whichPlayer == playerA ? "A" : "B") << " Evaluation: " << utilityValue << endl;
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
            row += 1;
         }

         // Jump and fix
         column += jump;
         jump += 1;
         row -= 1;

         if (column >= 0 && column < BOARD_SIZE)
         {
            // Move top to bottom across board
            while (row >= 0)
            {
               location = BOARD_SIZE * row + column;

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
                           // cout << "Player" << (whichPlayer == playerA ? "A" : "B") << " Evaluation: " << utilityValue << endl;
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
               row -= 1;
            }
         }

         column -= jump;
         jump += 1;
         row += 1;
      }
   }

/*   for (location = 0; location < BOARD_SIZE * BOARD_SIZE; location += 1)
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
                  // cout << "Player" << (whichPlayer == playerA ? "A" : "B") << " Evaluation: " << utilityValue << endl;
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
*/

   if (depth == 0)
   {
      // cout << "Player" << (whichPlayer == playerA ? "A" : "B") << " Evaluation: " << utilityValue << endl;
      return tmpBestMove;
   }

   return utilityValue;
}


// Neural net takes in vector of vectors of weights, returns a double between 0 and 1
double hexGamePlayer::neuralNetHeuristic(const Board board, player whichPlayer)
{
	int location, jump, layer, rowOrigination, rowDestination;
	vector<int> boardState;
	vector<double> inputVector, outputVector;
	double summation;
	player thisOwner;

   // The board needs to look differently depending on which player we are
   if (whichPlayer == playerA)
   {

   	// First, extract the state of the current board
   	// We are using: -1 for opponent owned, 0 for empty, 1 for ours
      // May split this into its own function later
   	for (location = 0; location < BOARD_SIZE * BOARD_SIZE; location += 1)
   	{
         thisOwner = board.getBoard()[location].getOwner();

   		if (thisOwner == none)
   		{
            boardState.push_back(0);
         }
   		else if (thisOwner == whichPlayer)
         {
   			boardState.push_back(1);
         }
   		else
         {
   			boardState.push_back(-1);
         }
   	}
   }
   else
   {
      // Jump through each column before moving on to next row
      for (location = 0; location < BOARD_SIZE; location += 1)
      {
         for (jump = location; jump < BOARD_SIZE * BOARD_SIZE; jump += BOARD_SIZE)
         {
            thisOwner = board.getBoard()[jump].getOwner();

            if (thisOwner == none)
            {
               boardState.push_back(0);
            } else if (thisOwner == whichPlayer)
            {
               boardState.push_back(1);
            } else
            {
               boardState.push_back(-1);
            }
         }
      }
   }

	// Now, feed the inputs from the board into our Neural Net
	for (layer = 0; layer < neuralNetWeights.size(); ++layer)
	{
      // cout << endl << endl << endl << "Layer: " << layer << endl << endl << endl;
      outputVector.clear();

		for (rowDestination = 0; rowDestination < neuralNetWeights[layer].size(); ++rowDestination)
		{
			summation = 0;

			// The bias weight will always be the first element of the vector.
         summation += neuralNetWeights[layer][rowDestination][0];

         // The rowOrigination needs to be offset by 1, because of the bias weight.
			for (rowOrigination = 1; rowOrigination < neuralNetWeights[layer][rowDestination].size(); ++rowOrigination)
			{
				// If layer is 0, then we are feeding in the board state inputs
				if (layer == 0)
				{
               // Minus 1 to counteract the bias weight offset
					summation += (boardState[rowOrigination - 1] * neuralNetWeights[layer][rowDestination][rowOrigination]);
				}

				// Otherwise, we are feeding in the inputs from previous summations
				else
				{
               // Minus 1 to counteract the bias weight offset
					summation += (inputVector[rowOrigination - 1] * neuralNetWeights[layer][rowDestination][rowOrigination]);
				}
			}

			// The summation becomes the input for the next layer. The number of inputs for the
         // next layer will match up with the row depth.
			outputVector.push_back(sigmoidFunction(summation));
		}

      inputVector = outputVector;
	}

   // The last input vector should have only one entry -- the final output of
   // the neural net.
   return inputVector[0];
}

double hexGamePlayer::sigmoidFunction(double input)
{
   return 1.0 / (1.0 + exp(-input));
}

double hexGamePlayer::getWeight(int layer, int rowDestination, int rowOrigination)
{
   return neuralNetWeights[layer][rowDestination][rowOrigination];
}

void hexGamePlayer::setWeight(int layer, int rowDestination, int rowOrigination, double newWeight)
{
   neuralNetWeights[layer][rowDestination][rowOrigination] = newWeight;
}

/*
Prints out weights to a file (stream provided by fout) for machine to read in the future.
"stat" symbol guide indicates the next input is numGamesWon
"layer" symbol guide indicates that a new layer of weights is about to begin
"node" symbol guide indicates that the following inputs will be double values (a single origination node's weights)
"endPlayer" symbol guide indicates end of players weights
"//" symbol guide indicates that the line is a comment
*/
void hexGamePlayer::printWeights(ofstream &fout)
{
   fout << "stat " << gamesWon << endl;

   for (int layer = 0; layer < neuralNetWeights.size(); layer += 1)
   {
      fout << "\tlayer" << endl;

      for (int rowDestination = 0; rowDestination < neuralNetWeights[layer].size(); rowDestination += 1)
      {
         fout << "\t\tnode ";

         for (int rowOrigination = 0; rowOrigination < neuralNetWeights[layer][rowDestination].size(); rowOrigination += 1)
         {
            fout << neuralNetWeights[layer][rowDestination][rowOrigination] << ',';
         }

         fout << endl;
      }
   }
   fout << "endPlayer" << endl;
}
