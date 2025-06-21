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
        ASSERT_TRUE(createTables(db));
    }
   void TearDown() override {
        sqlite3_close(db);
    }
};
TEST_F(DatabaseTest, RegisterAndLoginUser) {
    std::string username = "testuser";
    std::string password = "123456";

    // Mock user input
    testing::internal::CaptureStdout();
    ASSERT_TRUE(registerUserGUI(db,username,password));
    testing::internal::GetCapturedStdout();

    int userId = getUserId(db, username);
    ASSERT_NE(userId, -1);  // User was registered successfully

    std::string loginName = username;
    testing::internal::CaptureStdout();
    ASSERT_TRUE(authenticateUserGUI(db, loginName,password));
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
    int user1 = 1, user2 = 2;
    std::string board = "XOXOXOXOX";

    int gameId = insertGameHistory(db, user1, user2, "Draw", board);
    ASSERT_GT(gameId, 0);

    std::vector<Move> moves = {
        {0, 0, 'X'}, {0, 1, 'O'}, {0, 2, 'X'}
    };
    ASSERT_TRUE(insertGameMoves(db, gameId, moves));

    std::vector<Move> loaded = loadMovesForGame(db, gameId);
    ASSERT_EQ(loaded.size(), moves.size());
    for (size_t i = 0; i < moves.size(); ++i) {
        EXPECT_EQ(loaded[i].row, moves[i].row);
        EXPECT_EQ(loaded[i].col, moves[i].col);
        EXPECT_EQ(loaded[i].player, moves[i].player);
    }
}

TEST_F(DatabaseTest, AuthenticateWithWrongPassword) {
    std::string username = "user1";
    std::string correctPassword = "correct123";
    std::string wrongPassword = "wrong456";

    // Register the user directly in DB (bypassing cin)
    authenticateUserGUI(db, username, correctPassword); // you may need to overload registerUser too

    // Test with the fake wrapper
    bool result = authenticateUserGUI(db, username, wrongPassword);
    EXPECT_FALSE(result);  // Should fail
}
TEST_F(DatabaseTest, RegisterDuplicateUsername) {
    std::string username = "duplicate";
    std::string password = "pass";

    EXPECT_TRUE(registerUserGUI(db, username, password));
    EXPECT_FALSE(registerUserGUI(db, username, password));  // Duplicate registration should fail
}
TEST_F(DatabaseTest, AuthenticateNonExistentUser) {
    EXPECT_FALSE(authenticateUserGUI(db, "ghost", "nopass"));
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


int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
