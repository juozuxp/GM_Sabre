#include "pch.h"
#include <CppUnitTest.h>
#include <pe/PERelocationEntry.hpp>
#include <Windows.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

TEST_CLASS(PERelocationEntry_Unit)
{
public:
	TEST_METHOD(Constructor_64bit)
	{
		uint8_t* page = new uint8_t[1 << 12];

		uint16_t dir_offset = 0x500;
		uint16_t dir_info = dir_offset | (IMAGE_REL_BASED_DIR64 << 12);

		*reinterpret_cast<uint64_t*>(page + dir_offset) = 0x1122334455667788;

		PERelocationEntry entry = PERelocationEntry(page, dir_info);

		Assert::AreEqual(entry.m_Offset, dir_offset);
		Assert::AreEqual<uint8_t>(entry.m_Type, IMAGE_REL_BASED_DIR64);
		Assert::AreEqual<uint64_t>(entry.m_Value, 0x1122334455667788);

		delete[] page;
	}

	TEST_METHOD(Constructor_32high)
	{
		uint8_t* page = new uint8_t[1 << 12];

		uint16_t dir_offset = 0x100;
		uint16_t dir_info = dir_offset | (IMAGE_REL_BASED_HIGH << 12);

		*reinterpret_cast<uint32_t*>(page + dir_offset) = 0xAABBCCDD;

		PERelocationEntry entry = PERelocationEntry(page, dir_info);

		Assert::AreEqual(entry.m_Offset, dir_offset);
		Assert::AreEqual<uint8_t>(entry.m_Type, IMAGE_REL_BASED_HIGH);
		Assert::AreEqual<uint64_t>(entry.m_Value, 0xAABBCCDD);

		delete[] page;
	}

	TEST_METHOD(Constructor_32low)
	{
		uint8_t* page = new uint8_t[1 << 12];

		uint16_t dir_offset = 0x100;
		uint16_t dir_info = dir_offset | (IMAGE_REL_BASED_LOW << 12);

		*reinterpret_cast<uint32_t*>(page + dir_offset) = 0xAABBCCDD;

		PERelocationEntry entry = PERelocationEntry(page, dir_info);

		Assert::AreEqual(entry.m_Offset, dir_offset);
		Assert::AreEqual<uint8_t>(entry.m_Type, IMAGE_REL_BASED_LOW);
		Assert::AreEqual<uint64_t>(entry.m_Value, 0xAABBCCDD);

		delete[] page;
	}

	TEST_METHOD(Constructor_32highlow)
	{
		uint8_t* page = new uint8_t[1 << 12];

		uint16_t dir_offset = 0x100;
		uint16_t dir_info = dir_offset | (IMAGE_REL_BASED_HIGHLOW << 12);

		*reinterpret_cast<uint32_t*>(page + dir_offset) = 0xAABBCCDD;

		PERelocationEntry entry = PERelocationEntry(page, dir_info);

		Assert::AreEqual(entry.m_Offset, dir_offset);
		Assert::AreEqual<uint8_t>(entry.m_Type, IMAGE_REL_BASED_HIGHLOW);
		Assert::AreEqual<uint64_t>(entry.m_Value, 0xAABBCCDD);

		delete[] page;
	}

	TEST_METHOD(Constructor_32highadj)
	{
		uint8_t* page = new uint8_t[1 << 12];

		uint16_t dir_offset = 0x100;
		uint16_t dir_info = dir_offset | (IMAGE_REL_BASED_HIGHADJ << 12);

		*reinterpret_cast<uint32_t*>(page + dir_offset) = 0xAABBCCDD;

		PERelocationEntry entry = PERelocationEntry(page, dir_info);

		Assert::AreEqual(entry.m_Offset, dir_offset);
		Assert::AreEqual<uint8_t>(entry.m_Type, IMAGE_REL_BASED_HIGHADJ);
		Assert::AreEqual<uint64_t>(entry.m_Value, 0xAABBCCDD);

		delete[] page;
	}

	TEST_METHOD(Constructor_absolute)
	{
		uint8_t* page = new uint8_t[1 << 12];

		uint16_t dir_offset = 0x100;
		uint16_t dir_info = dir_offset | (IMAGE_REL_BASED_ABSOLUTE << 12);

		*reinterpret_cast<uint32_t*>(page + dir_offset) = 0xAABBCCDD;

		PERelocationEntry entry = PERelocationEntry(page, dir_info);

		Assert::AreEqual(entry.m_Offset, dir_offset);
		Assert::AreEqual<uint8_t>(entry.m_Type, IMAGE_REL_BASED_ABSOLUTE);

		delete[] page;
	}
};