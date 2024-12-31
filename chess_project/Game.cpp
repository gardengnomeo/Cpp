#include <cassert>
#include "Game.h"
#include "Exceptions.h"
#include "Piece.h"

namespace Chess
{
	/////////////////////////////////////
	// DO NOT MODIFY THIS FUNCTION!!!! //
	/////////////////////////////////////
	Game::Game() : is_white_turn(true) {
		// Add the pawns
		for (int i = 0; i < 8; i++) {
			board.add_piece(Position('A' + i, '1' + 1), 'P');
			board.add_piece(Position('A' + i, '1' + 6), 'p');
		}

		// Add the rooks
		board.add_piece(Position( 'A'+0 , '1'+0 ) , 'R' );
		board.add_piece(Position( 'A'+7 , '1'+0 ) , 'R' );
		board.add_piece(Position( 'A'+0 , '1'+7 ) , 'r' );
		board.add_piece(Position( 'A'+7 , '1'+7 ) , 'r' );

		// Add the knights
		board.add_piece(Position( 'A'+1 , '1'+0 ) , 'N' );
		board.add_piece(Position( 'A'+6 , '1'+0 ) , 'N' );
		board.add_piece(Position( 'A'+1 , '1'+7 ) , 'n' );
		board.add_piece(Position( 'A'+6 , '1'+7 ) , 'n' );

		// Add the bishops
		board.add_piece(Position( 'A'+2 , '1'+0 ) , 'B' );
		board.add_piece(Position( 'A'+5 , '1'+0 ) , 'B' );
		board.add_piece(Position( 'A'+2 , '1'+7 ) , 'b' );
		board.add_piece(Position( 'A'+5 , '1'+7 ) , 'b' );

		// Add the kings and queens
		board.add_piece(Position( 'A'+3 , '1'+0 ) , 'Q' );
		board.add_piece(Position( 'A'+4 , '1'+0 ) , 'K' );
		board.add_piece(Position( 'A'+3 , '1'+7 ) , 'q' );
		board.add_piece(Position( 'A'+4 , '1'+7 ) , 'k' );
	}

