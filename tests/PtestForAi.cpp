#define _WIN32_WINNT 0x0A00  // Target Windows 10; adjust as needed
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include <psapi.h>

#include <gtest/gtest.h>
#include <chrono>
#include <iostream>
#include <memory>
#include "ai.h"



// Get memory usage (returns in KB)
size_t GetMemoryUsageKB() {
    PROCESS_MEMORY_COUNTERS memInfo;
    if (GetProcessMemoryInfo(GetCurrentProcess(), &memInfo, sizeof(memInfo))) {
        return memInfo.WorkingSetSize / 1024;
    }
    return 0;
}

TEST(PerformanceTestAi, initAI_Average) {
    using namespace std::chrono;
    
    const int iterations = 10;  // Number of iterations for accurate measurements
    long long totalDuration = 0;    // To accumulate execution times in microseconds
    size_t totalMemoryDelta = 0;      // To accumulate memory usage differences
    
    // Warm-up run (optional): Ensure any one-time overhead doesn't skew results.
    {
        Ai* warmup = new Ai(true, hard);
        delete warmup;
    }

    for (int i = 0; i < iterations; ++i) {
        // Measure memory before initialization
        size_t memBefore = GetMemoryUsageKB();
        
        // Measure time: record start
        auto start = high_resolution_clock::now();
        
        // Functionality under test: initialize AI
        Ai* ai_ = new Ai(true, hard);
        
        // Record end time and calculate duration
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(end - start).count();
        
        // Measure memory after initialization
        size_t memAfter = GetMemoryUsageKB();
        
        // Update accumulators
        totalDuration += duration;
        totalMemoryDelta += (memAfter - memBefore);
        
        delete ai_;
    }
    
    // Calculate averages
    double avgDuration = totalDuration / static_cast<double>(iterations);
    double avgMemoryUsage = totalMemoryDelta / static_cast<double>(iterations);
    
    std::cout << "Average Execution Time: " << avgDuration / 1000.0 << " ms" << std::endl;
    std::cout << "Average Memory Used: " << avgMemoryUsage << " KB" << std::endl;
    
    // Optional Assertions: The thresholds reflect the averages over iterations.
    EXPECT_LT(avgDuration, 500000);          // Average must finish in under 0.5 sec (500,000 usec)
    EXPECT_LT(avgMemoryUsage, 60000);          // Average must use less than 60 MB
}

TEST(PerformanceTestAi, deleteAi) {
    using namespace std::chrono;

    Ai* ai_ = new Ai(true, hard);;

    // Measure memory before
    size_t memBefore = GetMemoryUsageKB();

    // Measure time
    auto start = high_resolution_clock::now();

    delete ai_;


    auto end = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end - start).count();

    // Measure memory after
    size_t memAfter = GetMemoryUsageKB();

    std::cout << "Execution Time: " << duration/1000.0 << " ms" << std::endl;
    std::cout << "Memory before: " << (memBefore) << " KB , Memory after: " << memAfter << " KB" << std::endl;
    std::cout << "Memory Used: " << (memAfter - memBefore) << " KB" << std::endl;

    // Optional: Assertions
    EXPECT_LT(duration, 500000);          // Must finish in under 0.5 sec
    EXPECT_LT(memAfter,memBefore);        // Must be less
}

TEST(PerformanceTestAi, playMoveAiHard) {
    using namespace std::chrono;

    std::unique_ptr<Ai> ai_;
    ai_ = std::make_unique<Ai>(false, hard);
    int move ;
    // Measure memory before
    size_t memBefore = GetMemoryUsageKB();

    // Measure time
    auto start = high_resolution_clock::now();

    ai_->movePlayer(5);
    ai_->moveAi(&move);

    auto end = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end - start).count();

    // Measure memory after
    size_t memAfter = GetMemoryUsageKB();

    std::cout << "Execution Time: " << duration << " usec" << std::endl;
    std::cout << "Memory before: " << (memBefore) << " KB , Memory after: " << memAfter << " KB" << std::endl;
    std::cout << "Memory Used: " << (memAfter - memBefore) << " KB" << std::endl;

    // Optional: Assertions
    EXPECT_LT(duration, 1000);   // Must finish in under 1 msec
    EXPECT_LT(memAfter - memBefore, 10); 
}

