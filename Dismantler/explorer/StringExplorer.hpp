#pragma once
#include <unordered_set>
#include <unordered_map>
#include <string>
#include <vector>
#include <Disassembler.hpp>

#include "pe/PEBuffer.hpp"
#include "utility/LowTrustString.hpp"

class StringExplorer
{
public:
	enum class CrossReferenceOrigin : uint8_t
	{
		None,
		Data,
		Code
	};

	struct CrossReference
	{
		CrossReference() = default;
		CrossReference(uintptr_t base, CrossReferenceOrigin origin);

		uintptr_t m_Base;
		CrossReferenceOrigin m_Origin;
	};

	struct Entry
	{
		bool m_IsWide;
		uintptr_t m_Base;

		std::wstring m_String;
		std::vector<CrossReference> m_CrossReferences;
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
	void CollectCodeCrossReferences();
	void CollectRelocationCrossReferences();

private:
	const PEBuffer* m_Buffer;
	LowTrustString m_LowTrustString;

	std::unordered_set<const void*> m_Explored;
	std::unordered_map<uintptr_t, std::vector<CrossReference>> m_ReferenceAddresses;
};