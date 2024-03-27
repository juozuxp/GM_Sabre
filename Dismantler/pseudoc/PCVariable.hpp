#pragma once
#include <string>
#include <ArchitectureIL.hpp>

struct PCVariable
{
	enum class Type
	{
		None,
		Local,
		Static,
		Argument
	};

	Type m_Type;
	std::string m_Name;

	uint32_t m_Bind; // 0 - no bind, 1 - return, 1< - argument
	ILOperandScale m_Size;
};