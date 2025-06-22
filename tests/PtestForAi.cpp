#include <gtest/gtest.h>
#include <chrono>
#include <windows.h>
#include <psapi.h>
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

TEST(PerformanceTest, initAI) {
    using namespace std::chrono;

    std::unique_ptr<Ai> ai_;

    // Measure memory before
    size_t memBefore = GetMemoryUsageKB();

    // Measure time
    auto start = high_resolution_clock::now();

    ai_ = std::make_unique<Ai>(true, hard);


    auto end = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end - start).count();

    // Measure memory after
    size_t memAfter = GetMemoryUsageKB();

    std::cout << "Execution Time: " << duration/1000 << " ms" << std::endl;
    std::cout << "Memory before: " << (memBefore) << " KB , Memory after: " << memAfter << std::endl;
    std::cout << "Memory Used: " << (memAfter - memBefore) << " KB" << std::endl;

    // Optional: Assertions
    EXPECT_LT(duration, 500000);          // Must finish in under 0.5 sec
    EXPECT_LT(memAfter - memBefore, 52000); // Must use less than 50MB
}

TEST(PerformanceTest, deleteAi) {
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

    std::cout << "Execution Time: " << duration/1000 << " ms" << std::endl;
    std::cout << "Memory before: " << (memBefore) << " KB , Memory after: " << memAfter << std::endl;
    std::cout << "Memory Used: " << (memAfter - memBefore) << " KB" << std::endl;

    // Optional: Assertions
    EXPECT_LT(duration, 500000);          // Must finish in under 0.5 sec
    EXPECT_LT(memAfter,memBefore);        // Must be less
}

TEST(PerformanceTest, playMoveAiHard) {
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
    std::cout << "Memory before: " << (memBefore) << " KB , Memory after: " << memAfter << std::endl;
    std::cout << "Memory Used: " << (memAfter - memBefore) << " KB" << std::endl;

    // Optional: Assertions
    EXPECT_LT(duration, 1000);   // Must finish in under 1 msec
    EXPECT_LT(memAfter - memBefore, 10); 
}

TEST(PerformanceTest, playMoveAiNormal) {
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
    std::cout << "Memory before: " << (memBefore) << " KB , Memory after: " << memAfter << std::endl;
    std::cout << "Memory Used: " << (memAfter - memBefore) << " KB" << std::endl;

    // Optional: Assertions
    EXPECT_LT(duration, 1000);   // Must finish in under 1 msec
    EXPECT_LT(memAfter - memBefore, 10); 
}

TEST(PerformanceTest, playMoveAiEasy) {
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
    std::cout << "Memory before: " << (memBefore) << " KB , Memory after: " << memAfter << std::endl;
    std::cout << "Memory Used: " << (memAfter - memBefore) << " KB" << std::endl;

    // Optional: Assertions
    EXPECT_LT(duration, 1000);   // Must finish in under 1 msec
    EXPECT_LT(memAfter - memBefore, 10); 
}

TEST(PerformanceTest, AIcompleteGame) {
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
    std::cout << "Memory before: " << (memBefore) << " KB , Memory after: " << memAfter << std::endl;
    std::cout << "Memory Used: " << (memAfter - memBefore) << " KB" << std::endl;

    // Optional: Assertions
    EXPECT_LT(duration, 10000);   // Must finish in under 10 msec
    EXPECT_LT(memAfter - memBefore, 10); 
}

TEST(PerformanceTest, StressTestHardMode) {
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

TEST(PerformanceTest, StressTestNormslMode) {
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

TEST(PerformanceTest, StressTestEasylMode) {
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