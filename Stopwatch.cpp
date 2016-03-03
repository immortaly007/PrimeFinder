#include "Stopwatch.h"

Stopwatch::Stopwatch()
{
	Reset();
}

Stopwatch::~Stopwatch()
{
}

void Stopwatch::Reset()
{
	begin = clock();
}

double Stopwatch::GetTime()
{
	return double(clock() - begin) / CLOCKS_PER_SEC;
}