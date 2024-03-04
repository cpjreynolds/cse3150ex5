# CSE3150 - Exercise 5

`make all` to compile.

`make check` to compile and run tests.

`make run` to compile and run `main()`

## Note

I was explicit in my interfaces only taking `uint32_t` to keep the cost of
naively computing divisors somewhat reasonable, given it only requires trial
division of all 16-bit-representable primes in the worst case.
