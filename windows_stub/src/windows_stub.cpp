#ifdef _WIN32
#include "windows_stub.h"

int getrusage(int /*who*/, rusage* usage) {
    if (!usage) return -1;

    FILETIME creation, exit, kernel, user;
    if (!GetProcessTimes(GetCurrentProcess(), &creation, &exit, &kernel, &user)) return -1;

    auto to_timeval = [](const FILETIME& ft, timeval& tv) {
        ULARGE_INTEGER uli;
        uli.LowPart = ft.dwLowDateTime;
        uli.HighPart = ft.dwHighDateTime;
        long micro = static_cast<long>(uli.QuadPart / 10);  // 100ns to µs
        tv.tv_sec = micro / 1'000'000;
        tv.tv_usec = micro % 1'000'000;
    };

    to_timeval(user, usage->ru_utime);
    to_timeval(kernel, usage->ru_stime);

    PROCESS_MEMORY_COUNTERS pmc;
    if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc))) {
        usage->ru_maxrss = static_cast<long>(pmc.PeakWorkingSetSize / 1024);
    } else {
        usage->ru_maxrss = 0;
    }

    return 0;
}
#endif