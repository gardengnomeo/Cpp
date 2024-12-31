
#include "Queen.h"
#include <cmath>

namespace Chess {
  bool Queen::legal_move_shape(const Position& start, const Position& end) const {
    int row_diff = std::abs(start.second - end.second); // vertical difference
    int col_diff = std::abs(start.first - end.first);   // horizontal difference

    // valid if move is diagonal (like bishop) or straight line (like rook)
    return (row_diff == col_diff) || (row_diff == 0 || col_diff == 0);
  }
}
