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

	TEST_METHOD(Construct_2)
	{
		std::vector<std::string> variations;

		variations.push_back("xmm/m128");

		DescriptorOperand operand = DescriptorOperand(variations);

		Assert::IsFalse(operand.GetFlagMask().m_Rex);
		Assert::IsFalse(operand.GetFlagMask().m_Constant);
		Assert::AreEqual<uint8_t>(operand.GetFlagMask().m_RegisterIndex, 0);

		Assert::IsFalse(operand.GetSizeMask().m_Mem.m_Override1);
		Assert::IsFalse(operand.GetSizeMask().m_Mem.m_Override2);

		Assert::AreEqual(operand.GetSizeMask().m_Mem.m_Size, DescriptorOperand::Size::base_128);

		Assert::IsFalse(operand.GetSizeMask().m_Reg.m_Override1);
		Assert::IsFalse(operand.GetSizeMask().m_Reg.m_Override2);

		Assert::AreEqual(operand.GetSizeMask().m_Reg.m_Size, DescriptorOperand::Size::base_128);

		Assert::AreEqual(operand.GetTypeMask().m_Type, DescriptorOperand::Type::modrm);
		Assert::AreEqual(operand.GetTypeMask().m_Register, DescriptorOperand::Register::xmm);
	}

	TEST_METHOD(Construct_3)
	{
		std::vector<std::string> variations;

		variations.push_back("imm32");
		variations.push_back("16");

		DescriptorOperand operand = DescriptorOperand(variations);

		Assert::IsFalse(operand.GetFlagMask().m_Rex);
		Assert::IsFalse(operand.GetFlagMask().m_Constant);
		Assert::AreEqual<uint8_t>(operand.GetFlagMask().m_RegisterIndex, 0);

		Assert::IsTrue(operand.GetSizeMask().m_Mem.m_Override1);
		Assert::IsFalse(operand.GetSizeMask().m_Mem.m_Override2);

		Assert::AreEqual(operand.GetSizeMask().m_Mem.m_Size, DescriptorOperand::Size::base_32);

		Assert::IsFalse(operand.GetSizeMask().m_Reg.m_Override1);
		Assert::IsFalse(operand.GetSizeMask().m_Reg.m_Override2);

		Assert::AreEqual(operand.GetSizeMask().m_Reg.m_Size, DescriptorOperand::Size::undefined);

		Assert::AreEqual(operand.GetTypeMask().m_Type, DescriptorOperand::Type::imm);
		Assert::AreEqual(operand.GetTypeMask().m_Register, DescriptorOperand::Register::general);
	}

	TEST_METHOD(Construct_4)
	{
		std::vector<std::string> variations;

		variations.push_back("rel32");
		variations.push_back("16");

		DescriptorOperand operand = DescriptorOperand(variations);

		Assert::IsFalse(operand.GetFlagMask().m_Rex);
		Assert::IsFalse(operand.GetFlagMask().m_Constant);
		Assert::AreEqual<uint8_t>(operand.GetFlagMask().m_RegisterIndex, 0);

		Assert::IsTrue(operand.GetSizeMask().m_Mem.m_Override1);
		Assert::IsFalse(operand.GetSizeMask().m_Mem.m_Override2);

		Assert::AreEqual(operand.GetSizeMask().m_Mem.m_Size, DescriptorOperand::Size::base_32);

		Assert::IsFalse(operand.GetSizeMask().m_Reg.m_Override1);
		Assert::IsFalse(operand.GetSizeMask().m_Reg.m_Override2);

		Assert::AreEqual(operand.GetSizeMask().m_Reg.m_Size, DescriptorOperand::Size::undefined);

		Assert::AreEqual(operand.GetTypeMask().m_Type, DescriptorOperand::Type::rel);
		Assert::AreEqual(operand.GetTypeMask().m_Register, DescriptorOperand::Register::general);
	}

	TEST_METHOD(Construct_5)
	{
		std::vector<std::string> variations;

		variations.push_back("m16:32");
		variations.push_back("16");
		variations.push_back("64");

		DescriptorOperand operand = DescriptorOperand(variations);

		Assert::IsFalse(operand.GetFlagMask().m_Rex);
		Assert::IsFalse(operand.GetFlagMask().m_Constant);
		Assert::AreEqual<uint8_t>(operand.GetFlagMask().m_RegisterIndex, 0);

		Assert::IsTrue(operand.GetSizeMask().m_Mem.m_Override1);
		Assert::IsTrue(operand.GetSizeMask().m_Mem.m_Override2);

		Assert::AreEqual(operand.GetSizeMask().m_Mem.m_Size, DescriptorOperand::Size::far_32);

		Assert::IsFalse(operand.GetSizeMask().m_Reg.m_Override1);
		Assert::IsFalse(operand.GetSizeMask().m_Reg.m_Override2);

		Assert::AreEqual(operand.GetSizeMask().m_Reg.m_Size, DescriptorOperand::Size::undefined);

		Assert::AreEqual(operand.GetTypeMask().m_Type, DescriptorOperand::Type::mem);
		Assert::AreEqual(operand.GetTypeMask().m_Register, DescriptorOperand::Register::general);
	}

	TEST_METHOD(Construct_6)
	{
		std::vector<std::string> variations;

		variations.push_back("st");

		DescriptorOperand operand = DescriptorOperand(variations);

		Assert::IsFalse(operand.GetFlagMask().m_Rex);
		Assert::IsFalse(operand.GetFlagMask().m_Constant);
		Assert::AreEqual<uint8_t>(operand.GetFlagMask().m_RegisterIndex, 0);

		Assert::IsFalse(operand.GetSizeMask().m_Mem.m_Override1);
		Assert::IsFalse(operand.GetSizeMask().m_Mem.m_Override2);

		Assert::AreEqual(operand.GetSizeMask().m_Mem.m_Size, DescriptorOperand::Size::undefined);

		Assert::IsFalse(operand.GetSizeMask().m_Reg.m_Override1);
		Assert::IsFalse(operand.GetSizeMask().m_Reg.m_Override2);

		Assert::AreEqual(operand.GetSizeMask().m_Reg.m_Size, DescriptorOperand::Size::base_80);

		Assert::AreEqual(operand.GetTypeMask().m_Type, DescriptorOperand::Type::reg);
		Assert::AreEqual(operand.GetTypeMask().m_Register, DescriptorOperand::Register::st);
	}

	TEST_METHOD(Construct_7)
	{
		std::vector<std::string> variations;

		variations.push_back("cr");

		DescriptorOperand operand = DescriptorOperand(variations);

		Assert::IsFalse(operand.GetFlagMask().m_Rex);
		Assert::IsFalse(operand.GetFlagMask().m_Constant);
		Assert::AreEqual<uint8_t>(operand.GetFlagMask().m_RegisterIndex, 0);

		Assert::IsFalse(operand.GetSizeMask().m_Mem.m_Override1);
		Assert::IsFalse(operand.GetSizeMask().m_Mem.m_Override2);

		Assert::AreEqual(operand.GetSizeMask().m_Mem.m_Size, DescriptorOperand::Size::undefined);

		Assert::IsFalse(operand.GetSizeMask().m_Reg.m_Override1);
		Assert::IsFalse(operand.GetSizeMask().m_Reg.m_Override2);

		Assert::AreEqual(operand.GetSizeMask().m_Reg.m_Size, DescriptorOperand::Size::base_64);

		Assert::AreEqual(operand.GetTypeMask().m_Type, DescriptorOperand::Type::reg);
		Assert::AreEqual(operand.GetTypeMask().m_Register, DescriptorOperand::Register::cr);
	}

	TEST_METHOD(Construct_8)
	{
		std::vector<std::string> variations;

		variations.push_back("esi");
		variations.push_back("si");
		variations.push_back("rsi");
		variations.push_back("rex");

		DescriptorOperand operand = DescriptorOperand(variations);

		Assert::IsTrue(operand.GetFlagMask().m_Rex);
		Assert::IsTrue(operand.GetFlagMask().m_Constant);
		Assert::AreEqual<uint8_t>(operand.GetFlagMask().m_RegisterIndex, 6);

		Assert::IsFalse(operand.GetSizeMask().m_Mem.m_Override1);
		Assert::IsFalse(operand.GetSizeMask().m_Mem.m_Override2);

		Assert::AreEqual(operand.GetSizeMask().m_Mem.m_Size, DescriptorOperand::Size::undefined);

		Assert::IsTrue(operand.GetSizeMask().m_Reg.m_Override1);
		Assert::IsTrue(operand.GetSizeMask().m_Reg.m_Override2);

		Assert::AreEqual(operand.GetSizeMask().m_Reg.m_Size, DescriptorOperand::Size::base_32);

		Assert::AreEqual(operand.GetTypeMask().m_Type, DescriptorOperand::Type::reg);
		Assert::AreEqual(operand.GetTypeMask().m_Register, DescriptorOperand::Register::general);
	}

	TEST_METHOD(Construct_9)
	{
		std::vector<std::string> variations;

		variations.push_back("dr");

		DescriptorOperand operand = DescriptorOperand(variations);

		Assert::IsFalse(operand.GetFlagMask().m_Rex);
		Assert::IsFalse(operand.GetFlagMask().m_Constant);
		Assert::AreEqual<uint8_t>(operand.GetFlagMask().m_RegisterIndex, 0);

		Assert::IsFalse(operand.GetSizeMask().m_Mem.m_Override1);
		Assert::IsFalse(operand.GetSizeMask().m_Mem.m_Override2);

		Assert::AreEqual(operand.GetSizeMask().m_Mem.m_Size, DescriptorOperand::Size::undefined);

		Assert::IsFalse(operand.GetSizeMask().m_Reg.m_Override1);
		Assert::IsFalse(operand.GetSizeMask().m_Reg.m_Override2);

		Assert::AreEqual(operand.GetSizeMask().m_Reg.m_Size, DescriptorOperand::Size::base_64);

		Assert::AreEqual(operand.GetTypeMask().m_Type, DescriptorOperand::Type::reg);
		Assert::AreEqual(operand.GetTypeMask().m_Register, DescriptorOperand::Register::dr);
	}

	TEST_METHOD(Construct_10)
	{
		std::vector<std::string> variations;

		variations.push_back("bnd/m128");

		DescriptorOperand operand = DescriptorOperand(variations);

		Assert::IsFalse(operand.GetFlagMask().m_Rex);
		Assert::IsFalse(operand.GetFlagMask().m_Constant);
		Assert::AreEqual<uint8_t>(operand.GetFlagMask().m_RegisterIndex, 0);

		Assert::IsFalse(operand.GetSizeMask().m_Mem.m_Override1);
		Assert::IsFalse(operand.GetSizeMask().m_Mem.m_Override2);

		Assert::AreEqual(operand.GetSizeMask().m_Mem.m_Size, DescriptorOperand::Size::base_128);

		Assert::IsFalse(operand.GetSizeMask().m_Reg.m_Override1);
		Assert::IsFalse(operand.GetSizeMask().m_Reg.m_Override2);

		Assert::AreEqual(operand.GetSizeMask().m_Reg.m_Size, DescriptorOperand::Size::base_128);

		Assert::AreEqual(operand.GetTypeMask().m_Type, DescriptorOperand::Type::modrm);
		Assert::AreEqual(operand.GetTypeMask().m_Register, DescriptorOperand::Register::bnd);
	}

	TEST_METHOD(Construct_11)
	{
		std::vector<std::string> variations;

		variations.push_back("mm/m64");

		DescriptorOperand operand = DescriptorOperand(variations);

		Assert::IsFalse(operand.GetFlagMask().m_Rex);
		Assert::IsFalse(operand.GetFlagMask().m_Constant);
		Assert::AreEqual<uint8_t>(operand.GetFlagMask().m_RegisterIndex, 0);

		Assert::IsFalse(operand.GetSizeMask().m_Mem.m_Override1);
		Assert::IsFalse(operand.GetSizeMask().m_Mem.m_Override2);

		Assert::AreEqual(operand.GetSizeMask().m_Mem.m_Size, DescriptorOperand::Size::base_64);

		Assert::IsFalse(operand.GetSizeMask().m_Reg.m_Override1);
		Assert::IsFalse(operand.GetSizeMask().m_Reg.m_Override2);

		Assert::AreEqual(operand.GetSizeMask().m_Reg.m_Size, DescriptorOperand::Size::base_64);

		Assert::AreEqual(operand.GetTypeMask().m_Type, DescriptorOperand::Type::modrm);
		Assert::AreEqual(operand.GetTypeMask().m_Register, DescriptorOperand::Register::mm);
	}

	TEST_METHOD(Construct_12)
	{
		std::vector<std::string> variations;

		variations.push_back("mm");

		DescriptorOperand operand = DescriptorOperand(variations);

		Assert::IsFalse(operand.GetFlagMask().m_Rex);
		Assert::IsFalse(operand.GetFlagMask().m_Constant);
		Assert::AreEqual<uint8_t>(operand.GetFlagMask().m_RegisterIndex, 0);

		Assert::IsFalse(operand.GetSizeMask().m_Mem.m_Override1);
		Assert::IsFalse(operand.GetSizeMask().m_Mem.m_Override2);

		Assert::AreEqual(operand.GetSizeMask().m_Mem.m_Size, DescriptorOperand::Size::undefined);

		Assert::IsFalse(operand.GetSizeMask().m_Reg.m_Override1);
		Assert::IsFalse(operand.GetSizeMask().m_Reg.m_Override2);

		Assert::AreEqual(operand.GetSizeMask().m_Reg.m_Size, DescriptorOperand::Size::base_64);

		Assert::AreEqual(operand.GetTypeMask().m_Type, DescriptorOperand::Type::reg);
		Assert::AreEqual(operand.GetTypeMask().m_Register, DescriptorOperand::Register::mm);
	}

	TEST_METHOD(Construct_13)
	{
		std::vector<std::string> variations;

		variations.push_back("bnd");

		DescriptorOperand operand = DescriptorOperand(variations);

		Assert::IsFalse(operand.GetFlagMask().m_Rex);
		Assert::IsFalse(operand.GetFlagMask().m_Constant);
		Assert::AreEqual<uint8_t>(operand.GetFlagMask().m_RegisterIndex, 0);

		Assert::IsFalse(operand.GetSizeMask().m_Mem.m_Override1);
		Assert::IsFalse(operand.GetSizeMask().m_Mem.m_Override2);

		Assert::AreEqual(operand.GetSizeMask().m_Mem.m_Size, DescriptorOperand::Size::undefined);

		Assert::IsFalse(operand.GetSizeMask().m_Reg.m_Override1);
		Assert::IsFalse(operand.GetSizeMask().m_Reg.m_Override2);

		Assert::AreEqual(operand.GetSizeMask().m_Reg.m_Size, DescriptorOperand::Size::base_128);

		Assert::AreEqual(operand.GetTypeMask().m_Type, DescriptorOperand::Type::reg);
		Assert::AreEqual(operand.GetTypeMask().m_Register, DescriptorOperand::Register::bnd);
	}

	TEST_METHOD(Construct_14)
	{
		std::vector<std::string> variations;

		variations.push_back("bnd.");

		DescriptorOperand operand = DescriptorOperand(variations);

		Assert::AreEqual(operand.GetTypeMask().m_Type, DescriptorOperand::Type::none);
	}

	TEST_METHOD(Construct_15)
	{
		std::vector<std::string> variations;

		variations.push_back("1");

		DescriptorOperand operand = DescriptorOperand(variations);

		Assert::IsFalse(operand.GetFlagMask().m_Rex);
		Assert::IsTrue(operand.GetFlagMask().m_Constant);
		Assert::AreEqual<uint8_t>(operand.GetFlagMask().m_RegisterIndex, 1);

		Assert::IsFalse(operand.GetSizeMask().m_Mem.m_Override1);
		Assert::IsFalse(operand.GetSizeMask().m_Mem.m_Override2);

		Assert::AreEqual(operand.GetSizeMask().m_Mem.m_Size, DescriptorOperand::Size::base_8);

		Assert::IsFalse(operand.GetSizeMask().m_Reg.m_Override1);
		Assert::IsFalse(operand.GetSizeMask().m_Reg.m_Override2);

		Assert::AreEqual(operand.GetSizeMask().m_Reg.m_Size, DescriptorOperand::Size::undefined);

		Assert::AreEqual(operand.GetTypeMask().m_Type, DescriptorOperand::Type::imm);
		Assert::AreEqual(operand.GetTypeMask().m_Register, DescriptorOperand::Register::general);
	}

	TEST_METHOD(Construct_16)
	{
		std::vector<std::string> variations;

		variations.push_back("r");

		DescriptorOperand operand = DescriptorOperand(variations);

		Assert::IsFalse(operand.GetFlagMask().m_Rex);
		Assert::IsFalse(operand.GetFlagMask().m_Constant);
		Assert::AreEqual<uint8_t>(operand.GetFlagMask().m_RegisterIndex, 0);

		Assert::IsFalse(operand.GetSizeMask().m_Mem.m_Override1);
		Assert::IsFalse(operand.GetSizeMask().m_Mem.m_Override2);

		Assert::AreEqual(operand.GetSizeMask().m_Mem.m_Size, DescriptorOperand::Size::undefined);

		Assert::IsFalse(operand.GetSizeMask().m_Reg.m_Override1);
		Assert::IsFalse(operand.GetSizeMask().m_Reg.m_Override2);

		Assert::AreEqual(operand.GetSizeMask().m_Reg.m_Size, DescriptorOperand::Size::undefined);

		Assert::AreEqual(operand.GetTypeMask().m_Type, DescriptorOperand::Type::reg);
		Assert::AreEqual(operand.GetTypeMask().m_Register, DescriptorOperand::Register::general);
	}

	TEST_METHOD(Construct_17)
	{
		std::vector<std::string> variations;

		variations.push_back("r/m8");

		DescriptorOperand operand = DescriptorOperand(variations);

		Assert::IsFalse(operand.GetFlagMask().m_Rex);
		Assert::IsFalse(operand.GetFlagMask().m_Constant);
		Assert::AreEqual<uint8_t>(operand.GetFlagMask().m_RegisterIndex, 0);

		Assert::IsFalse(operand.GetSizeMask().m_Mem.m_Override1);
		Assert::IsFalse(operand.GetSizeMask().m_Mem.m_Override2);

		Assert::AreEqual(operand.GetSizeMask().m_Mem.m_Size, DescriptorOperand::Size::base_8);

		Assert::IsFalse(operand.GetSizeMask().m_Reg.m_Override1);
		Assert::IsFalse(operand.GetSizeMask().m_Reg.m_Override2);

		Assert::AreEqual(operand.GetSizeMask().m_Reg.m_Size, DescriptorOperand::Size::base_8);

		Assert::AreEqual(operand.GetTypeMask().m_Type, DescriptorOperand::Type::modrm);
		Assert::AreEqual(operand.GetTypeMask().m_Register, DescriptorOperand::Register::general);
	}

	TEST_METHOD(Construct_18)
	{
		std::vector<std::string> variations;

		variations.push_back("r16/m8");

		DescriptorOperand operand = DescriptorOperand(variations);

		Assert::IsFalse(operand.GetFlagMask().m_Rex);
		Assert::IsFalse(operand.GetFlagMask().m_Constant);
		Assert::AreEqual<uint8_t>(operand.GetFlagMask().m_RegisterIndex, 0);

		Assert::IsFalse(operand.GetSizeMask().m_Mem.m_Override1);
		Assert::IsFalse(operand.GetSizeMask().m_Mem.m_Override2);

		Assert::AreEqual(operand.GetSizeMask().m_Mem.m_Size, DescriptorOperand::Size::base_8);

		Assert::IsFalse(operand.GetSizeMask().m_Reg.m_Override1);
		Assert::IsFalse(operand.GetSizeMask().m_Reg.m_Override2);

		Assert::AreEqual(operand.GetSizeMask().m_Reg.m_Size, DescriptorOperand::Size::base_16);

		Assert::AreEqual(operand.GetTypeMask().m_Type, DescriptorOperand::Type::modrm);
		Assert::AreEqual(operand.GetTypeMask().m_Register, DescriptorOperand::Register::general);
	}

	TEST_METHOD(Construct_19)
	{
		std::vector<std::string> variations;

		variations.push_back("m");

		DescriptorOperand operand = DescriptorOperand(variations);

		Assert::IsFalse(operand.GetFlagMask().m_Rex);
		Assert::IsFalse(operand.GetFlagMask().m_Constant);
		Assert::AreEqual<uint8_t>(operand.GetFlagMask().m_RegisterIndex, 0);

		Assert::IsFalse(operand.GetSizeMask().m_Mem.m_Override1);
		Assert::IsFalse(operand.GetSizeMask().m_Mem.m_Override2);

		Assert::AreEqual(operand.GetSizeMask().m_Mem.m_Size, DescriptorOperand::Size::undefined);

		Assert::IsFalse(operand.GetSizeMask().m_Reg.m_Override1);
		Assert::IsFalse(operand.GetSizeMask().m_Reg.m_Override2);

		Assert::AreEqual(operand.GetSizeMask().m_Reg.m_Size, DescriptorOperand::Size::undefined);

		Assert::AreEqual(operand.GetTypeMask().m_Type, DescriptorOperand::Type::mem);
		Assert::AreEqual(operand.GetTypeMask().m_Register, DescriptorOperand::Register::general);
	}

	TEST_METHOD(Construct_20)
	{
		std::vector<std::string> variations;

		variations.push_back("moffs8");

		DescriptorOperand operand = DescriptorOperand(variations);

		Assert::IsFalse(operand.GetFlagMask().m_Rex);
		Assert::IsFalse(operand.GetFlagMask().m_Constant);
		Assert::AreEqual<uint8_t>(operand.GetFlagMask().m_RegisterIndex, 0);

		Assert::IsFalse(operand.GetSizeMask().m_Mem.m_Override1);
		Assert::IsFalse(operand.GetSizeMask().m_Mem.m_Override2);

		Assert::AreEqual(operand.GetSizeMask().m_Mem.m_Size, DescriptorOperand::Size::base_8);

		Assert::IsFalse(operand.GetSizeMask().m_Reg.m_Override1);
		Assert::IsFalse(operand.GetSizeMask().m_Reg.m_Override2);

		Assert::AreEqual(operand.GetSizeMask().m_Reg.m_Size, DescriptorOperand::Size::undefined);

		Assert::AreEqual(operand.GetTypeMask().m_Type, DescriptorOperand::Type::moffs);
		Assert::AreEqual(operand.GetTypeMask().m_Register, DescriptorOperand::Register::general);
	}

	TEST_METHOD(Construct_21)
	{
		std::vector<std::string> variations;

		variations.push_back("moff.");

		DescriptorOperand operand = DescriptorOperand(variations);

		Assert::AreEqual(operand.GetTypeMask().m_Type, DescriptorOperand::Type::mem);
	}

	TEST_METHOD(Construct_22)
	{
		std::vector<std::string> variations;

		variations.push_back("ea");

		DescriptorOperand operand = DescriptorOperand(variations);

		Assert::AreEqual(operand.GetTypeMask().m_Type, DescriptorOperand::Type::none);
	}

	TEST_METHOD(Construct_23)
	{
		std::vector<std::string> variations;

		variations.push_back("eas");

		DescriptorOperand operand = DescriptorOperand(variations);

		Assert::AreEqual(operand.GetTypeMask().m_Type, DescriptorOperand::Type::none);
	}

	TEST_METHOD(Construct_24)
	{
		std::vector<std::string> variations;

		variations.push_back("bh");

		DescriptorOperand operand = DescriptorOperand(variations);

		Assert::IsFalse(operand.GetFlagMask().m_Rex);
		Assert::IsTrue(operand.GetFlagMask().m_Constant);
		Assert::AreEqual<uint8_t>(operand.GetFlagMask().m_RegisterIndex, 7);

		Assert::IsFalse(operand.GetSizeMask().m_Mem.m_Override1);
		Assert::IsFalse(operand.GetSizeMask().m_Mem.m_Override2);

		Assert::AreEqual(operand.GetSizeMask().m_Mem.m_Size, DescriptorOperand::Size::undefined);

		Assert::IsFalse(operand.GetSizeMask().m_Reg.m_Override1);
		Assert::IsFalse(operand.GetSizeMask().m_Reg.m_Override2);

		Assert::AreEqual(operand.GetSizeMask().m_Reg.m_Size, DescriptorOperand::Size::base_8);

		Assert::AreEqual(operand.GetTypeMask().m_Type, DescriptorOperand::Type::reg);
		Assert::AreEqual(operand.GetTypeMask().m_Register, DescriptorOperand::Register::general);
	}

	TEST_METHOD(Construct_25)
	{
		std::vector<std::string> variations;

		variations.push_back("moff");

		DescriptorOperand operand = DescriptorOperand(variations);

		Assert::AreEqual(operand.GetTypeMask().m_Type, DescriptorOperand::Type::mem);
	}

	TEST_METHOD(Construct_26)
	{
		std::vector<std::string> variations;

		variations.push_back("mm7");

		DescriptorOperand operand = DescriptorOperand(variations);

		Assert::IsFalse(operand.GetFlagMask().m_Rex);
		Assert::IsTrue(operand.GetFlagMask().m_Constant);
		Assert::AreEqual<uint8_t>(operand.GetFlagMask().m_RegisterIndex, 7);

		Assert::IsFalse(operand.GetSizeMask().m_Mem.m_Override1);
		Assert::IsFalse(operand.GetSizeMask().m_Mem.m_Override2);

		Assert::AreEqual(operand.GetSizeMask().m_Mem.m_Size, DescriptorOperand::Size::undefined);

		Assert::IsFalse(operand.GetSizeMask().m_Reg.m_Override1);
		Assert::IsFalse(operand.GetSizeMask().m_Reg.m_Override2);

		Assert::AreEqual(operand.GetSizeMask().m_Reg.m_Size, DescriptorOperand::Size::base_64);

		Assert::AreEqual(operand.GetTypeMask().m_Type, DescriptorOperand::Type::reg);
		Assert::AreEqual(operand.GetTypeMask().m_Register, DescriptorOperand::Register::mm);
	}

	TEST_METHOD(Construct_27)
	{
		std::vector<std::string> variations;

		variations.push_back("mm_");

		DescriptorOperand operand = DescriptorOperand(variations);

		Assert::AreEqual(operand.GetTypeMask().m_Type, DescriptorOperand::Type::mem);
	}

	TEST_METHOD(Construct_28)
	{
		std::vector<std::string> variations;

		variations.push_back("gs");

		DescriptorOperand operand = DescriptorOperand(variations);

		Assert::IsFalse(operand.GetFlagMask().m_Rex);
		Assert::IsTrue(operand.GetFlagMask().m_Constant);
		Assert::AreEqual<uint8_t>(operand.GetFlagMask().m_RegisterIndex, 5);

		Assert::IsFalse(operand.GetSizeMask().m_Mem.m_Override1);
		Assert::IsFalse(operand.GetSizeMask().m_Mem.m_Override2);

		Assert::AreEqual(operand.GetSizeMask().m_Mem.m_Size, DescriptorOperand::Size::undefined);

		Assert::IsTrue(operand.GetSizeMask().m_Reg.m_Override1);
		Assert::IsFalse(operand.GetSizeMask().m_Reg.m_Override2);

		Assert::AreEqual(operand.GetSizeMask().m_Reg.m_Size, DescriptorOperand::Size::base_64);

		Assert::AreEqual(operand.GetTypeMask().m_Type, DescriptorOperand::Type::reg);
		Assert::AreEqual(operand.GetTypeMask().m_Register, DescriptorOperand::Register::sreg);
	}

	TEST_METHOD(Construct_29)
	{
		std::vector<std::string> variations;

		variations.push_back("sreg");

		DescriptorOperand operand = DescriptorOperand(variations);

		Assert::IsFalse(operand.GetFlagMask().m_Rex);
		Assert::IsFalse(operand.GetFlagMask().m_Constant);
		Assert::AreEqual<uint8_t>(operand.GetFlagMask().m_RegisterIndex, 0);

		Assert::IsFalse(operand.GetSizeMask().m_Mem.m_Override1);
		Assert::IsFalse(operand.GetSizeMask().m_Mem.m_Override2);

		Assert::AreEqual(operand.GetSizeMask().m_Mem.m_Size, DescriptorOperand::Size::undefined);

		Assert::IsFalse(operand.GetSizeMask().m_Reg.m_Override1);
		Assert::IsFalse(operand.GetSizeMask().m_Reg.m_Override2);

		Assert::AreEqual(operand.GetSizeMask().m_Reg.m_Size, DescriptorOperand::Size::base_16);

		Assert::AreEqual(operand.GetTypeMask().m_Type, DescriptorOperand::Type::reg);
		Assert::AreEqual(operand.GetTypeMask().m_Register, DescriptorOperand::Register::sreg);
	}
};