#pragma once
#include <string>
#include <ArchitectureIL.hpp>

struct KaraVariable
{
	enum class Type
	{
		None,
		Local,
		Static,
		Result,
		Argument
	};

	Type m_Type;
	uint32_t m_Bind; // 0 - no bind, 1 - return, 1< - argument
	ILOperandScale m_Size;

	union
	{
		uint32_t m_Index; // for arguments
		uint64_t m_Address; // for static
	};
};