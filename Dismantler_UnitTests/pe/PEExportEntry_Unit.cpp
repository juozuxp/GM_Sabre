#include "pch.h"
#include <CppUnitTest.h>
#include <pe/PEExportEntry.hpp>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

TEST_CLASS(PEExportEntry_Unit)
{
public:
	TEST_METHOD(Constructor_nameless)
	{
		PEExportEntry entry = PEExportEntry(10, 0x255, "hello world");
		
		Assert::AreEqual<uint16_t>(entry.m_Ordinal, 10);
		Assert::AreEqual<uint32_t>(entry.m_Function, 0x255);

		Assert::AreEqual<uint32_t>(entry.m_Forwarder.m_Size, sizeof("hello world") - 1);
		Assert::IsTrue(wcsncmp(entry.m_Forwarder.m_Buffer, L"hello world", sizeof("hello world") - 1) == 0);

		Assert::AreEqual<uint32_t>(entry.m_Name.m_Size, 0);
		Assert::AreEqual<wchar_t*>(entry.m_Name.m_Buffer, nullptr);
	}

	TEST_METHOD(Constructor_named)
	{
		PEExportEntry entry = PEExportEntry(10, 0x255, "name", nullptr);

		Assert::AreEqual<uint16_t>(entry.m_Ordinal, 10);
		Assert::AreEqual<uint32_t>(entry.m_Function, 0x255);

		Assert::AreEqual<uint32_t>(entry.m_Forwarder.m_Size, 0);
		Assert::AreEqual<wchar_t*>(entry.m_Forwarder.m_Buffer, nullptr);

		Assert::AreEqual<uint32_t>(entry.m_Name.m_Size, sizeof("name") - 1);
		Assert::IsTrue(wcsncmp(entry.m_Name.m_Buffer, L"name", sizeof("name") - 1) == 0);
	}
};