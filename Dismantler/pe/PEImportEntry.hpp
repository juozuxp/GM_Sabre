#pragma once
#include "managed/ManagedString.hpp"
#include <Windows.h>

struct PEImportEntry : public ManagedObject
{
	PEImportEntry() = default;
	PEImportEntry(uint16_t ordinal, uint32_t functionAddressBase);
	PEImportEntry(const IMAGE_IMPORT_BY_NAME* name, uint32_t functionAddressBase);

	uint16_t m_Ordinal;
	uint32_t m_FunctionAddressBase;

	ManagedString m_Name;
};

