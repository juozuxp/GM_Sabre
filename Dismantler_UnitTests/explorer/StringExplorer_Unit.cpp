#include "pch.h"
#include <CppUnitTest.h>
#include <explorer/StringExplorer.hpp>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

TEST_CLASS(StringExplorer_Unit)
{
public:
	TEST_METHOD(Construct)
	{
		PEBuffer pe = PEBuffer(L"kernel32.dll");
		XRefExplorer xref = XRefExplorer(pe);
		StringExplorer strings = StringExplorer(pe, xref);

		pe = PEBuffer(L"kernel32_32bit.dll");
		xref = XRefExplorer(pe);
		strings = StringExplorer(pe, xref);
	}

	TEST_METHOD(ExploreExecutable)
	{
		PEBuffer pe = PEBuffer(L"kernel32.dll");
		XRefExplorer xref = XRefExplorer(pe);
		StringExplorer strings = StringExplorer(pe, xref);

		strings.ExploreExecutable();

		pe = PEBuffer(L"kernel32_32bit.dll");
		xref = XRefExplorer(pe);
		strings = StringExplorer(pe, xref);

		strings.ExploreExecutable();
	}
};