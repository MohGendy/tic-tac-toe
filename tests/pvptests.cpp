#include <gtest/gtest.h>
#include "gameStructure.h"


//valid move test
TEST(BoardTest, ValidMove) {
    Board board;
    // Player X makes a valid move at row 1, col 1
    bool result = board.makeMove(1, 1, 'X');
    char cellValue = board.getCell(1, 1);  // Retrieve cell value

    // Check that move was successful
    EXPECT_TRUE(result) << "Move was not placed correctly!";
    EXPECT_EQ(cellValue, 'X') << "Expected 'X', but found '" << cellValue << "' in cell (1,1)";
}
//invalid move tests
TEST(BoardTest, PreventOverWrite) {
    // Test that a cell cannot be overwritten
    Board board;
    EXPECT_TRUE(board.makeMove(0, 0, 'X'));
    EXPECT_FALSE(board.makeMove(0, 0, 'O'));
}

TEST(BoardTest, outOfBoundsIndex) {
    Board board;
    EXPECT_FALSE(board.makeMove(3, 3, 'X')); // Out of bounds move
    EXPECT_FALSE(board.makeMove(-1, -1, 'O')); // Negative indices
}
//win condition test
TEST(BoardTest, CheckWin) {
    Board board;
    board.makeMove(0, 0, 'X');
    board.makeMove(0, 1, 'X');
    board.makeMove(0, 2, 'X'); // X wins
    EXPECT_TRUE(board.checkWin('X'));
    EXPECT_FALSE(board.checkWin('O')); // O should not win
}
//tie condition test
TEST(BoardTest, DetectTie) {
    Board board;
    // Fill board without a winner (alternating X and O)
    char moves[3][3] = {
        {'X', 'O', 'X'},
        {'X', 'X', 'O'},
        {'O', 'X', 'O'}
    };

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            board.makeMove(i, j, moves[i][j]);
        }
    }
    // Board should be full AND checkWin() should return false for both players
    EXPECT_TRUE(board.isFull()) << "Board should be full!";
    EXPECT_FALSE(board.checkWin('X')) << "X incorrectly detected as winner!";
    EXPECT_FALSE(board.checkWin('O')) << "O incorrectly detected as winner!";
}