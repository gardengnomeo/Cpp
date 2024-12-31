#include "Rook.h"

namespace Chess {
  bool Rook::legal_move_shape(const Position& start, const Position& end) const {
    int row_diff = start.second - end.second; // vertical difference
    int col_diff = start.first - end.first;  // horizontal difference

    // horizontally or vertically
    return (row_diff == 0 && col_diff != 0) || (row_diff != 0 && col_diff == 0);
  }
}
