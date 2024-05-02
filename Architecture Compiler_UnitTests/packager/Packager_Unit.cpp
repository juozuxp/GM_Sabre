#include "pch.h"
#include "utility/Setup.hpp"
#include <packager/Packager.hpp>
#include <filesystem>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

TEST_CLASS(Packager_Unit)
{
public:
	TEST_METHOD(Construct)
	{
		Packager packager;

		packager.AddPackages(L"Instructions.asm64", L"Instructions_1.asm64");
		packager.AssemblePackage().OutputToHeader("packager_compiled.hpp");

		Assert::IsTrue(std::filesystem::exists("packager_compiled.hpp"));
	}
};