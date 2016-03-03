# PrimeFinder

PrimeFinder is a small efficient C++ application that can find all primes below a choosen number efficiently using the Sieve of Eratosthenes.
It uses efficient storage to store the primes, using 1 bit per number to indicate if it is a prime, and not storing this information for even numbers. Effectively, this mean that it requires 0.5 bits per number.

Note that the BitHelper.h file/class might be useful in other projects for quick bit operations, such as counting the number of set bits, or evaluating the value of a single bit in an integer. 

It can find all primes below:
 * 1 000 000 (1 million) in about 4 ms
 * 1 000 000 000 (1 billion) in about 5.2 seconds
 * 10 000 000 000 (10 billion) in about 68.1 seconds

I just made this for fun, but maybe someone will find it useful :)