#pragma once
#include "KaraOperand.hpp"

struct KaraInstruction
{
	enum class Type
	{
		None,
		Assign,
		Subtract,
		Addition,
		Compare,
		AndCompare,
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
	KaraOperand m_Operands[2];
};