#pragma once
#include "ManagedArray.hpp"
#include <string>

class ManagedString : public ManagedObject
{
public:
	ManagedString(const std::wstring_view& string)
	{
		m_Size = string.size();
		m_Buffer = new wchar_t[m_Size];

		memcpy(m_Buffer, string.data(), m_Size * sizeof(wchar_t));
	}

	~ManagedString() override
	{
		delete[] m_Buffer;
	}

private:
	wchar_t* m_Buffer;
	uint32_t m_Size;
};