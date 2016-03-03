#include "PrimeFinder.h"
#include "Stopwatch.h"
#include <fstream>

int main() {
	size_t until = 10000000000;

	PrimeFinder finder;
	printf("Expecting %zu primes (estimated)...\n", finder.GetExpectedPrimeCount(until));

	// Calculate the primes
	Stopwatch watch;
	watch.Reset();

	std::vector<size_t> primes = finder.FindPrimes(until);

	printf("Found     %zu primes in %f seconds.\n", primes.size(), watch.GetTime());

	// Write the primes to a file
	//printf("Writing to file...\n");
	//std::ofstream resultFile;
	//resultFile.open("primes.txt");
	//for (size_t prime : primes) {
	//	resultFile << prime << "\n";
	//}
	//resultFile.close();

	system("pause");
}