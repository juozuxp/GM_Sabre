#include "PEExportEntry.hpp"

PEExportEntry::PEExportEntry(uint16_t ordinal, uint32_t function, const char* forwarder)
{
	m_Ordinal = ordinal;
	m_Function = function;

	if (forwarder != nullptr)
	{
		m_Forwarder = ManagedString(forwarder);
	}
}

PEExportEntry::PEExportEntry(uint16_t ordinal, uint32_t function, const char* name, const char* forwarder) :
	PEExportEntry(ordinal, function, forwarder)
{
	m_Name = ManagedString(name);
}