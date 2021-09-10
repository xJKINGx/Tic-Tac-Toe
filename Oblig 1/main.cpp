#include <iostream>
#include <string>
#include <math.h>

void drawBoard();
void playGame(int players);
int endGame();
void refreshVariables();

bool game_running = true;
bool game_loop = true;
bool player1Turn = true;
std::string endGameAnswer{};
std::string playerAmount{};

const int row = 3;
const int col = 3;
std::string table[row][col] = { {"1","2","3"},{"4","5","6"},{"7","8","9"}};
// The 2D array is a string and not an int, to make it easier to avoid type conflicts and get
// inputs right.

int main() {
	std::srand(time(0));
	while (game_running) {
		//Start game, show start screen
		refreshVariables();
		std::cout << "---------- WELCOME TO TIC-TAC-TOE ---------" << std::endl  << std::endl;

		drawBoard();
		std::cout << std::endl;

		// End startup screen
		// Ask for amount of players, 1 = vs. AI, 2 = 2 players
		// If the users input anything but "1" or "2", the program will stop them
		// The parameter in the playGame() function is used for determining what type of game it's supposed 
		// to run

		std::cout << "How many players are playing? (1 or 2): ";
		getline(std::cin, playerAmount);
		if (playerAmount == "1") {
			player1Turn = true;
			playGame(1);
		}
		else if (playerAmount == "2") {
			player1Turn = true;
			playGame(2);
		}
		else {
			std::cout << "There can be a maximum of 2 players, please enter either '1' or '2'" << std::endl;
			continue;
		}

		// End of the game, ask to exit or keep playing.
		// If the user does not write either "Yes" or "No" the computer will continue playing.
		// You can only exit during this prompt and by typing exactly "No"
		
		std::cout << "Do you want to continue playing? (Yes/No): ";
		getline(std::cin, endGameAnswer);
		if (endGameAnswer == "No") {
			exit(0);
		}
		else if (endGameAnswer == "Yes") {
			continue;		
		}		
	}
}

void playGame(int players) {
	// The game_loop boolean and the while loop will serve as the game's gameplay loop
	// The while loop will be exited when a player wins or if the board is filled.
	// Once the while loops is done, the code returns to the main function, more spesifically the end of
	// "while(game_running), where the players will be asked if they wish to continue playing
	game_loop = true;
	while (game_loop) {
		std::string selectedPiece = "0";
		// Default value for selectedPiece will be "0"
		drawBoard();
		// Player 1 will always start
		if (player1Turn) {
			std::cout << "Type a number to place a 'X' in that spot: ";
			getline(std::cin, selectedPiece);

			// These for loops go through the entire board to see if their number is there, if so
			// it will replace that number with their piece (either "X" or "0"), otherwise it will continue
			// Because of the way this is written, if the players enters something that is not in the 2D
			// array, it will loop back around and the same player will be able to move again until they 
			// find a spot they can fill
			for (int i = 0; i < row; i++) {
				for (int j = 0; j < col; j++) {
					if (selectedPiece == table[i][j]) {
						//This if sentence is here so that players can't take over eachothers pieces
						if (table[i][j] == "X" || table[i][j] == "0") {
							std::cout << "Nice try" << std::endl;
							continue;
						}
						// The for loops found the value, now they replace it and end the player's turn
						// The loop will continue to the end of the 2D array, however each value will be unique
						// apart from "X" or "0", but that's already been accounted for in the if statment above
						// so it doesn't really matter if the loops go through the enitre thing
						table[i][j] = "X";
						player1Turn = false;
					}
				}
			}
		}
		else {
			//Player 2 is either AI or other player
			if (players == 1) {
				// Playing vs. AI
				std::string rdmValue{};
				// Since we cannot compare a string to an int, we need to randomly chose a set of strings
				// We can do this by chosing a random spot in an array.
				// This way the computer can chose a random number, and we can still use a string
				// so that it doesn't break the rest of the code.
				std::string stringArray[9] = { "1","2","3","4","5","6","7","8","9" };
				rdmValue = stringArray[std::rand() % 8];

				for (int i = 0; i < row; i++) {
					for (int j = 0; j < col; j++) {
						if (rdmValue == table[i][j]) {
							table[i][j] = "0";
							player1Turn = true;
						}
					}
				}
				
			}
			else {
				// Playing vs. Player
				// This code is basically the exact same as the player 1 code, the only difference is that
				// pieces will be replaced with "0" and not "X"
				std::cout << "Type a number to place a '0' in that spot: ";
				getline(std::cin, selectedPiece);

				for (int i = 0; i < row; i++) {
					for (int j = 0; j < col; j++) {
						if (selectedPiece == table[i][j]) {
							//This if sentence is here so that players can't take over eachothers pieces
							if (table[i][j] == "X" || table[i][j] == "0") {
								std::cout << "Nice try" << std::endl;
								continue;
							}
							table[i][j] = "0";
							player1Turn = true;
						}
					}
				}
			}
		}

		// The turn has been completed, now check if the player has won, if not, continue until the end
		// The code will run the endGame() function, and see what it returns
		// If it returns 1, player 1 won
		// If it returns 2, player 2 won
		// If it returns 3, the board was filled, with no winner
		// If it returns 4, no end condition has been met, and the game will continue

		if (endGame() == 1) {
			// Player 1 Won
			std::cout << "Player 1 Won!" << std::endl;
			game_loop = false;
		}
		else if (endGame() == 2) {
			// Player 2/AI won
			std::cout << "Player 2 Won!" << std::endl;
			game_loop = false;
		}
		else if (endGame() == 3) {
			std::cout << "GAME OVER - DRAW" << std::endl;
			game_loop = false;
		}
		else {
			continue;
		}
	}
}