TEST(PerformanceTestAi, playMoveAiNormal) {
    using namespace std::chrono;

    std::unique_ptr<Ai> ai_;
    ai_ = std::make_unique<Ai>(false,normal);
    int move ;
    // Measure memory before
    size_t memBefore = GetMemoryUsageKB();

    // Measure time
    auto start = high_resolution_clock::now();

    ai_->movePlayer(5);
    ai_->moveAi(&move);

    auto end = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end - start).count();

    // Measure memory after
    size_t memAfter = GetMemoryUsageKB();

    std::cout << "Execution Time: " << duration << " usec" << std::endl;
    std::cout << "Memory before: " << (memBefore) << " KB , Memory after: " << memAfter << " KB" << std::endl;
    std::cout << "Memory Used: " << (memAfter - memBefore) << " KB" << std::endl;

    // Optional: Assertions
    EXPECT_LT(duration, 1000);   // Must finish in under 1 msec
    EXPECT_LT(memAfter - memBefore, 10); 
}

TEST(PerformanceTestAi, playMoveAiEasy) {
    using namespace std::chrono;

    std::unique_ptr<Ai> ai_;
    ai_ = std::make_unique<Ai>(false,easy);
    int move ;
    // Measure memory before
    size_t memBefore = GetMemoryUsageKB();

    // Measure time
    auto start = high_resolution_clock::now();

    ai_->movePlayer(5);
    ai_->moveAi(&move);

    auto end = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end - start).count();

    // Measure memory after
    size_t memAfter = GetMemoryUsageKB();

    std::cout << "Execution Time: " << duration << " usec" << std::endl;
    std::cout << "Memory before: " << (memBefore) << " KB , Memory after: " << memAfter << " KB" << std::endl;
    std::cout << "Memory Used: " << (memAfter - memBefore) << " KB" << std::endl;

    // Optional: Assertions
    EXPECT_LT(duration, 1000);   // Must finish in under 1 msec
    EXPECT_LT(memAfter - memBefore, 10); 
}

TEST(PerformanceTestAi, AIcompleteGame) {
    using namespace std::chrono;

    std::unique_ptr<Ai> ai1_;
    std::unique_ptr<Ai> ai2_;
    ai1_ = std::make_unique<Ai>(true,hard);
    ai2_ = std::make_unique<Ai>(false,hard);
    int move ;
    // Measure memory before
    size_t memBefore = GetMemoryUsageKB();

    // Measure time
    auto start = high_resolution_clock::now();

    for(int i = 0;i<9;i++){
        if(i%2){
            ai1_->moveAi(&move);
            ai2_->movePlayer(move);
        }else{
            ai2_->moveAi(&move);
            ai1_->movePlayer(move);
        }
    }

    auto end = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end - start).count();

    // Measure memory after
    size_t memAfter = GetMemoryUsageKB();

    std::cout << "Execution Time: " << duration << " usec" << std::endl;
    std::cout << "Memory before: " << (memBefore) << " KB , Memory after: " << memAfter << " KB" << std::endl;
    std::cout << "Memory Used: " << (memAfter - memBefore) << " KB" << std::endl;

    // Optional: Assertions
    EXPECT_LT(duration, 10000);   // Must finish in under 10 msec
    EXPECT_LT(memAfter - memBefore, 10); 
}

