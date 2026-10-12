#include <iostream>
#include <array>
#include <limits>  // needed for the cin.ignore thing later, took me a sec to remember this
 
// board is just 9 chars. starts as '1'-'9' so players can see which number = which square
using Board = std::array<char, 9>;
 
// prints the board in a 3x3 grid
void printBoard(const Board& b) {
    std::cout << "\n";
    for (int row = 0; row < 3; ++row) {
        // row * 3 gets us to the start of each row in the 1D array
        std::cout << " " << b[row * 3] << " | " << b[row * 3 + 1] << " | " << b[row * 3 + 2] << "\n";
        // don't draw a line under the last row, looked weird when i did
        if (row < 2) std::cout << "---+---+---\n";
    }
    std::cout << "\n";
}
 
// checks if player p has 3 in a row anywhere
bool hasWon(const Board& b, char p) {
    // just hardcoded every winning combo, there's only 8 so why overthink it
    const int lines[8][3] = {
        {0, 1, 2}, {3, 4, 5}, {6, 7, 8},  // rows
        {0, 3, 6}, {1, 4, 7}, {2, 5, 8},  // columns
        {0, 4, 8}, {2, 4, 6}              // diagonals
    };
    for (const auto& l : lines) {
        if (b[l[0]] == p && b[l[1]] == p && b[l[2]] == p) return true;
    }
    return false;
}
 
// if every square is an X or O then nobody can move anymore = draw
bool isFull(const Board& b) {
    for (char c : b) {
        if (c != 'X' && c != 'O') return false;
    }
    return true;
}
 
// keeps asking until the player gives a valid move, returns the index (0-8)
int askForMove(const Board& b, char player) {
    while (true) {
        std::cout << "Player " << player << ", choose a square (1-9): ";
        int choice;
 
        // if someone types "banana" cin breaks, so we have to handle that
        if (!(std::cin >> choice)) {
            if (std::cin.eof()) return -1;  // input got closed (ctrl+d), bail out so we don't loop forever
            std::cin.clear();  // reset the error flag
            // throw away whatever junk they typed. this line is ugly but it works
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Please enter a number.\n";
            continue;
        }
 
        if (choice < 1 || choice > 9) {
            std::cout << "That's not between 1 and 9.\n";
        } else if (b[choice - 1] == 'X' || b[choice - 1] == 'O') {
            // square already has something in it
            std::cout << "That square is already taken.\n";
        } else {
            return choice - 1;  // minus 1 because arrays start at 0 (classic)
        }
    }
}
 
int main() {
    char playAgain = 'y';
 
    // outer loop = lets you rematch without restarting the program
    while (playAgain == 'y' || playAgain == 'Y') {
        Board board = {'1', '2', '3', '4', '5', '6', '7', '8', '9'};
        char current = 'X';  // X always goes first
        bool finished = false;
 
        // one full game happens in here
        while (!finished) {
            printBoard(board);
            int move = askForMove(board, current);
            if (move == -1) return 0;  // input closed, just quit
            board[move] = current;
 
            if (hasWon(board, current)) {
                printBoard(board);
                std::cout << "Player " << current << " wins!\n";
                finished = true;
            } else if (isFull(board)) {
                // only checking for a draw AFTER checking for a win, otherwise a win on the last move
                // would count as a draw (almost made that mistake lol)
                printBoard(board);
                std::cout << "It's a draw!\n";
                finished = true;
            } else {
                // swap turns
                current = (current == 'X') ? 'O' : 'X';
            }
        }
 
        std::cout << "Play again? (y/n): ";
        std::cin >> playAgain;
    }
 
    std::cout << "Thanks for playing!\n";
    return 0;
}
