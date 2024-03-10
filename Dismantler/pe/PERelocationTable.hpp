#pragma once
#include <Windows.h>
#include <managed/ManagedArray.hpp>

#include "PERelocationEntry.hpp"

struct PERelocationTable : public ManagedObject
{
	PERelocationTable() = default;
	PERelocationTable(const void* base, const IMAGE_BASE_RELOCATION* descriptor);

	IMAGE_BASE_RELOCATION m_Descriptor;

	ManagedArray<PERelocationEntry> m_Relocations;
};