int endGame() {
	int winner{};
	//
	// There are 8 ways to win the game 3 are to fill the 3 horizontal lines, 3 are to fill the 3 vertical lines
	// The last 2 are for diagonals, top left to bottom right, and top right to bottom left
	// We can test all these win conditions and if any of these lines are filled with X's or 0's, we know who won
	// If there are no 3-in-a-rows, and the board is filled, there is no winner, it's a draw.
	//

	//First we'll check horizontal lines.
	if (table[0][0] == "X" && table[0][1] == "X" && table[0][2] == "X") {
		winner = 1;
	} else if (table[0][0] == "0" && table[0][1] == "0" && table[0][2] == "0") {
		winner = 2;
	} else if (table[1][0] == "X" && table[1][1] == "X" && table[1][2] == "X") {
		winner = 1;
	} else if (table[1][0] == "0" && table[1][1] == "0" && table[1][2] == "0") {
		winner = 2;
	} else if (table[2][0] == "X" && table[2][1] == "X" && table[2][2] == "X") {
		winner = 1;
	} else if (table[2][0] == "0" && table[2][1] == "0" && table[2][2] == "0") {
		winner = 2;
	}

	//Secondly, the horizontal lines
	if (table[0][0] == "X" && table[1][0] == "X" && table[2][0] == "X") {
		winner = 1;
	} else if (table[0][0] == "0" && table[1][0] == "0" && table[2][0] == "0") {
		winner = 2;
	} else if (table[0][1] == "X" && table[1][1] == "X" && table[2][1] == "X") {
		winner = 1;
	} else if (table[0][1] == "0" && table[1][1] == "0" && table[2][1] == "0") {
		winner = 2;
	} else if (table[0][2] == "X" && table[1][2] == "X" && table[2][2] == "X") {
		winner = 1;
	} else if (table[0][2] == "0" && table[1][2] == "0" && table[2][2] == "0") {
		winner = 2;
	}

	//Lastly, the diagonals
	if (table[0][0] == "X" && table[1][1] == "X" && table[2][2] == "X") {
		winner = 1;
	} else if (table[0][0] == "0" && table[1][1] == "0" && table[2][2] == "0") {
		winner = 2;
	} else if(table[0][2] == "X" && table[1][1] == "X" && table[2][0] == "X") {
		winner = 1;
	} else if (table[0][2] == "0" && table[1][1] == "0" && table[2][0] == "0") {
		winner = 2;
	}

	// If there was no winner, see if the board was filled
	
	if (!(winner == 1 || winner == 2)) {
		// First check if the entire board is filled
		for (int i = 0; i < row; i++) {
			for (int j = 0; j < col; j++) {
				//std::cout << "Entered For loop";
				if (table[i][j] == "X") {
					continue;
				}
				else if (table[i][j] == "0") {
					continue;
				}
				else {
					winner = 4;
					// If it reached this stage, there are pieces on the board that can be filled
					// and the players have not gotten a 3 in a row
					break;
				}
			}
		}
	}

	// This if sentence says that if neither player 1 or 2 won, and the board is filled, set winner to 3
	if (winner != 1 && winner != 2 && winner != 4) {
		winner = 3;
	}
	return winner;
}

// It was important to get a drawboard function so that it would be possible to draw the board frequently
// Since we do not know how long the games would last, it would be impossible to hard code
// The layout isn't dependant on anything, meaning that the layout will remain the same, no matter what has
// been replaced on the board
void drawBoard() {
	for (int i = 0; i < row; i++) {
		std::cout << "---------------" << std::endl;
		for (int j = 0; j < col; j++) {
			std::cout << "| " << table[i][j] << " |";
		}
		std::cout << std::endl;
	}
	std::cout << "---------------" << std::endl;
}

// If the players wish to continue playing, this is here to reset the game's variables
// Since we're replacing the values in the 2D array, they must be replenished, otherwise
// the game will continue with last game's board.
void refreshVariables() {
	game_running = true;
	game_loop = true;
	player1Turn = true;
	std::string endGameAnswer{};
	std::string playerAmount{};
	table[0][0] = "1";
	table[0][1] = "2";
	table[0][2] = "3";
	table[1][0] = "4";
	table[1][1] = "5";
	table[1][2] = "6";
	table[2][0] = "7";
	table[2][1] = "8";
	table[2][2] = "9";
}