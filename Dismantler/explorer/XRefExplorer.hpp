#pragma once
#include "pe/PEBuffer.hpp"
#include <unordered_map>
#include <vector>

class XRefExplorer
{
public:
	enum class Origin : uint8_t
	{
		None,
		Code,
		Data
	};

	struct Entry
	{
		Entry() = default;
		Entry(uintptr_t address, Origin origin);

		Origin m_Origin;
		uintptr_t m_Address;
	};

public:
	XRefExplorer() = default;
	XRefExplorer(const PEBuffer& buffer);

public:
	std::vector<Entry> GetXReferences(uintptr_t address) const;
	
private:
	void CollectCodeXRefs(const PEBuffer& buffer);
	void CollectDataXRefs(const PEBuffer& buffer);

private:
	std::unordered_map<uintptr_t, std::vector<Entry>> m_XRefs;
};