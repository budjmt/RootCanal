#pragma once
#include <Windows.h>

class DebugBenchmark
{
public:
	static DebugBenchmark& getInstance();

	void start(); 
	double end();
private:
	DebugBenchmark() { start(); };
	//~DebugBenchmark();
	DebugBenchmark(const DebugBenchmark&) = delete;
	void operator=(const DebugBenchmark&) = delete;

	LARGE_INTEGER timer;
	double elapsed;
};

