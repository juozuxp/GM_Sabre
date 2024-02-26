#pragma once
#include "managed/ManagedString.hpp"
#include <Windows.h>

struct PEImportEntry : public ManagedObject
{
	PEImportEntry() = default;
	PEImportEntry(uint16_t ordinal, uint32_t firstThunk, uint32_t originalFirstThunk);
	PEImportEntry(const IMAGE_IMPORT_BY_NAME* name, uint32_t firstThunk, uint32_t originalFirstThunk);

	uint16_t m_Ordinal;
	uint32_t m_FirstThunk;
	uint32_t m_OriginalFirstThunk;

	ManagedString m_Name;
};

