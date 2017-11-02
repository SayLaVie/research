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
int hexGamePlayer::play(const Board &board, Player whichPlayer)
{
   int moveToMake;
   Board copyBoard;

   if (MAX_DEPTH == 0)
   {
      moveToMake = boardEvalLearning(board, whichPlayer);
   }

   else
   {
   	// Because thresholding is removed for last layer, the infinities need to be changed accordingly
      // moveToMake = miniMax(board, whichPlayer, 0, -MAX_DEPTH + 1, MAX_DEPTH + 1);
      moveToMake = miniMax(board, whichPlayer, 0, -100000, 100000);
   }

   if (board.isValidMove(moveToMake))
   {
      return moveToMake;
   }

   // There should never be an invalid move returned, so exit with an error here
   cerr << "Invalid move selected at turn " << board.getTurn() << " by player " << (whichPlayer == PlayerA ? "PlayerA" : "PlayerB") << endl;
   board.printBoard();
   cout << endl;
   printWeights();
   // exit(1);
   return -1;
}

double hexGamePlayer::boardEvalLearning(Board board, Player whichPlayer)
{
   int location, tmpBestMove;
   double utilityValue, eval;
   Board copyBoard;

   // If this is ever returned, an error will be thrown.
   tmpBestMove = -1;

   // Set utilityValue to positive/negative 'infinity'
   utilityValue = -100000;

   // Go through every potential move and evaluate the board after a 
   // hypothetical move there.
   for (location = 0; location < BOARD_SIZE * BOARD_SIZE; location += 1)
   {
   	if (board.isValidMove(location))
   	{
   		copyBoard = board;
   		copyBoard.makeMove(location, whichPlayer);

   		eval = neuralNetHeuristic(copyBoard, whichPlayer);

   		if (utilityValue < eval)
   		{
   			utilityValue = eval;

   			tmpBestMove = location;
   		}
   	}
   }

   // If an error is about to be thrown, print out some data for debugging
   if (tmpBestMove == -1)
   { 
	   // for (location = 0; location < BOARD_SIZE * BOARD_SIZE; location += 1)
	   // {
	   // 	if (board.isValidMove(location))
	   // 	{
	   // 		copyBoard = board;
	   // 		copyBoard.makeMove(location, whichPlayer);

	   // 		eval = neuralNetHeuristic(copyBoard, whichPlayer);

	   // 		cout << "Hypothetical move: " << location << "\tEval: " << eval << endl;
	   // 	}
	   // }  	
   }

   return tmpBestMove;
}

