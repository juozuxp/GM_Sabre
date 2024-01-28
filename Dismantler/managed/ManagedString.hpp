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

public:
	~ManagedString() override
	{
		if (m_Buffer == nullptr)
		{
			return;
		}

		delete[] m_Buffer;
	}

	ManagedString(ManagedString&& move)
	{
		operator=(std::move(move));
	}

	ManagedString& operator=(ManagedString&& move)
	{
		m_Size = move.m_Size;
		m_Buffer = move.m_Buffer;

		move.m_Size = 0;
		move.m_Buffer = nullptr;
	}

private:
	wchar_t* m_Buffer = nullptr;
	uint32_t m_Size = 0;
};