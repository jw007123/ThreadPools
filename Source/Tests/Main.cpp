#ifdef _WIN32
	#define _CRT_SECURE_NO_WARNINGS 1
#endif

#include <iostream>
#include <thread>

#include "tracy/Tracy.hpp"
#include "Eigen/Dense"

#include "../Futex.cpp"
#include "../Semaphore.cpp"
#include "../Locking.cpp"

bool RunTests()
{
    return true;
}
   
void RunBenchmarks()
{
	ZoneScopedN("Benchmarks");

	Eigen::MatrixXd f(10, 10);
	f.setIdentity();
	f(0, 0) = 2.0;

	const Eigen::MatrixXd f_inv = f.inverse();
	std::cout << std::to_string(f_inv(0, 0)) << std::endl;
}

int main()
{
    if (RunTests())
    {
        RunBenchmarks();
    }

    return 0;
}
