#include "pch.h"
#include <CppUnitTest.h>
#include <pe/PEHeaders.hpp>
#include <pe/PEBuffer.hpp>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

TEST_CLASS(PEHeaders_Unit)
{
public:
	TEST_METHOD(Constructor_64bit)
	{
		PEBuffer buffer = PEBuffer(L"kernel32.dll");
		PEHeaders headers = PEHeaders(buffer.GetBuffer());

		Assert::AreEqual<uint64_t>(headers.m_Base, 0x0000000180000000);
		Assert::AreEqual<uint32_t>(headers.m_Sections.m_Size, 0x7);
		Assert::AreEqual<uint32_t>(headers.m_Imports.m_Size, 0x60);
		Assert::AreEqual<uint32_t>(headers.m_Exports.m_Entries.m_Size, 0x662);
		Assert::AreEqual<uint32_t>(headers.m_Relocations.m_Size, 0x5);
		Assert::AreEqual<uint32_t>(headers.m_DelayImports.m_Size, 0x2);
	}

	TEST_METHOD(Constructor_32bit)
	{
		PEBuffer buffer = PEBuffer(L"kernel32_32bit.dll");
		PEHeaders headers = PEHeaders(buffer.GetBuffer());

		Assert::AreEqual<uint64_t>(headers.m_Base, 0x000000006b800000);
		Assert::AreEqual<uint32_t>(headers.m_Sections.m_Size, 0x6);
		Assert::AreEqual<uint32_t>(headers.m_Imports.m_Size, 0x5f);
		Assert::AreEqual<uint32_t>(headers.m_Exports.m_Entries.m_Size, 0x648);
		Assert::AreEqual<uint32_t>(headers.m_Relocations.m_Size, 0x6b);
		Assert::AreEqual<uint32_t>(headers.m_DelayImports.m_Size, 0x2);
	}
};