#ifndef GAME_STRUCTURE_H
#define GAME_STRUCTURE_H

class Board {
private:
    char grid[3][3];
public:
    Board();
    void displayBoard();
    bool makeMove(int row, int col, char playerSymbol);
    bool checkWin(char playerSymbol);
    bool isFull();//detects a tie game
};

class Player {
public:
    char symbol;
    Player(char s) : symbol(s) {}; // Constructor to initialize player symbol;
};

class Game {
private:
    Board board;
    Player player1, player2;
public:
    Game();
    void play();
};

#endif // GAME_STRUCTURE_H