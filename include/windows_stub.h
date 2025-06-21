#pragma once

#ifdef _WIN32
#include <windows.h>
#include <psapi.h>

// Define timeval if not available
#ifndef _TIMEVAL_DEFINED
#define _TIMEVAL_DEFINED
struct timeval {
    long tv_sec;
    long tv_usec;
};
#endif

// Define rusage struct
#ifndef _RUSAGE_DEFINED
#define _RUSAGE_DEFINED
struct rusage {
    timeval ru_utime;  // user CPU time used
    timeval ru_stime;  // system CPU time used
    long ru_maxrss;    // max resident set size in KB
};
#endif

#define RUSAGE_SELF 0

// Declare getrusage for Windows
int getrusage(int who, rusage* usage);

#else
// For Linux/macOS, use the real POSIX header
#include <sys/resource.h>
#endif