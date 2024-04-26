#include "LowTrustString.hpp"
#include <windows.h>

bool LowTrustString::GetStringLength(const char* string, size_t& length)
{
	length = 0;

	__try
	{
		for (; *string != '\0'; string++, length++)
		{
			if (!isgraph(*string) &&
				!isspace(*string))
			{
				return false;
			}
		}
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		return false;
	}

	return true;
}

bool LowTrustString::GetStringLength(const wchar_t* string, size_t& length)
{
	length = 0;

	__try
	{
		length = wcslen(string);
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		return false;
	}

	int32_t flags = IS_TEXT_UNICODE_REVERSE_MASK | IS_TEXT_UNICODE_UNICODE_MASK | IS_TEXT_UNICODE_NOT_ASCII_MASK;
	return IsTextUnicode(string, length, &flags);
}