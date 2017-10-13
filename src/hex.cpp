/**********************
Hex Research Project

author: Michael McCarver
advisor: Dr. Rob LeGrand
**********************/

#include <hex.h>

// Constructor used during board creation
Tile::Tile(int location, int size)
{
   owner = none;
   turnTaken = 0;
   rank = 0;
   parent = location;
   this->location = location;
   row = location / size;
   column = location % size;
   bitFlag = false;
}

// Constructor using board size
Board::Board(int size)
{
   this->size = size;
   turnCounter = 0;

   for (int i = 0; i < size * size; ++i)
   {
      BoardLayout.push_back(Tile(i, size));
   }
}

// Copy constructor
Board::Board(const Board &copy)
{
   this->BoardLayout.clear();

   this->BoardLayout = copy.getBoard();

   this->size = copy.getSize();   
}

Board::Board(vector<Tile> BoardLayout)
{
   this->BoardLayout.clear();

   this->BoardLayout = BoardLayout;

   this->size = BoardLayout.size();   
}

// Assignment operator
Board& Board::operator=(const Board &rhs)
{
   this->BoardLayout.clear();

   this->size = rhs.getSize();

   this->BoardLayout = rhs.getBoard();

   return *this;
}

// Union, Link, and FindSet are used to maintain which tiles are connected to a border
void Board::Union(int x, int y)
{
   Link(FindSet(x), FindSet(y));
   // FindSet(x);
   // FindSet(y);
}

void Board::Link(int x, int y)
{
   bool tmp = BoardLayout[x].getFlag() | BoardLayout[y].getFlag();

   if (BoardLayout[x].getRank() > BoardLayout[y].getRank())
   {
      BoardLayout[y].setParent(x);
      BoardLayout[x].setFlag(tmp);
   }
   else
   {
      BoardLayout[x].setParent(y);

      BoardLayout[y].setFlag(tmp);

      if (BoardLayout[x].getRank() == BoardLayout[y].getRank())
      {
         BoardLayout[y].incrementRank();
      }
   }
}

int Board::FindSet(int x)
{
   if (BoardLayout[x].getParent() != x)
   {
      BoardLayout[x].setParent(FindSet(BoardLayout[x].getParent()));
   }

   return BoardLayout[x].getParent();
}

// Updates the owner of a tile, and calls findNeighbors to determine flag value
void Board::makeMove(int location, player mover)
{
   Tile *locationTile = &(BoardLayout[location]);
   int row, column;

   row = locationTile->getRow();
   column = locationTile->getColumn();

   if (isValidMove(location))
   {
      locationTile->setOwner(mover);

      locationTile->setTurnTaken(turnCounter / 2);

      turnCounter += 1;

      // Set the border flag for this tile
      locationTile->setFlag(locationTile->getOwner() == playerA ? column == 0 : row == 0);

      findNeighbors(location);
   }
}

