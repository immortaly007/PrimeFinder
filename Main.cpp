#include "PrimeFinder.h"
#include "Stopwatch.h"
#include <fstream>

int main() {
	// Calculate the primes
	Stopwatch watch;
	watch.Reset();

	PrimeFinder finder;
	std::vector<size_t> primes = finder.FindPrimes(1000000000);

	printf("Found %zu primes in %f seconds, writing to file...", primes.size(), watch.GetTime());

	// Write the primes to a file
	std::ofstream resultFile;
	resultFile.open("primes.txt");
	for (size_t prime : primes) {
		resultFile << prime << "\n";
	}
	resultFile.close();

	system("pause");
}