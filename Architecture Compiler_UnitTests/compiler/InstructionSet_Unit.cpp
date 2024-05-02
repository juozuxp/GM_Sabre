#include "pch.h"
#include "utility/Setup.hpp"
#include <compiler/BytePackage.hpp>
#include <compiler/Instruction.hpp>
#include <compiler/FullRedirection.hpp>
#include <Windows.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

TEST_CLASS(Instruction_Unit)
{
public:
	TEST_METHOD(Get)
	{
		FullRedirection redirect = FullRedirection();

		Assert::AreEqual(redirect.GetPackageType(), ByteEntry::PackageType::FullRedirection);

		std::shared_ptr<Instruction> entries[256];

		for (uint32_t i = 0; i < ARRAYSIZE(entries); i++)
		{
			entries[i] = std::shared_ptr<Instruction>();
			redirect.Get(i) = entries[i];
		}

		for (uint32_t i = 0; i < ARRAYSIZE(entries); i++)
		{
			Assert::AreEqual<const void*>(redirect.Get(i).get(), entries[i].get());
		}
	}
};