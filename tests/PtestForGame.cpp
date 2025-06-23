#include <gtest/gtest.h>
#include <chrono>
#include <iostream>
#include <vector>
#include <memory>
#include <functional>
#include <numeric>
#include <iomanip>
#include <sstream>
#include <streambuf>

#ifdef _WIN32
#  include "windows_stub.h"
#else
#  include <sys/resource.h>
#endif

#include "gamestructure.h"

class GameStructurePerformanceTest : public ::testing::Test {
protected:
    void SetUp() override {
        test_start_ = std::chrono::high_resolution_clock::now();
    }

    void TearDown() override {
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::high_resolution_clock::now() - test_start_);
        std::cout << "[TOTAL TEST TIME] " << duration.count() << " ms ("
                  << std::fixed << std::setprecision(6) << (duration.count() / 1000.0) << " s)\n";
    }

    struct ProfileData {
        double real_ms, user_ms, sys_ms, memory_kb;
    };

ProfileData profile(std::function<void()> op) {
    rusage start{}, end{};
    getrusage(RUSAGE_SELF, &start);

    auto t1 = std::chrono::high_resolution_clock::now();
    op();
    auto t2 = std::chrono::high_resolution_clock::now();

    getrusage(RUSAGE_SELF, &end);

    // real elapsed (ms)
    double real_ms = std::chrono::duration<double, std::milli>(t2 - t1).count();

#ifdef _WIN32
    // helper to go from FILETIME (100-ns units) to milliseconds
    auto ftToMs = [](const FILETIME &ft) {
        ULARGE_INTEGER li;
        li.LowPart  = ft.dwLowDateTime;
        li.HighPart = ft.dwHighDateTime;
        // 1 count = 100 ns => /10 000 = ms
        return double(li.QuadPart) / 10000.0;
    };

    double user_ms   = ftToMs(end.ru_utime) - ftToMs(start.ru_utime);
    double sys_ms    = ftToMs(end.ru_stime) - ftToMs(start.ru_stime);
    double memory_kb = double(end.ru_maxrss) / 1024.0;

#else
    double user_ms   = (end.ru_utime.tv_sec  - start.ru_utime.tv_sec ) * 1000.0
                     + (end.ru_utime.tv_usec - start.ru_utime.tv_usec) / 1000.0;
    double sys_ms    = (end.ru_stime.tv_sec  - start.ru_stime.tv_sec ) * 1000.0
                     + (end.ru_stime.tv_usec - start.ru_stime.tv_usec) / 1000.0;
    double memory_kb = double(end.ru_maxrss);  // already in KB on POSIX

#endif

    return ProfileData{ real_ms, user_ms, sys_ms, memory_kb };
}
    void printProfile(const ProfileData& p, const std::string& name) {
        double cpu_percent = (p.real_ms > 0) ? ((p.user_ms + p.sys_ms) / p.real_ms * 100.0) : 0.0;
        std::cout << "[" << name << "] Real: " << std::fixed << std::setprecision(6)
                  << p.real_ms << "ms | CPU: " << (p.user_ms + p.sys_ms)
                  << "ms | Memory: " << (p.memory_kb/1024.0) << "MB | CPU%: "
                  << std::setprecision(6) << cpu_percent << "%\n";
    }

    std::chrono::high_resolution_clock::time_point test_start_;
};

