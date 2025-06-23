#ifndef GAME_STRUCTURE_H
#define GAME_STRUCTURE_H
#include <iostream>
using namespace std;

class Board {
private:
    char grid[3][3];
public:
    Board();
    void displayBoard();
    bool makeMove(int row, int col, char playerSymbol);
    bool checkWin(char playerSymbol);
    bool isFull();//detects a tie game
    char getCell(int row, int col) const; //help in testing
};

class Player {
public:
    std::string name;
    char symbol;
    Player(char s) : symbol(s) {}; // Constructor to initialize player symbol;
};

class megaBoard{
    private:
    char megaGrid[3][3][9];
    char winningGrids[3][3]; 
    public:
    megaBoard();
    void megaDisplay();
    bool megaMakeMove(int row, int col,int subgrid, char playerSymbol);
    bool checkSubgridWin(int subgrid, char playerSymbol);
    bool megaCheckWin(char playerSymbol);
    bool megaIsFull();
    bool iswinningGridsFull();
    bool isSubgridFull(int subgrid); 
    bool isGridwon(int subgrid);
    char megaGetCell(int row, int col, int subgrid) const ;//help in testing
    char getWinningGrid(int subgid) const; //help in testing
    void displayWinningGrids();//help in testing
};

class Game {
private:
    Board board;
    megaBoard megaboard;
    Player player1, player2;
public:
    Game();
    void getMove(int &row ,int &col);
    void megaGetMove(int &row ,int &col ,int &subgrid);
    void play();
    void megaPlay();
    void playAi();
};

#endif // GAME_STRUCTURE_H