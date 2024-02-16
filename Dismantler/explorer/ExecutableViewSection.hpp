#pragma once
#include "managed/ManagedGenericArray.hpp"
#include <Disassembler.hpp>
#include <Visualizer.hpp>

struct ExecutableViewSection
{
	enum class Type
	{
		Bytes,
		Code
	};

	Type m_Type = Type::Bytes;

	uint32_t m_Start;
	uint32_t m_Size;

	ManagedGenericArray<Visualizer::Visual> m_Visuals;
};