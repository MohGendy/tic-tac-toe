#include <gtest/gtest.h>
#include "gameStructure.h"

TEST(BoardTest, ValidMove) {
    Board board;
    EXPECT_TRUE(board.makeMove(0, 0, 'X'));
    EXPECT_FALSE(board.makeMove(0, 0, 'O'));
}