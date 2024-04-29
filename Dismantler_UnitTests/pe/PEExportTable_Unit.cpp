#include "pch.h"
#include <CppUnitTest.h>
#include <pe/PEExportTable.hpp>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

TEST_CLASS(PEExportTable_Unit)
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

		nt64->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].Size = 0x500;
		nt64->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress = 0x100;

		IMAGE_EXPORT_DIRECTORY* directory = reinterpret_cast<IMAGE_EXPORT_DIRECTORY*>(pages + nt64->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);

		directory->NumberOfNames = 1;
		directory->NumberOfFunctions = 2;
		
		directory->Base = 1;

		directory->AddressOfFunctions = 0x200;
		directory->AddressOfNames = 0x250;
		directory->AddressOfNameOrdinals = 0x300;

		directory->Name = 0x350;

		memcpy(pages + directory->Name, "export name", sizeof("export name"));

		uint32_t* names = reinterpret_cast<uint32_t*>(reinterpret_cast<uintptr_t>(pages) + directory->AddressOfNames);
		uint32_t* functions = reinterpret_cast<uint32_t*>(reinterpret_cast<uintptr_t>(pages) + directory->AddressOfFunctions);
		uint16_t* nameOrdinals = reinterpret_cast<uint16_t*>(reinterpret_cast<uintptr_t>(pages) + directory->AddressOfNameOrdinals);

		names[0] = 0x600;
		nameOrdinals[0] = 1;

		functions[0] = 0x450;
		functions[1] = 0x1000;

		memcpy(pages + names[0], "function name", sizeof("function name"));
		memcpy(pages + functions[1], "forward name", sizeof("forward name"));

		PEExportTable table = PEExportTable(pages);

		Assert::IsTrue(memcmp(&table.m_Descriptor, directory, sizeof(table.m_Descriptor)) == 0);

		Assert::AreEqual<uint32_t>(table.m_Entries.m_Size, 2);

		Assert::AreEqual<uint32_t>(table.m_Name.m_Size, sizeof("export name") - 1);
		Assert::IsTrue(wcsncmp(table.m_Name.m_Buffer, L"export name", sizeof("export name") - 1) == 0);

		IMAGE_NT_HEADERS32* nt32 = reinterpret_cast<IMAGE_NT_HEADERS32*>(pages + dos->e_lfanew);

		nt32->FileHeader.SizeOfOptionalHeader = sizeof(nt32->OptionalHeader);

		nt32->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].Size = 0x500;
		nt32->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress = 0x100;

		table = PEExportTable(pages);

		Assert::IsTrue(memcmp(&table.m_Descriptor, directory, sizeof(table.m_Descriptor)) == 0);

		Assert::AreEqual<uint32_t>(table.m_Entries.m_Size, 2);

		Assert::AreEqual<uint32_t>(table.m_Name.m_Size, sizeof("export name") - 1);
		Assert::IsTrue(wcsncmp(table.m_Name.m_Buffer, L"export name", sizeof("export name") - 1) == 0);

		nt32->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].Size = 0;
		nt32->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress = 0x100;

		table = PEExportTable(pages);

		delete[] pages;
	}
};