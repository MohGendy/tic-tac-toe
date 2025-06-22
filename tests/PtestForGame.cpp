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

TEST(PerformanceTestGame, initAI) {
    using namespace std::chrono;

    // required initializations

    // Measure memory before
    size_t memBefore = GetMemoryUsageKB();

    // Measure time
    auto start = high_resolution_clock::now();

    // functionality to test


    auto end = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end - start).count();

    // Measure memory after
    size_t memAfter = GetMemoryUsageKB();

    std::cout << "Execution Time: " << duration/1000 << " ms" << std::endl;
    std::cout << "Memory before: " << (memBefore) << " KB , Memory after: " << memAfter << " KB" << std::endl;
    std::cout << "Memory Used: " << (memAfter - memBefore) << " KB" << std::endl;

    // Optional: Assertions
    EXPECT_LT(duration, 500000);          // Must finish in under 0.5 sec
    EXPECT_LT(memAfter - memBefore, 52000); // Must use less than 50MB
}
