#pragma once
#include "ManagedObject.hpp"
#include <utility>
#include <vector>

template<typename T>
class ManagedGenericArray : public ManagedObject
{
public:
	ManagedGenericArray(uint32_t capacity = 0)
	{
		if (capacity == 0)
		{
			return;
		}

		Reserve(capacity);
	}

	ManagedGenericArray(const std::vector<T>& array)
	{
		m_Size = array.size();

		Reserve(m_Size);

		for (uint32_t i = 0; i < array.size(); i++)
		{
			m_Array[i] = array[i];
		}
	}

public:
	ManagedGenericArray(ManagedGenericArray&& move)
	{
		operator=(std::move(move));
	}

	~ManagedGenericArray() override
	{
		if (m_Array == nullptr)
		{
			return;
		}

		for (uint32_t i = 0; i < m_Size; i++)
		{
			m_Array[i].~T();
		}

		free(m_Array);
	}

	ManagedGenericArray& operator=(ManagedGenericArray&& move)
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
	void Add(const T& element)
	{
		if (m_Size >= m_Capacity)
		{
			Reserve(m_Size + 1);
		}

		m_Array[m_Size++] = element;
	}
	
	void Add(T&& element)
	{
		if (m_Size >= m_Capacity)
		{
			Reserve(m_Size + 1);
		}

		m_Array[m_Size++] = std::move(element);
	}

	void Reserve(uint32_t capacity)
	{
		if (capacity < m_Capacity)
		{
			return;
		}

		T* preAlloc = m_Array;

		m_Array = reinterpret_cast<T*>(realloc(m_Array, (capacity * sizeof(T))));
		if (m_Array == nullptr)
		{
			m_Array = preAlloc;
			return;
		}

		memset(m_Array + m_Capacity, 0, ((capacity - m_Capacity) * sizeof(T)));

		m_Capacity = capacity;
	}

private:
	T* m_Array = nullptr;

	uint32_t m_Size = 0;
	uint32_t m_Capacity = 0;
};

