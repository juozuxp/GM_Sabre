#pragma once
#include <ArchitectureIL.hpp>

struct PCOperand
{
	enum class Type
	{
		None,
		Variable,
		Dereference
	};

	Type m_Type;
	union
	{
		struct
		{
			uint32_t m_Index;
		} m_Variable;
		struct
		{
			ILOperandScale m_Scale;

			uint32_t m_BaseVariable;
			uint32_t m_IndexVariable;

			int32_t m_Offset;
			uint8_t m_Multiplier;
		} m_Dereference;
	};
};