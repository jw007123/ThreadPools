#ifdef _WIN32
	#define _CRT_SECURE_NO_WARNINGS 1
#endif

#include <iostream>
#include <thread>
#include <stack>

#include "tracy/Tracy.hpp"
#include "Eigen/Dense"

#include "Jobs.h"
#include "Locking.h"

template <class RESULT_T, class... JOB_T>
bool RunLockingTest(const std::function<RESULT_T(JOB_T...)> job_function_)
{
	std::stack<std::tuple<JOB_T...>> job_stack;
	std::mutex job_mutex;
	std::queue<RESULT_T> result_queue;
	std::mutex result_mutex;

	Pools::Locking<RESULT_T, JOB_T...> thread_pool(
		job_stack,
		job_mutex,
		result_queue,
		result_mutex,
		job_function_
	);

	return true;
}

bool RunTests()
{
	return RunLockingTest(std::bind(SparseSolve, std::placeholders::_1, std::placeholders::_2));
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

template Pools::Locking<Eigen::VectorXd, const Eigen::SparseMatrix<double>&, const Eigen::VectorXd&>;