// Looks for neighbors. Calls Union-Find functions to set appropriate border-connection flag
void Board::findNeighbors(int location)
{
   Tile *locationTile = &(BoardLayout[location]), *neighborTile;

   int row, column, neighbor;
   bool left, right, up, down, originalFlag;
   player mover;

   mover = BoardLayout[location].getOwner();
   row = locationTile->getRow();
   column = locationTile->getColumn();

   left = column - 1 >= 0;
   right = column + 1 < size;
   up = row + 1 < size;
   down = row - 1 >= 0;

   do
   {
      originalFlag = locationTile->getFlag();

      // Start looking left and upper-left
      if (left)
      {
         // Check directly left
         neighbor = size * row + (column - 1);
         neighborTile = &(BoardLayout[neighbor]);

         //Check if we own that tile
         if (neighborTile->getOwner() == mover)
         {
            // A neighbor hex has the same owner, so merge this new tile into that set
            Union(location, neighbor);
         }

         // Check upper-left
         if (up)
         {
            neighbor = size * (row + 1) + (column - 1);
            neighborTile = &(BoardLayout[neighbor]);

            if (neighborTile->getOwner() == mover)
            {
               // A neighbor hex has the same owner, so merge this new tile into that set
               Union(location, neighbor);
            }
         }
      }

      // Check tiles to the right
      if (right)
      {
         // Check directly right
         neighbor = size * row + (column + 1);
         neighborTile = &(BoardLayout[neighbor]);

         // Check if we own that tile
         if (neighborTile->getOwner() == mover)
         {
            // A neighbor hex has the same owner, so merge this new tile into that set
            Union(location, neighbor);
         }

         // Check lower-right
         if (down)
         {
            neighbor = size * (row - 1) + (column + 1);
            neighborTile = &(BoardLayout[neighbor]);

            // Check if we own that tile
            if (neighborTile->getOwner() == mover)
            {
               // A neighbor hex has the same owner, so merge this new tile into that set
               Union(location, neighbor);
            }
         }
      }

      // Check tile above
      if (up)
      {
         neighbor = size * (row + 1) + column;
         neighborTile = &(BoardLayout[neighbor]);

         // Check if we own that tile
         if (neighborTile->getOwner() == mover)
         {
            // A neighbor hex has the same owner, so merge this new tile into that set
            Union(location, neighbor);
         }
      }

      // Check tile below
      if (down)
      {
         neighbor = size * (row - 1) + column;
         neighborTile = &(BoardLayout[neighbor]);

         // Check if we own that tile
         if (neighborTile->getOwner() == mover)
         {
            // A neighbor hex has the same owner, so merge this new tile into that set
            Union(location, neighbor);
         }
      }
   } while (originalFlag != locationTile->getFlag());
}

// Checks if the hypothetical move is out-of-bounds, or if it is already owned by a player
bool Board::isValidMove(int location) const
{
   return location >= 0 && location < size * size && BoardLayout[location].getOwner() == none;
}

// Searches the border associated with a player and checks the flag's of those tiles to see if they are connected to the border on the opposite side
bool Board::isGameOver()
{
   int column = size - 1, row = 0, location;

   //Check if PlayerA has won
   while (row < size)
   {
      location = size * row + column;

      if (BoardLayout[location].getOwner() == playerA && BoardLayout[FindSet(location)].getFlag() == true)
      {
         return true;
      }
      ++row;
   }

   row = size - 1;
   column = 0;

   // Check if PlayerB has won
   while (column < size)
   {
      location = size * row + column;

      if (BoardLayout[location].getOwner() == playerB && BoardLayout[FindSet(location)].getFlag() == true)
      {
         return true;
      }
      ++column;
   }

   // Nobody has won yet
   return false;
}

void Board::printBoard() const // Prints the board out
{
   int column, hex, row, offset, move, width;

   width = (size - 1) * 3;

   cout << endl << setw(width + BOARD_SIZE) << "";

   for (column = 0; column < size; ++column)
   {
      cout << setw(3) << right << "*" << (char)(65 + column) << "*";
   }
   cout << endl;

   for (row = size - 1; row >= 0; --row)
   {
      cout << endl << setw(width) << "*" << row << "*";

      for (column = 0; column < size; ++column)
      {
         hex = size * row + column;

         move = BoardLayout[hex].getTurnTaken();
         offset = 1;

         while (move / 10)
         {
            move /= 10;
            ++offset;
         }

         if (BoardLayout[hex].getOwner() == playerA)
         {
            cout << setw(5 - offset) << right << "A" << BoardLayout[hex].getTurnTaken();
         }
         else if (BoardLayout[hex].getOwner() == playerB)
         {
            cout << setw(5 - offset) << right << "B" << BoardLayout[hex].getTurnTaken();
         }
         else
         {
            cout << setw(5) << right << "-";
         }
      }

      cout << setw(5) << right << "*" << row << "*" << endl;

      width -= 3;

      // cout << endl;
   }

   cout << endl << setw(1) << "";
   for (column = 0; column < size; ++column)
   {
      cout << setw(3) << right << "*" << (char)(65 + column) << "*";
   }
}


