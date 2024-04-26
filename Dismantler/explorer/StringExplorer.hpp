#pragma once
#include <unordered_set>
#include <string>
#include <vector>
#include <Disassembler.hpp>

#include "pe/PEBuffer.hpp"
#include "utility/LowTrustString.hpp"

class StringExplorer
{
public:
	struct Entry
	{
		bool m_IsWide;
		uintptr_t m_Base;

		std::wstring m_String;
		std::vector<uintptr_t> m_CrossReferences;
	};

public:
	StringExplorer() = default;
	StringExplorer(const PEBuffer& buffer);

public:
	std::vector<Entry> ExploreExecutable();

private:
	void SearchWide(std::vector<Entry>& entries);
	void SearchASCII(std::vector<Entry>& entries);

private:
	void FindCrossReferences(Entry& entry);

private:
	const PEBuffer* m_Buffer;
	LowTrustString m_LowTrustString;

	std::unordered_set<const void*> m_Explored;
	std::vector<std::pair<uintptr_t, uintptr_t>> m_ReferenceAddresses;
};