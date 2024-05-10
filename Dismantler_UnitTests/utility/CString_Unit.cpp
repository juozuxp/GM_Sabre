#include "pch.h"
#include <CppUnitTest.h>
#include <utility/CString.hpp>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

TEST_CLASS(CString_Unit)
{
public:
	TEST_METHOD(ConvertReadable_wide)
	{
		std::wstring wide = CString::ConvertReadable(L"\f\n\r\v\\abcd");

		Assert::AreEqual<size_t>(wide.length(), 14);

		Assert::AreEqual(wide[0], L'\\');
		Assert::AreEqual(wide[1], L'f');
		Assert::AreEqual(wide[2], L'\\');
		Assert::AreEqual(wide[3], L'n');
		Assert::AreEqual(wide[4], L'\\');
		Assert::AreEqual(wide[5], L'r');
		Assert::AreEqual(wide[6], L'\\');
		Assert::AreEqual(wide[7], L'v');
		Assert::AreEqual(wide[8], L'\\');
		Assert::AreEqual(wide[9], L'\\');
		Assert::AreEqual(wide[10], L'a');
		Assert::AreEqual(wide[11], L'b');
		Assert::AreEqual(wide[12], L'c');
		Assert::AreEqual(wide[13], L'd');
	}

	TEST_METHOD(ConvertReadable_ascii)
	{
		std::string ascii = CString::ConvertReadable("\f\n\r\v\\abcd");

		Assert::AreEqual<size_t>(ascii.length(), 14);

		Assert::AreEqual(ascii[0], '\\');
		Assert::AreEqual(ascii[1], 'f');
		Assert::AreEqual(ascii[2], '\\');
		Assert::AreEqual(ascii[3], 'n');
		Assert::AreEqual(ascii[4], '\\');
		Assert::AreEqual(ascii[5], 'r');
		Assert::AreEqual(ascii[6], '\\');
		Assert::AreEqual(ascii[7], 'v');
		Assert::AreEqual(ascii[8], '\\');
		Assert::AreEqual(ascii[9], '\\');
		Assert::AreEqual(ascii[10], 'a');
		Assert::AreEqual(ascii[11], 'b');
		Assert::AreEqual(ascii[12], 'c');
		Assert::AreEqual(ascii[13], 'd');
	}

	TEST_METHOD(ConvertDefinition_wide)
	{
		std::wstring wide = CString::ConvertDefinition(L"\f\n\r\v\\\'\"abcd");

		Assert::AreEqual<size_t>(wide.length(), 18);

		Assert::AreEqual(wide[0], L'\\');
		Assert::AreEqual(wide[1], L'f');
		Assert::AreEqual(wide[2], L'\\');
		Assert::AreEqual(wide[3], L'n');
		Assert::AreEqual(wide[4], L'\\');
		Assert::AreEqual(wide[5], L'r');
		Assert::AreEqual(wide[6], L'\\');
		Assert::AreEqual(wide[7], L'v');
		Assert::AreEqual(wide[8], L'\\');
		Assert::AreEqual(wide[9], L'\\');
		Assert::AreEqual(wide[10], L'\\');
		Assert::AreEqual(wide[11], L'\'');
		Assert::AreEqual(wide[12], L'\\');
		Assert::AreEqual(wide[13], L'\"');
		Assert::AreEqual(wide[14], L'a');
		Assert::AreEqual(wide[15], L'b');
		Assert::AreEqual(wide[16], L'c');
		Assert::AreEqual(wide[17], L'd');
	}

	TEST_METHOD(ConvertDefinition_ascii)
	{
		std::string ascii = CString::ConvertDefinition("\f\n\r\v\\\'\"abcd");

		Assert::AreEqual<size_t>(ascii.length(), 18);

		Assert::AreEqual(ascii[0], '\\');
		Assert::AreEqual(ascii[1], 'f');
		Assert::AreEqual(ascii[2], '\\');
		Assert::AreEqual(ascii[3], 'n');
		Assert::AreEqual(ascii[4], '\\');
		Assert::AreEqual(ascii[5], 'r');
		Assert::AreEqual(ascii[6], '\\');
		Assert::AreEqual(ascii[7], 'v');
		Assert::AreEqual(ascii[8], '\\');
		Assert::AreEqual(ascii[9], '\\');
		Assert::AreEqual(ascii[10], '\\');
		Assert::AreEqual(ascii[11], '\'');
		Assert::AreEqual(ascii[12], '\\');
		Assert::AreEqual(ascii[13], '\"');
		Assert::AreEqual(ascii[14], 'a');
		Assert::AreEqual(ascii[15], 'b');
		Assert::AreEqual(ascii[16], 'c');
		Assert::AreEqual(ascii[17], 'd');
	}
};