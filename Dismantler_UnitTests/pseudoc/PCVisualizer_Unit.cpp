#include "pch.h"
#include <CppUnitTest.h>
#include <pseudoc/PCVisualizer.hpp>
#include <explorer/FunctionExplorer.hpp>
#include <pseudoc/PCConverter.hpp>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

TEST_CLASS(PCVisualizer_Unit)
{
public:
	TEST_METHOD(Construct)
	{
		PEBuffer pe = PEBuffer(L"Kernel32.dll");
		PCVisualizer visualizer = PCVisualizer(pe);
	}

	TEST_METHOD(Convert)
	{
		PEBuffer pe = PEBuffer(L"kernel32.dll");
		PCVisualizer visualizer = PCVisualizer(pe);
		PCConverter converter = PCConverter(pe);

		XRefExplorer xrefs = XRefExplorer(pe);
		FunctionExplorer explorer = FunctionExplorer(pe, xrefs);

		for (FunctionExplorer::Function function : explorer.ExploreExecutable())
		{
			visualizer.ToString(converter.Convert(function.m_Base));
		}
	}
};