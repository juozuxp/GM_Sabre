#include "PEImportEntry.hpp"

PEImportEntry::PEImportEntry(uint16_t ordinal, uint32_t functionAddressBase)
{
	m_Ordinal = ordinal;
	m_FunctionAddressBase = functionAddressBase;
}

PEImportEntry::PEImportEntry(const IMAGE_IMPORT_BY_NAME* name, uint32_t functionAddressBase)
{
	m_Ordinal = name->Hint;
	m_FunctionAddressBase = functionAddressBase;

	m_Name = ManagedString(name->Name);
}