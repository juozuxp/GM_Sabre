#include "pch.h"
#include <CppUnitTest.h>
#include <explorer/StringExplorer.hpp>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

TEST_CLASS(StringExplorer_Unit)
{
public:
	TEST_METHOD(Construct_64bit)
	{
		PEBuffer pe = PEBuffer(L"kernel32.dll");
		XRefExplorer xref = XRefExplorer(pe);
		StringExplorer strings = StringExplorer(pe, xref);
	}

	TEST_METHOD(Construct_32bit)
	{
		PEBuffer pe = PEBuffer(L"kernel32_32bit.dll");
		XRefExplorer xref = XRefExplorer(pe);
		StringExplorer strings = StringExplorer(pe, xref);
	}

	TEST_METHOD(ExploreExecutable_64bit)
	{
		PEBuffer pe = PEBuffer(L"kernel32.dll");
		XRefExplorer xref = XRefExplorer(pe);
		StringExplorer strings = StringExplorer(pe, xref);

		strings.ExploreExecutable();
	}

	TEST_METHOD(ExploreExecutable_32bit)
	{
		PEBuffer pe = PEBuffer(L"kernel32_32bit.dll");
		XRefExplorer xref = XRefExplorer(pe);
		StringExplorer strings = StringExplorer(pe, xref);

		strings.ExploreExecutable();
	}
};