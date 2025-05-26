#pragma once

#ifdef _WIN32
	#include <WinBase.h>
#else
	// TODO
#endif

#include <string>

/*
*	Helper functions to be used across the various implementations.
*/

/// Sets the name of the calling thread. Useful for debugging purposes
void SetThreadName(const std::string& name_);
