#include "pch.h"
#include <CppUnitTest.h>
#include <utility/LowTrustString.hpp>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

TEST_CLASS(LowTrustString_Unit)
{
public:
	TEST_METHOD(GetStringLength_wide)
	{
		LowTrustString lts;

		size_t length;

		Assert::IsTrue(lts.GetStringLength(L"これは実験です", length));

		Assert::AreEqual(length, (sizeof(L"これは実験です") / sizeof(wchar_t)) - 1);
	}

	TEST_METHOD(GetStringLength_wideinvalid)
	{
		LowTrustString lts;

		size_t length;

		Assert::IsFalse(lts.GetStringLength(L"これは実験です\x5", length));

		Assert::AreEqual(length, (sizeof(L"これは実験です") / sizeof(wchar_t)) - 1);
	}

	TEST_METHOD(GetStringLength_widenull)
	{
		LowTrustString lts;

		size_t length;

		Assert::IsFalse(lts.GetStringLength(static_cast<const wchar_t*>(nullptr), length));
	}

	TEST_METHOD(GetStringLength_ascii)
	{
		LowTrustString lts;

		size_t length;

		Assert::IsTrue(lts.GetStringLength("hello world", length));

		Assert::AreEqual(length, sizeof("hello world") - 1);
	}

	TEST_METHOD(GetStringLength_asciiinvalid)
	{
		LowTrustString lts;

		size_t length;

		Assert::IsFalse(lts.GetStringLength("hello world\x5", length));

		Assert::AreEqual(length, sizeof("hello world") - 1);
	}

	TEST_METHOD(GetStringLength_asciinull)
	{
		LowTrustString lts;

		size_t length;

		Assert::IsFalse(lts.GetStringLength(static_cast<const char*>(nullptr), length));
	}
};