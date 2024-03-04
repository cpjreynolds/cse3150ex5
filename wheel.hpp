#ifndef WHEEL_HPP
#define WHEEL_HPP

#include <cstdint>
#include <iterator>
#include <memory>
#include <set>
#include <iostream>

const std::set<uint32_t>& primes(uint32_t least);

// implements wheel for Sieve of Pritchard
class wheel {
public:
    class roll_iterator;

    // creates wheel W[i]
    wheel(uint32_t i, uint32_t limit = uint16_t(-1));

    wheel next() const; // returns W[i+1]
    bool advance();     // advances to W[i+1]

    std::set<uint32_t> primes() const; // returns current set of primes

    uint32_t prime_limit() const { return first_prime() * first_prime(); }

    roll_iterator roll() const;           // infinite iterator of the wheel
    roll_iterator upto(uint32_t n) const; // end iterator for given limit `n`

    friend std::ostream& operator<<(std::ostream& os, const wheel& self);

    class roll_iterator {
    public:
        friend class wheel;
        using iterator_category = std::forward_iterator_tag;
        using value_type = uint32_t;
        using difference_type = ptrdiff_t;
        using pointer = void;
        using reference = const uint32_t&;

        value_type operator*() const;
        roll_iterator& operator++();
        roll_iterator operator++(int);

        friend bool operator==(const roll_iterator& lhs,
                               const roll_iterator& rhs);
        friend bool operator!=(const roll_iterator& lhs,
                               const roll_iterator& rhs);

    private:
        roll_iterator(const wheel& whl) : _whl(std::addressof(whl)){};
        roll_iterator(uint32_t upto)
            : _whl{nullptr}, _vals_it{nullptr}, _loop{upto} {};

        bool is_sentinel() const { return _whl == nullptr; };

        const wheel* _whl;
        std::set<uint32_t>::const_iterator _vals_it = _whl->_vals.cbegin();
        uint32_t _loop = 0; // n-times through vals, or max-value if sentinel
    };

private:
    // returns 1st prime after 1 in vals.
    uint32_t first_prime() const { return *++_vals.begin(); };

private:
    uint32_t _i;
    uint32_t _len;
    uint32_t _limit;
    std::set<uint32_t> _primes;
    std::set<uint32_t> _vals;
};

#endif
