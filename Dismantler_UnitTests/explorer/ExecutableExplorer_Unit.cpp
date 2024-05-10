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

	TEST_METHOD(GetHeaders_64bit)
	{
		ExecutableExplorer explorer = ExecutableExplorer(L"kernel32.dll");

		delete explorer.GetHeaders();
	}

	TEST_METHOD(GetHeaders_nofile)
	{
		ExecutableExplorer explorer = ExecutableExplorer(L"NonDll.dll");

		Assert::AreEqual<const void*>(explorer.GetHeaders(), nullptr);
	}

	TEST_METHOD(GetExecutableView_64bit)
	{
		ExecutableExplorer explorer = ExecutableExplorer(L"kernel32.dll");

		delete explorer.GetExecutableView();
	}

	TEST_METHOD(GetExecutableView_nofile)
	{
		ExecutableExplorer explorer = ExecutableExplorer(L"NonDll.dll");

		Assert::AreEqual<const void*>(explorer.GetExecutableView(), nullptr);
	}

	TEST_METHOD(GetPCFunction_64bit)
	{
		ExecutableExplorer explorer = ExecutableExplorer(L"kernel32.dll");

		delete explorer.GetPCFunction(0x1800112B0);
	}

	TEST_METHOD(GetPCFunction_nofile)
	{
		ExecutableExplorer explorer = ExecutableExplorer(L"NonDll.dll");

		Assert::AreEqual<const void*>(explorer.GetPCFunction(0x1800112B0), nullptr);
	}

	TEST_METHOD(GetExecutableFunctions_64bit)
	{
		ExecutableExplorer explorer = ExecutableExplorer(L"kernel32.dll");

		delete explorer.GetExecutableFunctions();
	}

	TEST_METHOD(GetExecutableFunctions_nofile)
	{
		ExecutableExplorer explorer = ExecutableExplorer(L"NonDll.dll");

		Assert::AreEqual<const void*>(explorer.GetExecutableFunctions(), nullptr);
	}

	TEST_METHOD(GetExecutableStrings_64bit)
	{
		ExecutableExplorer explorer = ExecutableExplorer(L"kernel32.dll");

		delete explorer.GetExecutableStrings();
	}

	TEST_METHOD(GetExecutableStrings_nofile)
	{
		ExecutableExplorer explorer = ExecutableExplorer(L"NonDll.dll");

		Assert::AreEqual<const void*>(explorer.GetExecutableStrings(), nullptr);
	}

	TEST_METHOD(GetAddressXRefs_64bit)
	{
		ExecutableExplorer explorer = ExecutableExplorer(L"kernel32.dll");

		delete explorer.GetAddressXRefs(0x180011228);
	}

	TEST_METHOD(GetAddressXRefs_nofile)
	{
		ExecutableExplorer explorer = ExecutableExplorer(L"NonDll.dll");

		Assert::AreEqual<const void*>(explorer.GetAddressXRefs(0x180011228), nullptr);
	}

	TEST_METHOD(GetBuffer)
	{
		ExecutableExplorer explorer = ExecutableExplorer(L"kernel32.dll");

		explorer.GetBuffer();
	}
};