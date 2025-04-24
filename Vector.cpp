#include "ReplayVector.hpp"
#include <iostream>

ReplayVector::ReplayVector(const std::vector<Move>& gameMoves) : moves(gameMoves), currentIndex(0) {
    resetBoard();
}

void ReplayVector::resetBoard() {
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            board[i][j] = ' ';
}

void ReplayVector::nextMove() {
    if (currentIndex < moves.size()) {
        Move m = moves[currentIndex++];
        board[m.row][m.col] = m.player;
    }
}

void ReplayVector::prevMove() {
    if (currentIndex > 0) {
        currentIndex--;
        resetBoard();
        for (int i = 0; i < currentIndex; ++i) {
            Move m = moves[i];
            board[m.row][m.col] = m.player;
        }
    }
}

void ReplayVector::displayBoard() const {
    std::cout << "Current board:\n";
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            std::cout << board[i][j] << " ";
        }
        std::cout << "\n";
    }
}

