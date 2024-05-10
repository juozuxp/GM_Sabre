#include "pch.h"
#include <CppUnitTest.h>
#include <pe/PEDelayImportTable.hpp>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

TEST_CLASS(PEDelayImportTable_Unit)
{
public:
	TEST_METHOD(Constructor_64bit)
	{
		uint8_t* pages = new uint8_t[2 << 12];

		memset(pages, 0, 2 << 12);

		IMAGE_DOS_HEADER* dos = reinterpret_cast<IMAGE_DOS_HEADER*>(pages);

		dos->e_lfanew = 0x20;

		IMAGE_NT_HEADERS64* nt = reinterpret_cast<IMAGE_NT_HEADERS64*>(pages + dos->e_lfanew);

		nt->FileHeader.SizeOfOptionalHeader = sizeof(nt->OptionalHeader);

		IMAGE_DELAYLOAD_DESCRIPTOR* import = reinterpret_cast<IMAGE_DELAYLOAD_DESCRIPTOR*>(pages + 0x100);

		import->ImportNameTableRVA = 0x200;
		import->ImportAddressTableRVA = 0x250;
		import->TimeDateStamp = 0;

		import->DllNameRVA = 0x300;

		memcpy(pages + import->DllNameRVA, "import name", sizeof("import name"));

		IMAGE_THUNK_DATA64* firstThunk = reinterpret_cast<IMAGE_THUNK_DATA64*>(pages + import->ImportNameTableRVA);
		IMAGE_THUNK_DATA64* originalThunk = reinterpret_cast<IMAGE_THUNK_DATA64*>(pages + import->ImportAddressTableRVA);

		firstThunk->u1.AddressOfData = IMAGE_ORDINAL_FLAG64 | 10;
		(firstThunk + 1)->u1.AddressOfData = 0x500;

		IMAGE_IMPORT_BY_NAME* name = reinterpret_cast<IMAGE_IMPORT_BY_NAME*>(pages + (firstThunk + 1)->u1.AddressOfData);

		memcpy(name->Name, "hello world", sizeof("hello world"));

		PEDelayImportTable table = PEDelayImportTable(pages, import);

		Assert::IsTrue(memcmp(&table.m_Descriptor, import, sizeof(table.m_Descriptor)) == 0);

		Assert::AreEqual<uint32_t>(table.m_Entries.m_Size, 2);

		Assert::AreEqual<uint32_t>(table.m_Name.m_Size, sizeof("import name") - 1);
		Assert::IsTrue(wcsncmp(table.m_Name.m_Buffer, L"import name", sizeof("import name") - 1) == 0);

		delete[] pages;
	}

	TEST_METHOD(Constructor_32bit)
	{
		uint8_t* pages = new uint8_t[2 << 12];

		memset(pages, 0, 2 << 12);

		IMAGE_DOS_HEADER* dos = reinterpret_cast<IMAGE_DOS_HEADER*>(pages);

		dos->e_lfanew = 0x20;

		IMAGE_NT_HEADERS32* nt = reinterpret_cast<IMAGE_NT_HEADERS32*>(pages + dos->e_lfanew);

		nt->FileHeader.SizeOfOptionalHeader = sizeof(nt->OptionalHeader);

		IMAGE_DELAYLOAD_DESCRIPTOR* import = reinterpret_cast<IMAGE_DELAYLOAD_DESCRIPTOR*>(pages + 0x100);

		import->ImportNameTableRVA = 0x200;
		import->ImportAddressTableRVA = 0x250;
		import->TimeDateStamp = 0;

		import->DllNameRVA = 0x300;

		memcpy(pages + import->DllNameRVA, "import name", sizeof("import name"));

		IMAGE_THUNK_DATA32* firstThunk = reinterpret_cast<IMAGE_THUNK_DATA32*>(pages + import->ImportNameTableRVA);
		IMAGE_THUNK_DATA32* originalThunk = reinterpret_cast<IMAGE_THUNK_DATA32*>(pages + import->ImportAddressTableRVA);

		firstThunk->u1.AddressOfData = IMAGE_ORDINAL_FLAG32 | 10;
		(firstThunk + 1)->u1.AddressOfData = 0x500;

		IMAGE_IMPORT_BY_NAME* name = reinterpret_cast<IMAGE_IMPORT_BY_NAME*>(pages + (firstThunk + 1)->u1.AddressOfData);

		memcpy(name->Name, "hello world", sizeof("hello world"));

		PEDelayImportTable table = PEDelayImportTable(pages, import);

		Assert::IsTrue(memcmp(&table.m_Descriptor, import, sizeof(table.m_Descriptor)) == 0);

		Assert::AreEqual<uint32_t>(table.m_Entries.m_Size, 2);

		Assert::AreEqual<uint32_t>(table.m_Name.m_Size, sizeof("import name") - 1);
		Assert::IsTrue(wcsncmp(table.m_Name.m_Buffer, L"import name", sizeof("import name") - 1) == 0);

		delete[] pages;
	}
};