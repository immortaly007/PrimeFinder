#pragma once
#include <ctime>

class Stopwatch
{
public:
	Stopwatch();
	~Stopwatch();

	void	Reset();
	// Returns the time in seconds
	double	GetTime();

private:
	clock_t begin;
};