TEST_F(GameStructurePerformanceTest, BoardOperationsProfile) {
    auto start = std::chrono::high_resolution_clock::now();
    std::cout << "\n=== Board Operations Performance ===\n";
    
    std::vector<double> init_times, move_times, win_check_times, display_times;
    const int iterations = 1000;
    
    for (int i = 0; i < iterations; ++i) {
        auto p1 = profile([&]() { Board board; });
        init_times.push_back(p1.real_ms);
        
        Board board;
        std::vector<std::pair<int, int>> moves = {{0,0}, {0,1}, {1,1}, {2,2}, {1,0}};
        for (auto move : moves) {
            auto p2 = profile([&]() { board.makeMove(move.first, move.second, 'X'); });
            move_times.push_back(p2.real_ms);
        }
        
        auto p3 = profile([&]() { board.checkWin('X'); board.checkWin('O'); });
        win_check_times.push_back(p3.real_ms);
        
        auto p4 = profile([&]() {
            std::streambuf* orig = std::cout.rdbuf();
            std::ostringstream capture;
            std::cout.rdbuf(capture.rdbuf());
            board.displayBoard();
            std::cout.rdbuf(orig);
        });
        display_times.push_back(p4.real_ms);
        
        if ((i + 1) % 200 == 0) {
            std::cout << "Completed " << (i + 1) << "/" << iterations << " cycles\n";
        }
    }
    
    double avg_init = std::accumulate(init_times.begin(), init_times.end(), 0.0) / init_times.size();
    double avg_move = std::accumulate(move_times.begin(), move_times.end(), 0.0) / move_times.size();
    double avg_win_check = std::accumulate(win_check_times.begin(), win_check_times.end(), 0.0) / win_check_times.size();
    double avg_display = std::accumulate(display_times.begin(), display_times.end(), 0.0) / display_times.size();
    
    std::cout << "Average Init: " << std::fixed << std::setprecision(6) << avg_init << "ms | "
              << "Move: " << avg_move << "ms | Win Check: " << avg_win_check 
              << "ms | Display: " << avg_display << "ms\n";
    
    EXPECT_LT(avg_init, 0.1) << "Board initialization too slow: " << avg_init << "ms";
    EXPECT_LT(avg_move, 0.05) << "Move making too slow: " << avg_move << "ms";
    EXPECT_LT(avg_win_check, 0.02) << "Win checking too slow: " << avg_win_check << "ms";
    EXPECT_LT(avg_display, 1.0) << "Board display too slow: " << avg_display << "ms";
    
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::high_resolution_clock::now() - start);
    std::cout << "[TEST TIME] BoardOperationsProfile: " << duration.count() << "ms\n";
}

TEST_F(GameStructurePerformanceTest, MegaBoardOperationsProfile) {
    auto start = std::chrono::high_resolution_clock::now();
    std::cout << "\n=== MegaBoard Operations Performance ===\n";
    
    std::vector<double> init_times, move_times, subgrid_win_times, mega_win_times;
    const int iterations = 500;
    
    for (int i = 0; i < iterations; ++i) {
        auto p1 = profile([&]() { megaBoard megaboard; });
        init_times.push_back(p1.real_ms);
        
        megaBoard megaboard;
        std::vector<std::tuple<int, int, int>> mega_moves = {
            {0,0,0}, {1,1,4}, {2,2,8}, {0,1,1}, {1,0,3}, {2,1,7}
        };
        
        for (auto move : mega_moves) {
            auto p2 = profile([&]() {
                megaboard.megaMakeMove(std::get<0>(move), std::get<1>(move), 
                                     std::get<2>(move), 'X');
            });
            move_times.push_back(p2.real_ms);
            
            auto p3 = profile([&]() {
                megaboard.checkSubgridWin(std::get<2>(move), 'X');
            });
            subgrid_win_times.push_back(p3.real_ms);
        }
        
        auto p4 = profile([&]() {
            megaboard.megaCheckWin('X');
            megaboard.megaCheckWin('O');
        });
        mega_win_times.push_back(p4.real_ms);
        
        if ((i + 1) % 100 == 0) {
            std::cout << "Completed " << (i + 1) << "/" << iterations << " cycles\n";
        }
    }
    
    double avg_init = std::accumulate(init_times.begin(), init_times.end(), 0.0) / init_times.size();
    double avg_move = std::accumulate(move_times.begin(), move_times.end(), 0.0) / move_times.size();
    double avg_subgrid_win = std::accumulate(subgrid_win_times.begin(), subgrid_win_times.end(), 0.0) / subgrid_win_times.size();
    double avg_mega_win = std::accumulate(mega_win_times.begin(), mega_win_times.end(), 0.0) / mega_win_times.size();
    
    std::cout << "Average Init: " << std::fixed << std::setprecision(6) << avg_init << "ms | "
              << "Move: " << avg_move << "ms | Subgrid Win: " << avg_subgrid_win 
              << "ms | Mega Win: " << avg_mega_win << "ms\n";
    
    EXPECT_LT(avg_init, 0.5) << "MegaBoard initialization too slow: " << avg_init << "ms";
    EXPECT_LT(avg_move, 0.1) << "Mega move making too slow: " << avg_move << "ms";
    EXPECT_LT(avg_subgrid_win, 0.05) << "Subgrid win checking too slow: " << avg_subgrid_win << "ms";
    EXPECT_LT(avg_mega_win, 0.02) << "Mega win checking too slow: " << avg_mega_win << "ms";
    
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::high_resolution_clock::now() - start);
    std::cout << "[TEST TIME] MegaBoardOperationsProfile: " << duration.count() << "ms\n";
}

