#include <gtest/gtest.h>
#include "gameStructure.h"
#include "ai.h"

//ai move logic validation
TEST(AiTests,validMoves){
    Ai aiEasy = Ai(0, easy);
    Ai aiNormal = Ai(0, normal);
    Ai aiHard = Ai(0, hard);
    int move;

    EXPECT_GT(aiEasy.moveAi(&move), -1) << "Easy AI should return a valid move";
    EXPECT_GT(aiNormal.moveAi(&move), -1) << "Normal AI should return a valid move";
    EXPECT_GT(aiHard.moveAi(&move), -1) << "Hard AI should return a valid move";

    EXPECT_LT(aiEasy.moveAi(&move), 9) << "Easy AI move should be within the board range";
    EXPECT_LT(aiNormal.moveAi(&move), 9) << "Normal AI move should be within the board range";
    EXPECT_LT(aiHard.moveAi(&move), 9) << "Hard AI move should be within the board range";
}

TEST(AiTests, AiBlockPlayerWinning) {
    Tree tree(false);// Initialize the game tree player start first
    
    // Set up board so Player ('X') has a winning opportunity
    tree.move(0, true);  // 'X' moves at (0,0)
    tree.move(1, false); // 'O' moves at (0,1)
    tree.move(3, true);  // 'X' moves at (0,2)

    //tree.print(); // Print current board state

    // AI should now play at (2,0) (index 6) to prevent player from winning
    int move;
    EXPECT_TRUE(tree.bestMove(&move));
    EXPECT_EQ(move, 6) << "AI should play at (2,0) to win";
}

TEST(AiTests, AiWinningMove) {
    Tree tree(true); // Initialize the game tree ai start first
    
    // Set up board so AI ('X') can win
    tree.move(0, false);  // 'X' moves at (0,0)
    tree.move(1, true); // 'O' moves at (0,1)
    tree.move(2, false);  // 'X' moves at (0,2)
    tree.move(3, true); // 'O' moves at (1,0)
    tree.move(4, false);  // 'X' moves at (1,1)
    tree.move(6, true); // 'O' moves at (1,2)

    //tree.print(); // Print current board state
    //tree.rates(new int[9]); // Get rates for debuggings

    // AI should now play at (2,2) (index 8) to win
    int move;
    EXPECT_TRUE(tree.bestMove(&move));
    EXPECT_EQ(move, 8) << "AI should play at (2,2) to win";

}

TEST(AiTests, AiBlock2movesinadvance){
    Tree tree(false); // Initialize the game tree player start first
    
    // Set up board so Player ('X') has a winning opportunity in two moves
    tree.move(0, true);  // 'X' moves at (0,0)
    tree.move(1, false); // 'O' moves at (0,1)
    tree.move(2, true);  // 'X' moves at (0,2)

    //tree.print(); // Print current board state

    // AI should now play at (1,1) (index 4) to block player from winning in next move
    int move;
    EXPECT_TRUE(tree.bestMove(&move));
    EXPECT_EQ(move, 4) << "AI should play at (1,1) to block player from winning";
}

