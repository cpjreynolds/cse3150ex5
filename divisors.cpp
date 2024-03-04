#include "wheel.hpp"
#include <cmath>
#include <numeric>
#include <functional>
#include "divisors.hpp"

// returns a cached set of at least all primes smaller than `n`.
//
// The set is cached to its largest extent, so the set may contain primes
// greater than `n`.
// It is only guaranteed that all primes smaller than `n` are contained in the
// set.
const std::set<uint32_t>& primes(uint32_t n)
{
    static thread_local wheel whl(2);
    static thread_local std::set<uint32_t> pcache = whl.primes();

    if (whl.prime_limit() < n) {
        while (whl.prime_limit() < n) {
            whl.advance();
        }
        pcache = whl.primes();
    }
    return pcache;
}

// returns the set of (unique) subsets.
static std::set<std::multiset<uint32_t>> powerset(std::multiset<uint32_t> s)
{
    std::set<std::multiset<uint32_t>> pset;
    auto setsz = s.size();
    uint32_t psetsz = std::pow(2, setsz);

    for (auto i = 0u; i < psetsz; ++i) {
        std::multiset<uint32_t> curr;
        auto it = s.cbegin();
        for (auto j = 0u; j < setsz; ++j, ++it) {
            if (i & (1 << j)) {
                curr.insert(*it);
            }
        }
        pset.insert(std::move(curr));
    }
    return pset;
}

std::set<uint32_t> divisors(uint32_t n)
{
    if (n == 0) {
        return {};
    }
    else if (n == 1) {
        return {1};
    }
    std::multiset<uint32_t> pfacts; // prime factors < sqrt(n)
    uint32_t sq = std::ceil(std::sqrt(n));
    auto ps = primes(sq);
    // find multiplicities of primes less than sqrt(n)
    for (const auto& pf : ps) {
        if (pf > sq) {
            // necessary because cached prime list may be much longer
            break;
        }
        if (n % pf == 0) {
            // found a prime factor, insert it `multiplicity` times.
            for (auto r = n; r % pf == 0; r /= pf) {
                pfacts.insert(pf);
            }
        }
    }
    // generate the other factors
    auto pset = powerset(pfacts);
    std::set<uint32_t> divs{1};
    for (const auto& fs : pset) {
        uint32_t d = std::reduce(fs.cbegin(), fs.cend(), 1u,
                                 std::multiplies<uint32_t>());
        divs.insert(d);
        divs.insert(n / d);
    }
    divs.erase(n);
    return divs;
}

#ifdef TESTING
#include "doctest.h"

TEST_CASE("divisors")
{
    // NOTE: large divisor sets were taken from Mathematica.

    SUBCASE("zero")
    {
        CHECK(divisors(0) == std::set<uint32_t>{});
    }

    SUBCASE("one")
    {
        CHECK(divisors(1) == std::set<uint32_t>{1});
    }

    SUBCASE("two")
    {
        CHECK(divisors(2) == std::set<uint32_t>{1});
    }

    SUBCASE("three")
    {
        CHECK(divisors(3) == std::set<uint32_t>{1});
    }

    SUBCASE("four")
    {
        CHECK(divisors(4) == std::set<uint32_t>{1, 2});
    }

    SUBCASE("nine")
    {
        CHECK(divisors(9) == std::set<uint32_t>{1, 3});
    }

    SUBCASE("16")
    {
        CHECK(divisors(16) == std::set<uint32_t>{1, 2, 4, 8});
    }

    SUBCASE("25")
    {
        CHECK(divisors(25) == std::set<uint32_t>{1, 5});
    }

    SUBCASE("144")
    {
        CHECK(divisors(144) == std::set<uint32_t>{1, 2, 3, 4, 6, 8, 9, 12, 16,
                                                  18, 24, 36, 48, 72});
    }

    SUBCASE("28")
    {
        CHECK(divisors(28) == std::set<uint32_t>{1, 2, 4, 7, 14});
    }

    SUBCASE("123456789")
    {
        std::set<uint32_t> t{1,     3,     9,     3607,     3803,    10821,
                             11409, 32463, 34227, 13717421, 41152263};

        CHECK(divisors(123456789) == t);
    }

    SUBCASE("uint16_t::max")
    {
        std::set<uint32_t> t{1,   3,   5,    15,   17,   51,    85,   255,
                             257, 771, 1285, 3855, 4369, 13107, 21845};
        CHECK(divisors(uint16_t(-1)) == t);
    }

    SUBCASE("uint32_t::max")
    {
        std::set<uint32_t> t{
            1,         3,        5,        15,        17,        51,
            85,        255,      257,      771,       1285,      3855,
            4369,      13107,    21845,    65535,     65537,     196611,
            327685,    983055,   1114129,  3342387,   5570645,   16711935,
            16843009,  50529027, 84215045, 252645135, 286331153, 858993459,
            1431655765};
        CHECK(divisors(-1) == t);
    }
}

TEST_CASE("powerset")
{
    std::multiset<uint32_t> x{1, 2, 3};
    std::set<std::multiset<uint32_t>> t{{},     {1},    {2},    {3},
                                        {1, 2}, {1, 3}, {2, 3}, {1, 2, 3}};

    CHECK(powerset(x) == t);
}

#endif
