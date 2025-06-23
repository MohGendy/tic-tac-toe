#pragma once

#ifdef _WIN32

#ifndef WIN32_LEAN_AND_MEAN
# define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <psapi.h>

// Define the one and only “who” for getrusage
#define RUSAGE_SELF 0

// Mirror only the fields we care about
struct rusage {
    FILETIME ru_utime;   // user-time
    FILETIME ru_stime;   // system-time
    SIZE_T   ru_maxrss;  // peak working set in bytes
};

// Stub getrusage() in terms of Win32 calls
inline int getrusage(int /*who*/, rusage* usage) {
    if (!usage) return -1;

    // 1) CPU times
    FILETIME createT, exitT, kernelT, userT;
    if (!GetProcessTimes(GetCurrentProcess(),
                         &createT, &exitT,
                         &kernelT, &userT))
        return -1;
    usage->ru_utime = userT;
    usage->ru_stime = kernelT;

    // 2) Peak memory
    PROCESS_MEMORY_COUNTERS pmc = {};
    if (!GetProcessMemoryInfo(GetCurrentProcess(),
                              &pmc, sizeof(pmc)))
        return -1;
    usage->ru_maxrss = pmc.PeakWorkingSetSize;

    return 0;
}

#endif // _WIN32