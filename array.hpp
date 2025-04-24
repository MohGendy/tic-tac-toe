struct Move {
    int row, col;
    char player;
};

class ReplayStack {
private:
    Move moveStack[9]; // Max 9 moves
    int top;
    char board[3][3];

public:
    ReplayStack();
    void pushMove(int row, int col, char player);
    void undoMove();
    void displayBoard() const;
    void resetBoard();
};

