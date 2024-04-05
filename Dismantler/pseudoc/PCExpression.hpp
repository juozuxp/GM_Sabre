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
	PCExpression(const PCExpression& copy);

	PCExpression& operator=(PCExpression&& move);
	PCExpression& operator=(const PCExpression& copy);

	Type m_Type;

	union
	{
		uint64_t m_Literal;
		uint32_t m_Variable;

		std::unique_ptr<PCExpression> m_Dereference;

		struct Operation
		{
			Operation() = default;

			Operation(Operation&& move)
			{
				operator=(std::move(move));
			}

			Operation(const Operation& copy)
			{
				operator=(copy);
			}

			Operation& operator=(Operation&& move)
			{
				m_Expression = move.m_Expression;

				m_Left = std::move(move.m_Left);
				m_Right = std::move(move.m_Right);

				return *this;
			}

			Operation& operator=(const Operation& copy)
			{
				m_Expression = copy.m_Expression;

				m_Left = std::make_unique<PCExpression>(*copy.m_Left);
				m_Right = std::make_unique<PCExpression>(*copy.m_Right);

				return *this;
			}

			Expression m_Expression;

			std::unique_ptr<PCExpression> m_Left;
			std::unique_ptr<PCExpression> m_Right;
		} m_Operation;
	};
};

