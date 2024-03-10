#include "PERelocationEntry.hpp"
#include <Windows.h>

PERelocationEntry::PERelocationEntry(const void* page, uint16_t info)
{
	m_Type = info >> 12;
	m_Offset = info & ((1 << 12) - 1);

	const void* entry = reinterpret_cast<const uint8_t*>(page) + m_Offset;
	switch (m_Type)
	{
	case IMAGE_REL_BASED_HIGH:
	case IMAGE_REL_BASED_LOW:
	case IMAGE_REL_BASED_HIGHLOW:
	case IMAGE_REL_BASED_HIGHADJ:
	{
		m_Value = *reinterpret_cast<const uint32_t*>(entry);
	} break;
	case IMAGE_REL_BASED_DIR64:
	{
		m_Value = *reinterpret_cast<const uint64_t*>(entry);
	} break;
	}
}