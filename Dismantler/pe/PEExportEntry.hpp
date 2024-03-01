#pragma once
#include <managed/ManagedString.hpp>

struct PEExportEntry : public ManagedObject
{
	PEExportEntry() = default;
	PEExportEntry(uint16_t ordinal, uint32_t function, const char* forwarder);
	PEExportEntry(uint16_t ordinal, uint32_t function, const char* name, const char* forwarder);

	uint16_t m_Ordinal;
	uint32_t m_Function;

	ManagedString m_Name;
	ManagedString m_Forwarder;
};