TEST_F(GameStructurePerformanceTest, GameSimulation) {
    auto start = std::chrono::high_resolution_clock::now();
    std::cout << "\n=== Game Simulation Performance ===\n";
    
    const int num_games = 100;
    std::vector<double> game_times;
    
    std::vector<std::vector<std::pair<int, int>>> patterns = {
        {{0,0}, {0,1}, {1,0}, {1,1}, {2,0}},
        {{0,0}, {1,1}, {0,1}, {1,0}, {0,2}, {1,2}},
        {{0,0}, {1,1}, {0,1}, {0,2}, {1,0}, {2,0}, {1,2}, {2,1}, {2,2}},
        {{1,1}, {0,0}, {0,2}, {2,0}, {2,1}},
    };
    
    for (int game = 0; game < num_games; ++game) {
        const auto& pattern = patterns[game % patterns.size()];
        
        auto p = profile([&]() {
            Board board;
            char current_player = 'X';
            
            for (auto move : pattern) {
                if (board.makeMove(move.first, move.second, current_player)) {
                    if (board.checkWin(current_player) || board.isFull()) {
                        break;
                    }
                    current_player = (current_player == 'X') ? 'O' : 'X';
                }
            }
        });
        
        game_times.push_back(p.real_ms);
        
        if ((game + 1) % 20 == 0) {
            std::cout << "Completed " << (game + 1) << "/" << num_games << " games\n";
        }
    }
    
    double avg_game = std::accumulate(game_times.begin(), game_times.end(), 0.0) / game_times.size();
    double games_per_sec = num_games / (std::accumulate(game_times.begin(), game_times.end(), 0.0) / 1000.0);
    
    std::cout << "Average Game: " << std::fixed << std::setprecision(6) << avg_game << "ms | "
              << "Games/sec: " << std::setprecision(6) << games_per_sec << "\n";
    
    EXPECT_LT(avg_game, 2.0) << "Game simulation too slow: " << avg_game << "ms";
    
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::high_resolution_clock::now() - start);
    std::cout << "[TEST TIME] GameSimulation: " << duration.count() << "ms\n";
}

TEST_F(GameStructurePerformanceTest, StressTest) {
    auto start = std::chrono::high_resolution_clock::now();
    std::cout << "\n=== Stress Test ===\n";
    
    const int iterations = 200;
    
    auto p = profile([&]() {
        for (int i = 0; i < iterations; ++i) {
            Board board;
            for (int pos = 0; pos < 9; ++pos) {
                int row = pos / 3, col = pos % 3;
                char player = (pos % 2 == 0) ? 'X' : 'O';
                board.makeMove(row, col, player);
                board.checkWin(player);
                board.isFull();
            }
            
            megaBoard megaboard;
            for (int subgrid = 0; subgrid < 9; ++subgrid) {
                for (int pos = 0; pos < 9; ++pos) {
                    int row = pos / 3, col = pos % 3;
                    char player = (pos % 2 == 0) ? 'X' : 'O';
                    if (megaboard.megaMakeMove(row, col, subgrid, player)) {
                        megaboard.checkSubgridWin(subgrid, player);
                        megaboard.megaCheckWin(player);
                        if (megaboard.isGridwon(subgrid)) break;
                    }
                }
            }
            
            if ((i + 1) % 40 == 0) {
                std::cout << "Completed " << (i + 1) << "/" << iterations << " stress cycles\n";
            }
        }
    });
    
    printProfile(p, "Stress Test");
    
    double avg_per_cycle = p.real_ms / iterations;
    double cycles_per_sec = iterations / (p.real_ms / 1000.0);
    
    std::cout << "Cycles: " << iterations << " | Cycles/sec: " << std::fixed << std::setprecision(6)
              << cycles_per_sec << " | Avg/cycle: " << std::setprecision(6) << avg_per_cycle << "ms\n";
    
    EXPECT_LT(avg_per_cycle, 15.0) << "Stress test degraded: " << avg_per_cycle << "ms/cycle";
    
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::high_resolution_clock::now() - start);
    std::cout << "[TEST TIME] StressTest: " << duration.count() << "ms\n";
}

// int main(int argc, char **argv) {
//     auto suite_start = std::chrono::high_resolution_clock::now();
//     ::testing::InitGoogleTest(&argc, argv);
    
//     std::cout << "=== Game Structure Performance Test Suite ===\n";
//     std::cout << "Testing Board, MegaBoard, and Game operations\n\n";
    
//     int result = RUN_ALL_TESTS();
    
//     auto suite_duration = std::chrono::duration_cast<std::chrono::milliseconds>(
//         std::chrono::high_resolution_clock::now() - suite_start);
//     std::cout << "\n[SUITE TOTAL TIME] " << suite_duration.count() << " ms ("
//               << std::fixed << std::setprecision(6) << (suite_duration.count() / 1000.0) << " s)\n";
    
//     return result;
// }
