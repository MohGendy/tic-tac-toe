#include <iostream>
using namespace std;
#include "gameStructure.h"
#include "ai.h"
//game functions 
    Board::Board() {
        // Initialize the grid with empty spaces
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                grid[i][j] = ' ';
            }
        }
    }
    
    void Board::displayBoard() { // Display in the console
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                cout << grid[i][j];
                if (j < 2) std::cout << " | ";
            }
                cout << "\n";
            if (i < 2) std::cout << "---------\n";
        }
    }

    bool Board::makeMove(int row, int col, char playerSymbol) {
        if (row < 0 || row >= 3 || col < 0 || col >= 3) return false; // Prevent invalid indices
        if (grid[row][col] == ' ') { // Check if the cell is empty
            grid[row][col] = playerSymbol; //assign the symbol to the cell
            return true;
        }
        return false;
    }

    bool Board::checkWin(char playerSymbol) {
        // Check rows and columns for a win
        for (int i = 0; i < 3; i++) {
            if ((grid[i][0] == playerSymbol && grid[i][1] == playerSymbol && grid[i][2] == playerSymbol) ||
                (grid[0][i] == playerSymbol && grid[1][i] == playerSymbol && grid[2][i] == playerSymbol)) {
                return true;
            }
        }
        // Check diagonals for a win
        if ((grid[0][0] == playerSymbol && grid[1][1] == playerSymbol && grid[2][2] == playerSymbol) ||
            (grid[0][2] == playerSymbol && grid[1][1] == playerSymbol && grid[2][0] == playerSymbol)) {
            return true;
        }
        return false;
    }

    bool Board::isFull() {
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (grid[i][j] == ' ') return false; // If any cell is empty, the board is not full
            }
        }
        return true; // All cells are filled
    }

   Game::Game():player1('X'), player2('O') { // Temporary values for initialization

        char p1Symbol;
        cout << "Player 1, choose your symbol (X/O): ";
        cin >> p1Symbol;
        p1Symbol = toupper(p1Symbol); // Convert to uppercase for consistency
    
        if (p1Symbol != 'X' && p1Symbol != 'O') {
            cout << "Invalid choice! Defaulting to X for Player 1.\n";
            p1Symbol = 'X';
        }
    
        player1.symbol = p1Symbol;
        player2.symbol = (p1Symbol == 'X' ? 'O' : 'X'); //assign the other symbol to player 2
    }
    
    void Game::play() {
        int row, col;
        char currentPlayerSymbol = player1.symbol; // Start with player 1
        while (true) {
            board.displayBoard(); // Display the board
            cout << "Player " << currentPlayerSymbol << ", enter your move (row and column): ";
            cin >> row >> col; // Get the move from the player
            if (board.makeMove(row, col, currentPlayerSymbol)) { // If the move is valid
                if (board.checkWin(currentPlayerSymbol)) { // Check for a win
                    board.displayBoard(); // Display the final board
                    cout << "Player " << currentPlayerSymbol << " wins!" << endl;
                    break;
                }
                if (board.isFull()) { // Check for a tie
                    board.displayBoard(); // Display the final board
                    cout << "It's a tie!" << endl;
                    break;
                }
                // Switch players
                currentPlayerSymbol = (currentPlayerSymbol == player1.symbol) ? player2.symbol : player1.symbol;
            } else {
                cout << "Invalid move. Try again." << endl; // Invalid move message
            }
        }
    }

    void Game::playAi() {
        
        int row, col;
        bool start;
        char sel;
        int level;
        cout << "select level (0 1 2)\n";
        cin >> level;
        cout << "will player start first? (y,n)\n";
        cin >> sel;
        start = (sel == 'y');
        char currentPlayerSymbol = (start?player1.symbol:player2.symbol);
        Ai ai = Ai(!start,level);
        while (true) {
            board.displayBoard(); // Display the board
            if(currentPlayerSymbol == player2.symbol){
                int move;
                int x[9] = {0};
                
                if(ai.moveAi(&move)){
                   row = move/3;
                   col = move%3;
                }else{
                    cout << "some error happend";
                    break;
                }
                cout << "Ai played at: " << row << " " << col << endl;
            }else{
                cout << "Player " << currentPlayerSymbol << ", enter your move (row and column): ";
                cin >> row >> col; // Get the move from the player
            }
            if (board.makeMove(row, col, currentPlayerSymbol)) { // If the move is valid
                if (board.checkWin(currentPlayerSymbol)) { // Check for a win
                    board.displayBoard(); // Display the final board
                    cout << "Player " << currentPlayerSymbol << " wins!" << endl;
                    break;
                }
                if (board.isFull()) { // Check for a tie
                    board.displayBoard(); // Display the final board
                    cout << "It's a tie!" << endl;
                    break;
                }
                // Switch players
                if(currentPlayerSymbol == player1.symbol){
                    if(!ai.movePlayer(row*3+col)){
                        cout << "some error happend";
                        break;
                    }
                }
                currentPlayerSymbol = (currentPlayerSymbol == player1.symbol) ? player2.symbol : player1.symbol;
            } else {
                cout << "Invalid move. Try again." << endl; // Invalid move message
                cin.ignore();
            }
        }
    }