#include <gtest/gtest.h>
#include <chrono>
#include <iostream>
#include <vector>
#include <memory>
#include <functional>
#include <numeric>
#include <iomanip>
//#include <sys/resource.h>
#include<windows.h>
#include <sstream>
#include <streambuf>
#include "ai.h"
#include "windows_stub.h"

class AIPerformanceTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Initialize AI for hard mode
        ai_ = std::make_unique<Ai>(true, hard);
        
        // Create simple test scenarios
        scenarios_ = {
            {},           // Empty board
            {0, 4, 1},    // Few moves
            {0, 1, 4, 2}  // Mid-game
        };
        
        test_start_ = std::chrono::high_resolution_clock::now();
    }
    
    void TearDown() override {
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::high_resolution_clock::now() - test_start_);
        std::cout << "[TOTAL TEST TIME] " << duration.count() << " ms ("
                  << std::fixed << std::setprecision(2) << (duration.count() / 1000.0) << " s)\n";
    }
    
    struct ProfileData {
        double real_ms, user_ms, sys_ms, memory_kb;
    };
    
    ProfileData profile(std::function<void()> op) {
        struct rusage start, end;
        getrusage(RUSAGE_SELF, &start);
        auto t1 = std::chrono::high_resolution_clock::now();
        op();
        auto t2 = std::chrono::high_resolution_clock::now();
        getrusage(RUSAGE_SELF, &end);
        
        return {
            std::chrono::duration<double, std::milli>(t2 - t1).count(),
            (end.ru_utime.tv_sec - start.ru_utime.tv_sec) * 1000.0 +
            (end.ru_utime.tv_usec - start.ru_utime.tv_usec) / 1000.0,
            (end.ru_stime.tv_sec - start.ru_stime.tv_sec) * 1000.0 +
            (end.ru_stime.tv_usec - start.ru_stime.tv_usec) / 1000.0,
            (double)end.ru_maxrss
        };
    }
    
    void printProfile(const ProfileData& p, const std::string& name) {
        std::cout << "[" << name << "] Real: " << std::fixed << std::setprecision(3)
                  << p.real_ms << "ms | CPU: " << (p.user_ms + p.sys_ms)
                  << "ms | Memory: " << (p.memory_kb/1024.0) << "MB | CPU%: "
                  << std::setprecision(1) << ((p.user_ms + p.sys_ms) / p.real_ms * 100.0) << "%\n";
    }
    
    void setupScenario(Ai* ai, const std::vector<int>& moves) {
        bool aiTurn = true;
        for (int move : moves) {
            if (aiTurn) {
                int aiMove;
                // Suppress AI debug output
                std::ostringstream devnull;
                std::streambuf* orig = std::cout.rdbuf();
                std::cout.rdbuf(devnull.rdbuf());
                
                ai->moveAi(&aiMove);
                
                // Restore cout
                std::cout.rdbuf(orig);
            } else {
                // Suppress AI debug output
                std::ostringstream devnull;
                std::streambuf* orig = std::cout.rdbuf();
                std::cout.rdbuf(devnull.rdbuf());
                
                ai->movePlayer(move);
                
                // Restore cout
                std::cout.rdbuf(orig);
            }
            aiTurn = !aiTurn;
        }
    }
    
    // Helper function to execute AI operations silently
    void executeAISilently(std::function<void()> op) {
        std::ostringstream devnull;
        std::streambuf* orig = std::cout.rdbuf();
        std::cout.rdbuf(devnull.rdbuf());
        
        op();
        
        std::cout.rdbuf(orig);
    }
    
    std::unique_ptr<Ai> ai_;
    std::vector<std::vector<int>> scenarios_;
    std::chrono::high_resolution_clock::time_point test_start_;
};

