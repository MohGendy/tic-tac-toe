// #include <gtest/gtest.h>
// #include <sqlite3.h>
// #include "database.h"

// class DatabaseTest : public ::testing::Test {
// protected:
//     sqlite3* db;

//     void SetUp() override {
//         // In-memory DB for test
//         ASSERT_EQ(sqlite3_open(":memory:", &db), SQLITE_OK);
//         ASSERT_TRUE(enableForeignKeys(db));
//         ASSERT_TRUE(createTables(db));
//     }
//    void TearDown() override {
//         sqlite3_close(db);
//     }
// };
// TEST_F(DatabaseTest, RegisterAndLoginUser) {
//     std::string username = "testuser";
//     std::string password = "123456";

//     // Mock user input
//     testing::internal::CaptureStdout();
//     ASSERT_TRUE(registerUser(db));
//     testing::internal::GetCapturedStdout();

//     int userId = getUserId(db, username);
//     ASSERT_NE(userId, -1);  // User was registered successfully

//     std::string loginName = username;
//     testing::internal::CaptureStdout();
//     ASSERT_TRUE(authenticateUser(db, loginName));
//     testing::internal::GetCapturedStdout();
// }
// TEST_F(DatabaseTest, InsertGameHistoryAndRetrieve) {
//     std::string player1 = "Alice", player2 = "Bob";
//     std::string pass = "pass";

//     // Register users
//     EXPECT_TRUE(registerUser(db));
//     EXPECT_TRUE(registerUser(db));
    
//     int id1 = getUserId(db, player1);
//     int id2 = getUserId(db, player2);

//     std::string board = "XOXOXOXOX";
//     int game_id = insertGameHistory(db, id1, id2, "Draw", board);
//     ASSERT_GT(game_id, 0);  // Successfully inserted

//     // You can call `showGameHistoryForPlayer()` and capture stdout if needed
// }
// TEST_F(DatabaseTest, InsertAndLoadGameMoves) {
//     int user1 = 1, user2 = 2;
//     std::string board = "XOXOXOXOX";

//     int gameId = insertGameHistory(db, user1, user2, "Draw", board);
//     ASSERT_GT(gameId, 0);

//     std::vector<Move> moves = {
//         {0, 0, 'X'}, {0, 1, 'O'}, {0, 2, 'X'}
//     };
//     ASSERT_TRUE(insertGameMoves(db, gameId, moves));

//     std::vector<Move> loaded = loadMovesForGame(db, gameId);
//     ASSERT_EQ(loaded.size(), moves.size());
//     for (size_t i = 0; i < moves.size(); ++i) {
//         EXPECT_EQ(loaded[i].row, moves[i].row);
//         EXPECT_EQ(loaded[i].col, moves[i].col);
//         EXPECT_EQ(loaded[i].player, moves[i].player);
//     }
// }
// TEST_F(DatabaseTest, AuthenticateWithWrongPassword) {
//     std::string username = "user1";
//     std::string correctPassword = "correct123";
//     std::string wrongPassword = "wrong456";

    
//     testing::internal::CaptureStdout();
//     EXPECT_TRUE(registerUser(db));  
//     testing::internal::GetCapturedStdout();

//     // Step 2: Manually try to authenticate with wrong password
//     std::string inputUsername = username;

//     // Temporarily replace standard input with wrong password
//     // Here we assume authenticateUser asks for input from cin (this won't work in real automation without redirect)
//     // So for actual automated test, you should refactor authenticateUser to accept parameters.

//     // This example assumes you've refactored authenticateUser:
//     // bool authenticateUser(sqlite3* db, const std::string& username, const std::string& inputPassword);

//     bool result = authenticateUser(db, inputUsername, wrongPassword);  // Custom overload needed
//     EXPECT_FALSE(result);  // Should fail
// }
