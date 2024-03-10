#include "PERelocationTable.hpp"

PERelocationTable::PERelocationTable(const void* base, const IMAGE_BASE_RELOCATION* descriptor)
{
	m_Descriptor = *descriptor;

	uint32_t count = (descriptor->SizeOfBlock - sizeof(IMAGE_BASE_RELOCATION)) / sizeof(uint16_t);

	m_Relocations.Reserve(count);

	const uint16_t* entries = reinterpret_cast<const uint16_t*>(descriptor + 1);
	const uint8_t* page = reinterpret_cast<const uint8_t*>(base) + descriptor->VirtualAddress;
	for (uint32_t i = 0; i < count; i++, entries++)
	{
		m_Relocations.Add(PERelocationEntry(page, *entries));
	}
}