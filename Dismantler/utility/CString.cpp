#include "CString.hpp"

std::string CString::ConvertReadable(const std::string_view& string)
{
	std::string converted;

	converted.reserve(GetLengthReadable(string));
	for (char letter : string)
	{
		switch (letter)
		{
		case '\f':
		{
			converted.push_back('\\');
			converted.push_back('f');
		} break;
		case '\n':
		{
			converted.push_back('\\');
			converted.push_back('n');
		} break;
		case '\r':
		{
			converted.push_back('\\');
			converted.push_back('r');
		} break;
		case '\v':
		{
			converted.push_back('\\');
			converted.push_back('v');
		} break;
		case '\\':
		{
			converted.push_back('\\');
			converted.push_back('\\');
		} break;
		default:
		{
			converted.push_back(letter);
		} break;
		}
	}

	return converted;
}

std::wstring CString::ConvertReadable(const std::wstring_view& string)
{
	std::wstring converted;

	converted.reserve(GetLengthReadable(string));
	for (wchar_t letter : string)
	{
		switch (letter)
		{
		case L'\f':
		{
			converted.push_back(L'\\');
			converted.push_back(L'f');
		} break;
		case L'\n':
		{
			converted.push_back(L'\\');
			converted.push_back(L'n');
		} break;
		case L'\r':
		{
			converted.push_back(L'\\');
			converted.push_back(L'r');
		} break;
		case L'\v':
		{
			converted.push_back(L'\\');
			converted.push_back(L'v');
		} break;
		case L'\\':
		{
			converted.push_back(L'\\');
			converted.push_back(L'\\');
		} break;
		default:
		{
			converted.push_back(letter);
		} break;
		}
	}

	return converted;
}

std::string CString::ConvertDefinition(const std::string_view& string)
{
	std::string converted;

	converted.reserve(GetLengthDefinition(string));
	for (char letter : string)
	{
		switch (letter)
		{
		case '\f':
		{
			converted.push_back('\\');
			converted.push_back('f');
		} break;
		case '\n':
		{
			converted.push_back('\\');
			converted.push_back('n');
		} break;
		case '\r':
		{
			converted.push_back('\\');
			converted.push_back('r');
		} break;
		case '\v':
		{
			converted.push_back('\\');
			converted.push_back('v');
		} break;
		case '\\':
		{
			converted.push_back('\\');
			converted.push_back('\\');
		} break;
		case '\'':
		{
			converted.push_back('\\');
			converted.push_back('\'');
		} break;
		case '\"':
		{
			converted.push_back('\\');
			converted.push_back('\"');
		} break;
		default:
		{
			converted.push_back(letter);
		} break;
		}
	}

	return converted;
}

std::wstring CString::ConvertDefinition(const std::wstring_view& string)
{
	std::wstring converted;

	converted.reserve(GetLengthDefinition(string));
	for (wchar_t letter : string)
	{
		switch (letter)
		{
		case L'\f':
		{
			converted.push_back(L'\\');
			converted.push_back(L'f');
		} break;
		case L'\n':
		{
			converted.push_back(L'\\');
			converted.push_back(L'n');
		} break;
		case L'\r':
		{
			converted.push_back(L'\\');
			converted.push_back(L'r');
		} break;
		case L'\v':
		{
			converted.push_back(L'\\');
			converted.push_back(L'v');
		} break;
		case L'\\':
		{
			converted.push_back(L'\\');
			converted.push_back(L'\\');
		} break;
		case L'\'':
		{
			converted.push_back(L'\\');
			converted.push_back(L'\'');
		} break;
		case L'\"':
		{
			converted.push_back(L'\\');
			converted.push_back(L'\"');
		} break;
		default:
		{
			converted.push_back(letter);
		} break;
		}
	}

	return converted;
}

size_t CString::GetLengthReadable(const std::string_view& string)
{
	constexpr char spaces[] = { '\f', '\n', '\r', '\v', '\\' };

	size_t length = string.size();
	for (char letter : string)
	{
		for (char space : spaces)
		{
			if (letter == space)
			{
				length++;
				break;
			}
		}
	}

	return length;
}

size_t CString::GetLengthReadable(const std::wstring_view& string)
{
	constexpr wchar_t spaces[] = { '\f', '\n', '\r', '\v', '\\' };

	size_t length = string.size();
	for (wchar_t letter : string)
	{
		for (wchar_t space : spaces)
		{
			if (letter == space)
			{
				length++;
				break;
			}
		}
	}

	return length;
}

size_t CString::GetLengthDefinition(const std::string_view& string)
{
	constexpr char spaces[] = { '\f', '\n', '\r', '\v', '\\', '\'', '\"' };

	size_t length = string.size();
	for (char letter : string)
	{
		for (char space : spaces)
		{
			if (letter == space)
			{
				length++;
				break;
			}
		}
	}

	return length;
}

size_t CString::GetLengthDefinition(const std::wstring_view& string)
{
	constexpr wchar_t spaces[] = { '\f', '\n', '\r', '\v', '\\', '\'', '\"' };

	size_t length = string.size();
	for (wchar_t letter : string)
	{
		for (wchar_t space : spaces)
		{
			if (letter == space)
			{
				length++;
				break;
			}
		}
	}

	return length;
}