TEST(PerformanceTestAi, StressTestHardMode) {
    using namespace std::chrono;

    std::unique_ptr<Ai> ai1_;
    std::unique_ptr<Ai> ai2_;
    ai1_ = std::make_unique<Ai>(true,hard);
    ai2_ = std::make_unique<Ai>(false,hard);
    int move;
    // Measure memory before
    int sum=0;
    size_t memBefore = GetMemoryUsageKB();
    for(int i = 0 ; i < 10000 ; i++){
        auto start = high_resolution_clock::now();

        for(int i = 0;i<9;i++){
            if(i%2){
                ai1_->moveAi(&move);
                ai2_->movePlayer(move);
            }else{
                ai2_->moveAi(&move);
                ai1_->movePlayer(move);
            }
        }

        auto end = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(end - start).count();
        sum+=duration;
        ai1_->reset();
        ai2_->reset();
        if(!((i+1)%1000)){
            std::cout << (i+1)/100<< " % done \n";
            Sleep(10);
        } 
            
    }

    // Measure memory after
    size_t memAfter = GetMemoryUsageKB();

    std::cout << "Execution Time: " << sum << " usec , Avg Time per run:" << sum/10000.0 << " usec" << std::endl;
    std::cout << "Memory before: " << (memBefore) << " KB , Memory after: " << memAfter << " KB" << std::endl;
    std::cout << "Memory Used: " << (memAfter - memBefore) << " KB" << std::endl;

    // Optional: Assertions
    EXPECT_LT(sum, 100000);   // Must finish in under 100 msec
    EXPECT_LT(memAfter - memBefore, 50); 
}

TEST(PerformanceTestAi, StressTestNormslMode) {
    using namespace std::chrono;

    std::unique_ptr<Ai> ai1_;
    std::unique_ptr<Ai> ai2_;
    ai1_ = std::make_unique<Ai>(true,normal);
    ai2_ = std::make_unique<Ai>(false,normal);
    int move;
    // Measure memory before
    int sum=0;
    size_t memBefore = GetMemoryUsageKB();
    for(int i = 0 ; i < 10000 ; i++){
        auto start = high_resolution_clock::now();

        for(int i = 0;i<5;i++){ //play 5 moves
            if(i%2){
                ai1_->moveAi(&move);
                ai2_->movePlayer(move);
            }else{
                ai2_->moveAi(&move);
                ai1_->movePlayer(move);
            }
        }

        auto end = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(end - start).count();
        sum+=duration;
        ai1_->reset();
        ai2_->reset();
        if(!((i+1)%1000)){
            std::cout << (i+1)/100<< " % done \n";
            Sleep(10);
        } 
            
    }

    // Measure memory after
    size_t memAfter = GetMemoryUsageKB();

    std::cout << "Execution Time: " << sum << " usec , Avg Time per run:" << sum/10000.0 << " usec" << std::endl;
    std::cout << "Memory before: " << (memBefore) << " KB , Memory after: " << memAfter << " KB" << std::endl;
    std::cout << "Memory Used: " << (memAfter - memBefore) << " KB" << std::endl;

    // Optional: Assertions
    EXPECT_LT(sum, 100000);   // Must finish in under 100 msec
    EXPECT_LT(memAfter - memBefore, 50); 
}

TEST(PerformanceTestAi, StressTestEasylMode) {
    using namespace std::chrono;

    std::unique_ptr<Ai> ai1_;
    std::unique_ptr<Ai> ai2_;
    ai1_ = std::make_unique<Ai>(true,easy);
    ai2_ = std::make_unique<Ai>(false,easy);
    int move;
    // Measure memory before
    int sum=0;
    size_t memBefore = GetMemoryUsageKB();
    for(int i = 0 ; i < 10000 ; i++){
        auto start = high_resolution_clock::now();

        for(int i = 0;i<5;i++){ //play 5 moves
            if(i%2){
                ai1_->moveAi(&move);
                ai2_->movePlayer(move);
            }else{
                ai2_->moveAi(&move);
                ai1_->movePlayer(move);
            }
        }

        auto end = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(end - start).count();
        sum+=duration;
        ai1_->reset();
        ai2_->reset();
        if(!((i+1)%1000)){
            std::cout << (i+1)/100<< " % done \n";
            Sleep(10);
        } 
            
    }

    // Measure memory after
    size_t memAfter = GetMemoryUsageKB();

    std::cout << "Execution Time: " << sum << " usec , Avg Time per run:" << sum/10000.0 << " usec" << std::endl;
    std::cout << "Memory before: " << (memBefore) << " KB , Memory after: " << memAfter << " KB" << std::endl;
    std::cout << "Memory Used: " << (memAfter - memBefore) << " KB" << std::endl;

    // Optional: Assertions
    EXPECT_LT(sum, 100000);   // Must finish in under 100 msec
    EXPECT_LT(memAfter - memBefore, 50); 
}