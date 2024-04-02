#pragma once
#include <vector>
#include "PCExpression.hpp"

struct PCLine
{
	enum class Type
	{
		None,
		Conditional,
		Assign
	};

	PCLine();
	~PCLine();

	Type m_Type;

	union
	{
		struct Assign
		{
			uint32_t m_Variable;
			PCExpression m_Expression;
		} m_Assign;
		struct Conditional
		{
			PCExpression m_Left;
			PCExpression m_Right;
		} m_Conditional;
	};
};

