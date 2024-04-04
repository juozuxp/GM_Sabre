#pragma once
#include <memory>

struct PCExpression
{
	enum class Expression
	{
		Add,
		Sub,
		Mul,
		Xor,
		Or,
		And,
		Not,
	};

	enum class Type
	{
		None,
		Literal,
		Variable,
		Operation,
		Dereference,
		Reference
	};

	PCExpression();
	~PCExpression();

	PCExpression(PCExpression&& move);
	PCExpression(const PCExpression& copy) = delete;

	PCExpression& operator=(PCExpression&& move);
	PCExpression& operator=(const PCExpression& copy) = delete;

	Type m_Type;

	union
	{
		uint32_t m_Literal;
		uint32_t m_Variable;

		std::unique_ptr<PCExpression> m_Dereference;

		struct Operation
		{
			Expression m_Expression;

			std::unique_ptr<PCExpression> m_Left;
			std::unique_ptr<PCExpression> m_Right;
		} m_Operation;
	};
};

