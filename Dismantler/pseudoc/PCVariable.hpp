#pragma once

struct PCVariable
{
	enum class Size
	{
		int8,
		int16,
		int32,
		int64,
	};

	enum class Type
	{
		None,
		Local,
		Static
	};

	Type m_Type;
	Size m_Size;
};