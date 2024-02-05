#pragma once
#include <string_view>

#include "pe/PEBuffer.hpp"
#include "managed/ManagedObject.hpp"

class ExecutableExplorer : public ManagedObject
{
public:
	ExecutableExplorer() = default;
	ExecutableExplorer(const std::wstring_view& path);

public:
	PEHeaders* GetHeaders();

private:
	PEBuffer m_Buffer;
};