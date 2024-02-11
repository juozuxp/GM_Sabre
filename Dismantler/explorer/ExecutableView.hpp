#pragma once
#include "managed/ManagedObject.hpp"
#include "managed/ManagedArray.hpp"
#include "pe/PEBuffer.hpp"

#include "ExecutableViewSection.hpp"
#include "FunctionExplorer.hpp"

struct ExecutableView : public ManagedObject
{
	ExecutableView(const PEBuffer& buffer, const FunctionExplorer& explorer);

	intptr_t m_BaseAddress; // image base address
	const void* m_DataAddress; // actual mapped address

	//ManagedGenericArray<ExecutableViewSection> m_Sections;
};