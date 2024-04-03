#include "PCExpression.hpp"

PCExpression::PCExpression() :
	m_Type(Type::None)
{
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
	case Type::Operator:
	{
		m_Operator.~Operator();
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
	switch (m_Type)
	{
	case Type::Literal:
	{
		m_Literal = move.m_Literal;
	} break;
	case Type::Variable:
	{
		m_Variable = move.m_Variable;
	} break;
	case Type::Operator:
	{
		m_Operator = std::move(move.m_Operator);
	} break;
	case Type::Dereference:
	{
		m_Dereference = std::move(move.m_Dereference);
	} break;
	}

	return *this;
}