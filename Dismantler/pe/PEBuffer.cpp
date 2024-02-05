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

	m_Buffer = malloc(size);
	if (m_Buffer == nullptr)
	{
		CloseHandle(handle);
		return;
	}

	if (!ReadFile(handle, m_Buffer, size, nullptr, nullptr))
	{
		CloseHandle(handle);
		free(m_Buffer);
		return;
	}

	CloseHandle(handle);
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

PEHeaders* PEBuffer::GetHeaders()
{
	return new PEHeaders(m_Buffer);
}