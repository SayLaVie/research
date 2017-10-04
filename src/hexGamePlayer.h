/**********************
Hex Research Project

author: Michael McCarver
advisor: Dr. Rob LeGrand
**********************/

#ifndef HEXGAMEPLAYER_H
#define HEXGAMEPLAYER_H

#include "hex.h"
#include <cmath>
#include <fstream>

class hexGamePlayer
{
private:
   // neuralNetWeights is a 3D vector. Of the form: neuralNetWeights[column][rowDestination][rowOrigination]
   vector<vector<vector<double> > > neuralNetWeights;

   // Private data about this players played games
   int gamesWon, movesMade;

   double miniMax(Board board, player whichPlayer, int depth, double alpha, double beta);
   // double neuralNetHeuristic(const Board board, player whichPlayer);
   double neuralNetHeuristic(const Board board, player whichPlayer);
   double sigmoidFunction(double input);

public:
   hexGamePlayer(vector<vector<vector<double> > > neuralNetWeights);
   int play(const Board &board, player whichPlayer);

   // Data modifiers
   void addGameWon() {gamesWon += 1;}
   int getGamesWon() {return gamesWon;}

   // Not currently keeping track of moves during games
   void addMoves(int numMoves) {movesMade += numMoves;}

   double getWeight(int layer, int rowOrigination, int rowDestination);
   void printWeights(ofstream &fout);
};

#endif
