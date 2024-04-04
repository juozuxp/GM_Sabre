#pragma once
#include <vector>
#include "PCExpression.hpp"

struct PCLine
{
	enum class Type
	{
		None,
		Assign,
		Invoke,
		Return,
		Equal,
		NotEqual,
		Less,
		Greater,
		LessEqual,
		GreaterEqual
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

		struct Invoke
		{
			PCExpression m_Result;
			PCExpression m_Function;
			std::vector<PCExpression> m_Arguments;
		} m_Invoke;

		struct Return
		{
			PCExpression m_Result;
		} m_Return;

		struct Condition
		{
			PCExpression m_Left;
			PCExpression m_Right;

			std::shared_ptr<PCLine> m_Else;
		} m_Condition;
	};

	std::shared_ptr<PCLine> m_Next;
};

