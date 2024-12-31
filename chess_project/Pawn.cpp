#include "Pawn.h"
#include <cmath>

namespace Chess {
  // forward moves only
  bool Pawn::legal_move_shape(const Position& start, const Position& end) const {
    int row_diff = end.second - start.second; // positive for white, negative for black
    int col_diff = std::abs(end.first - start.first); // column difference

    // white pawns move up (positive row_diff), black pawns move down (negative row_diff)
    if (is_white()) {
      // white: move one square forward or two squares forward from the initial position
      if (col_diff == 0 && (row_diff == 1 || (row_diff == 2 && start.second == '2'))) {
        return true;
      }
    } else {
      // black: move one square forward or two squares forward from the initial position
      if (col_diff == 0 && (row_diff == -1 || (row_diff == -2 && start.second == '7'))) {
        return true;
      }
    }

    return false; // all other moves are invalid
  }

  // diagonal moves
  bool Pawn::legal_capture_shape(const Position& start, const Position& end) const {
    int row_diff = end.second - start.second; // Positive for white, negative for black
    int col_diff = std::abs(end.first - start.first); // Column difference

    // captures are one square diagonally forward
    if (is_white()) {
      if (row_diff == 1 && col_diff == 1) {
        return true;
      }
    } else {
      if (row_diff == -1 && col_diff == 1) {
        return true;
      }
    }

    return false; // all other captures are invalid
  }
}
