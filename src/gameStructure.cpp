#include <iostream>
using namespace std;
#include "gameStructure.h"
//########################################struct Board Functions########################################
    Board::Board() { //normal tic tac toe
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
        if (grid[row][col] == ' ') {        // Check if the cell is empty
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
//########################################struct megaBoard Functions########################################
    megaBoard::megaBoard(){
        // Initialize the grid with empty spaces
       for (int k = 0; k < 9; k++){
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    megaGrid[i][j][k] = ' ';
                }
            }
        }
        //intialize the winning mega grid 
        for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    winningGrids[i][j] = ' ';
                }
            }
    }
    
   bool megaBoard::checkSubgridWin(int subGrid, char playerSymbol) {
        // Check rows and columns for a win
        for (int i = 0; i < 3; i++) {
            if ((megaGrid[i][0][subGrid] == playerSymbol && megaGrid[i][1][subGrid] == playerSymbol && megaGrid[i][2][subGrid] == playerSymbol) ||
                (megaGrid[0][i][subGrid] == playerSymbol && megaGrid[1][i][subGrid] == playerSymbol && megaGrid[2][i][subGrid] == playerSymbol)) {
                    winningGrids[subGrid/3][(subGrid%3)-3]=playerSymbol;
                    return true;
            }
        }
        // Check diagonals for a win
        if ((megaGrid[0][0][subGrid] == playerSymbol && megaGrid[1][1][subGrid] == playerSymbol && megaGrid[2][2][subGrid] == playerSymbol) ||
            (megaGrid[0][2][subGrid] == playerSymbol && megaGrid[1][1][subGrid] == playerSymbol && megaGrid[2][0][subGrid] == playerSymbol)) {
                winningGrids[subGrid/3][(subGrid%3)-3]=playerSymbol;
                return true;
        }
        return false;
    }
    bool megaBoard::megaCheckWin(char playerSymbol){
        // Check rows and columns for a win
        for (int i = 0; i < 3; i++) {
            if ((winningGrids[i][0] == playerSymbol && winningGrids[i][1] == playerSymbol && winningGrids[i][2] == playerSymbol) ||
                (winningGrids[0][i] == playerSymbol && winningGrids[1][i] == playerSymbol && winningGrids[2][i] == playerSymbol)) {
                return true;
            }
        }
        // Check diagonals for a win
        if ((winningGrids[0][0] == playerSymbol && winningGrids[1][1] == playerSymbol && winningGrids[2][2] == playerSymbol) ||
            (winningGrids[0][2] == playerSymbol && winningGrids[1][1] == playerSymbol && winningGrids[2][0] == playerSymbol)) {
                return true;
        }
        return false;
        }
    bool megaBoard::megaIsFull(){
        for(int k = 0; k < 9; k++){        
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    if (megaGrid[i][j][k] == ' ') return false; // If any cell is empty, the board is not full
                }
            }
        }
        return true; // All cells are filled
    }    
void megaBoard::megaDisplay() {
    for (int megaRow = 0; megaRow < 3; megaRow++) {
        for (int subRow = 0; subRow < 3; subRow++) {  
            // Display each subgrid row
            for (int megaCol = 0; megaCol < 3; megaCol++) {
                for (int subCol = 0; subCol < 3; subCol++) {
                    cout << megaGrid[subRow][subCol][megaRow * 3 + megaCol];
                    if (subCol < 2) std::cout << " | ";  // Column separator inside subgrid
                }
                if (megaCol < 2) std::cout << "\t || \t"; // Separator between subgrids
            }
            std::cout << "\n";
            
            // **Add horizontal separators within each subgrid**
            if (subRow < 2) std::cout << "---------\t\t---------\t\t---------\n";
        }

        // **Major row separation between subgrid blocks**
        if (megaRow < 2) std::cout << "==============================================================\n";
    }
    cout<<"\n\n";
}

bool megaBoard::megaMakeMove(int row, int col,int subgrid, char playerSymbol){
        if (row < 0 || row >= 3 || col < 0 || col >= 3 || subgrid < 0 || subgrid > 8) return false; // Prevent invalid indices
        if (megaGrid[row][col][subgrid] == ' ') {        // Check if the cell is empty
            megaGrid[row][col][subgrid] = playerSymbol; //assign the symbol to the cell
            return true;
        }
        return false;
}
bool megaBoard::isSubgridFull(int subgrid) {
    // Validate subgrid index
    if (subgrid < 0 || subgrid > 8)
        return false;

    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 3; col++) {
            if (megaGrid[row][col][subgrid] == ' ')
                return false; // At least one empty cell found
        }
    }
    return true; // All cells filled
}


//########################################struct Game Functions########################################
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
            cout<<"\n\n";
            if (board.makeMove(row, col, currentPlayerSymbol)) { // If the move is valid
                if (board.checkWin(currentPlayerSymbol)) {      // Check for a win
                    board.displayBoard(); // Display the final board
                    cout << "Player " << currentPlayerSymbol << " wins!" << endl;
                    break;
                }
                if (board.isFull()) {       // Check for a tie
                    board.displayBoard();  // Display the final board
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

    void Game::megaPlay(){
        int row,col,subgrid;
        int allowedSubgrid = -1;                    //at first move any subgrid is allowed
        char currentPlayerSymbol = player1.symbol; // Start with player 1
        while(true){
            megaboard.megaDisplay(); // Display the board
            cout << "Player " << currentPlayerSymbol << ", enter your move (row and column and subgrid): ";
            cin >> row >> col >>subgrid; // Get the move from the player
        if ((megaboard.winningGrids[subgrid/3][subgrid%3 - 3] == ' ')){    
            if((allowedSubgrid == -1 ||subgrid == allowedSubgrid ) || 
                            (megaboard.isSubgridFull(allowedSubgrid) ) ){ //mega rules
                if (megaboard.megaMakeMove(row, col, subgrid , currentPlayerSymbol) ){// If the move is valid
                    if (megaboard.checkSubgridWin(subgrid,currentPlayerSymbol)){
                        cout << " Player " << currentPlayerSymbol << " wins subgrid: " <<subgrid << endl;
                    }

                    if (megaboard.megaCheckWin(currentPlayerSymbol)) {      // Check for a win
                        megaboard.megaDisplay();                           // Display the final board
                        cout << "Player " << currentPlayerSymbol << " wins!" << endl;
                        break;
                    }

                    if (megaboard.megaIsFull()) {  // Check for a tie
                        megaboard.megaDisplay();  // Display the final board
                        cout << "It's a tie!" << endl;
                        break;
                    }
                    // Switch players
                    currentPlayerSymbol = (currentPlayerSymbol == player1.symbol) ? player2.symbol : player1.symbol;
                    allowedSubgrid = row * 3 + col; //next player allowed subgrid
                    }
            }
            else {
                std::cout << "Invalid move. You must play in subgrid " << allowedSubgrid << ".\n";
            }
    }
    else {
        cout << "Invalid move.player: " <<megaboard.winningGrids[subgrid/3][subgrid%3 - 3]<< " already won subgrid " << subgrid << ".\n";
    }

        }

    }    