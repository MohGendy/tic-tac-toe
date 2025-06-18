// ReplayManager.cpp
// #include "ReplayManager.hpp"
#include "gameStructure.h"
#include <iostream>


int main() {
    Game game; // Create a new game instance
    // int choice;
    // cout << "enter 1 for pvp or 2 for ai vs player\n";
    // cin >> choice; // Get the choice from the user
    // switch (choice)
    // {
    // case 1:
    //     cout << "enter 1 for classic mode and 2 for mega mode\n";
    //     int mode;
    //     cin >> mode; // Get the mode choice from the user
    //     if (mode == 1) {
    //         cout << "Starting classic Tic Tac Toe...\n";
    //         game.play(); // Start the classic game
    //     } else if (mode == 2) {
    //         cout << "Starting Mega Tic Tac Toe...\n";
    //         game.megaPlay(); // Start the mega game
    //     } else {
    //         cout << "Invalid mode selected. Defaulting to classic mode.\n";
    //         game.play(); // Start the classic game
    //     }
    //     break;
    // case 2:
    //     cout << "starting AI vs Player Tic Tac Toe...\n";
    //     game.playAi(); //todo Start the AI vs Player game
    //     break;
    
    // default:
    //     cout << "Starting classic Tic Tac Toe...\n";
    //     game.play(); // Start the classic game
    //     break;
    // }


    game.play(); // Start the game

    return 0;
}

// ReplayManager::ReplayManager() : xWins(0), oWins(0), draws(0), currentMoveIndex(0) {
//     resetBoard();
// }

// void ReplayManager::resetBoard() {
//     for (int i = 0; i < 3; ++i)
//         for (int j = 0; j < 3; ++j)
//             board[i][j] = ' ';
//     moveStack.clear();
//     currentMoveIndex = 0;
// }

// void ReplayManager::pushMove(int row, int col, char player) {
//     moveStack.push_back({row, col, player});
//     board[row][col] = player;
// }

// void ReplayManager::undoMove() {
//     if (!moveStack.empty()) {
//         Move last = moveStack.back();
//         moveStack.pop_back();
//         board[last.row][last.col] = ' ';
//     }
// }

// void ReplayManager::endGame() {
//     GameRecord record;
//     for (const auto& move : moveStack) {
//         record.moves.push_back(move);
//     }

//     char winner = checkWinner();
//     if (winner == 'X') {
//         record.result = "X wins!";
//         xWins++;
//     } else if (winner == 'O') {
//         record.result = "O wins!";
//         oWins++;
//     } else if (isBoardFull()) {
//         record.result = "Draw";
//         draws++;
//     } else {
//         record.result = "Game not finished";
//     }

//     gameHistory.push_back(record);
//     resetBoard();
// }

// void ReplayManager::displayBoard() const {
//     std::cout << "Current board:\n";
//     for (int i = 0; i < 3; ++i) {
//         for (int j = 0; j < 3; ++j) {
//             std::cout << board[i][j] << " ";
//         }
//         std::cout << "\n";
//     }
// }

// void ReplayManager::displayHistory() const {
//     std::cout << "\nGame History:\n";
//     for (size_t i = 0; i < gameHistory.size(); ++i) {
//         std::cout << "Game " << i + 1 << ": " << gameHistory[i].result << "\n";
//         for (const auto& move : gameHistory[i].moves) {
//             std::cout << "  Player " << move.player 
//                       << " to (" << move.row << "," << move.col << ")\n";
//         }
//     }
//     std::cout << "\nStatistics:\n";
//     std::cout << "X Wins: " << xWins << "\n";
//     std::cout << "O Wins: " << oWins << "\n";
//     std::cout << "Draws: " << draws << "\n";
// }

// void ReplayManager::replayGame(int gameIndex) {
//     if (gameIndex < 0 || gameIndex >= (int)gameHistory.size()) {
//         std::cout << "Invalid game index.\n";
//         return;
//     }
//     const auto& record = gameHistory[gameIndex];
//     resetBoard();
//     currentMoveIndex = 0;

//     char command;
//     while (true) {
//         displayBoard();
//         std::cout << "Move " << currentMoveIndex << "/" << record.moves.size() << "\n";
//         std::cout << "n: next move, p: previous move, q: quit replay\n";
//         std::cin >> command;
//         if (command == 'n') {
//             if (currentMoveIndex < (int)record.moves.size()) {
//                 Move move = record.moves[currentMoveIndex];
//                 board[move.row][move.col] = move.player;
//                 currentMoveIndex++;
//             } else {
//                 std::cout << "End of game.\n";
//             }
//         } else if (command == 'p') {
//             if (currentMoveIndex > 0) {
//                 currentMoveIndex--;
//                 Move move = record.moves[currentMoveIndex];
//                 board[move.row][move.col] = ' ';
//             } else {
//                 std::cout << "Start of game.\n";
//             }
//         } else if (command == 'q') {
//             break;
//         }
//     }
// }

// char ReplayManager::checkWinner() const {
//     for (int i = 0; i < 3; ++i) {
//         if (board[i][0] != ' ' &&
//             board[i][0] == board[i][1] &&
//             board[i][1] == board[i][2])
//             return board[i][0];
//         if (board[0][i] != ' ' &&
//             board[0][i] == board[1][i] &&
//             board[1][i] == board[2][i])
//             return board[0][i];
//     }
//     if (board[0][0] != ' ' &&
//         board[0][0] == board[1][1] &&
//         board[1][1] == board[2][2])
//         return board[0][0];
//     if (board[0][2] != ' ' &&
//         board[0][2] == board[1][1] &&
//         board[1][1] == board[2][0])
//         return board[0][2];
//     return ' ';
// }

// bool ReplayManager::isBoardFull() const {
//     return moveStack.size() == 9;
// }

