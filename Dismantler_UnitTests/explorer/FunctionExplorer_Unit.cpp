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

	TEST_METHOD(ExploreExecutable_64bit)
	{
		PEBuffer pe = PEBuffer(L"kernel32.dll");
		XRefExplorer xref = XRefExplorer(pe);
		FunctionExplorer functions = FunctionExplorer(pe, xref);

		functions.ExploreExecutable();
	}

	TEST_METHOD(ExploreExecutable_32bit)
	{
		PEBuffer pe = PEBuffer(L"kernel32_32bit.dll");
		XRefExplorer xref = XRefExplorer(pe);
		FunctionExplorer functions = FunctionExplorer(pe, xref);

		functions.ExploreExecutable();
	}
};