// This is the recursive minimax function, with a depth limit (maxDepth set in the hex.h file).
// If it finishes at level 0, will return the location of the best move found.
double hexGamePlayer::miniMax(Board board, Player whichPlayer, int depth, double alpha, double beta)
{
   int location, tmpBestMove;
   double utilityValue, eval, heuristicValue;
   bool maximizer;
   Board copyBoard;
   Player truePlayer;

   maximizer = depth % 2 == 0;

   if (board.isGameOver())
   {
      // Incentivize quick playing by decreasing the win value the further depth away it is
      return maximizer ? -1 - (double)MAX_DEPTH / depth : 1 + (double)MAX_DEPTH / depth;
   }

   if (depth >= MAX_DEPTH)
   {
      // This method below is likely incorrect
      // heuristicValue = neuralNetHeuristic(board, whichPlayer);
      // return maximizer ? heuristicValue : -heuristicValue;

      truePlayer = maximizer ? whichPlayer : (whichPlayer == PlayerA ? PlayerB : PlayerA);
      heuristicValue = neuralNetHeuristic(board, truePlayer);

      return heuristicValue;
   }

   // Set utilityValue to appropriate 'infinity'
   utilityValue = maximizer ? -100000 : 100000;

   // If this is ever returned, an error will be thrown
   tmpBestMove = -1;

   // Evaluate each move with alpha-beta pruning
   for (location = 0; location < BOARD_SIZE * BOARD_SIZE; location += 1)
   {
      if (board.isValidMove(location))
      {
         copyBoard = board;
         copyBoard.makeMove(location, whichPlayer);

         eval = miniMax(copyBoard, whichPlayer == PlayerA ? PlayerB : PlayerA, depth + 1, alpha, beta);

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
double hexGamePlayer::neuralNetHeuristic(const Board board, Player whichPlayer)
{
	int location, jump, layer, rowOrigination, rowDestination;
	vector<int> boardState;
	vector<double> inputVector, outputVector;
	double summation;
	Player thisOwner;

   // The board needs to look differently depending on which player we are
   if (whichPlayer == PlayerA)
   {

   	// First, extract the state of the current board
   	// We are using: -1 for opponent owned, 0 for empty, 1 for ours
      // May split this into its own function later
   	for (location = 0; location < BOARD_SIZE * BOARD_SIZE; location += 1)
   	{
         thisOwner = board.getBoard()[location].getOwner();

   		if (thisOwner == None)
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

            if (thisOwner == None)
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
   }

	// Now, feed the inputs from the board into our Neural Net
	for (layer = 0; layer < neuralNetWeights.size(); ++layer)
	{
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
         // next layer will match up with the row depth. If this is the last layer, we don't
         // need the threshold function.
         if (layer == neuralNetWeights.size() - 1)
         {
         	if (std::isnan(summation))
         	{
         		cerr << "isnan in un-thresholded summation" << endl;
         		cerr << "\tlayer " << layer << " rowDest " << rowDestination << " rowOrigination " << rowOrigination << endl;
         		cerr << "bias " << neuralNetWeights[layer][rowDestination][0] << endl;

         		for (rowOrigination = 1; rowOrigination < neuralNetWeights[layer][rowDestination].size(); ++rowOrigination)
         		{
         			cerr << "rowOrigination " << rowOrigination << endl;
         			cerr << "\t\tinput " << inputVector[rowOrigination - 1] << endl;
         			cerr << "\t\tweight " << neuralNetWeights[layer][rowDestination][rowOrigination] << endl;
         			cerr << endl;
         		}
         	}

         	outputVector.push_back(summation);
         }
         else
         {
         	// Bug finding things
         	if (std::isnan(sigmoidFunction(summation)))
         	{
         		cerr << "sigmoid function returns nan" << endl;
         		cerr << "\tlayer " << layer << " rowDest " << rowDestination << " rowOrigination " << rowOrigination << endl;

         		for (rowOrigination = 1; rowOrigination < neuralNetWeights[layer][rowDestination].size(); ++rowOrigination)
         		{
         			cerr << "rowOrigination " << rowOrigination << endl;
         			cerr << "\t\tinput " << boardState[rowOrigination - 1] << endl;
         			cerr << "\t\tweight " << neuralNetWeights[layer][rowDestination][rowOrigination] << endl;
         			cerr << endl;
         		}         		
         	}

				outputVector.push_back(sigmoidFunction(summation));
			}
		}

      inputVector = outputVector;
	}

   // The last input vector should have only one entry -- the final output of
   // the neural net.

	if (std::isnan(sigmoidFunction(summation)))
	{
		// cerr << "Summation: " << summation << endl;
		// cerr << "Layer " << layer << endl;
		// cerr << "RowDestination " << rowDestination << endl;
	}

   return inputVector[0];
}

double hexGamePlayer::sigmoidFunction(double input)
{
   return 1 / (1 + exp(-input));
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

void hexGamePlayer::printWeights()
{
   cerr << "stat " << gamesWon << endl;

   for (int layer = 0; layer < neuralNetWeights.size(); layer += 1)
   {
      cerr << "\tlayer" << endl;

      for (int rowDestination = 0; rowDestination < neuralNetWeights[layer].size(); rowDestination += 1)
      {
         cerr << "\t\tnode ";

         for (int rowOrigination = 0; rowOrigination < neuralNetWeights[layer][rowDestination].size(); rowOrigination += 1)
         {
            cerr << neuralNetWeights[layer][rowDestination][rowOrigination] << ',';
         }

         cerr << endl;
      }
   }
   cerr << "endPlayer" << endl;
}
