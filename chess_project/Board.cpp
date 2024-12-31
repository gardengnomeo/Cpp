#include "Piece.h"
#include <iostream>
#include <utility>
#include <map>
#ifndef _WIN32
#include "Terminal.h"
#endif // !_WIN32
#include "Board.h"
#include "CreatePiece.h"
#include "Exceptions.h"

using std::cout;
using std::endl; 

namespace Chess
{
  /////////////////////////////////////
  // DO NOT MODIFY THIS FUNCTION!!!! //
  /////////////////////////////////////
  Board::Board(){}

  //default destructor
  Board::~Board() {
    //iterate through map
   for (std::map<Position, Piece*>::const_iterator map = occ.begin(); 
    map != occ.end(); ++map) {
      //delete piece pointer
      delete map->second;
    }
    
    //clear map
    occ.clear();
  }

  //defualt constructor
  Board::Board(const Board &board) {
    for (std::map<Position, Piece*>::const_iterator map = board.occ.begin(); 
    map != board.occ.end(); ++map) {
      //check if piece there
      Position pos = map->first;
      const Piece* piece = map->second;
      if(piece){
        occ[pos] = create_piece(piece->to_ascii());
      } else {
        occ[pos] = nullptr;
      }
    }
  }

  //deletes the board when called in the >> overload
  void Board::delete_board(){
    //iterate through map
   for (std::map<Position, Piece*>::const_iterator map = occ.begin(); 
    map != occ.end(); ++map) {
      //delete piece pointer
      delete map->second;
    }
    
    //clear map
    occ.clear();
  }
  const Piece* Board::operator()(const Position& position) const {
    std::map<Position, Chess::Piece*>::const_iterator it = occ.find(position);
    if (it == occ.end()) {
        return nullptr; // Position not found
    }
    return it->second; // Return the piece at the position
  }
  
 void Board::add_piece(const Position& position, const char& piece_designator) {
    
    // remove the existing piece if adding '-' (clearing the position)
    if (piece_designator == '-') {
        occ.erase(position); // clear the position
        return;
    }

    // position not on the board
    if ((position.first < 'A' || position.first > 'H') || 
        (position.second < '1' || position.second > '8')) {
        throw Exception("invalid position");
    }

    //check if piece already in position
    if(occ.find(position) != occ.end()){
      throw Exception("position is occupied");
    }

    //check for invalid piece designator
    Piece* piece = create_piece(piece_designator);
    if(!piece){
      delete piece;
      throw Exception("invalid designator");
    }

    // add the new piece
    occ[position] = piece;
  }


  void Board::make_move(const Position& start, const Position& end) {
    Piece* moving_piece = occ[start];
    const Piece* end_position = occ[end];

    //check if there is a piece at end position and go through capturing
    if(end_position){

      //check if capturing own piece
      if(moving_piece->is_white() == end_position->is_white()){
        throw Exception("cannot capture own piece");
      }

      //check if legal capture shape for pawn
      if(!moving_piece->legal_capture_shape(start, end)){
        throw Exception("illegal capture shape");
      }

      //remove piece from end position as will need empty square to check legal move
      delete end_position;
      occ[end] = nullptr;

    } else {
      if (!moving_piece->legal_move_shape(start, end)) {
        throw Exception("illegal move shape");
      }
    }

    //check if piece needs path clear
    char piece = std::tolower(moving_piece->to_ascii());
    //set default to true for pawn, queen, rook, bishopp and mystery straight
    bool path_check = true;

    //check knight and mystery
    if(piece == 'n'){
      path_check = false;
    }
    if(piece == 'm'){
      int row_diff = std::abs(start.second - end.second); // vertical difference
      int col_diff = std::abs(start.first - end.first);   // horizontal difference

      //check if mystery moves funky or straight line
      if((row_diff == col_diff) || start.first == end.first || start.second == end.second){
        path_check = true;
      } else {
        path_check = false;
      }
    }

    //check if path clear if it needs to check
    if(path_check && !path_clear(start, end)){
      throw Exception("path is not clear");
    }

    //move the piece as all checks have been passed
    occ[end] = moving_piece;
    occ.erase(start);

    //promotion logic for pawn if reaches end
    if(piece == 'p'){
      if(moving_piece->is_white() && end.second == '8'){
        delete occ[end];
        occ[end] = create_piece('Q');
      } else if(!moving_piece->is_white() && end.second == '1'){
        delete occ[end];
        occ[end] = create_piece('q');
      }
    }
  }


//checks to see fi the path is blocked
  bool Board::path_clear(const Position& start, const Position& end){
    //get rows and columns step size
    int row_step = (end.second > start.second) ? 1 : (end.second < start.second) ? -1 : 0;
    int col_step = (end.first > start.first) ? 1 : (end.first < start.first) ? -1 : 0;

    //get start position
    char current_row = start.second + row_step;
    char current_col = start.first + col_step;

    //check intermediate positions for blocking pieces
    while (current_row != end.second || current_col != end.first) {
      
      //check if piece blocking path
      if (occ.find(Position(current_col, current_row)) != occ.end()) {
        return false;
      }
      //incriment
      current_row += row_step;
      current_col += col_step;
    }
    return true;
  }



