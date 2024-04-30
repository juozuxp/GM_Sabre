#include "pch.h"
#include <CppUnitTest.h>
#include <explorer/ExecutableExplorer.hpp>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

TEST_CLASS(ExecutableExplorer_Unit)
{
public:
	TEST_METHOD(Construct)
	{
		ExecutableExplorer explorer = ExecutableExplorer(L"kernel32.dll");
	}

	TEST_METHOD(GetHeaders)
	{
		ExecutableExplorer explorer = ExecutableExplorer(L"kernel32.dll");

		delete explorer.GetHeaders();
	}

	TEST_METHOD(GetExecutableView)
	{
		ExecutableExplorer explorer = ExecutableExplorer(L"kernel32.dll");

		delete explorer.GetExecutableView();

		explorer = ExecutableExplorer(L"NonDll.dll");

		Assert::AreEqual<const void*>(explorer.GetExecutableView(), nullptr);
	}

	TEST_METHOD(GetPCFunction)
	{
		ExecutableExplorer explorer = ExecutableExplorer(L"kernel32.dll");

		delete explorer.GetPCFunction(0x1800112B0);

		explorer = ExecutableExplorer(L"NonDll.dll");

		Assert::AreEqual<const void*>(explorer.GetPCFunction(0x1800112B0), nullptr);
	}

	TEST_METHOD(GetExecutableFunctions)
	{
		ExecutableExplorer explorer = ExecutableExplorer(L"kernel32.dll");

		delete explorer.GetExecutableFunctions();

		explorer = ExecutableExplorer(L"NonDll.dll");

		Assert::AreEqual<const void*>(explorer.GetExecutableFunctions(), nullptr);
	}

	TEST_METHOD(GetExecutableStrings)
	{
		ExecutableExplorer explorer = ExecutableExplorer(L"kernel32.dll");

		delete explorer.GetExecutableStrings();

		explorer = ExecutableExplorer(L"NonDll.dll");

		Assert::AreEqual<const void*>(explorer.GetExecutableStrings(), nullptr);
	}

	TEST_METHOD(GetAddressXRefs)
	{
		ExecutableExplorer explorer = ExecutableExplorer(L"kernel32.dll");

		delete explorer.GetAddressXRefs(0x180011228);

		explorer = ExecutableExplorer(L"NonDll.dll");

		Assert::AreEqual<const void*>(explorer.GetAddressXRefs(0x180011228), nullptr);
	}

	TEST_METHOD(GetBuffer)
	{
		ExecutableExplorer explorer = ExecutableExplorer(L"kernel32.dll");

		explorer.GetBuffer();
	}
};