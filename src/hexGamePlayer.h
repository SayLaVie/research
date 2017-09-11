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
   // neuralNetWeights is a 3D vector. Indexes refer to absolute column position, row position for link origination, and row position for link destination
   vector<vector<vector<int> > > neuralNetWeights;

   double miniMax(Board board, player whichPlayer, int depth, int alpha, int beta);
   double neuralNetHeuristic();

public:
   hexGamePlayer(vector<vector<vector<int> > > neuralNetWeights, const Board &board) {this->neuralNetWeights = neuralNetWeights;}
   int play(const Board &board, player whichPlayer);
}; 

#endif
