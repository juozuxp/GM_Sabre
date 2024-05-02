#include "pch.h"
#include "utility/Setup.hpp"
#include <compiler/Operand.hpp>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

TEST_CLASS(Operand_Unit)
{
public:
	TEST_METHOD(Construct_0)
	{
		std::vector<std::string> variations;

		variations.push_back("imm32");
		variations.push_back("16");
		variations.push_back("64");

		DescriptorOperand descriptor = DescriptorOperand(variations);
		Operand operand = Operand(descriptor, false);

		const Operand::Package& package = operand.GetPackage();

		Assert::AreEqual(package.m_Type, Operand::Type::imm);
		Assert::AreEqual(package.m_Register, descriptor.GetTypeMask().m_Register);

		Assert::AreEqual(package.m_Rex, descriptor.GetFlagMask().m_Rex);
		Assert::AreEqual(package.m_Constant, descriptor.GetFlagMask().m_Constant);
		Assert::AreEqual(package.m_RegisterIndex, descriptor.GetFlagMask().m_RegisterIndex);

		Assert::AreEqual(package.m_Reg.m_Size, descriptor.GetSizeMask().m_Reg.m_Size);
		Assert::AreEqual(package.m_Reg.m_Override1, descriptor.GetSizeMask().m_Reg.m_Override1);
		Assert::AreEqual(package.m_Reg.m_Override2, descriptor.GetSizeMask().m_Reg.m_Override2);

		Assert::AreEqual(package.m_Mem.m_Size, descriptor.GetSizeMask().m_Mem.m_Size);
		Assert::AreEqual(package.m_Mem.m_Override1, descriptor.GetSizeMask().m_Mem.m_Override1);
		Assert::AreEqual(package.m_Mem.m_Override2, descriptor.GetSizeMask().m_Mem.m_Override2);
	}

	TEST_METHOD(Construct_1)
	{
		std::vector<std::string> variations;

		variations.push_back("r32");
		variations.push_back("16");
		variations.push_back("64");

		DescriptorOperand descriptor = DescriptorOperand(variations);
		Operand operand = Operand(descriptor, true);

		const Operand::Package& package = operand.GetPackage();

		Assert::AreEqual(package.m_Type, Operand::Type::modrm);
		Assert::AreEqual(package.m_Register, descriptor.GetTypeMask().m_Register);

		Assert::AreEqual(package.m_Rex, descriptor.GetFlagMask().m_Rex);
		Assert::AreEqual(package.m_Constant, descriptor.GetFlagMask().m_Constant);
		Assert::AreEqual(package.m_RegisterIndex, descriptor.GetFlagMask().m_RegisterIndex);

		Assert::AreEqual(package.m_Reg.m_Size, descriptor.GetSizeMask().m_Reg.m_Size);
		Assert::AreEqual(package.m_Reg.m_Override1, descriptor.GetSizeMask().m_Reg.m_Override1);
		Assert::AreEqual(package.m_Reg.m_Override2, descriptor.GetSizeMask().m_Reg.m_Override2);

		Assert::AreEqual(package.m_Mem.m_Size, descriptor.GetSizeMask().m_Mem.m_Size);
		Assert::AreEqual(package.m_Mem.m_Override1, descriptor.GetSizeMask().m_Mem.m_Override1);
		Assert::AreEqual(package.m_Mem.m_Override2, descriptor.GetSizeMask().m_Mem.m_Override2);
	}
};