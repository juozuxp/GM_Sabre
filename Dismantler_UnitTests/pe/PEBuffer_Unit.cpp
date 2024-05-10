#include "pch.h"
#include <CppUnitTest.h>
#include <pe/PEBuffer.hpp>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

TEST_CLASS(PEBuffer_Unit)
{
public:
	TEST_METHOD(Constructor_64bit)
	{
		PEBuffer buffer = PEBuffer(L"kernel32.dll");

		Assert::AreNotEqual<const void*>(buffer.GetBuffer(), nullptr);
	}

	TEST_METHOD(Constructor_32bit)
	{
		PEBuffer buffer = PEBuffer(L"kernel32_32bit.dll");

		Assert::AreNotEqual<const void*>(buffer.GetBuffer(), nullptr);
	}

	TEST_METHOD(Constructor_nonfile)
	{
		PEBuffer buffer = PEBuffer(L"NotExe.txt");

		Assert::AreEqual<const void*>(buffer.GetBuffer(), nullptr);
	}

	TEST_METHOD(Constructor_nonexe)
	{
		PEBuffer buffer = PEBuffer(L"NotAFile");

		Assert::AreEqual<const void*>(buffer.GetBuffer(), nullptr);
	}

	TEST_METHOD(Move_Construct)
	{
		PEBuffer pe = PEBuffer(L"kernel32.dll");

		const void* buffer = pe.GetBuffer();

		PEBuffer constructor = PEBuffer(std::move(pe));

		Assert::AreEqual(constructor.GetBuffer(), buffer);
		Assert::AreEqual<const void*>(pe.GetBuffer(), nullptr);
	}

	TEST_METHOD(Move_Equal)
	{
		PEBuffer pe = PEBuffer(L"kernel32.dll");

		const void* buffer = pe.GetBuffer();

		PEBuffer equal;

		equal = std::move(pe);

		Assert::AreEqual(equal.GetBuffer(), buffer);
		Assert::AreEqual<const void*>(pe.GetBuffer(), nullptr);
	}

	TEST_METHOD(GetHeaders_64bit)
	{
		PEBuffer pe = PEBuffer(L"kernel32.dll");

		PEHeaders* headers = pe.GetHeaders();

		Assert::AreNotEqual<void*>(headers, nullptr);

		delete headers;

		pe = PEBuffer();

		headers = pe.GetHeaders();

		Assert::AreEqual<void*>(headers, nullptr);
	}

	TEST_METHOD(GetHeaders_nonfile)
	{
		PEBuffer pe = PEBuffer();
		PEHeaders* headers = pe.GetHeaders();

		Assert::AreEqual<void*>(headers, nullptr);
	}
};