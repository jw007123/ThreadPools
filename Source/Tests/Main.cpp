#ifdef _WIN32
	#define _CRT_SECURE_NO_WARNINGS 1
#endif

#include <iostream>
#include <thread>

#include "tracy/Tracy.hpp"
#include "Eigen/Dense"

#include "Locking.h"

bool RunTests()
{
    return true;
}
   
void RunBenchmarks()
{
	ZoneScopedN("Benchmarks");

	
}

int main()
{
    if (RunTests())
    {
        RunBenchmarks();
    }

    return 0;
}
