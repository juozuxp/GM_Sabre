#pragma once
#include <string>

class CString
{
public:
	static std::string ConvertReadable(const std::string_view& string);
	static std::wstring ConvertReadable(const std::wstring_view& string);

	static std::string ConvertDefinition(const std::string_view& string);
	static std::wstring ConvertDefinition(const std::wstring_view& string);

private:
	static size_t GetLengthReadable(const std::string_view& string);
	static size_t GetLengthReadable(const std::wstring_view& string);

	static size_t GetLengthDefinition(const std::string_view& string);
	static size_t GetLengthDefinition(const std::wstring_view& string);
};

