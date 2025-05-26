#include "Utility.h"

void SetThreadName(const std::string& name_)
{
#ifdef _WIN32
	wchar_t buff[256];
	mbstowcs(buff, name_.c_str(), name_.length() + 1);
	SetThreadDescription(GetCurrentThread(), buff);
#else
	// TODO
#endif
}
