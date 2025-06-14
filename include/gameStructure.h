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
};

#endif // GAME_STRUCTURE_H