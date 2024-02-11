#pragma once
#include "managed/ManagedGenericArray.hpp"
#include <Visualizer.hpp>

struct ExecutableViewSection
{
	enum class Type
	{
		Bytes,
		Function
	};

	union
	{
		struct
		{
			uint32_t m_Start;
			uint32_t m_Size;
		} m_Bytes;

		struct
		{
			uint32_t m_Start;
			ManagedGenericArray<Visualizer::Visual> m_Visuals;
		} m_Function;
	};
};