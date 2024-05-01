#include "pch.h"
#include "utility/Setup.hpp"
#include <parser/DescriptorOperand.hpp>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

TEST_CLASS(DescriptorOperand_Unit)
{
public:
	TEST_METHOD(Construct_0)
	{
		std::vector<std::string> variations;

		variations.push_back("r32");
		variations.push_back("16");
		variations.push_back("64");

		DescriptorOperand operand = DescriptorOperand(variations);

		Assert::IsFalse(operand.GetFlagMask().m_Rex);
		Assert::IsFalse(operand.GetFlagMask().m_Constant);
		Assert::AreEqual<uint8_t>(operand.GetFlagMask().m_RegisterIndex, 0);

		Assert::IsFalse(operand.GetSizeMask().m_Mem.m_Override1);
		Assert::IsFalse(operand.GetSizeMask().m_Mem.m_Override2);

		Assert::AreEqual(operand.GetSizeMask().m_Mem.m_Size, DescriptorOperand::Size::undefined);

		Assert::IsTrue(operand.GetSizeMask().m_Reg.m_Override1);
		Assert::IsTrue(operand.GetSizeMask().m_Reg.m_Override2);

		Assert::AreEqual(operand.GetSizeMask().m_Reg.m_Size, DescriptorOperand::Size::base_32);

		Assert::AreEqual(operand.GetTypeMask().m_Type, DescriptorOperand::Type::reg);
		Assert::AreEqual(operand.GetTypeMask().m_Register, DescriptorOperand::Register::general);
	}

	TEST_METHOD(Construct_1)
	{
		std::vector<std::string> variations;

		variations.push_back("r32");
		variations.push_back("16/m16");

		DescriptorOperand operand = DescriptorOperand(variations);

		Assert::IsFalse(operand.GetFlagMask().m_Rex);
		Assert::IsFalse(operand.GetFlagMask().m_Constant);
		Assert::AreEqual<uint8_t>(operand.GetFlagMask().m_RegisterIndex, 0);

		Assert::IsFalse(operand.GetSizeMask().m_Mem.m_Override1);
		Assert::IsFalse(operand.GetSizeMask().m_Mem.m_Override2);

		Assert::AreEqual(operand.GetSizeMask().m_Mem.m_Size, DescriptorOperand::Size::base_16);

		Assert::IsTrue(operand.GetSizeMask().m_Reg.m_Override1);
		Assert::IsFalse(operand.GetSizeMask().m_Reg.m_Override2);

		Assert::AreEqual(operand.GetSizeMask().m_Reg.m_Size, DescriptorOperand::Size::base_32);

		Assert::AreEqual(operand.GetTypeMask().m_Type, DescriptorOperand::Type::modrm);
		Assert::AreEqual(operand.GetTypeMask().m_Register, DescriptorOperand::Register::general);
	}
};