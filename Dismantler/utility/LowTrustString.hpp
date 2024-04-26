#pragma once
#include <string>

class LowTrustString
{
public:
	static bool GetStringLength(const char* string, size_t& length); // get length of a low trust string
	static bool GetStringLength(const wchar_t* string, size_t& length); // get length of a low trust string
};