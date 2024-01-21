#pragma once
#include "ManagedObject.hpp"
#include <vector>

template<typename T>
class ManagedArray : public ManagedObject
{
public:
	ManagedArray(const std::vector<T*>& array)
	{
		m_Size = array.size();
		m_Array = new T*[m_Size];

		memcpy(m_Array, array.data(), m_Size * sizeof(T*));
	}

	~ManagedArray() override
	{
		for (uint32_t i = 0; i < m_Size; i++)
		{
			delete m_Array[i];
		}

		delete[] m_Array;
	}

private:
	T** m_Array;
	uint32_t m_Size;
};