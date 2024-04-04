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
	case Type::Conditional:
	{
		m_Conditional.~Conditional();
	} break;
	}
}