TEST_F(AIPerformanceTest, AllDifficultiesProfile) {
    auto start = std::chrono::high_resolution_clock::now();
    
    std::vector<std::pair<std::string, levels>> modes = {
        {"easy", easy},
        {"normal", normal}, 
        {"hard", hard}
    };
    
    for (const auto& [mode_name, level] : modes) {
        std::vector<double> times;
        std::cout << "\n=== " << mode_name << " Mode ===\n";
        
        for (size_t i = 0; i < scenarios_.size(); ++i) {
            auto p = profile([&]() { 
                executeAISilently([&]() {
                    Ai test_ai(true, level);
                    setupScenario(&test_ai, scenarios_[i]);
                    int move;
                    test_ai.moveAi(&move);
                });
            });
            times.push_back(p.real_ms);
            printProfile(p, "Scenario " + std::to_string(i+1));
        }
        
        double avg = std::accumulate(times.begin(), times.end(), 0.0) / times.size();
        std::cout << "Average: " << std::setprecision(2) << avg << "ms\n";
        
        // Performance assertions
        double limit = (mode_name == "easy") ? 200.0 : (mode_name == "normal") ? 300.0 : 500.0;
        EXPECT_LT(avg, limit) << mode_name << " mode too slow: " << avg << "ms";
    }
    
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::high_resolution_clock::now() - start);
    std::cout << "[TEST TIME] AllDifficultiesProfile: " << duration.count() << "ms\n";
}

TEST_F(AIPerformanceTest, MemoryUsageTest) {
    auto start = std::chrono::high_resolution_clock::now();
    std::cout << "\n=== Memory Usage Test ===\n";
    
    long mem_before = profile([&]() {}).memory_kb;
    
    // Run 50 operations
    for (int i = 0; i < 20; ++i) {
        for (const auto& scenario : scenarios_) {
            executeAISilently([&]() {
                Ai test_ai(true, hard);
                setupScenario(&test_ai, scenario);
                int move;
                test_ai.moveAi(&move);
            });
        }
        
        if ((i + 1) % 5 == 0) {
            std::cout << "Completed " << (i + 1) << "/20 sets\n";
        }
    }
    
    long mem_after = profile([&]() {}).memory_kb;
    long mem_delta = mem_after - mem_before;
    
    std::cout << "Memory before: " << mem_before << " KB\n";
    std::cout << "Memory after: " << mem_after << " KB\n";
    std::cout << "Memory delta: " << mem_delta << " KB\n";
    
    EXPECT_LT(mem_delta, 1024) << "Memory leak detected: " << mem_delta << " KB";
    
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::high_resolution_clock::now() - start);
    std::cout << "[TEST TIME] MemoryUsageTest: " << duration.count() << "ms\n";
}

TEST_F(AIPerformanceTest, StressTest) {
    auto start = std::chrono::high_resolution_clock::now();
    std::cout << "\n=== Stress Test (Hard Mode) ===\n";
    
    const int iterations = 30;
    auto p = profile([&]() {
        for (int i = 0; i < iterations; ++i) {
            for (const auto& scenario : scenarios_) {
                executeAISilently([&]() {
                    Ai test_ai(true, hard);
                    setupScenario(&test_ai, scenario);
                    int move;
                    test_ai.moveAi(&move);
                });
            }
            if ((i + 1) % 10 == 0) {
                std::cout << "Completed " << (i + 1) << "/" << iterations << " iterations\n";
            }
        }
    });
    
    printProfile(p, "Stress Test");
    
    int total_ops = iterations * scenarios_.size();
    double ops_per_sec = total_ops / (p.real_ms / 1000.0);
    double avg_per_op = p.real_ms / total_ops;
    
    std::cout << "Total ops: " << total_ops << " | Ops/sec: " << std::setprecision(1)
              << ops_per_sec << " | Avg/op: " << std::setprecision(3) << avg_per_op << "ms\n";
    
    EXPECT_LT(avg_per_op, 100.0) << "Stress test degraded: " << avg_per_op << "ms/op";
    
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::high_resolution_clock::now() - start);
    std::cout << "[TEST TIME] StressTest: " << duration.count() << "ms\n";
}

int main(int argc, char **argv) {
    auto suite_start = std::chrono::high_resolution_clock::now();
    
    ::testing::InitGoogleTest(&argc, argv);
    
    std::cout << "=== AI Performance Test Suite ===\n";
    std::cout << "CPU/Memory profiling | All difficulty levels | Stress testing\n\n";
    
    int result = RUN_ALL_TESTS();
    
    auto suite_duration = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::high_resolution_clock::now() - suite_start);
    
    std::cout << "\n[SUITE TOTAL TIME] " << suite_duration.count() << " ms ("
              << std::fixed << std::setprecision(2) << (suite_duration.count() / 1000.0) << " s)\n";
    
    return result;
}
