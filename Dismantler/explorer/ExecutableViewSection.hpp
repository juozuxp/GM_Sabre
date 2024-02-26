#pragma once
#include "managed/ManagedArray.hpp"
#include <Disassembler.hpp>
#include <Visualizer.hpp>

struct ExecutableViewSection : public ManagedObject
{
	enum class Type
	{
		Bytes,
		Code
	};

	Type m_Type = Type::Bytes;

	uint32_t m_Start;
	uint32_t m_Size;

	ManagedArray<Visualizer::Visual> m_Visuals;
};