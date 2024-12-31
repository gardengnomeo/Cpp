#include "Knight.h"
#include <cmath> // For std::abs

namespace Chess {
  bool Knight::legal_move_shape(const Position& start, const Position& end) const {
    // calculate difference in rows and columns
    int row_diff = std::abs(start.second - end.second);
    int col_diff = std::abs(start.first - end.first);

    // two squares in one direction and one in the other (L)
    return (row_diff == 2 && col_diff == 1) || (row_diff == 1 && col_diff == 2);
  }
}
