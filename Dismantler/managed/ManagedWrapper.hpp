#pragma once
#include "ManagedObject.hpp"

template<typename T>
class ManagedWrapper : public ManagedObject
{
public:
	ManagedWrapper(const T& element)
	{
		m_Element = element;
	}

	ManagedWrapper(T& element)
	{
		m_Element = std::move(element);
	}

private:
	T m_Element;
};