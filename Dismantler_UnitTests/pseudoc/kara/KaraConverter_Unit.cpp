#include "pch.h"
#include <CppUnitTest.h>
#include <pseudoc/kara/KaraConverter.hpp>
#include <explorer/FunctionExplorer.hpp>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

TEST_CLASS(KaraConverter_Unit)
{
public:
	TEST_METHOD(Construct)
	{
		PEBuffer pe = PEBuffer(L"kernel32.dll");
		KaraConverter converter = KaraConverter(pe);

		Assert::AreEqual<const void*>(&pe, converter.m_Buffer);
	}

	TEST_METHOD(Convert)
	{
		PEBuffer pe = PEBuffer(L"kernel32.dll");
		KaraConverter converter = KaraConverter(pe);
		XRefExplorer xrefs = XRefExplorer(pe);
		FunctionExplorer explorer = FunctionExplorer(pe, xrefs);

		for (FunctionExplorer::Function function : explorer.ExploreExecutable())
		{
			converter.Convert(function.m_Base);
		}
	}
};