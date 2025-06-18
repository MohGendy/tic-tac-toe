#include <gtest/gtest.h>
#include "gameStructure.h"

// Test for valid move
TEST(megaBoardTest, ValidMove) {
    megaBoard megaboard;
    // Player X makes a valid move in subgrid 0 at row 1, col 1
    bool result = megaboard.megaMakeMove(1, 1, 0, 'X');
    char cellValue = megaboard.megaGetCell(1, 1, 0); // Retrieve cell value from subgrid 0

    // Check that move was successful
    EXPECT_TRUE(result) << "Move was not placed correctly!";
    EXPECT_EQ(cellValue, 'X') << "Expected 'X', but found '" << cellValue << "' in subgrid (0) cell (1,1)";
}
// Test for invalid move (overwriting a cell)
TEST(megaBoardTest, PreventOverWrite) {
    megaBoard megaboard;
    EXPECT_TRUE(megaboard.megaMakeMove(0, 0, 0, 'X')); // First move should succeed
    EXPECT_FALSE(megaboard.megaMakeMove(0, 0, 0, 'O')); // Overwriting should fail
}
// Test for out-of-bounds move
TEST(megaBoardTest, OutOfBoundsIndex) {
    megaBoard megaboard;
    EXPECT_FALSE(megaboard.megaMakeMove(3, 3, 0, 'X')); // Out of bounds move
    EXPECT_FALSE(megaboard.megaMakeMove(-1, -1, 0, 'O')); // Negative indices
}
// Test for win condition in a subgrid
TEST(megaBoardTest, CheckSubgridWin) {
    megaBoard megaboard;
    // Fill subgrid 0 with a winning condition for 'X'
    megaboard.megaMakeMove(0, 0, 0, 'X');
    megaboard.megaMakeMove(0, 1, 0, 'X');
    megaboard.megaMakeMove(0, 2, 0, 'X'); // X wins in subgrid 0
    EXPECT_TRUE(megaboard.checkSubgridWin(0, 'X'));
    EXPECT_FALSE(megaboard.checkSubgridWin(0, 'O')); // O should not win
    EXPECT_EQ(megaboard.getWinningGrid(0), 'X'); // Check the winning grid for subgrid 0
}
// Test for tie condition in a subgrid
TEST(megaBoardTest, DetectSubgridTie) {
    megaBoard megaboard;
    // Fill subgrid 0 without a winner (alternating X and O)
    char moves[3][3] = {
        {'X', 'O', 'X'},
        {'X', 'X', 'O'},
        {'O', 'X', 'O'}
    };

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            megaboard.megaMakeMove(i, j, 0, moves[i][j]);
        }
    }
    // Subgrid should be full AND checkSubgridWin() should return false for both players
    EXPECT_TRUE(megaboard.isSubgridFull(0)) << "Subgrid should be full!";
    EXPECT_FALSE(megaboard.checkSubgridWin(0, 'X')) << "X incorrectly detected as winner!";
    EXPECT_FALSE(megaboard.checkSubgridWin(0, 'O')) << "O incorrectly detected as winner!";
}
//Correct sub grid targeting after moves
TEST(megaBoardTest, CorrectSubgridTargeting) {
    megaBoard megaboard;
    int allowedSubgrid = -1; // Initially, any subgrid is allowed
    int row, col, subgrid;

    // Player X makes a move in subgrid 0
    row = 1; col = 1; subgrid = 0;
    EXPECT_TRUE(megaboard.megaMakeMove(row, col, subgrid, 'X'));
    allowedSubgrid = row * 3 + col; // Next player allowed subgrid

    // Player O tries to play in the same subgrid
    row = 1; col = 1; subgrid = 0;
    EXPECT_FALSE(megaboard.megaMakeMove(row, col, subgrid, 'O')); // Should fail next move should be in subgrid 4

    // Player O plays in the next allowed subgrid
    row = 0; col = 0; subgrid = allowedSubgrid;
    EXPECT_TRUE(megaboard.megaMakeMove(row, col, subgrid, 'O'));
}

    //Test for winning condition across the mega board
