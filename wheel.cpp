#include "wheel.hpp"

// ==[ wheel ] ==

wheel::wheel(uint32_t i, uint32_t limit) : _i{i}, _limit{limit}
{
    if (_i == 0) {
        _vals = {1};
        _len = 1;
    }
    if (_i == 1) {
        _vals = {1};
        _primes = {2};
        _len = 2;
    }
    if (_i == 2) {
        _vals = {1, 5};
        _primes = {2, 3};
        _len = 6;
    }
};

wheel wheel::next() const
{
    wheel cpy = *this;
    cpy.advance();
    return cpy;
}

bool wheel::advance()
{
    // nprime = first prime after 1
    auto nprime = first_prime();

    if (nprime * nprime >= _limit) {
        return false;
    }

    auto nlen = std::min(_len * nprime, _limit);

    if (nlen == _len) {
        for (auto it = _vals.rbegin(); it != _vals.rend(); ++it) {
            _vals.erase(nprime * *it);
        }
    }
    else {
        auto rolled = std::set(roll(), upto(nlen));
        for (auto it = _vals.crbegin(); it != _vals.crend(); ++it) {
            rolled.erase(nprime * *it);
        }
        _vals = rolled;
    }

    _primes.insert(nprime);
    _len = nlen;
    ++_i;
    return true;
}

std::set<uint32_t> wheel::primes() const
{
    auto p = first_prime();
    auto bound = _vals.lower_bound(p * p);
    auto pcpy = _primes;
    pcpy.insert(std::next(_vals.cbegin()), bound);
    return pcpy;
}

wheel::roll_iterator wheel::roll() const
{
    return roll_iterator(*this);
}

wheel::roll_iterator wheel::upto(uint32_t n) const
{
    return roll_iterator(n);
}

std::ostream& operator<<(std::ostream& os, const wheel& self)
{
    os << "W[" << self._i << "]: len(" << self._len << ")\n";
    os << "primes: ";
    for (auto p : self._primes) {
        os << p << " ";
    }
    os << "\nvals: ";
    for (auto v : self._vals) {
        os << v << " ";
    }
    os << '\n';
    return os;
}

// ==[ roll_iterator ]==

wheel::roll_iterator::value_type wheel::roll_iterator::operator*() const
{
    uint32_t v = *_vals_it;
    return v + (_whl->_len * _loop);
}

wheel::roll_iterator& wheel::roll_iterator::operator++()
{
    ++_vals_it;
    if (_vals_it == _whl->_vals.cend()) {
        _vals_it = _whl->_vals.cbegin();
        ++_loop;
    }
    return *this;
}

wheel::roll_iterator wheel::roll_iterator::operator++(int)
{
    auto cpy = *this;
    ++(*this);
    return cpy;
}

bool operator==(const wheel::roll_iterator& lhs,
                const wheel::roll_iterator& rhs)
{
    if (lhs.is_sentinel()) {
        return *rhs >= lhs._loop;
    }
    else if (rhs.is_sentinel()) {
        return *lhs >= rhs._loop;
    }
    else {
        return lhs._whl == rhs._whl && lhs._vals_it == rhs._vals_it &&
               lhs._loop == rhs._loop;
    }
}

bool operator!=(const wheel::roll_iterator& lhs,
                const wheel::roll_iterator& rhs)
{
    return !(lhs == rhs);
}
