#include "pch.h"
#include <CppUnitTest.h>
#include <pe/PEBuffer.hpp>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

TEST_CLASS(PEBuffer_Unit)
{
public:
	TEST_METHOD(Constructor)
	{
		PEBuffer buffer = PEBuffer(L"kernel32.dll");

		Assert::AreNotEqual<const void*>(buffer.GetBuffer(), nullptr);

		buffer = PEBuffer(L"kernel32_32bit.dll");

		Assert::AreNotEqual<const void*>(buffer.GetBuffer(), nullptr);

		buffer = PEBuffer(L"NotExe.txt");

		Assert::AreEqual<const void*>(buffer.GetBuffer(), nullptr);

		buffer = PEBuffer(L"NotAFile");

		Assert::AreEqual<const void*>(buffer.GetBuffer(), nullptr);
	}

	TEST_METHOD(Move)
	{
		PEBuffer pe = PEBuffer(L"kernel32.dll");

		const void* buffer = pe.GetBuffer();

		PEBuffer constructor = PEBuffer(std::move(pe));

		Assert::AreEqual(constructor.GetBuffer(), buffer);
		Assert::AreEqual<const void*>(pe.GetBuffer(), nullptr);

		PEBuffer equal;

		equal = std::move(constructor);

		Assert::AreEqual(equal.GetBuffer(), buffer);
		Assert::AreEqual<const void*>(constructor.GetBuffer(), nullptr);
	}

	TEST_METHOD(GetHeaders)
	{
		PEBuffer pe = PEBuffer(L"kernel32.dll");

		PEHeaders* headers = pe.GetHeaders();

		Assert::AreNotEqual<void*>(headers, nullptr);

		delete headers;

		pe = PEBuffer();

		headers = pe.GetHeaders();

		Assert::AreEqual<void*>(headers, nullptr);
	}
};