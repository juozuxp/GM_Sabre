#pragma once
#include <stdint.h>

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
		Static,
		Argument
	};

	Type m_Type;
	Size m_Size;

	union
	{
		uint64_t m_Static;
		uint32_t m_Argument;
	};
};