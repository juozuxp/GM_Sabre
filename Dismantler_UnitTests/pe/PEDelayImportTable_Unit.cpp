#include "pch.h"
#include <CppUnitTest.h>
#include <pe/PEDelayImportTable.hpp>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

TEST_CLASS(PEDelayImportTable_Unit)
{
public:
	TEST_METHOD(Constructor)
	{
		uint8_t* pages = new uint8_t[2 << 12];

		memset(pages, 0, 2 << 12);

		IMAGE_DOS_HEADER* dos = reinterpret_cast<IMAGE_DOS_HEADER*>(pages);

		dos->e_lfanew = 0x20;

		IMAGE_NT_HEADERS64* nt64 = reinterpret_cast<IMAGE_NT_HEADERS64*>(pages + dos->e_lfanew);

		nt64->FileHeader.SizeOfOptionalHeader = sizeof(nt64->OptionalHeader);

		IMAGE_DELAYLOAD_DESCRIPTOR* import = reinterpret_cast<IMAGE_DELAYLOAD_DESCRIPTOR*>(pages + 0x100);

		import->ImportNameTableRVA = 0x200;
		import->ImportAddressTableRVA = 0x250;
		import->TimeDateStamp = 0;

		import->DllNameRVA = 0x300;

		memcpy(pages + import->DllNameRVA, "import name", sizeof("import name"));

		IMAGE_THUNK_DATA64 * firstThunk64 = reinterpret_cast<IMAGE_THUNK_DATA64*>(pages + import->ImportNameTableRVA);
		IMAGE_THUNK_DATA64 * originalThunk64 = reinterpret_cast<IMAGE_THUNK_DATA64*>(pages + import->ImportAddressTableRVA);

		firstThunk64->u1.AddressOfData = IMAGE_ORDINAL_FLAG64 | 10;
		(firstThunk64 + 1)->u1.AddressOfData = 0x500;

		IMAGE_IMPORT_BY_NAME* name = reinterpret_cast<IMAGE_IMPORT_BY_NAME*>(pages + (firstThunk64 + 1)->u1.AddressOfData);

		memcpy(name->Name, "hello world", sizeof("hello world"));

		PEDelayImportTable table = PEDelayImportTable(pages, import);

		Assert::IsTrue(memcmp(&table.m_Descriptor, import, sizeof(table.m_Descriptor)) == 0);

		Assert::AreEqual<uint32_t>(table.m_Entries.m_Size, 2);

		Assert::AreEqual<uint32_t>(table.m_Name.m_Size, sizeof("import name") - 1);
		Assert::IsTrue(wcsncmp(table.m_Name.m_Buffer, L"import name", sizeof("import name") - 1) == 0);

		IMAGE_NT_HEADERS32* nt32 = reinterpret_cast<IMAGE_NT_HEADERS32*>(pages + dos->e_lfanew);

		nt32->FileHeader.SizeOfOptionalHeader = sizeof(nt32->OptionalHeader);

		IMAGE_THUNK_DATA32 * firstThunk32 = reinterpret_cast<IMAGE_THUNK_DATA32*>(pages + import->ImportNameTableRVA);
		IMAGE_THUNK_DATA32 * originalThunk32 = reinterpret_cast<IMAGE_THUNK_DATA32*>(pages + import->ImportAddressTableRVA);

		firstThunk32->u1.AddressOfData = IMAGE_ORDINAL_FLAG32 | 10;
		(firstThunk32 + 1)->u1.AddressOfData = 0x500;
		(firstThunk32 + 2)->u1.AddressOfData = 0;

		table = PEDelayImportTable(pages, import);

		Assert::IsTrue(memcmp(&table.m_Descriptor, import, sizeof(table.m_Descriptor)) == 0);

		Assert::AreEqual<uint32_t>(table.m_Entries.m_Size, 2);

		Assert::AreEqual<uint32_t>(table.m_Name.m_Size, sizeof("import name") - 1);
		Assert::IsTrue(wcsncmp(table.m_Name.m_Buffer, L"import name", sizeof("import name") - 1) == 0);
	}
};