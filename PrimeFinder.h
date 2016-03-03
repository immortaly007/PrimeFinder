#pragma once
#include <vector>

class PrimeFinder
{
public:
	PrimeFinder();
	~PrimeFinder();

	std::vector<size_t> FindPrimes(size_t until);
};

