#include "Bishop.h"
#include <cmath> // For std::abs

namespace Chess
{
  bool Bishop::legal_move_shape(const Position& start, const Position& end) const {
    // Check if move is diagonal
    int row_diff = std::abs(start.second - end.second); // .second for the row
    int col_diff = std::abs(start.first - end.first);   // .first for the column

    // A legal move for a bishop must have equal row and column differences
    return (row_diff == col_diff);
  }
}
