#include "pch.h"
#include <CppUnitTest.h>
#include <pe/PERelocationEntry.hpp>
#include <Windows.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

TEST_CLASS(PERelocationEntry_Unit)
{
public:
	TEST_METHOD(Constructor)
	{
		uint8_t* page = new uint8_t[1 << 12];

		uint16_t dir64_offset = 0x500;
		uint16_t dir64_info = dir64_offset | (IMAGE_REL_BASED_DIR64 << 12);

		*reinterpret_cast<uint64_t*>(page + dir64_offset) = 0x1122334455667788;

		PERelocationEntry entry = PERelocationEntry(page, dir64_info);

		Assert::AreEqual(entry.m_Offset, dir64_offset);
		Assert::AreEqual<uint8_t>(entry.m_Type, IMAGE_REL_BASED_DIR64);
		Assert::AreEqual<uint64_t>(entry.m_Value, 0x1122334455667788);

		uint16_t dir32_offset = 0x100;
		uint16_t dir32_info = dir32_offset | (IMAGE_REL_BASED_HIGH << 12);

		*reinterpret_cast<uint32_t*>(page + dir32_offset) = 0xAABBCCDD;

		entry = PERelocationEntry(page, dir32_info);

		Assert::AreEqual(entry.m_Offset, dir32_offset);
		Assert::AreEqual<uint8_t>(entry.m_Type, IMAGE_REL_BASED_HIGH);
		Assert::AreEqual<uint64_t>(entry.m_Value, 0xAABBCCDD);

		dir32_info = dir32_offset | (IMAGE_REL_BASED_LOW << 12);

		entry = PERelocationEntry(page, dir32_info);

		Assert::AreEqual(entry.m_Offset, dir32_offset);
		Assert::AreEqual<uint8_t>(entry.m_Type, IMAGE_REL_BASED_LOW);
		Assert::AreEqual<uint64_t>(entry.m_Value, 0xAABBCCDD);

		dir32_info = dir32_offset | (IMAGE_REL_BASED_HIGHLOW << 12);

		entry = PERelocationEntry(page, dir32_info);

		Assert::AreEqual(entry.m_Offset, dir32_offset);
		Assert::AreEqual<uint8_t>(entry.m_Type, IMAGE_REL_BASED_HIGHLOW);
		Assert::AreEqual<uint64_t>(entry.m_Value, 0xAABBCCDD);

		dir32_info = dir32_offset | (IMAGE_REL_BASED_HIGHADJ << 12);

		entry = PERelocationEntry(page, dir32_info);

		Assert::AreEqual(entry.m_Offset, dir32_offset);
		Assert::AreEqual<uint8_t>(entry.m_Type, IMAGE_REL_BASED_HIGHADJ);
		Assert::AreEqual<uint64_t>(entry.m_Value, 0xAABBCCDD);

		dir32_info = dir32_offset;

		entry = PERelocationEntry(page, dir32_info);

		delete[] page;
	}
};