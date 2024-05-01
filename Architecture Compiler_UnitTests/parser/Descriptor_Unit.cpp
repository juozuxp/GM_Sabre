#include "pch.h"
#include "utility/Setup.hpp"
#include <parser/Descriptor.hpp>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

TEST_CLASS(Descriptor_Unit)
{
public:
	TEST_METHOD(Construct)
	{
		Descriptor descriptor = Descriptor(Token("0F BA /5 -> BTS r/m32|16|64, imm8"));

		Assert::AreEqual<uint8_t>(descriptor.GetUpperOperand(), 5);

		Assert::AreEqual<size_t>(descriptor.GetBytes().size(), 2);

		Assert::AreEqual<uint8_t>(descriptor.GetBytes()[0], 0x0F);
		Assert::AreEqual<uint8_t>(descriptor.GetBytes()[1], 0xBA);

		Assert::AreEqual(descriptor.GetName().c_str(), "bts", true);

		Assert::AreEqual<size_t>(descriptor.GetOperands().size(), 2);

		const DescriptorOperand& first = descriptor.GetOperands()[0];

		Assert::IsFalse(first.GetFlagMask().m_Rex);
		Assert::IsFalse(first.GetFlagMask().m_Constant);
		Assert::AreEqual<uint8_t>(first.GetFlagMask().m_RegisterIndex, 0);

		Assert::IsTrue(first.GetSizeMask().m_Mem.m_Override1);
		Assert::IsTrue(first.GetSizeMask().m_Mem.m_Override2);

		Assert::AreEqual(first.GetSizeMask().m_Mem.m_Size, DescriptorOperand::Size::base_32);

		Assert::IsTrue(first.GetSizeMask().m_Reg.m_Override1);
		Assert::IsTrue(first.GetSizeMask().m_Reg.m_Override2);

		Assert::AreEqual(first.GetSizeMask().m_Reg.m_Size, DescriptorOperand::Size::base_32);

		Assert::AreEqual(first.GetTypeMask().m_Type, DescriptorOperand::Type::modrm);
		Assert::AreEqual(first.GetTypeMask().m_Register, DescriptorOperand::Register::general);

		const DescriptorOperand& second = descriptor.GetOperands()[1];

		Assert::IsFalse(second.GetFlagMask().m_Rex);
		Assert::IsFalse(second.GetFlagMask().m_Constant);
		Assert::AreEqual<uint8_t>(second.GetFlagMask().m_RegisterIndex, 0);

		Assert::IsFalse(second.GetSizeMask().m_Mem.m_Override1);
		Assert::IsFalse(second.GetSizeMask().m_Mem.m_Override2);

		Assert::AreEqual(second.GetSizeMask().m_Mem.m_Size, DescriptorOperand::Size::base_8);

		Assert::IsFalse(second.GetSizeMask().m_Reg.m_Override1);
		Assert::IsFalse(second.GetSizeMask().m_Reg.m_Override2);

		Assert::AreEqual(second.GetSizeMask().m_Reg.m_Size, DescriptorOperand::Size::undefined);

		Assert::AreEqual(second.GetTypeMask().m_Type, DescriptorOperand::Type::imm);
		Assert::AreEqual(second.GetTypeMask().m_Register, DescriptorOperand::Register::general);

		descriptor = Descriptor(Token("0F 1 CA -> CLAC"));

		Assert::AreEqual<uint8_t>(descriptor.GetUpperOperand(), ~0);

		Assert::AreEqual<size_t>(descriptor.GetBytes().size(), 3);

		Assert::AreEqual<uint8_t>(descriptor.GetBytes()[0], 0x0F);
		Assert::AreEqual<uint8_t>(descriptor.GetBytes()[1], 0x01);
		Assert::AreEqual<uint8_t>(descriptor.GetBytes()[2], 0xCA);

		Assert::AreEqual(descriptor.GetName().c_str(), "clac", true);

		Assert::AreEqual<size_t>(descriptor.GetOperands().size(), 0);

		descriptor = Descriptor(Token("0F R CA -> CLAC"));

		Assert::AreEqual<uint8_t>(descriptor.GetUpperOperand(), ~0);

		Assert::AreEqual<size_t>(descriptor.GetBytes().size(), 3);

		Assert::AreEqual<uint8_t>(descriptor.GetBytes()[0], 0x0F);
		Assert::AreEqual<uint8_t>(descriptor.GetBytes()[1], 0x00);
		Assert::AreEqual<uint8_t>(descriptor.GetBytes()[2], 0xCA);

		Assert::AreEqual(descriptor.GetName().c_str(), "clac", true);

		Assert::AreEqual<size_t>(descriptor.GetOperands().size(), 0);
	}
};