#include "pch.h"
#include <CppUnitTest.h>
#include <pe/PERelocationTable.hpp>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

TEST_CLASS(PERelocationTable_Unit)
{
public:
	TEST_METHOD(Constructor)
	{
		uint8_t* pages = new uint8_t[2 << 12];

		IMAGE_BASE_RELOCATION* relocation = reinterpret_cast<IMAGE_BASE_RELOCATION*>(pages);

		relocation->VirtualAddress = 1 << 12;
		relocation->SizeOfBlock = sizeof(IMAGE_BASE_RELOCATION) + sizeof(uint16_t) * 2;

		uint16_t* infos = reinterpret_cast<uint16_t*>(relocation + 1);

		uint16_t firstOffset = 0x780;
		uint16_t secondOffset = 0x508;

		infos[0] = firstOffset | (IMAGE_REL_BASED_DIR64 << 12);
		infos[1] = secondOffset | (IMAGE_REL_BASED_DIR64 << 12);

		*reinterpret_cast<uint64_t*>(pages + relocation->VirtualAddress + firstOffset) = 0x1122334455667788;
		*reinterpret_cast<uint64_t*>(pages + relocation->VirtualAddress + secondOffset) = 0x0123456789ABCDEF;

		PERelocationTable table = PERelocationTable(pages, relocation);

		Assert::AreEqual<uint32_t>(table.m_Relocations.m_Size, 2);
		Assert::IsTrue(memcmp(&table.m_Descriptor, relocation, sizeof(IMAGE_BASE_RELOCATION)) == 0);

		delete[] pages;
	}
};