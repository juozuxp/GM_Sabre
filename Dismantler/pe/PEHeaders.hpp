#pragma once
#include <Windows.h>

#include "managed/ManagedObject.hpp"
#include "managed/ManagedGenericArray.hpp"

struct PEHeaders : public ManagedObject
{
	PEHeaders(const void* base);

	IMAGE_DOS_HEADER m_Dos;

	IMAGE_NT_HEADERS32 m_NT32;
	IMAGE_NT_HEADERS64 m_NT64;

	ManagedGenericArray<IMAGE_SECTION_HEADER> m_Sections;
};