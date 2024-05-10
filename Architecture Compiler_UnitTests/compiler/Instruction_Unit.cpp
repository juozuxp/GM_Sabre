#include "pch.h"
#include "utility/Setup.hpp"
#include <compiler/BytePackage.hpp>
#include <compiler/Instruction.hpp>
#include <compiler/Redirection.hpp>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

TEST_CLASS(Instruction_Unit)
{
public:
	TEST_METHOD(Construct_0)
	{
		constexpr Redirection::Prefix valids[] = { Redirection::Prefix::x66, Redirection::Prefix::RexW };

		Instruction instruction = Instruction(Descriptor(Token("69 -> IMUL r32|16|64, r/m32|16|64, imm32|16")), 10);

		std::vector<uint8_t> prefixes = instruction.GetCompatiblePrefixes();

		Assert::AreEqual<size_t>(prefixes.size(), 2);
		Assert::AreEqual(instruction.GetPackageType(), ByteEntry::PackageType::Instruction);

		for (uint8_t prefix : prefixes)
		{
			bool present = false;
			for (Redirection::Prefix valid : valids)
			{
				if (static_cast<Redirection::Prefix>(prefix) == valid)
				{
					present = true;
				}
			}

			Assert::IsTrue(present);
		}
	}

	TEST_METHOD(Construct_1)
	{
		Instruction instruction = Instruction(Descriptor(Token("F3 0F C7 /6 -> SENDUIPI r64")), 80);

		std::vector<uint8_t> prefixes = instruction.GetCompatiblePrefixes();

		Assert::AreEqual<size_t>(prefixes.size(), 0);
		Assert::AreEqual(instruction.GetPackageType(), ByteEntry::PackageType::Instruction);
	}

	TEST_METHOD(Construct_2)
	{
		Instruction instruction = Instruction(Descriptor(Token("DC /0 -> FADD m64")), 123);

		std::vector<uint8_t> prefixes = instruction.GetCompatiblePrefixes();

		Assert::AreEqual<size_t>(prefixes.size(), 0);
		Assert::AreEqual(instruction.GetPackageType(), ByteEntry::PackageType::Instruction);
	}

	TEST_METHOD(Construct_3)
	{
		constexpr Redirection::Prefix valids[] = { Redirection::Prefix::x66, Redirection::Prefix::RexW };

		Instruction instruction = Instruction(Descriptor(Token("6B -> IMUL r32|16|64, r/m32|16|64, imm8, RAX, RDX")), 123);

		std::vector<uint8_t> prefixes = instruction.GetCompatiblePrefixes();

		Assert::AreEqual<size_t>(prefixes.size(), 2);
		Assert::AreEqual(instruction.GetPackageType(), ByteEntry::PackageType::Instruction);

		for (uint8_t prefix : prefixes)
		{
			bool present = false;
			for (Redirection::Prefix valid : valids)
			{
				if (static_cast<Redirection::Prefix>(prefix) == valid)
				{
					present = true;
				}
			}

			Assert::IsTrue(present);
		}
	}
};