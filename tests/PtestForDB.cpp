#define _WIN32_WINNT 0x0A00  // Target Windows 10; adjust as needed
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include <psapi.h>

#include <gtest/gtest.h>
#include <chrono>
#include <iostream>
#include <memory>
#include <vector>
#include "database.h"    // Assumes your database functions and Move struct are declared here.

// Get memory usage (returns in KB)
size_t GetMemoryUsageKB2() {
    PROCESS_MEMORY_COUNTERS memInfo;
    if (GetProcessMemoryInfo(GetCurrentProcess(), &memInfo, sizeof(memInfo))) {
        return memInfo.WorkingSetSize / 1024;
    }
    return 0;
}

TEST(PerformanceTestDatabase, RegisterAndAuthenticatePerformance) {
    using namespace std::chrono;
    
    // Required initializations: Create an in-memory database and enable required settings.
    sqlite3* db = nullptr;
    ASSERT_EQ(sqlite3_open(":memory:", &db), SQLITE_OK);
    ASSERT_TRUE(enableForeignKeys(db));
    ASSERT_TRUE(createTables(db)) << sqlite3_errmsg(db);
    
    // Measure memory before
    size_t memBefore = GetMemoryUsageKB2();
    
    // Measure time
    auto start = high_resolution_clock::now();
    
    // Functionality to test: Register a user and immediately authenticate.
    std::string username = "perfUser";
    std::string password = "perfPass";
    int regResult = registerUserGUI(db, username, password);
    int authResult = authenticateUserGUI(db, username, password);
    
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end - start).count();
    
    // Measure memory after
    size_t memAfter = GetMemoryUsageKB2();
    
    std::cout << "Execution Time: " << duration/1000 << " ms" << std::endl;
    std::cout << "Memory before: " << memBefore << " KB, Memory after: " << memAfter << " KB" << std::endl;
    std::cout << "Memory Used: " << (memAfter - memBefore) << " KB" << std::endl;
    
    // Optional: Assertions
    EXPECT_LT(duration, 1000000);          // Must finish in under 1 sec
    EXPECT_LT(memAfter - memBefore, 52000); // Must use less than 50 MB
    
    sqlite3_close(db);
}

TEST(PerformanceTestDatabase, InsertGameHistoryAndMovesPerformance) {
    using namespace std::chrono;
    
    // Required initializations: Create an in-memory database and enable required settings.
    sqlite3* db = nullptr;
    ASSERT_EQ(sqlite3_open(":memory:", &db), SQLITE_OK);
    ASSERT_TRUE(enableForeignKeys(db));
    ASSERT_TRUE(createTables(db)) << sqlite3_errmsg(db);
    
    // Pre-register two players for game history insertion.
    registerUserGUI(db, "Alice", "pass");
    registerUserGUI(db, "Bob", "pass");
    int id1 = getUserId(db, "Alice");
    int id2 = getUserId(db, "Bob");
    
    // Measure memory before
    size_t memBefore = GetMemoryUsageKB2();
    
    // Measure time
    auto start = high_resolution_clock::now();
    
    // Functionality to test: Insert game history and moves.
    std::string board = "XOXOXOXOX";
    int gameId = insertGameHistory(db, id1, id2, "Draw", board);
    EXPECT_GT(gameId, 0);
    
    std::vector<Move> moves = {
        {0, 0, 'X'},
        {0, 1, 'O'},
        {0, 2, 'X'}
    };
    ASSERT_TRUE(insertGameMoves(db, gameId, moves));
    std::vector<Move> loadedMoves = loadMovesForGame(db, gameId);
    EXPECT_EQ(loadedMoves.size(), moves.size());
    
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end - start).count();
    
    // Measure memory after
    size_t memAfter = GetMemoryUsageKB2();
    
    std::cout << "Execution Time: " << duration/1000 << " ms" << std::endl;
    std::cout << "Memory before: " << memBefore << " KB, Memory after: " << memAfter << " KB" << std::endl;
    std::cout << "Memory Used: " << (memAfter - memBefore) << " KB" << std::endl;
    
    // Optional: Assertions
    EXPECT_LT(duration, 500000);          // Must finish in under 0.5 sec
    EXPECT_LT(memAfter - memBefore, 52000); // Must use less than 50 MB
    
    sqlite3_close(db);
}

// Stress Test: Bulk User Registration
//
TEST(PerformanceTestDatabase, BulkUserRegistration) {
    using namespace std::chrono;
    
    // Required initializations: open an in-memory SQLite DB and set it up.
    sqlite3* db = nullptr;
    ASSERT_EQ(sqlite3_open(":memory:", &db), SQLITE_OK);
    ASSERT_TRUE(enableForeignKeys(db));
    ASSERT_TRUE(createTables(db)) << sqlite3_errmsg(db);
    
    // Number of iterations to simulate heavy load.
    const int iterations = 100;
    
    // Measure memory before
    size_t memBefore = GetMemoryUsageKB2();
    
    // Measure time: capture the start time.
    auto start = high_resolution_clock::now();
    
    // Functionality to test: register users in a loop.
    // Each username is made unique to avoid duplicate errors.
    for (int i = 0; i < iterations; ++i) {
        std::string username = "stress_user_" + std::to_string(i);
        std::string password = "stress_pass";
        registerUserGUI(db, username, password);
        if(!((i+1)%10)){
            std::cout << (i+1)<< " % done \n";
            Sleep(10);
        } 
    }
    
    // Capture end time.
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end - start).count();
    
    // Measure memory after
    size_t memAfter = GetMemoryUsageKB2();
    
    std::cout << "Execution Time: " << duration / 1000 << " ms" << std::endl;
    std::cout << "Memory before: " << memBefore << " KB, Memory after: " << memAfter << " KB" << std::endl;
    std::cout << "Memory Used: " << (memAfter - memBefore) << " KB" << std::endl;
    
    // Optional: Assertions to ensure total operation time is acceptable.
    // For example, you might expect all registrations to finish within 50 seconds.
    EXPECT_LT(duration, 100000000);  // 50,000,000 microseconds = 50 sec
    
    sqlite3_close(db);
}
