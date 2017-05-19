// CS 4318, spring 2017
// Agent Challenge 3: Off-road rally
//
// Here are the #includes and definitions available to each agent.

#ifndef HEX_H
#define HEX_H

#include <iomanip>
#include <iostream>
#include <vector>

using namespace std;

enum player {playerA, playerB, none};

class Tile
{
private:
   player owner;
   bool bitFlag;
   int location, row, column, rank, parent, turn;

public:
   Tile(int location, int size);
   player getOwner() const{return owner;}
   void setOwner(player takenBy) {owner = takenBy;}

   bool getFlag() const{return bitFlag;} // The flag is whether or not the square is attached to the border
   void setFlag(bool newFlag) {bitFlag = newFlag;}
   int getRank() {return rank;} // This is for Union-Find
   void incrementRank() {rank = rank + 1;}

   int getTurn() const{return turn;} // Turn is used for printBoard() -- it tells whech turn the tile was taken
   void setTurn(int newTurn) {turn = newTurn;}

   void setParent(int newParent) {parent = newParent;} // Parent's are used for Union-Find
   int getParent() {return parent;}
   int getColumn() {return column;}
   int getRow() {return row;}
};

// The board for the game to be played on
class Board
{
private:
   int size, aTurnInc, bTurnInc;
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
   bool isGameOver(); //Change name?
   void printBoard() const;
   int getSize() const {return size;}
   int getTileLocation(int row, int column) const;
   int getATurn() const {return aTurnInc;}
   int getBTurn() const {return bTurnInc;}
   void setATurn() {aTurnInc += 1;} //increment turn by one
   void setBTurn() {bTurnInc += 1;} //increment turn by one

   vector<Tile> getBoard() const{return BoardLayout;} // A little ugly, but I needed to access the board for the copy constructor and assignment operator overload

   Board& operator=(const Board &rhs); // Assignment operator
};

#endif // #ifndef HEX_H
