#ifdef _WIN32
#include <windows.h>
#include <psapi.h>

// Define a minimal timeval structure
// struct timeval {
//     long tv_sec;
//     long tv_usec;
// };

// Define a minimal rusage structure
struct rusage {
    timeval ru_utime;  // user CPU time used
    timeval ru_stime;  // system CPU time used
    long ru_maxrss;    // maximum resident set size
    // (Other fields from the POSIX rusage are omitted for simplicity)
};

struct ProfileData {
    double real_ms;
    double user_ms;
    double sys_ms;
    double memory_kb;
};

// Provide a getrusage implementation using Windows APIs.
int getrusage(int /*who*/, rusage* usage) {
    if (!usage)
        return -1;
    FILETIME creationTime, exitTime, kernelTime, userTime;
    if (!GetProcessTimes(GetCurrentProcess(), &creationTime, &exitTime, &kernelTime, &userTime))
        return -1;
    
    auto fileTimeToTimeval = [](const FILETIME &ft, timeval &tv) {
        ULARGE_INTEGER uli;
        uli.LowPart = ft.dwLowDateTime;
        uli.HighPart = ft.dwHighDateTime;
        // Convert the 100-nanosecond intervals to microseconds:
        long total_microseconds = static_cast<long>(uli.QuadPart / 10);
        tv.tv_sec = total_microseconds / 1000000;
        tv.tv_usec = total_microseconds % 1000000;
    };

    fileTimeToTimeval(userTime, usage->ru_utime);
    fileTimeToTimeval(kernelTime, usage->ru_stime);

    // Query peak working set size; ru_maxrss is in kilobytes
    PROCESS_MEMORY_COUNTERS pmc;
    if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc)))
        usage->ru_maxrss = static_cast<long>(pmc.PeakWorkingSetSize / 1024);
    else
        usage->ru_maxrss = 0;
    
    return 0;
}

// Define RUSAGE_SELF for compatibility
#define RUSAGE_SELF 0
#else
#include <sys/resource.h>
#endif