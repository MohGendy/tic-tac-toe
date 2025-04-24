struct Move {
    int row, col;
    char player;
};

class ReplayVector {
private:
    std::vector<Move> moves;
    int currentIndex;
    char board[3][3];

public:
    ReplayVector(const std::vector<Move>& gameMoves);
    void nextMove();
    void prevMove();
    void displayBoard() const;
    void resetBoard();
};
