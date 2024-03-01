#pragma once
#include <managed/ManagedString.hpp>
#include <Windows.h>

#include "PEExportEntry.hpp"

struct PEExportTable : public ManagedObject
{
	PEExportTable() = default;
	PEExportTable(const void* base);

	IMAGE_EXPORT_DIRECTORY m_Descriptor;

	ManagedString m_Name;
	ManagedArray<PEExportEntry> m_Entries;
};

