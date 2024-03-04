#include <iostream>
#include <bit>

#include "perfect.hpp"

#ifndef TESTING

template<typename T>
void print(const T& v)
{
    std::cout << "{";
    for (auto e : v) {
        std::cout << " " << e;
    }
    std::cout << "}\n";
}

int main(int argc, char** argv)
{
    auto c = 0;
    for (auto i = 1u; i < 1 << 14; ++i) {
        if (std::bit_ceil(i) > c) {
            c = std::bit_ceil(i);
            std::cout << c << std::endl;
        }
        if (is_perfect(i)) {
            std::cout << i << " is perfect\n";
        }
    }

    return 0;
}

#else
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#endif
