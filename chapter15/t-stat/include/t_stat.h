#ifndef THE_LINUX_PROGRAMMING_INTERFACE_T_STAT_H
#define THE_LINUX_PROGRAMMING_INTERFACE_T_STAT_H

int tStat(int, char *[]);

#endif //THE_LINUX_PROGRAMMING_INTERFACE_T_STAT_H

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    //define something for Windows (32-bit and 64-bit, this part is common)
    #ifdef _WIN64
          //define something for Windows (64-bit only)
    #else
          //define something for Windows (32-bit only)
    #endif
#elif __APPLE__

    #include <TargetConditionals.h>
    #include <sys/types.h>

    #if TARGET_IPHONE_SIMULATOR
    // iOS Simulator
    #elif TARGET_OS_IPHONE
    // iOS device
    #elif TARGET_OS_MAC
    // Other kinds of Mac OS
    #else
    #   error "Unknown Apple platform"
    #endif
#elif __linux__
    // linux
    #include <sys/sysmacros.h>
#elif __unix__ // all unices not caught above
    // Unix
#elif defined(_POSIX_VERSION)
    // POSIX
#else
#   error "Unknown compiler"
#endif