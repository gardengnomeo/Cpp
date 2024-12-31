#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include <map>
#include "Piece.h"
#include "Pawn.h"
#include "Rook.h"
#include "Knight.h"
#include "Bishop.h"
#include "Queen.h"
#include "King.h"
#include "Mystery.h"


namespace Chess
{
  class Board {

		// Throughout, we will be accessing board positions using Position defined in Piece.h.
		// The assumption is that the first value is the column with values in
		// {'A','B','C','D','E','F','G','H'} (all caps)
		// and the second is the row, with values in {'1','2','3','4','5','6','7','8'}

	public:
		// Default constructor
		Board();

		//Copy constructor
		Board(const Board &board);
		
		//destructor
		~Board();

		void delete_board();

		//move the piece on the board to check if it works
		void make_move(const Position& start, const Position& end);

		//checks if the path is clear depending on the piece:
		bool path_clear(const Position& start, const Position& end);

		// Returns a const pointer to the piece at a prescribed location if it exists,
		// or nullptr if there is nothing there.
		const Piece* operator() (const Position& position) const;

		// Attempts to add a new piece with the specified designator, at the given position.
		// Throw exception for the following cases:
		// -- the designator is invalid, throw exception with error message "invalid designator"
		// -- the specified position is not on the board, throw exception with error message "invalid position"
		// -- if the specified position is occupied, throw exception with error message "position is occupied"
		void add_piece(const Position& position, const char& piece_designator);

		// Displays the board by printing it to stdout
		void display() const;

		// Returns true if the board has the right number of kings on it
		bool has_valid_kings() const;

		// Update the board with the valid called move
		void update(const Position& start, const Position& end);
		
		//iterator to go through the board and return what is at each position
		class iterator {
			
			public:
				//default constructor
				iterator(char col = 'A', char row = '1') : col(col), row(row) {}

				//overide the ++ operator
				iterator& operator++();
				
				//overide the != operator
				bool operator!=(const iterator& other) const;
				
				//overide the dereferencing operator
				Position operator*() const;
			
			private:
				char col; //A-H first
				char row; //1-8 second
		};

		//methods for the begin and end iterators
		iterator begin() const;
		iterator end() const;

	private:
		// The sparse map storing the pieces, keyed off locations
		std::map<Position, Piece*> occ;

        // Write the board state to an output stream
        friend std::ostream& operator<< (std::ostream& os, const Board& board);
	};
}
#endif // BOARD_H
