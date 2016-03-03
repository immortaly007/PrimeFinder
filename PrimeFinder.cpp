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

// Returns the number at index i in the Sieve
constexpr size_t GetNumberAtIndex(size_t i)
{
	return (i * 2) + 3;
}

size_t PrimeFinder::GetExpectedPrimeCount(const size_t until)
{
	// Expected number of primes, based on the "Prime Number Theorem",
	// We use 1.1 because it generally overshoots the actual number of primes, preventing us from having to move the vector with primes.
	return until / (log(until) - 1.1);
}

inline void MarkMultiples(BoolArray& sieve, const size_t n, const size_t i, const size_t until)
{
	size_t multiplier = i + 1; // Start at the square of the number. 
	size_t cur = i + (multiplier * n);
	while (cur < sieve.Size())
	{
		// Since we're multiplying two odd numbers, the result is always odd, and thus in our sieve.
		sieve.Set(cur, false);
		multiplier++;
		cur = i + (multiplier * n);
	}
}

BoolArray GetSieve(const size_t until)
{
	// Initialize the sieve assuming all numbers are prime
	BoolArray sieve(GetIndex(until) + 1, true);

	size_t sqrtUntil = SqrtHelper::isqrt(until);
	size_t iSqrtUntil = GetIndex(sqrtUntil);
	for (size_t i = 0; i <= iSqrtUntil; i++)
	{
		// If the current number is prime, mark all multiples of it as being not prime:
		if (sieve[i]) {
			// n is the current number that is considered
			size_t n = GetNumberAtIndex(i);
			//MarkMultiples(sieve, n, until);
			MarkMultiples(sieve, n, i, until);
		}
	}
	return sieve;
}

std::vector<size_t> PrimeFinder::FindPrimes(const size_t until)
{
	// Find prime number using the sieve of Eratosthenes
	BoolArray sieve = GetSieve(until);

	size_t expected = GetExpectedPrimeCount(until);
	
	// Check the rest of the range (from sqrt(until) to until) for primes:
	std::vector<size_t> primes;
	primes.reserve(expected);
	primes.push_back(2); // Add 2, the only even prime number, and therefore the only one not in our sieve.
	for (size_t i = 0; i < sieve.Size(); i++)
		if (sieve[i]) 
			primes.push_back(GetNumberAtIndex(i));

	return primes;
}