TEST(megaBoardTest, MegaBoardWinCondition) { 
    megaBoard megaboard;
    // Fill the winning grids with a winning condition for 'X'
    megaboard.megaMakeMove(0, 0, 0, 'X');
    megaboard.megaMakeMove(0, 1, 0, 'X');
    megaboard.megaMakeMove(0, 2, 0, 'X'); // X wins in subgrid 0
    EXPECT_TRUE(megaboard.checkSubgridWin(0, 'X')); // Check if subgrid 0 is won
    megaboard.megaMakeMove(1, 0, 1, 'X');
    megaboard.megaMakeMove(1, 1, 1, 'X');
    megaboard.megaMakeMove(1, 2, 1, 'X'); // X wins in subgrid 1
    EXPECT_TRUE(megaboard.checkSubgridWin(1, 'X')); // Check if subgrid 1 is won
    megaboard.megaMakeMove(2, 0, 2, 'X');
    megaboard.megaMakeMove(2, 1, 2, 'X');
    megaboard.megaMakeMove(2, 2, 2, 'X'); // X wins in subgrid 2
    EXPECT_TRUE(megaboard.checkSubgridWin(2, 'X')); // Check if subgrid 2 is won
    
    char a = megaboard.getWinningGrid(0); // Check the winning grid for subgrid 0
    char b = megaboard.getWinningGrid(1); // Check the winning grid for subgrid 1
    char c = megaboard.getWinningGrid(2); // Check the winning grid for subgrid 2


    EXPECT_TRUE(megaboard.megaCheckWin('X'))<< "a: " << a << " b: " << b << " c: " << c << " X incorrectly detected as not winner!";
}

//Handling ties within sub grids
TEST(megaBoardTest, MegaBoardTieCondition) {
    megaBoard megaboard;
    // Fill the mega board without a winner (alternating X and O)
    char moves[3][3][9] = {
        {
            {'X', 'O', 'X', 'O', 'X', 'O', 'O', 'X', 'O'},
            {'O', 'X', 'O', 'X', 'O', 'X', 'X', 'O', 'X'},
            {'X', 'O', 'X', 'O', 'X', 'O', 'X', 'O', 'X'}
        },
        {
            {'O', 'X', 'O', 'X', 'O', 'X', 'X', 'O', 'X'},
            {'X', 'O', 'X', 'O', 'X', 'O', 'O', 'X', 'O'},
            {'O', 'X', 'O', 'X', 'O', 'X', 'X', 'O', 'X'}
        },
        {
            {'X', 'O', 'X', 'O', 'X', 'O', 'X', 'O', 'X'},
            {'O', 'X', 'O', 'X', 'O', 'X', 'O', 'X', 'O'},
            {'X', 'O', 'X', 'O', 'X', 'O', 'O', 'X', 'O'}
        }
    };

    for (int k = 0; k < 9; k++) {
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                megaboard.megaMakeMove(i, j, k, moves[i][j][k]);
            }
        }
    }

    for(int k = 0; k < 9; k++) {
        megaboard.checkSubgridWin(k, 'X'); // Check for subgrid wins
        megaboard.checkSubgridWin(k, 'O'); // Check for subgrid wins
    }

    //megaboard.displayWinningGrids(); // Display the winning grids

    //megaboard.megaDisplay();

    // Mega board should be full AND checkMegaWin() should return false for both players

    // Validate tie conditions: at least one must be true
    EXPECT_TRUE(megaboard.megaIsFull() || megaboard.iswinningGridsFull()) 
        << "Either mega board should be full or winning grids should be resolved!";
    // Check that no player has won the mega board
    EXPECT_FALSE(megaboard.megaCheckWin('X')) << "X incorrectly detected as winner!";
    EXPECT_FALSE(megaboard.megaCheckWin('O')) << "O incorrectly detected as winner!";
}
