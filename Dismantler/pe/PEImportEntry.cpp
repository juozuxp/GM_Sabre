#include "PEImportEntry.hpp"

PEImportEntry::PEImportEntry(uint16_t ordinal, uint32_t firstThunk, uint32_t originalFirstThunk)
{
	m_Ordinal = ordinal;
	m_FirstThunk = firstThunk;
	m_OriginalFirstThunk = originalFirstThunk;
}

PEImportEntry::PEImportEntry(const IMAGE_IMPORT_BY_NAME* name, uint32_t firstThunk, uint32_t originalFirstThunk)
{
	m_Ordinal = name->Hint;
	m_FirstThunk = firstThunk;
	m_OriginalFirstThunk = originalFirstThunk;

	m_Name = ManagedString(name->Name);
}