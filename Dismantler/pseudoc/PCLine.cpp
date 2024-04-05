#include "PCLine.hpp"

PCLine::PCLine()
{
	memset(this, 0, sizeof(*this));
}

PCLine::~PCLine()
{
	switch (m_Type)
	{
	case Type::Assign:
	{
		m_Assign.~Assign();
	} break;
	case Type::Invoke:
	{
		m_Invoke.~Invoke();
	} break;
	case Type::Equal:
	case Type::NotEqual:
	case Type::Less:
	case Type::Greater:
	case Type::LessEqual:
	case Type::GreaterEqual:
	{
		m_Condition.~Condition();
	} break;
	case Type::Goto:
	{
		m_Goto.~Goto();
	} break;
	}
}