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

    testing::internal::CaptureStdout();
    ASSERT_EQ(authenticateUserGUI(db, username, password), 3);
    testing::internal::GetCapturedStdout();
}

TEST_F(DatabaseTest, InsertGameHistoryAndRetrieve) {
    std::string player1 = "Alice", player2 = "Bob12";
    std::string pass = "pass123";

    // Register users
    EXPECT_EQ(registerUserGUI(db,player1,pass),4);
    EXPECT_EQ(registerUserGUI(db,player2,pass),4);
    
    int id1 = getUserId(db, player1);
    int id2 = getUserId(db, player2);

    EXPECT_NE(id1, -1);
    EXPECT_NE(id2, -1);

    std::string board = "XOXOXOXOX";
    int game_id = insertGameHistory(db, id1, id2, "Draw", board);
    ASSERT_GT(game_id, 0);  // Successfully inserted

    // You can call `showGameHistoryForPlayer()` and capture stdout if needed
}

TEST_F(DatabaseTest, InsertAndLoadGameMoves) {

    std::string username1 = "yasmeen";
    std::string password1 = "pass1234";
    std::string username2 = "basma";
    std::string password2 = "pass4321";
    EXPECT_EQ(registerUserGUI(db, username1, password1),4)<< "Failed to register username1 \n";
    EXPECT_EQ(registerUserGUI(db, username2, password2),4)<< "Failed to register username2 \n";
    int user1 = getUserId(db, username1);
    int user2 = getUserId(db, username2);
    EXPECT_NE(user1, -1)<< "User1 not found in DB \n";
    EXPECT_NE(user2, -1)<< "User2 not found in DB \n";

    std::string board = "XOXOXOXOX";

    int gameId = insertGameHistory(db, user1, user2, "Draw", board);
    EXPECT_GT(gameId, 0)<<"Failed to insert game history \n";

    std::vector<Move> moves = {
        {0, 0, 'X'}, {0, 1, 'O'}, {0, 2, 'X'}
    };
    EXPECT_TRUE(insertGameMoves(db, gameId, moves))<< "Failed to insert game moves \n";

    std::vector<Move> loaded;

    EXPECT_TRUE(loadMovesForGameGUI(db, gameId, loaded))<< "Failed to load game moves \n";

    EXPECT_EQ(loaded.size(), moves.size())<< "Loaded moves size mismatch \n";
    for (size_t i = 0; i < moves.size(); ++i) {
        EXPECT_EQ(loaded[i].row, moves[i].row) << i << " row mismatch \n";
        EXPECT_EQ(loaded[i].col, moves[i].col) << i << " col mismatch\n";
        EXPECT_EQ(loaded[i].player, moves[i].player) << i << " char mismatch from db: "<< loaded[i].player<<"\n";
    }
}

TEST_F(DatabaseTest, AuthenticateWithWrongPassword) {
    std::string username = "user1";
    std::string correctPassword = "123456789";
    std::string wrongPassword = "12345678";

    // Register the user directly in DB (bypassing cin)
    EXPECT_EQ(registerUserGUI(db, username, correctPassword),4);

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
