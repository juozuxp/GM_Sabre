#pragma once
#include "ManagedArray.hpp"
#include <string>

class ManagedString : public ManagedObject
{
public:
	ManagedString() = default;

	ManagedString(const std::wstring_view& string)
	{
		m_Size = string.size();
		m_Buffer = new wchar_t[m_Size];

		memcpy(m_Buffer, string.data(), m_Size * sizeof(wchar_t));
	}

	ManagedString(const char* string)
	{
		m_Size = strlen(string);
		m_Buffer = new wchar_t[m_Size + 1];

		mbstowcs_s(nullptr, m_Buffer, m_Size + 1, string, m_Size);
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

		return *this;
	}

private:
	wchar_t* m_Buffer = nullptr;
	uint32_t m_Size = 0;
};