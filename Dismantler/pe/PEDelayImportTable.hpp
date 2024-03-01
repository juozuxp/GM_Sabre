#pragma once
#include <Windows.h>
#include <managed/ManagedString.hpp>
#include "PEImportEntry.hpp"

struct PEDelayImportTable : public ManagedObject
{
	PEDelayImportTable() = default;
	PEDelayImportTable(const void* base, const IMAGE_DELAYLOAD_DESCRIPTOR* descriptor);

	IMAGE_DELAYLOAD_DESCRIPTOR m_Descriptor;

	ManagedString m_Name;
	ManagedArray<PEImportEntry> m_Entries;
};