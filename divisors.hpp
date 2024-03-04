#ifndef DIVISORS_HPP
#define DIVISORS_HPP

#include <cstdint>
#include <set>

// returns all divisors of `n` excluding `n`
std::set<uint32_t> divisors(uint32_t n);

#endif
