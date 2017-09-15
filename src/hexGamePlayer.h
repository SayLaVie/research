/**********************
Hex Research Project

author: Michael McCarver
advisor: Dr. Rob LeGrand
**********************/

#ifndef HEXGAMEPLAYER_H
#define HEXGAMEPLAYER_H

#include "hex.h"

class hexGamePlayer
{
private:
   // neuralNetWeights is a 3D vector. Of the form: neuralNetWeights[column][rowDestination][rowOrigination]
   vector<vector<vector<int> > > neuralNetWeights;

   // Private data about this players played games
   int gamesWon, movesMade;

   double miniMax(Board board, player whichPlayer, int depth, int alpha, int beta);
   double neuralNetHeuristic(Board board, player whichPlayer);
   double sigmoidFunction(double input);

public:
   hexGamePlayer(vector<vector<vector<int> > > neuralNetWeights);
   int play(const Board &board, player whichPlayer);

   // Data modifiers
   void addGameWon() {gamesWon += 1;}
   void addMove() {movesMade += 1;}   
};

#endif
