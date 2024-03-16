#pragma once
#include "PCOperand.hpp"

struct PCInstruction
{
	enum class Type
	{
		None,
		Equal,
	};

	Type m_Type;
	union
	{
		struct
		{
			PCOperand m_Lhs;
			PCOperand m_Rhs;
		} m_Equal;
	};
};