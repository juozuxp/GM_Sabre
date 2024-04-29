#pragma once
#include "ManagedObject.hpp"
#include <utility>
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

	ManagedArray(const std::vector<T>& array)
	{
		m_Size = array.size();

		Reserve(m_Size);

		for (uint32_t i = 0; i < array.size(); i++)
		{
			m_Array[i] = array[i];
		}
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

		delete[] m_Array;
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

		T* newArray = new T[capacity];
		if (newArray == nullptr)
		{
			return;
		}

		m_Capacity = capacity;
		if (m_Array != nullptr)
		{
			for (uint32_t i = 0; i < m_Size; i++)
			{
				newArray[i] = std::move(m_Array[i]);
			}

			std::swap(newArray, m_Array);

			delete[] newArray;
		}
		else
		{
			m_Array = newArray;
		}
	}

private:
	T* m_Array = nullptr;

	uint32_t m_Size = 0;
	uint32_t m_Capacity = 0;

private:
	friend class ManagedArray_Unit;
	friend class PERelocationTable_Unit;
	friend class PEImportTable_Unit;
	friend class PEExportTable_Unit;
	friend class PEDelayImportTable_Unit;
	friend class PEHeaders_Unit;
};

