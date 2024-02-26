#pragma once
#include "managed/ManagedArray.hpp"
#include "PEImportEntry.hpp"
#include <Windows.h>

struct PEImportTable : public ManagedObject
{
	PEImportTable() = default;
	PEImportTable(const void* base, const IMAGE_IMPORT_DESCRIPTOR* descriptor);

	IMAGE_IMPORT_DESCRIPTOR m_Descriptor;

	ManagedString m_Name;
	ManagedArray<PEImportEntry> m_Entries;
};