#include "DebugBenchmark.h"

DebugBenchmark& DebugBenchmark::getInstance() {
	static DebugBenchmark instance;
	return instance;
}

void DebugBenchmark::start() {
	QueryPerformanceCounter(&timer);
}

//numbers of milliseconds since timer start
double DebugBenchmark::end() {
	LARGE_INTEGER curr, freq;
	QueryPerformanceCounter(&curr);
	QueryPerformanceFrequency(&freq);
	double time = 1.0 * (curr.QuadPart - timer.QuadPart) / freq.QuadPart;
	return time * 1000;
}