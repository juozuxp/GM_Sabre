#include "pch.h"
#include <CppUnitTest.h>
#include <pe/PEImportEntry.hpp>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

TEST_CLASS(PEImportEntry_Unit)
{
public:
	TEST_METHOD(Constructor_ordinal)
	{
		PEImportEntry entry = PEImportEntry(12, 0x1000);

		Assert::AreEqual<uint16_t>(entry.m_Ordinal, 12);
		Assert::AreEqual<uint32_t>(entry.m_FunctionAddressBase, 0x1000);

		Assert::AreEqual<uint32_t>(entry.m_Name.m_Size, 0);
		Assert::AreEqual<wchar_t*>(entry.m_Name.m_Buffer, nullptr);
	}

	TEST_METHOD(Constructor_name)
	{
		uint8_t* page = new uint8_t[1 << 12];

		IMAGE_IMPORT_BY_NAME* name = reinterpret_cast<IMAGE_IMPORT_BY_NAME*>(page);

		name->Hint = 855;
		memcpy(name->Name, "hello world", sizeof("hello world"));

		PEImportEntry entry = PEImportEntry(name, 0x2000);

		Assert::AreEqual<uint16_t>(entry.m_Ordinal, 855);
		Assert::AreEqual<uint32_t>(entry.m_FunctionAddressBase, 0x2000);

		Assert::AreEqual<uint32_t>(entry.m_Name.m_Size, sizeof("hello world") - 1);
		Assert::IsTrue(wcsncmp(entry.m_Name.m_Buffer, L"hello world", sizeof("hello world") - 1) == 0);
	}
};