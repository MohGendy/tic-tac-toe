#include "ReplayVector.hpp"

int main() {
    std::vector<Move> gameMoves = {
        {0, 0, 'X'},
        {1, 1, 'O'},
        {0, 1, 'X'}
    };

    ReplayVector replay(gameMoves);
    replay.displayBoard();

    std::cout << "\nNext move...\n";
    replay.nextMove();
    replay.displayBoard();

    std::cout << "\nNext move...\n";
    replay.nextMove();
    replay.displayBoard();

    std::cout << "\nPrevious move...\n";
    replay.prevMove();
    replay.displayBoard();
}
