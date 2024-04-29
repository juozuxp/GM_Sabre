#pragma once
#include <utility>

class Object
{
public:
	Object() = default;

public:
	~Object()
	{
		m_Destroyed = true;
	}

	Object(Object&& move)
	{
		operator=(std::move(move));
	}

	Object(const Object& copy)
	{
		operator=(copy);
	}

	Object& operator=(Object&& move)
	{
		m_Move = true;
		move.m_Moved = true;

		return *this;
	}

	Object& operator=(const Object& copy)
	{
		m_Copy = true;

		return *this;
	}

public:
	bool m_Copy = false;
	bool m_Move = false;
	bool m_Moved = false;
	bool m_Destroyed = false;
};