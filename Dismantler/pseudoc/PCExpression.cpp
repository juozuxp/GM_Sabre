#include "PCExpression.hpp"

PCExpression::PCExpression()
{
	memset(this, 0, sizeof(*this));
}

PCExpression::~PCExpression()
{
	switch (m_Type)
	{
	case Type::Literal:
	{
		m_Literal.~uint32_t();
	} break;
	case Type::Variable:
	{
		m_Variable.~uint32_t();
	} break;
	case Type::Operation:
	{
		m_Operation.~Operation();
	} break;
	case Type::Dereference:
	{
		m_Dereference.~unique_ptr();
	} break;
	}
}

PCExpression::PCExpression(PCExpression&& move)
{
	operator=(std::move(move));
}

PCExpression& PCExpression::operator=(PCExpression&& move)
{
	switch (move.m_Type)
	{
	case Type::Literal:
	{
		m_Literal = move.m_Literal;
	} break;
	case Type::Variable:
	{
		m_Variable = move.m_Variable;
	} break;
	case Type::Operation:
	{
		m_Operation = std::move(move.m_Operation);
	} break;
	case Type::Dereference:
	{
		m_Dereference = std::move(move.m_Dereference);
	} break;
	}

	m_Type = move.m_Type;
	move.m_Type = Type::None;
	return *this;
}