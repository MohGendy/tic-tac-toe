#include "ReplayStack.hpp"

int main() {
    ReplayStack replay;
    replay.pushMove(0, 0, 'X');
    replay.pushMove(1, 1, 'O');
    replay.pushMove(0, 1, 'X');
    replay.displayBoard();

    std::cout << "\nUndo last move...\n";
    replay.undoMove();
    replay.displayBoard();
}
