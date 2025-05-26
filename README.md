# ThreadPools

Testing a range of thread pool implementations and graphing how each performs when running through a set of independent jobs. As a bonus, tacking on how setting thread/process priority changes things as well. CMake is set-up as a library + executable + externals to show off the amazing build system for cross-platform C++ development.

## Classes
* Locking: Standard STL blocking mutex implementation that involves getting data from a stack and pushing to a queue. Each of these operations is wrapped around a mutex and threads loop indefinitely whilst waiting for jobs.
* Semaphore: WinAPI and gcc semaphore implementation (not C++20 semaphores as the standard doesn't guarantee that this is more than a wrapper around STL atomic). Faster and much cleaner than Locking implementation, but requires going to the OS for semaphore wake-ups/sleep calls since it can't assume that we're not cross-process.
* Futex: WinAPI and gcc futex implementation that should avoid slow (timed as ~400us on gcc) OS calls. Futex calls are explicitly single-process, so we should expect faster timings on thread wake-ups etc than the semaphore implementation.

## Progress
* Four jobs of varying time-to-complete added. Aim is to have a range of jobs (20us -> 5000us in length) and test the futex implementation pretty thoroughly via that.
* Locking implementation compiling and in process of writing test. WinAPI needs adding to the CMake due to the SetThreadName function.
