#include "pch.h"
#include <CppUnitTest.h>
#include <explorer/XRefExplorer.hpp>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

TEST_CLASS(XRefExplorer_Unit)
{
public:
	TEST_METHOD(Construct_64bit)
	{
		PEBuffer pe = PEBuffer(L"kernel32.dll");
		XRefExplorer xref = XRefExplorer(pe);
	}

	TEST_METHOD(Construct_32bit)
	{
		PEBuffer pe = PEBuffer(L"kernel32_32bit.dll");
		XRefExplorer xref = XRefExplorer(pe);
	}

	TEST_METHOD(GetXReferences_64bit)
	{
		PEBuffer pe = PEBuffer(L"kernel32.dll");
		XRefExplorer xref = XRefExplorer(pe);

		xref.GetXReferences(0);
	}

	TEST_METHOD(GetXReferences_32bit)
	{
		PEBuffer pe = PEBuffer(L"kernel32_32bit.dll");
		XRefExplorer xref = XRefExplorer(pe);

		xref.GetXReferences(0);
	}
};