#include "PCLine.hpp"

PCLine::PCLine() :
	m_Type(Type::None)
{
}

PCLine::~PCLine()
{
	switch (m_Type)
	{
	case Type::Assign:
	{
		m_Assign.~Assign();
	} break;
	case Type::Conditional:
	{
		m_Conditional.~Conditional();
	} break;
	}
}