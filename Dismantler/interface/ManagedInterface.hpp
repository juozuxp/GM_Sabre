#pragma once
#include <Disassembler.hpp>

class ManagedInterface
{
public:
	template<typename T>
	struct ManagedArray
	{
	public:
		ManagedArray(std::vector<T> array)
		{
			m_Size = array.size();
			m_Array = new T[m_Size];

			memcpy(m_Array, array.data(), m_Size * sizeof(T));
		}

		void Free()
		{
			delete[] m_Array;
		}

	private:
		T* m_Array;
		uint64_t m_Size;
	};

public:
	static void Test(ILInstruction* ins);
};