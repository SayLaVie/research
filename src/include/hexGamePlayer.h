/**********************
Hex Research Project

author: Michael McCarver
advisor: Dr. Rob LeGrand
**********************/

#ifndef HEXGAMEPLAYER_H
#define HEXGAMEPLAYER_H

#include "hex.h"
#include <cmath>
#include <algorithm>

class hexGamePlayer
{
private:
   // neuralNetWeights is a 3D vector. Of the form: neuralNetWeights[column][rowDestination][rowOrigination]
   vector<vector<vector<double> > > neuralNetWeights;
   vector<pair<double, int> > moveOrder;

   // Private data about this players played games
   int gamesWon, movesMade;

   double miniMax(Board board, Player whichPlayer, int depth, double alpha, double beta);
   double boardEvalLearning(Board board, Player whichPlayer);

   double sigmoidFunction(double input);

public:
   // initialize neuralNets with empty vector if not supplied
   hexGamePlayer(vector<vector<vector<double> > > neuralNetWeights = {{{}}});
   hexGamePlayer(vector<vector<vector<double> > > neuralNetWeights, int numGamesWon);
   int play(const Board &board, Player whichPlayer);

   // neuralNetHeuristic made public to be called for troubleshooting
   double neuralNetHeuristic(const Board board, Player whichPlayer);

   // Data modifiers
   void addGameWon() {gamesWon += 1;}
   int getGamesWon() {return gamesWon;}

   // Not currently keeping track of moves during games
   void addMoves(int numMoves) {movesMade += numMoves;}

   double getWeight(int layer, int rowDestination, int rowOrigination);
   void setWeight(int layer, int rowDestination, int rowOrigination, double newWeight);
   void printWeights(ofstream &fout);
   void printWeights();

   vector<vector<vector<double> > > getNet() {return neuralNetWeights;}
};

#endif
