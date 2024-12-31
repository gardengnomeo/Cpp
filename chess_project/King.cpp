#include "King.h"
#include <cmath>

namespace Chess {
  bool King::legal_move_shape(const Position& start, const Position& end) const {
    // calculate difference in rows and columns
    int row_diff = std::abs(start.second - end.second);
    int col_diff = std::abs(start.first - end.first);

    // king can move one square in any direction
    return (row_diff <= 1 && col_diff <= 1) && !(row_diff == 0 && col_diff == 0);
  }
}
