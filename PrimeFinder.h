#pragma once
#include <vector>

class PrimeFinder
{
public:
	PrimeFinder();
	~PrimeFinder();

	// Returns a vector containing all primes smaller than "until"
	std::vector<size_t> FindPrimes(size_t until);

	// Returns a conservative estimate of the number of primes smaller than "until".
	// By conservative is meant that the actual number of primes is probably slightly lower.
	size_t GetExpectedPrimeCount(size_t until);
};