    void Game::make_move(const Position& start, const Position& end) {
        
        //check if the start and end position are valid
        if(start.first < 'A' || start.first > 'H' || start.second < '1' || start.second > '8'){
            throw Exception("start position is not on board");
        }
        if(end.first < 'A' || end.first > 'H' || end.second < '1' || end.second > '8'){
            throw Exception("end position is not on board");
        }

        const Piece* piece = board(start);
        // check if there's a piece at the start position
        if (!piece) {
            throw Exception("no piece at start position");
        }

        // check if it's the correct player's turn
        if (piece->is_white() != is_white_turn) {
            throw Exception("piece color and turn do not match");
        }

        //deep copy the game
        Game temp_game(*this);

        //attmept to move the piece on the temp board and check for check
        temp_game.board.make_move(start, end);
        if(temp_game.in_check(is_white_turn)){
            throw Exception("move exposes check");
        }

        //make move on real game if all passes and switch turns
        board.make_move(start, end);
        is_white_turn = !is_white_turn;
    }
    

bool Game::in_check(const bool& white) const {
    // find position of the king
    Position king_position;
    char king_designator = white ? 'K' : 'k';
    bool king_found = false;

    for (char row = '1'; row <= '8'; ++row) {
        for (char col = 'A'; col <= 'H'; ++col) {
            Position pos(col, row);
            const Piece* piece = board(pos);
            if (piece && piece->to_ascii() == king_designator) {
                king_position = pos;
                king_found = true;
                break;
            }
        }
        if (king_found) break; // stop searching once the king is found
    }

    // if the king is not found, assume it's not in check
    if (!king_found) {
        return false; // no king found; assume no check
    }

    // check if any opposing piece can attack the king
    for (char row = '1'; row <= '8'; ++row) {
        for (char col = 'A'; col <= 'H'; ++col) {
            Position pos(col, row);
            const Piece* piece = board(pos);

            // skip if no piece or same color
            if (!piece || piece->is_white() == white) {
                continue;
            }

            // handle pawn-specific attack rules
            if (piece->to_ascii() == 'P' || piece->to_ascii() == 'p') {
                int forward_step = piece->is_white() ? 1 : -1;
                if ((king_position.first == pos.first + 1 || king_position.first == pos.first - 1) &&
                    king_position.second == pos.second + forward_step) {
                    return true; // pawn attacks diagonally
                }
                continue; // pawns cannot attack forward
            }

            // check if the piece can legally capture the king
            if (piece->legal_capture_shape(pos, king_position)) {
                // handle path-blocking for sliding pieces
                if (piece->to_ascii() == 'R' || piece->to_ascii() == 'r' ||
                    piece->to_ascii() == 'B' || piece->to_ascii() == 'b' ||
                    piece->to_ascii() == 'Q' || piece->to_ascii() == 'q') {
                    int row_step = (king_position.second > pos.second) ? 1 : (king_position.second < pos.second) ? -1 : 0;
                    int col_step = (king_position.first > pos.first) ? 1 : (king_position.first < pos.first) ? -1 : 0;

                    char current_row = pos.second + row_step;
                    char current_col = pos.first + col_step;

                    // check each position for blocking pieces
                    while (current_row != king_position.second || current_col != king_position.first) {
                        if (board(Position(current_col, current_row)) != nullptr) {
                            goto not_in_check; // path is blocked
                        }
                        current_row += row_step;
                        current_col += col_step;
                    }

                    // if path is clear then king is in check
                    return true;
                } else {
                    // other pieces (knights, pawns)
                    return true;
                }
            }

        not_in_check:; // for skipping over blocked paths
        }
    }

    return false; // no attacking pieces found
}

bool Game::in_mate(const bool& white) const {
    // if the player is not in check, they cannot be in checkmate
    if (!in_check(white)) {
        return false;
    }

    // check all possible moves for the player
    for (char row = '1'; row <= '8'; ++row) {
        for (char col = 'A'; col <= 'H'; ++col) {
            Position start(col, row);
            const Piece* piece = board(start);

            // skip if no piece or not the player's color
            if (!piece || piece->is_white() != white) {
                continue;
            }

            // try all possible destinations
            for (char dest_row = '1'; dest_row <= '8'; ++dest_row) {
                for (char dest_col = 'A'; dest_col <= 'H'; ++dest_col) {
                    Position end(dest_col, dest_row);

                    // skip if move shape is invalid
                    if (!piece->legal_move_shape(start, end)) {
                        continue;
                    }

                    // ensure the destination is not occupied by a piece of the same color
                    const Piece* target = board(end);
                    if (target && target->is_white() == white) {
                        continue;
                    }

                    // handle pawn-specific logic
                    char piece_type = piece->to_ascii();
                    if (piece_type == 'P' || piece_type == 'p') {
                        bool is_white_pawn = piece->is_white();
                        int direction = is_white_pawn ? 1 : -1; // movement direction based on color
                        bool is_capture = (target != nullptr);

                        if (is_capture) {
                            // Pawns can only capture diagonally
                            if (std::abs(end.first - start.first) != 1 || end.second != start.second + direction) {
                                continue;
                            }
                            if (target->is_white() == white) {
                                continue;
                            }
                        } else {
                            // Pawns can only move forward
                            if (end.first != start.first) {
                                continue;
                            }

                            // Check for two-space move from starting position
                            if (std::abs(end.second - start.second) == 2) {
                                if ((is_white_pawn && start.second != '2') || (!is_white_pawn && start.second != '7') ||
                                    board(Position(start.first, (start.second + end.second) / 2)) != nullptr ||
                                    target != nullptr) {
                                    continue;
                                }
                            } else if (end.second != start.second + direction || target != nullptr) {
                                continue;
                            }
                        }
                    }

                    // handle sliding piece path validation
                    if (piece_type == 'R' || piece_type == 'r' ||
                        piece_type == 'B' || piece_type == 'b' ||
                        piece_type == 'Q' || piece_type == 'q') {
                        int row_step = (end.second > start.second) ? 1 : (end.second < start.second) ? -1 : 0;
                        int col_step = (end.first > start.first) ? 1 : (end.first < start.first) ? -1 : 0;

                        char current_row = start.second + row_step;
                        char current_col = start.first + col_step;

                        // check intermediate positions for blocking pieces
                        bool path_blocked = false;
                        while (current_row != end.second || current_col != end.first) {
                            const Piece* blocker = board(Position(current_col, current_row));
                            if (blocker != nullptr) {
                                path_blocked = true;
                                break;
                            }
                            current_row += row_step;
                            current_col += col_step;
                        }

                        if (path_blocked) {
                            continue; // skip this move if the path is blocked
                        }
                    }

                    // simulate the move on a temporary game
                    Game temp_game = *this; // copy the current game state
                    temp_game.board.update(start, end); // move the piece

                    // check if the move resolves the check
                    if (!temp_game.in_check(white)) {
                        return false; // legal move to escape check
                    }
                }
            }
        }
    }

    // no legal moves available to escape check
    return true;
}

