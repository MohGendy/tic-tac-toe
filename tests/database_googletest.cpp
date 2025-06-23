#include <gtest/gtest.h>
// #include "sqlite3.h"
#include "database.h"

class DatabaseTest : public ::testing::Test {
protected:
    sqlite3* db;

    void SetUp() override {
        // In-memory DB for test
        ASSERT_EQ(sqlite3_open(":memory:", &db), SQLITE_OK);

        ASSERT_TRUE(enableForeignKeys(db));
        ASSERT_TRUE(createTables(db))<< sqlite3_errmsg(db);
    }
   void TearDown() override {
        sqlite3_close(db);
    }
};
TEST_F(DatabaseTest, RegisterAndLoginUser) {
    std::string username = "testuser";
    std::string password = "12345678";

    // Mock user input
    testing::internal::CaptureStdout();
    ASSERT_EQ(registerUserGUI(db, username, password), 4);
    testing::internal::GetCapturedStdout();

    int userId = getUserId(db, username);
    ASSERT_NE(userId, -1);  // User was registered successfully

    std::string loginName = username;
    testing::internal::CaptureStdout();
    ASSERT_EQ(authenticateUserGUI(db, username, password), 3);
    testing::internal::GetCapturedStdout();
}
TEST_F(DatabaseTest, InsertGameHistoryAndRetrieve) {
    std::string player1 = "Alice", player2 = "Bob";
    std::string pass = "pass";

    // Register users
    EXPECT_TRUE(registerUserGUI(db,player1,pass));
    EXPECT_TRUE(registerUserGUI(db,player2,pass));
    
    int id1 = getUserId(db, player1);
    int id2 = getUserId(db, player2);

    std::string board = "XOXOXOXOX";
    int game_id = insertGameHistory(db, id1, id2, "Draw", board);
    ASSERT_GT(game_id, 0);  // Successfully inserted

    // You can call `showGameHistoryForPlayer()` and capture stdout if needed
}
TEST_F(DatabaseTest, InsertAndLoadGameMoves) {
    registerUserGUI(db, "Alice", "pass");
    registerUserGUI(db, "Bob", "pass");
    int user1 = getUserId(db, "Alice");
    int user2 = getUserId(db, "Bob");
    std::string board = "XOXOXOXOX";

    int gameId = insertGameHistory(db, user1, user2, "Draw", board);
    ASSERT_GT(gameId, 0);

    std::vector<Move> moves = {
        {gameId, 0, 'X'}, {gameId, 1, 'O'}, {gameId, 2, 'X'}
    };
    ASSERT_TRUE(insertGameMoves(db, gameId, moves));

    std::vector<Move> loaded;

    ASSERT_TRUE(loadMovesForGameGUI(db, gameId, loaded));

    ASSERT_EQ(loaded.size(), moves.size());
    for (size_t i = 0; i < moves.size(); ++i) {
        EXPECT_EQ(loaded[i].row, moves[i].row) << i << " row mismatch \n";
        EXPECT_EQ(loaded[i].col, moves[i].col) << i << " row mismatch\n";
        EXPECT_EQ(loaded[i].player, moves[i].player) << i << " row mismatch\n";
    }
}

TEST_F(DatabaseTest, AuthenticateWithWrongPassword) {
    std::string username = "user1";
    std::string correctPassword = "123456789";
    std::string wrongPassword = "12345678";

    // Register the user directly in DB (bypassing cin)
    registerUserGUI(db, username, correctPassword);

    // Test with the fake wrapper
    EXPECT_EQ(authenticateUserGUI(db, username, wrongPassword), 2);

}
TEST_F(DatabaseTest, RegisterDuplicateUsername) {
    std::string username = "duplicate";
    std::string password = "12345678";

    EXPECT_EQ(registerUserGUI(db, username, password), 4);
    EXPECT_EQ(registerUserGUI(db, username, password), 1);  // Should fail due to duplicate

}
TEST_F(DatabaseTest, AuthenticateNonExistentUser) {
    ASSERT_TRUE(createTables(db));
    EXPECT_EQ(authenticateUserGUI(db, "ghost", "nopass"), 1);  // User not found

}
TEST_F(DatabaseTest, LoadMovesEmpty) {
    int gameId = insertGameHistory(db, 1, 2, "Draw", "XOXOXOXOX");
    std::vector<Move> moves = loadMovesForGame(db, gameId);
    EXPECT_TRUE(moves.empty());
}
TEST_F(DatabaseTest, GetUserIdNotFound) {
    int userId = getUserId(db, "non_existent_user");
    EXPECT_EQ(userId, -1);
}
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
