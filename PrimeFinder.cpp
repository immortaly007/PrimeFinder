#include "PrimeFinder.h"
#include "BoolArray.h"
#include "SqrtHelper.h"

PrimeFinder::PrimeFinder()
{
}


PrimeFinder::~PrimeFinder()
{
}

// Returns true iff n is even
constexpr bool IsEven(size_t n)
{
	return n % 2 == 0;
}

// Checks if the number is in the sieve, since we skip even numbers and numbers smaller than 3.
constexpr bool HasIndex(size_t n)
{
	return !IsEven(n) && n >= 3;
}

// Returns the index of n in the sieve. Gives a lower bound index if n is not in the sieve
constexpr size_t GetIndex(size_t n)
{
	return (n - 3) / 2;
}

std::vector<size_t> PrimeFinder::FindPrimes(const size_t until)
{
	// Find prime number using the sieve of Eratosthenes

	// Initialize the sieve assuming all numbers are prime
	BoolArray sieve(GetIndex(until) + 1, true);
	std::vector<size_t> primes(1, 2); // initialize the list of primes to contain the number 2 exactly once

	size_t sqrtUntil = SqrtHelper::isqrt(until);
	for (size_t n = 3; n <= sqrtUntil; n += 2)
	{
		// Check if n is prime
		size_t i = GetIndex(n);
		if (sieve[i]) {
			primes.push_back(n);

			// If n is prime, mark all multiples of n as being not prime:
			size_t multiplier = 3;
			size_t cur = n;
			while (cur < until)
			{
				cur = n * multiplier;
				if (HasIndex(cur))
				{
					size_t j = GetIndex(cur);
					if (j < sieve.Size())
						sieve.Set(j, false);
				}

				multiplier += 2; // Skip even multipliers, as their result will never be in our sieve.
			}
		}
	}

	// Check the rest of the range (from sqrt(until) to until) for primes:
	if (IsEven(sqrtUntil)) sqrtUntil++;
	for (size_t n = sqrtUntil; n < until; n += 2)
	{
		// Check if n is prime
		size_t i = GetIndex(n);
		if (sieve[i])
			primes.push_back(n);
	}

	return primes;
}