#pragma once

#include <thread>
#include <chrono>
#include <mutex>
#include <atomic>
#include <queue>
#include <stack>

#include "tracy/Tracy.hpp"
#include "Eigen/Sparse"

#include "Jobs.h"
#include "Utility.h"

namespace Pools
{
	/*
	*	Thread-pool that uses a mutex as its syncro (I can't spell the word)
	*	mechanism for obtaining jobs from a LIFO stack. Templated in order to
	*	take in functions of the form:
	*
	*		RESULT_T job_function(JOB_T[0], ..., JOB_T[N-1])
	*
	*	for N possible arguments.
	*/
	template <class RESULT_T, class... JOB_T>
	class Locking
	{
	public:
		Locking(std::stack<std::tuple<JOB_T...>>& job_stack_,
				std::mutex& job_mutex_,
				std::queue<RESULT_T>& result_queue_,
				std::mutex& result_mutex_,
				const std::function<RESULT_T(JOB_T...)> job_function_,
				const size_t wait_time_ms_ = 5,
				const size_t n_threads_ = std::thread::hardware_concurrency() / 2) :
				job_stack(job_stack_),
				job_mutex(job_mutex_),
				result_queue(result_queue_),
				result_mutex(result_mutex_),
				job_function(job_function_),
				wait_time_ms(wait_time_ms_),
				shutdown_atom(false)
		{
			// Avoid reallocs to keep things tidy
			threads.reserve(n_threads_);
			
			for (size_t i = 0; i < n_threads_; ++i)
			{
				threads.push_back(std::thread(&Locking::Main, this, i));
				while (!threads[i].joinable())
				{
					// Spin until thread is up
				}
			}
		}

		~Locking()
		{
			shutdown_atom = true;

			// size() of vectors is equal to n_threads_ in ctor
			for (size_t i = 0; i < threads.size(); ++i)
			{
				// Signal thread and block until it's down
				threads[i].join();
			}
		}

		void Main(const size_t thread_idx_)
		{
			SetThreadName("Worker Thread" + std::to_string(thread_idx_))

			while (1)
			{
				// Avoid thrashing
				std::this_thread::sleep_for(std::chrono::milliseconds(wait_time_ms));

				if (shutdown_atom)
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

				auto QueueLockerFunc = [&](RESULT_T&& result_) -> void
				{
					// Move onto results queue. Block until queue becomes free
					std::scoped_lock results_lock(result_mutex);
					result_queue.emplace(result_);
				};

				auto StackUnlockerFunc = [&](std::tuple<JOB_T...>&& job_) -> RESULT_T
				{
					// Remove top item from stack. Item has been moved, so our job_ remains valid
					job_stack.pop();

					// Unlock mutex before job is forwarded to the function, otherwise we block
					// all other threads and are effectively single-threaded
					job_mutex.unlock();
					return std::apply(job_function, job_);
				};
#
				// i. Move top element from stack and pass && job into StackUnlockerFunc
				// ii. Pop the invalid item from the stack and unlock the job mutex
				// iii. Run job and pass && result into QueueLockerFunc
				// iv. Lock results mutex whilst we push into the queue
				// v. Unlock results mutex once we leave the scope
				QueueLockerFunc(StackUnlockerFunc(std::move(job_stack.top())));
			}
		}

	private:
		std::stack<std::tuple<JOB_T...>>& job_stack;
		std::mutex& job_mutex;
		std::queue<RESULT_T>& result_queue;
		std::mutex& result_mutex;
		const std::function<RESULT_T(JOB_T...)> job_function;
		const size_t wait_time_ms;

		std::vector<std::thread> threads;
		std::atomic_bool shutdown_atom;
	};
}
