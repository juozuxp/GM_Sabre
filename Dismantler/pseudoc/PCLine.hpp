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
			PCExpression m_Left;
			PCExpression m_Right;
		} m_Assign;

		struct Conditional
		{
			PCExpression m_Left;
			PCExpression m_Right;
		} m_Conditional;
	};

	std::shared_ptr<PCLine> m_Next;
};

