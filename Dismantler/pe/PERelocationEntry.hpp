#pragma once
#include <stdint.h>

struct PERelocationEntry
{
	PERelocationEntry() = default;
	PERelocationEntry(const void* page, uint16_t info);

	uint8_t m_Type = 0;
	uint16_t m_Offset = 0;
	uint64_t m_Value = 0;
};

