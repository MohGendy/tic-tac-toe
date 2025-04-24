#include "ReplayStack.hpp"
#include <iostream>

ReplayStack::ReplayStack() : top(-1) {
    resetBoard();
}

void ReplayStack::resetBoard() {
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            board[i][j] = ' ';
}

void ReplayStack::pushMove(int row, int col, char player) {
    if (top < 8) {
        moveStack[++top] = {row, col, player};
        board[row][col] = player;
    }
}

void ReplayStack::undoMove() {
    if (top >= 0) {
        Move last = moveStack[top--];
        board[last.row][last.col] = ' ';
    }
}

void ReplayStack::displayBoard() const {
    std::cout << "Current board:\n";
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            std::cout << board[i][j] << " ";
        }
        std::cout << "\n";
    }
}
