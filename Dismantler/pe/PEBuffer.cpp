#include "PEBuffer.hpp"

PEBuffer::PEBuffer(const std::wstring_view& path)
{
	HANDLE handle = CreateFileW(path.data(), GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (handle == INVALID_HANDLE_VALUE)
	{
		return;
	}

	DWORD size = GetFileSize(handle, nullptr);
	if (size == INVALID_FILE_SIZE)
	{
		CloseHandle(handle);
		return;
	}

	void* buffer = malloc(size);
	if (buffer == nullptr)
	{
		CloseHandle(handle);
		return;
	}

	if (!ReadFile(handle, buffer, size, nullptr, nullptr))
	{
		CloseHandle(handle);
		free(buffer);
		return;
	}

	CloseHandle(handle);

	const IMAGE_DOS_HEADER* dos = reinterpret_cast<const IMAGE_DOS_HEADER*>(buffer);
	const IMAGE_NT_HEADERS32* nt = reinterpret_cast<const IMAGE_NT_HEADERS32*>(reinterpret_cast<uintptr_t>(dos) + dos->e_lfanew);
	const IMAGE_FILE_HEADER* file = &nt->FileHeader;

	if (dos->e_magic != 'ZM')
	{
		free(buffer);
		return;
	}

	if (file->SizeOfOptionalHeader == sizeof(IMAGE_OPTIONAL_HEADER32))
	{
		const IMAGE_OPTIONAL_HEADER32* optional = &nt->OptionalHeader;

		m_Buffer = malloc(optional->SizeOfImage);
		if (m_Buffer == nullptr)
		{
			free(buffer);
			return;
		}

		memset(m_Buffer, 0, optional->SizeOfImage);
	}
	else
	{
		const IMAGE_OPTIONAL_HEADER64* optional = &reinterpret_cast<const IMAGE_NT_HEADERS64*>(nt)->OptionalHeader;

		m_Buffer = malloc(optional->SizeOfImage);
		if (m_Buffer == nullptr)
		{
			free(buffer);
			return;
		}

		memset(m_Buffer, 0, optional->SizeOfImage);
	}

	uint32_t headerSize = ~0;

	const IMAGE_SECTION_HEADER* section = IMAGE_FIRST_SECTION(nt);
	for (uint32_t i = 0; i < file->NumberOfSections; i++, section++)
	{
		if (section->SizeOfRawData == 0)
		{
			continue;
		}

		memcpy(reinterpret_cast<char*>(m_Buffer) + section->VirtualAddress, reinterpret_cast<const char*>(buffer) + section->PointerToRawData, section->SizeOfRawData);

		if (section->PointerToRawData < headerSize)
		{
			headerSize = section->PointerToRawData;
		}
	}

	if (headerSize == ~0) // Impossibility but VS won't shut up
	{
		free(m_Buffer);
		free(buffer);

		m_Buffer = nullptr;
		return;
	}

	memcpy(m_Buffer, buffer, headerSize);

	free(buffer);
}

PEBuffer::~PEBuffer()
{
	free(m_Buffer);
}

PEBuffer::PEBuffer(PEBuffer&& move)
{
	operator=(std::move(move));
}

PEBuffer& PEBuffer::operator=(PEBuffer&& move)
{
	m_Buffer = move.m_Buffer;

	move.m_Buffer = nullptr;
	return *this;
}

const void* PEBuffer::GetBuffer() const
{
	return m_Buffer;
}

PEHeaders* PEBuffer::GetHeaders() const
{
	if (m_Buffer == nullptr)
	{
		return nullptr;
	}

	return new PEHeaders(m_Buffer);
}

const IMAGE_DOS_HEADER& PEBuffer::GetDosHeader() const
{
	return *reinterpret_cast<const IMAGE_DOS_HEADER*>(m_Buffer);
}

const IMAGE_FILE_HEADER& PEBuffer::GetFileHeader() const
{
	const IMAGE_DOS_HEADER* dos = reinterpret_cast<const IMAGE_DOS_HEADER*>(m_Buffer);
	const IMAGE_NT_HEADERS32* nt = reinterpret_cast<const IMAGE_NT_HEADERS32*>(reinterpret_cast<uintptr_t>(dos) + dos->e_lfanew);
	
	return nt->FileHeader;
}

const IMAGE_NT_HEADERS32& PEBuffer::GetNtHeaders32() const
{
	const IMAGE_DOS_HEADER* dos = reinterpret_cast<const IMAGE_DOS_HEADER*>(m_Buffer);
	return *reinterpret_cast<const IMAGE_NT_HEADERS32*>(reinterpret_cast<uintptr_t>(dos) + dos->e_lfanew);
}

const IMAGE_NT_HEADERS64& PEBuffer::GetNtHeaders64() const
{
	const IMAGE_DOS_HEADER* dos = reinterpret_cast<const IMAGE_DOS_HEADER*>(m_Buffer);
	return *reinterpret_cast<const IMAGE_NT_HEADERS64*>(reinterpret_cast<uintptr_t>(dos) + dos->e_lfanew);
}

const IMAGE_OPTIONAL_HEADER32& PEBuffer::GetOptionalHeader32() const
{
	const IMAGE_DOS_HEADER* dos = reinterpret_cast<const IMAGE_DOS_HEADER*>(m_Buffer);
	const IMAGE_NT_HEADERS32* nt = reinterpret_cast<const IMAGE_NT_HEADERS32*>(reinterpret_cast<uintptr_t>(dos) + dos->e_lfanew);

	return nt->OptionalHeader;
}

const IMAGE_OPTIONAL_HEADER64& PEBuffer::GetOptionalHeader64() const
{
	const IMAGE_DOS_HEADER* dos = reinterpret_cast<const IMAGE_DOS_HEADER*>(m_Buffer);
	const IMAGE_NT_HEADERS64* nt = reinterpret_cast<const IMAGE_NT_HEADERS64*>(reinterpret_cast<uintptr_t>(dos) + dos->e_lfanew);

	return nt->OptionalHeader;
}