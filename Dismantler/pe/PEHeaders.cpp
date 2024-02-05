#include "PEHeaders.hpp"

PEHeaders::PEHeaders(const void* base)
{
	m_Dos = *reinterpret_cast<const IMAGE_DOS_HEADER*>(base);

	const IMAGE_NT_HEADERS32* nt = reinterpret_cast<const IMAGE_NT_HEADERS32*>(reinterpret_cast<intptr_t>(base) + m_Dos.e_lfanew);

	m_NT32 = *nt;
	m_NT64 = *reinterpret_cast<const IMAGE_NT_HEADERS64*>(nt);

	m_Sections = ManagedGenericArray<IMAGE_SECTION_HEADER>(nt->FileHeader.NumberOfSections);

	const IMAGE_SECTION_HEADER* section = IMAGE_FIRST_SECTION(nt);
	for (uint32_t i = 0; i < nt->FileHeader.NumberOfSections; i++, section++)
	{
		m_Sections.Add(*section);
	}
}