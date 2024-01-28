#pragma once
#include "ManagedObject.hpp"
#include <vector>

template<typename T>
class ManagedArray : public ManagedObject
{
public:
	ManagedArray(uint32_t capacity = 0)
	{
		if (capacity == 0)
		{
			return;
		}

		Reserve(capacity);
	}

	ManagedArray(const std::vector<T*>& array)
	{
		m_Size = array.size();

		Reserve(m_Size);

		memcpy(m_Array, array.data(), m_Size * sizeof(T*));
	}

public:
	ManagedArray(ManagedArray&& move)
	{
		operator=(std::move(move));
	}

	~ManagedArray() override
	{
		if (m_Array == nullptr)
		{
			return;
		}

		for (uint32_t i = 0; i < m_Size; i++)
		{
			delete m_Array[i];
		}

		free(m_Array);
	}

	ManagedArray& operator=(ManagedArray&& move)
	{
		m_Size = move.m_Size;
		m_Array = move.m_Array;
		m_Capacity = move.m_Capacity;

		move.m_Size = 0;
		move.m_Capacity = 0;
		move.m_Array = nullptr;

		return *this;
	}

public:
	void Add(T* element)
	{
		if (m_Size >= m_Capacity)
		{
			Reserve(m_Size + 1);
		}

		m_Array[m_Size++] = element;
	}

	void Reserve(uint32_t capacity)
	{
		if (capacity < m_Capacity)
		{
			return;
		}

		T** preAlloc = m_Array;

		m_Array = reinterpret_cast<T**>(realloc(m_Array, (capacity * sizeof(T*))));
		if (m_Array == nullptr)
		{
			m_Array = preAlloc;
			return;
		}

		m_Capacity = capacity;
	}

private:
	T** m_Array = nullptr;

	uint32_t m_Size = 0;
	uint32_t m_Capacity = 0;
};