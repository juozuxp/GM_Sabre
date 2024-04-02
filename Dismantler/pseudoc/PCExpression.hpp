#pragma once
#include <vector>

class PCExpression
{
	enum class Operator : uint32_t
	{
		Add,
		Sub
	};

	struct Entry
	{
		bool m_IsLiteral;

		union
		{
			uint32_t m_Literal;
			uint32_t m_Variable;
			Operator m_Operator;
		};
	};

	std::vector<Entry> m_Expression;
};

