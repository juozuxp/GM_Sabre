#include "pch.h"
#include <CppUnitTest.h>
#include <explorer/XRefExplorer.hpp>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

TEST_CLASS(XRefExplorer_Unit)
{
public:
	TEST_METHOD(Construct)
	{
		PEBuffer pe = PEBuffer(L"kernel32.dll");
		XRefExplorer xref = XRefExplorer(pe);

		pe = PEBuffer(L"kernel32_32bit.dll");
		xref = XRefExplorer(pe);
	}

	TEST_METHOD(GetXReferences)
	{
		PEBuffer pe = PEBuffer(L"kernel32.dll");
		XRefExplorer xref = XRefExplorer(pe);
	}
};