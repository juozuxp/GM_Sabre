#pragma once
#include <string>
#include <ArchitectureIL.hpp>

struct PCVariable
{
	enum class Type
	{
		None,
		Local,
		Static
	};

	Type m_Type;
	std::string m_Name;
	ILOperandScale m_Size;
};