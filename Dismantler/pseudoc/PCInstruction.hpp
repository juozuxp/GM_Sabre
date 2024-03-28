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
		Return,

		Goto,
		GotoA,
		GotoAE,
		GotoB,
		GotoBE,
		GotoE,
		GotoG,
		GotoGE,
		GotoL,
		GotoLE,
		GotoNE,
		GotoNO,
		GotoNS,
		GotoO,
		GotoPE,
		GotoPO,
		GotoCXZ,
		GotoS,

		Goto_Start = Goto,
		Goto_End = GotoS,
	};

	Type m_Type;
	PCOperand m_Operands[2];
};