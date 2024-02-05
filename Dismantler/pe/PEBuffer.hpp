#pragma once
#include <string_view>

#include "PEHeaders.hpp"

class PEBuffer
{
public:
	PEBuffer() = default;
	PEBuffer(const std::wstring_view& path);

	~PEBuffer();

	PEBuffer(PEBuffer&& move);
	PEBuffer(const PEBuffer&) = delete;

	PEBuffer& operator=(PEBuffer&& move);
	PEBuffer& operator=(const PEBuffer&) = delete;

public:
	PEHeaders* GetHeaders();

private:
	void* m_Buffer = nullptr;
};