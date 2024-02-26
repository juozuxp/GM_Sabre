#pragma once
#include <Windows.h>

#include "managed/ManagedObject.hpp"
#include "managed/ManagedArray.hpp"

#include "PEImportTable.hpp"

struct PEHeaders : public ManagedObject
{
	PEHeaders(const void* base);

	uintptr_t m_Base;

	IMAGE_DOS_HEADER m_Dos;

	IMAGE_NT_HEADERS32 m_NT32;
	IMAGE_NT_HEADERS64 m_NT64;

	ManagedArray<PEImportTable> m_Imports;
	ManagedArray<IMAGE_SECTION_HEADER> m_Sections;
};