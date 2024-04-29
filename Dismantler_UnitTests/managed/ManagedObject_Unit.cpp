#include "pch.h"
#include <CppUnitTest.h>
#include <managed/ManagedObject.hpp>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

TEST_CLASS(ManagedObject_Unit)
{
public:
	TEST_METHOD(Constructor)
	{
		ManagedObject* object = new ManagedObject();

		Assert::AreEqual<void*>(object, object->m_Handle);
	}
};