  void Board::display() const {
    //bool for square color
    //use iterator to go through
    //check bool color and then use terminal
    //then check if their is a piece
    //print square first and then piece
  //print out header
    cout << "  A B C D E F G H" << endl;

    //iterate through the rows
    for(char row = '8'; row > '0'; --row){

      //print out the row
      cout << row << "|";

      //check if row should start with white or black square
      bool white_square = !(row % 2);

      //iterate through each column
      for(char col = 'A'; col <= 'H'; ++col){
        
        //get the current position
        Position curr_pos(col,row);
        
        if (white_square) {
          Terminal::color_bg(Terminal::YELLOW);
          white_square = false;
        }
        else {
          Terminal::color_bg(Terminal::GREEN);
          white_square = true;
        }

        //get the piece at the current position and print it out
        std::map<Position, Piece*>::const_iterator piece = occ.find(curr_pos);
        if(piece == occ.end()){
          cout << "  ";
        } else {
          cout << piece->second->to_unicode() << " ";
        }
      }

      //reset to terminal color to not flood whole terminal
      Terminal::set_default();
      //print out row
      cout << "|" << row << endl;
    }

    //print out bottom
    cout << "  A B C D E F G H" << endl;
  }

  //overloaded ++ operator
  Board::iterator& Board::iterator::operator++() {
    //iterate through the cols until H and then go to next row if fully done with cols
    if(col < 'H'){
      ++col;
    } else if(row < '8') {
      col = 'A';
      ++row;
    } else {
      col = 'I';
      row = '9';
    }
    return *this;
  }
  
  //overloaded != operator
  bool Board::iterator::operator!=(const iterator& other) const{
    return col != other.col || row != other.row;
  }

  //overloaded dereferencing operator
  Position Board::iterator::operator*() const {
    return Position(col, row);
  }

  //setting begin method
  Board::iterator Board::begin() const {
    return iterator('A','1');
  }
  
  //setting end method with one past last
  Board::iterator Board::end() const {
    return iterator('I','9');
  }

  bool Board::has_valid_kings() const {
    int white_king_count = 0;
    int black_king_count = 0;
    for (std::map<std::pair<char, char>, Piece*>::const_iterator it = occ.begin();
	 it != occ.end();
	 it++) {
      if (it->second) {
	switch (it->second->to_ascii()) {
	case 'K':
	  white_king_count++;
	  break;
	case 'k':
	  black_king_count++;
	  break;
	}
      }
    }
    return (white_king_count == 1) && (black_king_count == 1);
  }

  /////////////////////////////////////
  // DO NOT MODIFY THIS FUNCTION!!!! //
  /////////////////////////////////////
  std::ostream& operator<<(std::ostream& os, const Board& board) {
    for(char r = '8'; r >= '1'; r--) {
      for(char c = 'A'; c <= 'H'; c++) {
	const Piece* piece = board(Position(c, r));
	if (piece) {
	  os << piece->to_ascii();
	} else {
	  os << '-';
	}
      }
      os << std::endl;
    }
    return os;
  }


  void Board::update(const Position& start, const Position& end) {
    std::map<Position, Chess::Piece*>::iterator it = occ.find(start);
    if (it == occ.end()) {
        throw Exception("start position is not on board");
    }
    Chess::Piece* p = it->second;
    occ[end] = p;
    occ.erase(it);
  }

}