	bool Game::in_stalemate(const bool& white) const {
    // check if player is in check
    if (in_check(white)) {
        return false;
    }

    // check if there are any legal moves for player
    for (char row = '1'; row <= '8'; ++row) {
        for (char col = 'A'; col <= 'H'; ++col) {
            Position start(col, row);
            const Piece* piece = board(start);

            // skip if no piece or piece does not belong to player
            if (!piece || piece->is_white() != white) {
                continue;
            }

            // try all possible destinations
            for (char dest_row = '1'; dest_row <= '8'; ++dest_row) {
                for (char dest_col = 'A'; dest_col <= 'H'; ++dest_col) {
                    Position end(dest_col, dest_row);

                    // skip if move shape is invalid
                    if (!piece->legal_move_shape(start, end)) {
                        continue;
                    }

                    // ensure destination is not occupied by player's piece
                    const Piece* target = board(end);
                    if (target && target->is_white() == white) {
                        continue;
                    }

                    // handle path validation for sliding pieces
                    char piece_type = piece->to_ascii();
                    if (piece_type == 'R' || piece_type == 'r' ||
                        piece_type == 'B' || piece_type == 'b' ||
                        piece_type == 'Q' || piece_type == 'q') {
                        int row_step = (end.second > start.second) ? 1 : (end.second < start.second) ? -1 : 0;
                        int col_step = (end.first > start.first) ? 1 : (end.first < start.first) ? -1 : 0;

                        char current_row = start.second + row_step;
                        char current_col = start.first + col_step;

                        // check intermediate positions for blocking pieces
                        bool path_blocked = false;
                        while (current_row != end.second || current_col != end.first) {
                            if (board(Position(current_col, current_row)) != nullptr) {
                                path_blocked = true;
                                break;
                            }
                            current_row += row_step;
                            current_col += col_step;
                        }

                        if (path_blocked) {
                            continue; // skip move if path is blocked
                        }
                    }

                    // simulate move on temporary game
                    Game temp_game = *this; // copy current game state
                    temp_game.board.update(start, end); // make move

                    // check if move resolves check
                    if (!temp_game.in_check(white)) {
                        return false; // legal move exists; not a stalemate
                    }
                }
            }
        }
    }

    // no legal moves and not in check; it's a stalemate
    return true;
}


	std::istream& operator>> (std::istream& is, Game& game) {

        //storage for input line
        std::string line;

        //delete the current board
        game.board.delete_board();

        for(char row = '8'; row > '0'; --row){
            
            //check if file is formated correctly
            if(!std::getline(is, line) || line.length() != 8){
                throw Exception("Invalid column length or not enough rows");
            }

            //iterate through columns
            for(int col = 0; col < 8; ++col){
                
                //check if there is a piece and find position
                char piece_char = line[col];
                if(piece_char != '-'){
                    Position pos('A' + col, row);
                    game.board.add_piece(pos, piece_char);
                }
            }
        }

        //take in the last line with whose turn it is
        if(!std::getline(is, line) || line.length() != 1){
            throw Exception("Invalid length or missing player turn input");
        }

        //error check
        char active_player = line[0];
        if(active_player != 'w' && active_player != 'b'){
            throw Exception("Invalid player turn letter");
        }

        //set whose turn it is
        if(active_player == 'w'){
            game.is_white_turn = true;
        } else {
            game.is_white_turn = false;
        }

		return is;
	}

    /////////////////////////////////////
    // DO NOT MODIFY THIS FUNCTION!!!! //
    /////////////////////////////////////
	std::ostream& operator<< (std::ostream& os, const Game& game) {
		// Write the board out and then either the character 'w' or the character 'b',
		// depending on whose turn it is
		return os << game.board << (game.turn_white() ? 'w' : 'b');
	}

    // Return the total material point value of the designated player
    int Game::point_value(const bool& white) const {

        int sum = 0;

        //iterate through the board and add up the piece values
        for (char row = '1'; row <= '8'; ++row) {
            for (char col = 'A'; col <= 'H'; ++col) {
                Position pos(col, row);
                const Piece* piece = board(pos);

                //check if piece exists and color
                if (piece && piece->is_white() == white) {
                    char piece_designator = piece->to_ascii();

                    switch(std::tolower(piece_designator)) {
                        case 'q':
                            sum = sum + 9;
                            break;
                        case 'b':
                            sum = sum + 3;
                            break;
                        case 'n':
                            sum = sum + 3;
                            break;
                        case 'r':
                            sum = sum + 5;
                            break;
                        case 'p':
                            ++sum;
                            break;
                        case 'm':
                            sum += dynamic_cast<const Mystery *>(piece)->point_value();
                            break;
                        default:
                            break;
                    }
                }
            }
        }
        return sum;
    }

}
