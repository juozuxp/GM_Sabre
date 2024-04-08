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
	const void* GetBuffer() const;

	const IMAGE_DOS_HEADER& GetDosHeader() const;

	const IMAGE_FILE_HEADER& GetFileHeader() const;

	const IMAGE_NT_HEADERS32& GetNtHeaders32() const;
	const IMAGE_NT_HEADERS64& GetNtHeaders64() const;

	const IMAGE_OPTIONAL_HEADER32& GetOptionalHeader32() const;
	const IMAGE_OPTIONAL_HEADER64& GetOptionalHeader64() const;

	PEHeaders* GetHeaders() const;

private:
	void* m_Buffer = nullptr;
};