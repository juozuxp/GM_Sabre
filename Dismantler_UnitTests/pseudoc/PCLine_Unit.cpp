#include "pch.h"
#include <CppUnitTest.h>
#include <pseudoc/PCLine.hpp>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

TEST_CLASS(PCLine_Unit)
{
public:
	TEST_METHOD(Construct)
	{
		PCLine first;
		PCLine second;

		memset(&second, 0, sizeof(second));

		Assert::IsTrue(memcmp(&first, &second, sizeof(first)) == 0);
	}
};