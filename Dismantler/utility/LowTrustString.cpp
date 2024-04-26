#include "LowTrustString.hpp"
#include <windows.h>
#include <vector>

bool LowTrustString::GetStringLength(const char* string, size_t& length)
{
	length = 0;

	size_t stringLength;
	__try
	{
		stringLength = strlen(string);
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		return false;
	}

	for (size_t i = 0; i < stringLength; i++)
	{
		if (static_cast<uint8_t>(string[i]) >= 128)
		{
			length = i;
			return false;
		}
	}

	m_CharacterTypes.resize(stringLength);

	if (!GetStringTypeA(LOCALE_SYSTEM_DEFAULT, CT_CTYPE1, string, stringLength, m_CharacterTypes.data()))
	{
		return false;
	}

	return CheckCharacterTypes(length);
}

bool LowTrustString::GetStringLength(const wchar_t* string, size_t& length)
{
	length = 0;

	size_t stringLength;
	__try
	{
		stringLength = wcslen(string);
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		return false;
	}

	for (size_t i = 0; i < stringLength; i++)
	{
		if (static_cast<uint16_t>(string[i]) >= 128 && static_cast<uint16_t>(string[i]) <= 255)
		{
			length = i;
			return false;
		}
	}

	m_CharacterTypes.resize(stringLength);

	if (!GetStringTypeW(CT_CTYPE1, string, stringLength, m_CharacterTypes.data()))
	{
		return false;
	}

	return CheckCharacterTypes(length);
}

bool LowTrustString::CheckCharacterTypes(size_t& length)
{
	for (uint16_t type : m_CharacterTypes)
	{
		if (!(type & (C1_ALPHA | C1_XDIGIT | C1_BLANK | C1_PUNCT | C1_SPACE | C1_DIGIT | C1_LOWER | C1_UPPER)))
		{
			return false;
		}

		length++;
	}

	return true;
}