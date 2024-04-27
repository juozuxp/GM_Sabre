#pragma once
#include <unordered_set>
#include <unordered_map>
#include <string>
#include <vector>
#include <Disassembler.hpp>

#include "pe/PEBuffer.hpp"
#include "utility/LowTrustString.hpp"
#include "XRefExplorer.hpp"

class StringExplorer
{
public:
	struct Entry
	{
		bool m_IsWide;
		uintptr_t m_Base;

		std::wstring m_String;
		std::vector<XRefExplorer::Entry> m_XRefs;
	};

public:
	StringExplorer() = default;
	StringExplorer(const PEBuffer& buffer, const XRefExplorer& xref);

public:
	std::vector<Entry> ExploreExecutable();

private:
	void SearchWide(std::vector<Entry>& entries);
	void SearchASCII(std::vector<Entry>& entries);

private:
	const PEBuffer* m_Buffer;
	const XRefExplorer* m_XRefExplorer;

	LowTrustString m_LowTrustString;
	std::unordered_set<const void*> m_Explored;
};