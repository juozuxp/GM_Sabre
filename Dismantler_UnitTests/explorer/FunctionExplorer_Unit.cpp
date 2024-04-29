#include "pch.h"
#include <CppUnitTest.h>
#include <explorer/FunctionExplorer.hpp>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

TEST_CLASS(FunctionExplorer_Unit)
{
public:
	TEST_METHOD(Construct)
	{
		PEBuffer pe = PEBuffer(L"kernel32.dll");
		XRefExplorer xref = XRefExplorer(pe);
		FunctionExplorer functions = FunctionExplorer(pe, xref);
	}

	TEST_METHOD(ExploreExecutable)
	{
		PEBuffer pe = PEBuffer(L"kernel32.dll");
		XRefExplorer xref = XRefExplorer(pe);
		FunctionExplorer functions = FunctionExplorer(pe, xref);

		functions.ExploreExecutable();

		pe = PEBuffer(L"kernel32_32bit.dll");
		xref = XRefExplorer(pe);
		functions = FunctionExplorer(pe, xref);

		functions.ExploreExecutable();
	}
};