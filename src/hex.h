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

using namespace std;

// Global variables
const int MAX_DEPTH = 5;
const int BOARD_SIZE = 7;
const int NUM_PLAYERS = 16;

enum player {playerA, playerB, none};


class Tile
{
private:
   player owner;
   bool bitFlag;
   int location, row, column, rank, parent;

public:
   Tile(int location, int size);
   player getOwner() const{return owner;}
   void setOwner(player takenBy) {owner = takenBy;}

   bool getFlag() const{return bitFlag;} // The flag is whether or not the square is attached to the border
   void setFlag(bool newFlag) {bitFlag = newFlag;}
   int getRank() {return rank;} // This is for Union-Find
   void incrementRank() {rank = rank + 1;}

   int getRow() {return row;}
   int getColumn() {return column;}

   void setParent(int newParent) {parent = newParent;} // Parent's are used for Union-Find
   int getParent() {return parent;}
};

// The board for the game to be played on
class Board
{
private:
   int size;
   vector<Tile> BoardLayout;
   void findNeighbors(int location);
   void Union(int x, int y);
   void Link(int x, int y);
   int FindSet(int x);

public:
   Board(int size = 0);
   Board(const Board &copy);
   void makeMove(int location, player mover); // Update the board with a player's move.
   bool isValidMove(int location) const;
   bool isGameOver();
   int getSize() const {return size;}

   vector<Tile> getBoard() const{return BoardLayout;} // A little ugly, but I needed to access the board for the copy constructor and assignment operator overload

   Board& operator=(const Board &rhs); // Assignment operator
};

#endif // #ifndef HEX_H
