#pragma once
#include "PCOperand.hpp"

struct PCInstruction
{
	enum class Type
	{
		None,
		Assign,
		Subtract,
		Addition,
		Xor,
		Or,
		And,
		Not,
		Invoke,
		Return
	};

	Type m_Type;
	PCOperand m_Operands[2];
};