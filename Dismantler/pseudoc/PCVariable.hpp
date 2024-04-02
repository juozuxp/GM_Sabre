#pragma once

struct PCVariable
{
	enum class Size
	{
		uint8,
		uint16,
		uint32,
		uint64,
	};

	enum class Type
	{
		None,
		Local,
		Static
	};

	Size m_Type;
};

