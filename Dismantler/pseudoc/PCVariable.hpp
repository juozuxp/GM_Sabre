#pragma once
#include <string>

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
};