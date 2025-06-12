// ReplayManager.hpp
#pragma once
#include <vector>
#include <string>

struct Move {
    int row, col;
    char player;
};

struct GameRecord {
    std::vector<Move> moves;
    std::string result;
};

class ReplayManager {
private:
    std::vector<Move> moveStack;
    std::vector<GameRecord> gameHistory;
    char board[3][3];
    int xWins, oWins, draws;
    int currentMoveIndex;

public:
    ReplayManager();

    void resetBoard();
    void pushMove(int row, int col, char player);
    void undoMove();
    void endGame();
    void displayBoard() const;
    void displayHistory() const;
    void replayGame(int gameIndex);

private:
    char checkWinner() const;
    bool isBoardFull() const;
};
