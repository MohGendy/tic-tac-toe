// main.cpp
#include "ReplayManager.hpp"

int main() {
    ReplayManager replay;

    // Example game 1
    replay.pushMove(0, 0, 'X');
    replay.pushMove(1, 1, 'O');
    replay.pushMove(0, 1, 'X');
    replay.pushMove(2, 2, 'O');
    replay.pushMove(0, 2, 'X'); // X wins
    replay.endGame();

    // Example game 2
    replay.pushMove(0, 0, 'O');
    replay.pushMove(1, 1, 'X');
    replay.pushMove(0, 1, 'O');
    replay.pushMove(2, 2, 'X');
    replay.pushMove(0, 2, 'O'); // O wins
    replay.endGame();

    replay.displayHistory();

    // Replay a game
    int gameToReplay;
    std::cout << "Enter game number to replay: ";
    std::cin >> gameToReplay;
    replay.replayGame(gameToReplay - 1); // because indexing starts from 0

    return 0;
}
