/**********************
Hex Research Project

author: Michael McCarver
advisor: Dr. Rob LeGrand
**********************/

#ifndef HEX_H
#define HEX_H

#include <iomanip>
#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

/**
Global variables
**/
// const int MAX_DEPTH = 1;
extern int MAX_DEPTH;
const int BOARD_SIZE = 5;

enum player {playerA, playerB, none};

class Tile
{
private:
   player owner;
   bool bitFlag;
   int location, row, column, rank, parent, turnTaken;

public:
   Tile(int location, int size);
   player getOwner() const{return owner;}
   void setOwner(player takenBy) {owner = takenBy;}

   // The flag is whether or not the square is attached to the border
   bool getFlag() const{return bitFlag;}
   void setFlag(bool newFlag) {bitFlag = newFlag;}
   // This is for Union-Find
   int getRank() {return rank;}
   void incrementRank() {rank = rank + 1;}

   // Turn is used for printBoard() -- it tells whech turn the tile was taken
   int getTurnTaken() const{return turnTaken;}
   void setTurnTaken(int turnTaken) {this->turnTaken = turnTaken;}

   int getRow() {return row;}
   int getColumn() {return column;}

   // Parent's are used for Union-Find
   void setParent(int newParent) {parent = newParent;}
   int getParent() {return parent;}
};

// The board for the game to be played on
class Board
{
private:
   int size, turnCounter;
   vector<Tile> BoardLayout;
   void findNeighbors(int location);
   void Union(int x, int y);
   void Link(int x, int y);
   int FindSet(int x);

public:
   Board(int size = 0);
   Board(const Board &copy);
   Board(vector<Tile> BoardLayout);
   void makeMove(int location, player mover);
   bool isValidMove(int location) const;
   bool isGameOver();
   int getSize() const {return size;}
   void printBoard() const;
   void printBoard(ofstream &fout);

   vector<Tile> getBoard() const{return BoardLayout;}
   Tile getTile(int location){return BoardLayout[location];}

   Board& operator=(const Board &rhs);
};

#endif // #ifndef HEX_H
