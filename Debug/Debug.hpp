#ifndef PULSAR_DEBUG_INCLUDED
#define PULSAR_DEBUG_INCLUDED

#include "core.hpp"
#include <iostream>

SSTD_BEGIN

SSTD_INLINE void print() {
    std::cout << std::endl;
}

template<typename T, typename ...TAIL>
SSTD_INLINE void print(const T& t, TAIL... tail)
{
    std::cout << std::fixed << t << " ";
    print(tail...);
}

SSTD_END

#endif