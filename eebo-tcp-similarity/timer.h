#ifndef TIMER_H
#define TIMER_H

#include <sys/time.h>
#include <iostream>

struct Timer {
    Timer() : start{get_time()}
    {}

    void log(std::string msg) const {
        double now = get_time();
        std::ios_base::fmtflags oldf = std::cout.flags(std::ios::right | std::ios::fixed);
        std::streamsize oldp = std::cout.precision(1);
        std::cout << (now - start) << "\t" << msg << std::endl;
        std::cout.flags(oldf);
        std::cout.precision(oldp);
        std::cout.copyfmt(std::ios(NULL));
    }

private:
    static double get_time() {
        struct timeval tm;
        gettimeofday(&tm, NULL);
        return static_cast<double>(tm.tv_sec) + static_cast<double>(tm.tv_usec) / 1E6;
    }

    const double start;
};

#endif
