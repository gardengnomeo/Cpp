#include <fstream>
#include <iostream>
#include <string>
#include <cassert>
#include "Game.h"

void show_commands() {
	std::cout << "List of commands:" << std::endl;
	std::cout << "\t'?':            show this list of options" << std::endl;
	std::cout << "\t'Q':            quit the game" << std::endl;
	std::cout << "\t'L' <filename>: load a game from the specified file" << std::endl;
	std::cout << "\t                <filename> is the name of the file to read from" <<std::endl;
	std::cout << "\t'S' <filename>: save a game to the specified file" << std::endl;
	std::cout << "\t                <filename> is the name of the file to write to" <<std::endl;
	std::cout << "\t'M' <move>:     try to make the specified move" << std::endl;
	std::cout << "\t                <move> is a four character string giving the" << std::endl;
	std::cout << "\t                column (['A'-'H']), row ('1'-'8') of the start position" << std::endl;
	std::cout << "\t                followed by the column and row of the end position" << std::endl;
}

int main(int argc, char* argv[]) {
	Chess::Game game;

	// Display command options
	show_commands();

	// Keep playing until the game is over
	bool game_over = false;

	while (!game_over) {
    try {
        // Display the board
        game.display();

        // Indicate whose turn it is
        if (game.turn_white()) {
            std::cout << "White's move." << std::endl;
        } else {
            std::cout << "Black's move." << std::endl;
        }

        std::cout << "Material point value: " << game.point_value(game.turn_white()) << std::endl;

        // Check for game-ending conditions
        if (game.in_mate(game.turn_white())) {
            std::cout << "Checkmate! Game over." << std::endl;
            game_over = true;
            break;
        } else if (game.in_check(game.turn_white())) {
            std::cout << "You are in check!" << std::endl;
        } else if (game.in_stalemate(game.turn_white())) {
            std::cout << "Stalemate! Game over." << std::endl;
            game_over = true;
            break;
        }

        // Get the next command
        std::string choice;
        std::pair<char, char> start, end;
        std::cout << "Next command: ";
        std::cin >> choice;

        // Validate that the command is a single character
        if (choice.length() != 1) {
            std::cerr << "Action specifier must be a single character, but length(" 
                      << choice << ") = " << choice.length() << std::endl;
        } else {
            // Process the different commands
            switch (choice[0]) {
                case '?':
                    // Show the commands
                    show_commands();
                    break;
                case 'Q': case 'q':
                    // Quit the game
                    game_over = true;
                    break;
                case 'L': case 'l': {
                    // Load a game from a file
                    try {
                        std::string argument;
                        std::cin >> argument;
                        std::ifstream ifs;
                        ifs.open(argument);

                        if (!ifs) {
                            throw std::ios_base::failure("Failed to open file for loading.");
                            ifs.close();
                        }
                        ifs >> game;
                        ifs.close();
                        // Check that the game is valid
                        if (!game.is_valid_game()) {
Instructor
| 12/14 at 2:58 am
Grading comment:
should assert(game.is_valid_game()) instead of throwing an exception

                            throw std::runtime_error("Invalid game state after loading.");
                        }
                    } catch (const std::exception& e) {
                        std::cerr << "Error loading game: " << e.what() << std::endl;
                        std::cerr << "Exiting program." << std::endl;
                        return -1;
                    }
                    break;
                }
                case 'S': case 's': {
                    // Write a game to a file
                    try {
                        std::string argument;
                        std::cin >> argument;
                        std::ofstream ofs(argument);
                        if (!ofs.is_open()) {
                            throw std::ios_base::failure("Failed to open file for saving.");
                        }
                        ofs << game;
                        ofs.close();
                    } catch (const std::exception& e) {
                        std::cerr << "Error saving game: " << e.what() << std::endl;
                    }
                    break;
                }
                case 'M': case 'm': {
                    // Make a move
                    try {
                        std::string argument;
                        std::cin >> argument;
                        // Validate that the move is correctly specified
                        if (argument.length() != 4) {
                            throw std::invalid_argument(
                                "Move specifier must be four characters.");
                        }
                        // Attempt to make the move
                        game.make_move(
                            std::make_pair(argument[0], argument[1]),
                            std::make_pair(argument[2], argument[3])
                        );
                    } catch (const std::invalid_argument& e) {
                        std::cerr << "Invalid move: " << e.what() << std::endl;
                    } catch (const std::exception& e) {
                        std::cerr << "Could not make move: " << e.what() 
                                  << std::endl;
                    }
                    break;
                }
                default:
                    // Unrecognized command
                    std::cerr << "Invalid action '" << choice << "'" << std::endl;
            }
        }
    } catch (const std::exception& e) {
        // General error handling for unexpected exceptions
        std::cerr << "An unexpected error occurred: " << e.what() << std::endl;
    }
}

	// Write out the state of the game to a file
	if (argc > 1) {
		std::ofstream ofs;
		ofs.open(argv[1]);
		ofs << game;
		ofs.close();
	}

	return 0;
}
