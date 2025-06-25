#ifndef AI_H
#define AI_H

#include <memory>

typedef enum{
    easy,
    normal,
    hard
} levels;

class State {
    public:
    int rate;
    State * nextState[9]= {0};
    State * free();
};

class Tree {
    private:
    State * current;
    short board[9] = {0};
    State * head;
    State * minimax(bool isMaximizing,int depth,int lastMove = -1);
    int checkBoard(int move);
    bool isFull();
    public:
    Tree(bool start);
    ~Tree();
    bool rates(int arr[]);
    bool move(int move,bool isPlayer);
    bool bestMove(int * move);
    void print();
    void treeBoard(int arr[]);
    void reset();
};

class Ai {
    int level;
    Tree tree;
    public:
    Ai(int start,int level);
    bool moveAi(int * move);
    bool movePlayer(int move);
    void reset();
    void print(); //for testing

};
#endif