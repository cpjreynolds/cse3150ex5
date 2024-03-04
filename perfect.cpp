#include <algorithm>

#include "divisors.hpp"
#include "perfect.hpp"

bool is_perfect(uint32_t n)
{
    if (n < 2) {
        return false;
    }
    const auto divs = divisors(n);

    uint32_t sum = 0;
    std::for_each(divs.cbegin(), divs.cend(), [&sum](auto& i) { sum += i; });

    return sum == n;
}

#ifdef TESTING
#include <iostream>
#include "doctest.h"
#include <set>

TEST_CASE("is_perfect")
{
    SUBCASE("finds_first_4")
    {
        std::set<uint32_t> perfects{6, 28, 496, 8128};
        std::set<uint32_t> candidate;
        for (uint32_t i = 0u; i < 1 << 13; ++i) {
            if (is_perfect(i)) {
                candidate.insert(i);
            }
        }
        CHECK(perfects == candidate);
    }

    SUBCASE("verifies perfect[5]")
    {
        CHECK(is_perfect(33550336));
    }
}

#endif
