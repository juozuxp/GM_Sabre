#include "pch.h"
#include <CppUnitTest.h>
#include <pseudoc/PCConverter.hpp>
#include <explorer/FunctionExplorer.hpp>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

TEST_CLASS(PCConverter_Unit)
{
public:
	TEST_METHOD(Construct)
	{
		PEBuffer pe = PEBuffer(L"Kernel32.dll");
		PCConverter converter = PCConverter(pe);
	}

	TEST_METHOD(Convert)
	{
		PEBuffer pe = PEBuffer(L"kernel32.dll");
		PCConverter converter = PCConverter(pe);
		XRefExplorer xrefs = XRefExplorer(pe);
		FunctionExplorer explorer = FunctionExplorer(pe, xrefs);

		for (FunctionExplorer::Function function : explorer.ExploreExecutable())
		{
			converter.Convert(function.m_Base);
		}
	}
};