#include "pch.h"
#include <CppUnitTest.h>
#include <managed/ManagedString.hpp>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

TEST_CLASS(ManagedString_Unit)
{
public:
	TEST_METHOD(Constructor_wstring_view)
	{
		std::wstring_view text = L"hello world";

		ManagedString string = ManagedString(text);

		Assert::AreNotEqual(string.m_Buffer, nullptr);
		Assert::AreEqual<uint32_t>(string.m_Size, text.length());

		Assert::AreEqual<int32_t>(wcsncmp(string.m_Buffer, text.data(), text.length()), 0);
	}

	TEST_METHOD(Constructor_const_char)
	{
		char ascii[] = "hello world";

		ManagedString string = ManagedString(ascii);

		Assert::AreNotEqual(string.m_Buffer, nullptr);
		Assert::AreEqual<uint32_t>(string.m_Size, sizeof(ascii) - 1);

		wchar_t utf16[] = L"hello world";

		Assert::AreEqual<int32_t>(wcsncmp(string.m_Buffer, utf16, sizeof(utf16) - 1), 0);
	}

	TEST_METHOD(Move_Constuct)
	{
		ManagedString string = ManagedString(L"hello world");

		uint32_t length = string.m_Size;
		wchar_t* buffer = string.m_Buffer;

		ManagedString constructor = ManagedString(std::move(string));

		Assert::AreEqual(constructor.m_Size, length);
		Assert::AreEqual(constructor.m_Buffer, buffer);

		Assert::AreEqual(string.m_Buffer, nullptr);
		Assert::AreEqual<uint32_t>(string.m_Size, 0);
	}

	TEST_METHOD(Move_Equal)
	{
		ManagedString string = ManagedString(L"hello world");

		uint32_t length = string.m_Size;
		wchar_t* buffer = string.m_Buffer;

		ManagedString equal;

		equal = std::move(string);

		Assert::AreEqual(equal.m_Size, length);
		Assert::AreEqual(equal.m_Buffer, buffer);

		Assert::AreEqual(string.m_Buffer, nullptr);
		Assert::AreEqual<uint32_t>(string.m_Size, 0);
	}
};