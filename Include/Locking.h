#pragma once

#include <thread>
#include <chrono>
#include <mutex>
#include <atomic>
#include <queue>
#include <stack>

#include "tracy/Tracy.hpp"
#include "Eigen/Sparse"

#include "Literals.h"
#include "Jobs.h"

namespace Pools
{
	/*
	*	Thread-pool that uses a mutex as its syncro (I can't spell the word)
	*	mechanism for obtaining jobs from a LIFO stack. Templated in order to
	*	take in functions of the form:
	*
	*		RESULT_T JOB_FUNC(JOB_T[0], ..., JOB_T[N-1])
	*
	*	for N possible arguments.
	*/
	template <class RESULT_T, class JOB_FUNC, class... JOB_T>
	class Locking
	{
	public:
		Locking(std::stack<JOB_T>& job_stack_,
				std::mutex& job_mutex_,
				std::queue<RESULT_T>& result_queue_,
				std::mutex& result_mutex_,
				const size_t wait_time_ms_ = 5,
				const size_t n_threads_ = std::thread::hardware_concurrency() / 2) :
			job_stack(job_stack_),
			job_mutex(job_mutex_),
			result_queue(result_queue_),
			result_mutex(result_mutex_),
			wait_time_ms(wait_time_ms_)
		{
			// Avoid reallocs to keep things tidy
			threads.reserve(n_threads_);
			shutdown_atoms.resize(n_threads_, false);

			for (size_t i = 0; i < n_threads_; ++i)
			{
				threads.push_back(std::thread(Locking::Main, this, i));
				while (!threads[i].joinable())
				{
					// Spin until thread is up
				}
			}
		}

		~Locking()
		{
			// size() of vectors is equal to n_threads_ in ctor
			for (size_t i = 0; i < threads.size(); ++i)
			{
				// Signal thread and block until it's down
				shutdown_atoms[i] = true;
				threads[i].join();
			}
		}

		void Main(const size_t thread_idx_)
		{
			while (1)
			{
				// Avoid thrashing
				std::this_thread::sleep_for(std::chrono::milliseconds(wait_time_ms));

				if (shutdown_atoms[thread_idx_])
				{
					// Finish thread execution
					return;
				}

				// Obtain lock on stack. Blocks thread
				job_mutex.lock();
				if (job_stack.empty())
				{
					// No jobs. Unlock mutex and reset
					job_mutex.unlock();
					continue;
				}

				auto UnlockerFunc = [&](JOB_T&&... job_) -> RESULT_T
				{
					// Remove top item from stack. Item has been moved, so our job_ remains valid
					job_stack.pop();

					// Unlock mutex before job is forwarded to the function, otherwise we block
					// all other threads and are effectively single-threaded
					job_mutex.unlock();
					return JOB_FUNC(std::forward<JOB_T>(job_)...);
				};

				// Move top element from stack and use to determine our result
				const RESULT_T result = UnlockerFunc(std::forward<JOB_T>(std::move(job_stack.top()))...);

				// Move onto results queue. Block until queue becomes free
				std::scoped_lock<std::mutex> results_lock(result_mutex);
				result_queue.emplace(std::move(result));
			}
		}

	private:
		std::stack<JOB_T>& job_stack;
		std::mutex& job_mutex;
		std::queue<RESULT_T>& result_queue;
		std::mutex& result_mutex;
		const size_t wait_time_ms;

		std::vector<std::thread> threads;
		std::vector<std::atomic_bool> shutdown_atoms;
	};
}
