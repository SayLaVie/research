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

   double miniMax(Board board, player whichPlayer, int depth, int alpha, int beta);
   double neuralNetHeuristic(Board board, player whichPlayer);

public:
   hexGamePlayer(vector<vector<vector<int> > > neuralNetWeights) {this->neuralNetWeights = neuralNetWeights;}
   int play(const Board &board, player whichPlayer);
}; 

#endif
