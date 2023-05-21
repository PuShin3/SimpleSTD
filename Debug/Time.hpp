#ifndef PULSAR_TIME_INCLUDED
#define PULSAR_TIME_INCLUDED

#include "core.hpp"
#include <chrono>

SSTD_BEGIN

template<typename R>
struct Time {
    R value;
    double asMilli;
    double asSec;

    template<class Functor>
    Time(Functor func) {
        std::chrono::steady_clock::time_point t1, t2;
        t1 = std::chrono::high_resolution_clock::now();
        value = func();
        t2 = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> msm = t2 - t1;
        asMilli = msm.count();
        std::chrono::duration<double, std::centi> msc = t2 - t1;
        asSec = msc.count();
    }
};

template<>
struct Time<void> {
    double asMilli;
    double asSec;

    template<class Functor>
    Time(Functor func) {
        std::chrono::steady_clock::time_point t1, t2;
        t1 = std::chrono::high_resolution_clock::now();
        func();
        t2 = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> msm = t2 - t1;
        asMilli = msm.count();
        std::chrono::duration<double, std::centi> msc = t2 - t1;
        asSec = msc.count();
    }
};

struct Clock {
    struct Result {
        Decimal asMilli;
        Decimal asSec;
    };

    std::chrono::steady_clock::time_point time;

    Clock(): time(std::chrono::high_resolution_clock::now()) {

    }

    Result End() {
        Result ret;
        auto t2 = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> msm = t2 - time;
        ret.asMilli = msm.count();
        std::chrono::duration<double, std::centi> msc = t2 - time;
        ret.asSec = msc.count();
        return ret;
    }
};

struct AverageTime {
    Decimal total, count;
    AverageTime() : total(0), count(0) {

    }

    void Register(Decimal t) {
        total += t;
        ++count;
    }

    SSTD_INLINE SSTD_CONSTEXPR Decimal getAverageTime() const {
        return total / count;
    }
};

SSTD_END


#endif