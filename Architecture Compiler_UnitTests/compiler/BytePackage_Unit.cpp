#include "pch.h"
#include "utility/Setup.hpp"
#include <Windows.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

TEST_CLASS(BytePackage_Unit)
{
public:
	std::vector<Operand> GetOperands(const Descriptor& descriptor)
	{
		bool regOnly = false;
		for (const DescriptorOperand& operand : descriptor.GetOperands())
		{
			if (operand.GetTypeMask().m_Type == DescriptorOperand::Type::mem)
			{
				regOnly = false;
				break;
			}
			else if (operand.GetTypeMask().m_Type == DescriptorOperand::Type::reg && !operand.GetFlagMask().m_Constant)
			{
				regOnly = true;
			}
			else if (operand.GetTypeMask().m_Type == DescriptorOperand::Type::modrm)
			{
				regOnly = false;
				break;
			}
		}

		std::vector<Operand> operands;

		operands.reserve(descriptor.GetOperands().size());

		for (const DescriptorOperand& operand : descriptor.GetOperands())
		{
			operands.push_back(Operand(operand, regOnly));
		}

		return operands;
	}

	void MatchOperands(const Instruction::Package& package, const Descriptor& descriptor)
	{
		std::vector<Operand> operands = GetOperands(descriptor);

		uint8_t index = 0;
		for (const Operand& operand : operands)
		{
			const Operand::Package& operandPack = operand.GetPackage();

			Assert::AreEqual(package.m_Operands[index].m_Type, operandPack.m_Type);
			Assert::AreEqual(package.m_Operands[index].m_Register, operandPack.m_Register);

			Assert::AreEqual(package.m_Operands[index].m_Rex, operandPack.m_Rex);
			Assert::AreEqual(package.m_Operands[index].m_Constant, operandPack.m_Constant);
			Assert::AreEqual(package.m_Operands[index].m_RegisterIndex, operandPack.m_RegisterIndex);

			Assert::AreEqual(package.m_Operands[index].m_Reg.m_Size, operandPack.m_Reg.m_Size);
			Assert::AreEqual(package.m_Operands[index].m_Reg.m_Override1, operandPack.m_Reg.m_Override1);
			Assert::AreEqual(package.m_Operands[index].m_Reg.m_Override2, operandPack.m_Reg.m_Override2);

			Assert::AreEqual(package.m_Operands[index].m_Mem.m_Size, operandPack.m_Mem.m_Size);
			Assert::AreEqual(package.m_Operands[index].m_Mem.m_Override1, operandPack.m_Mem.m_Override1);
			Assert::AreEqual(package.m_Operands[index].m_Mem.m_Override2, operandPack.m_Mem.m_Override2);

			index++;
		}

		for (uint8_t i = index; i < ARRAYSIZE(package.m_Operands); i++)
		{
			Assert::AreEqual(package.m_Operands[index].m_Type, Operand::Type::none);
			Assert::AreEqual(package.m_Operands[index].m_Register, DescriptorOperand::Register::general);

			Assert::IsFalse(package.m_Operands[index].m_Rex);
			Assert::IsFalse(package.m_Operands[index].m_Constant);
			Assert::AreEqual<uint8_t>(package.m_Operands[index].m_RegisterIndex, 0);

			Assert::AreEqual(package.m_Operands[index].m_Reg.m_Size, DescriptorOperand::Size::undefined);
			Assert::IsFalse(package.m_Operands[index].m_Reg.m_Override1);
			Assert::IsFalse(package.m_Operands[index].m_Reg.m_Override2);

			Assert::AreEqual(package.m_Operands[index].m_Mem.m_Size, DescriptorOperand::Size::undefined);
			Assert::IsFalse(package.m_Operands[index].m_Mem.m_Override1);
			Assert::IsFalse(package.m_Operands[index].m_Mem.m_Override2);
		}
	}

	TEST_METHOD(Package_0)
	{
		BytePackage core = BytePackage();
		Descriptor descriptor = Descriptor(Token("6B -> IMUL r32|16|64, r/m32|16|64, imm8"));
		BytePackage package = Instruction(descriptor, 123).GetPackage(0);

		core.AddChild(package);

		Assert::AreEqual<uint32_t>(package.GetCount(), 1);
		Assert::AreEqual<uint32_t>(core.GetCount(), 2);

		std::vector<BytePackage::Raw> raw = core.GetRaw();

		Assert::AreEqual<size_t>(raw.size(), 1);

		Assert::AreEqual(raw[0].m_Type, ByteEntry::PackageType::Instruction);
		Assert::AreEqual<uint16_t>(raw[0].m_Instruction.m_Type, 123);

		MatchOperands(raw[0].m_Instruction, descriptor);
	}

	TEST_METHOD(Package_1)
	{
		BytePackage core = BytePackage();
		BytePackage package = Prefix(Prefix::Type_Rex | Prefix::Type_RexW).GetPackage(0);

		core.AddChild(package);

		Assert::AreEqual<uint32_t>(package.GetCount(), 1);
		Assert::AreEqual<uint32_t>(core.GetCount(), 2);

		std::vector<BytePackage::Raw> raw = core.GetRaw();

		Assert::AreEqual<size_t>(raw.size(), 1);

		Assert::AreEqual(raw[0].m_Type, ByteEntry::PackageType::Prefix);
		Assert::AreEqual<uint16_t>(raw[0].m_Prefix.m_Instruction, 0);

		Assert::IsFalse(raw[0].m_Prefix.m_WAIT);
		Assert::IsFalse(raw[0].m_Prefix.m_REPZ);
		Assert::IsFalse(raw[0].m_Prefix.m_REPNZ);
		Assert::IsFalse(raw[0].m_Prefix.m_x66);
		Assert::IsTrue(raw[0].m_Prefix.m_REXW);

		Assert::IsTrue(raw[0].m_Prefix.m_REX);
		Assert::IsFalse(raw[0].m_Prefix.m_REXB);
		Assert::IsFalse(raw[0].m_Prefix.m_REXX);
		Assert::IsFalse(raw[0].m_Prefix.m_REXR);
		Assert::IsFalse(raw[0].m_Prefix.m_LOCK);
		Assert::IsFalse(raw[0].m_Prefix.m_x67);

		Assert::IsFalse(raw[0].m_Prefix.m_CS);
		Assert::IsFalse(raw[0].m_Prefix.m_SS);
		Assert::IsFalse(raw[0].m_Prefix.m_DS);
		Assert::IsFalse(raw[0].m_Prefix.m_ES);
		Assert::IsFalse(raw[0].m_Prefix.m_GS);
		Assert::IsFalse(raw[0].m_Prefix.m_FS);
	}

	TEST_METHOD(Package_2)
	{
		BytePackage core = BytePackage();
		Descriptor descriptor = Descriptor(Token("6B -> IMUL r32|16|64, r/m32|16|64, imm8"));
		std::shared_ptr<Instruction> instruction = std::make_shared<Instruction>(descriptor, 2);

		std::vector<Redirection::Entry> entries;

		entries.push_back(Redirection::Entry(Redirection::X0F383A::x0F38));
		entries.push_back(Redirection::Entry(Redirection::Prefix::x66));
		entries.push_back(Redirection::Entry(Redirection::Type::Mod, 0));
		entries.push_back(Redirection::Entry(Redirection::Type::Mod, 1));
		entries.push_back(Redirection::Entry(Redirection::Type::Mod, 2));
		entries.push_back(Redirection::Entry(Redirection::Type::Reg, 2));

		BytePackage package = Redirection::Insert(nullptr, instruction, entries)->GetPackage(0);

		core.AddChild(package);

		Assert::AreEqual<uint32_t>(package.GetCount(), 9);
		Assert::AreEqual<uint32_t>(core.GetCount(), 10);

		std::vector<BytePackage::Raw> raw = core.GetRaw();

		Assert::AreEqual<size_t>(raw.size(), 9);

		Assert::AreEqual<uint32_t>(raw[0].m_Redirection.m_BaseIndex, 0);
		Assert::AreEqual(raw[0].m_Redirection.m_Type, Redirection::Type::X0F383A);

		Assert::IsFalse(raw[0].m_Redirection.m_x0F383A.m_ValidDefault);
		Assert::IsTrue(raw[0].m_Redirection.m_x0F383A.m_Valid0F38);
		Assert::IsFalse(raw[0].m_Redirection.m_x0F383A.m_Valid0F3A);

		Assert::AreEqual<uint8_t>(raw[0].m_Redirection.m_x0F383A.m_IndexDefault, 0);
		Assert::AreEqual<uint8_t>(raw[0].m_Redirection.m_x0F383A.m_Index0F38, 0);
		Assert::AreEqual<uint8_t>(raw[0].m_Redirection.m_x0F383A.m_Index0F3A, 0);

		Assert::AreEqual<uint32_t>(raw[1].m_Redirection.m_BaseIndex, 1);
		Assert::AreEqual(raw[1].m_Redirection.m_Type, Redirection::Type::Prefix);

		Assert::IsFalse(raw[1].m_Redirection.m_Prefix.m_ValidRexW);
		Assert::IsFalse(raw[1].m_Redirection.m_Prefix.m_ValidWait);
		Assert::IsFalse(raw[1].m_Redirection.m_Prefix.m_ValidRepe);
		Assert::IsFalse(raw[1].m_Redirection.m_Prefix.m_ValidRepne);
		Assert::IsTrue(raw[1].m_Redirection.m_Prefix.m_Valid66);
		Assert::IsFalse(raw[1].m_Redirection.m_Prefix.m_ValidDefault);

		Assert::AreEqual<uint8_t>(raw[1].m_Redirection.m_Prefix.m_IndexRexW, 0);
		Assert::AreEqual<uint8_t>(raw[1].m_Redirection.m_Prefix.m_IndexWait, 0);
		Assert::AreEqual<uint8_t>(raw[1].m_Redirection.m_Prefix.m_IndexRepe, 0);
		Assert::AreEqual<uint8_t>(raw[1].m_Redirection.m_Prefix.m_IndexRepne, 0);
		Assert::AreEqual<uint8_t>(raw[1].m_Redirection.m_Prefix.m_Index66, 0);
		Assert::AreEqual<uint8_t>(raw[1].m_Redirection.m_Prefix.m_IndexDefault, 0);

		Assert::AreEqual<uint32_t>(raw[2].m_Redirection.m_BaseIndex, 2);
		Assert::AreEqual(raw[2].m_Redirection.m_Type, Redirection::Type::Mod);

		Assert::IsTrue(raw[2].m_Redirection.m_Mod.m_Valid00);
		Assert::IsTrue(raw[2].m_Redirection.m_Mod.m_Valid01);
		Assert::IsTrue(raw[2].m_Redirection.m_Mod.m_Valid10);
		Assert::IsFalse(raw[2].m_Redirection.m_Mod.m_Valid11);

		Assert::AreEqual<uint8_t>(raw[2].m_Redirection.m_Mod.m_Index00, 0);
		Assert::AreEqual<uint8_t>(raw[2].m_Redirection.m_Mod.m_Index01, 1);
		Assert::AreEqual<uint8_t>(raw[2].m_Redirection.m_Mod.m_Index10, 2);
		Assert::AreEqual<uint8_t>(raw[2].m_Redirection.m_Mod.m_Index11, 0);

		Assert::AreEqual<uint32_t>(raw[3].m_Redirection.m_BaseIndex, 5);
		Assert::AreEqual(raw[3].m_Redirection.m_Type, Redirection::Type::Reg);

		Assert::IsFalse(raw[3].m_Redirection.m_RegRM.m_Valid000);
		Assert::IsFalse(raw[3].m_Redirection.m_RegRM.m_Valid001);
		Assert::IsTrue(raw[3].m_Redirection.m_RegRM.m_Valid010);
		Assert::IsFalse(raw[3].m_Redirection.m_RegRM.m_Valid011);
		Assert::IsFalse(raw[3].m_Redirection.m_RegRM.m_Valid100);
		Assert::IsFalse(raw[3].m_Redirection.m_RegRM.m_Valid101);
		Assert::IsFalse(raw[3].m_Redirection.m_RegRM.m_Valid110);
		Assert::IsFalse(raw[3].m_Redirection.m_RegRM.m_Valid111);

		Assert::AreEqual<uint8_t>(raw[3].m_Redirection.m_RegRM.m_Index000, 0);
		Assert::AreEqual<uint8_t>(raw[3].m_Redirection.m_RegRM.m_Index001, 0);
		Assert::AreEqual<uint8_t>(raw[3].m_Redirection.m_RegRM.m_Index010, 0);
		Assert::AreEqual<uint8_t>(raw[3].m_Redirection.m_RegRM.m_Index011, 0);
		Assert::AreEqual<uint8_t>(raw[3].m_Redirection.m_RegRM.m_Index100, 0);
		Assert::AreEqual<uint8_t>(raw[3].m_Redirection.m_RegRM.m_Index101, 0);
		Assert::AreEqual<uint8_t>(raw[3].m_Redirection.m_RegRM.m_Index110, 0);
		Assert::AreEqual<uint8_t>(raw[3].m_Redirection.m_RegRM.m_Index111, 0);

		Assert::AreEqual<uint32_t>(raw[4].m_Redirection.m_BaseIndex, 6);
		Assert::AreEqual(raw[4].m_Redirection.m_Type, Redirection::Type::Reg);

		Assert::IsFalse(raw[4].m_Redirection.m_RegRM.m_Valid000);
		Assert::IsFalse(raw[4].m_Redirection.m_RegRM.m_Valid001);
		Assert::IsTrue(raw[4].m_Redirection.m_RegRM.m_Valid010);
		Assert::IsFalse(raw[4].m_Redirection.m_RegRM.m_Valid011);
		Assert::IsFalse(raw[4].m_Redirection.m_RegRM.m_Valid100);
		Assert::IsFalse(raw[4].m_Redirection.m_RegRM.m_Valid101);
		Assert::IsFalse(raw[4].m_Redirection.m_RegRM.m_Valid110);
		Assert::IsFalse(raw[4].m_Redirection.m_RegRM.m_Valid111);

		Assert::AreEqual<uint8_t>(raw[4].m_Redirection.m_RegRM.m_Index000, 0);
		Assert::AreEqual<uint8_t>(raw[4].m_Redirection.m_RegRM.m_Index001, 0);
		Assert::AreEqual<uint8_t>(raw[4].m_Redirection.m_RegRM.m_Index010, 0);
		Assert::AreEqual<uint8_t>(raw[4].m_Redirection.m_RegRM.m_Index011, 0);
		Assert::AreEqual<uint8_t>(raw[4].m_Redirection.m_RegRM.m_Index100, 0);
		Assert::AreEqual<uint8_t>(raw[4].m_Redirection.m_RegRM.m_Index101, 0);
		Assert::AreEqual<uint8_t>(raw[4].m_Redirection.m_RegRM.m_Index110, 0);
		Assert::AreEqual<uint8_t>(raw[4].m_Redirection.m_RegRM.m_Index111, 0);

		Assert::AreEqual<uint32_t>(raw[5].m_Redirection.m_BaseIndex, 7);
		Assert::AreEqual(raw[5].m_Redirection.m_Type, Redirection::Type::Reg);

		Assert::IsFalse(raw[5].m_Redirection.m_RegRM.m_Valid000);
		Assert::IsFalse(raw[5].m_Redirection.m_RegRM.m_Valid001);
		Assert::IsTrue(raw[5].m_Redirection.m_RegRM.m_Valid010);
		Assert::IsFalse(raw[5].m_Redirection.m_RegRM.m_Valid011);
		Assert::IsFalse(raw[5].m_Redirection.m_RegRM.m_Valid100);
		Assert::IsFalse(raw[5].m_Redirection.m_RegRM.m_Valid101);
		Assert::IsFalse(raw[5].m_Redirection.m_RegRM.m_Valid110);
		Assert::IsFalse(raw[5].m_Redirection.m_RegRM.m_Valid111);

		Assert::AreEqual<uint8_t>(raw[5].m_Redirection.m_RegRM.m_Index000, 0);
		Assert::AreEqual<uint8_t>(raw[5].m_Redirection.m_RegRM.m_Index001, 0);
		Assert::AreEqual<uint8_t>(raw[5].m_Redirection.m_RegRM.m_Index010, 0);
		Assert::AreEqual<uint8_t>(raw[5].m_Redirection.m_RegRM.m_Index011, 0);
		Assert::AreEqual<uint8_t>(raw[5].m_Redirection.m_RegRM.m_Index100, 0);
		Assert::AreEqual<uint8_t>(raw[5].m_Redirection.m_RegRM.m_Index101, 0);
		Assert::AreEqual<uint8_t>(raw[5].m_Redirection.m_RegRM.m_Index110, 0);
		Assert::AreEqual<uint8_t>(raw[5].m_Redirection.m_RegRM.m_Index111, 0);

		Assert::AreEqual(raw[6].m_Type, ByteEntry::PackageType::Instruction);
		Assert::AreEqual<uint16_t>(raw[6].m_Instruction.m_Type, 2);

		MatchOperands(raw[6].m_Instruction, descriptor);

		Assert::AreEqual(raw[7].m_Type, ByteEntry::PackageType::Instruction);
		Assert::AreEqual<uint16_t>(raw[7].m_Instruction.m_Type, 2);

		MatchOperands(raw[7].m_Instruction, descriptor);

		Assert::AreEqual(raw[8].m_Type, ByteEntry::PackageType::Instruction);
		Assert::AreEqual<uint16_t>(raw[8].m_Instruction.m_Type, 2);

		MatchOperands(raw[8].m_Instruction, descriptor);
	}

	TEST_METHOD(Package_3)
	{
		BytePackage core = BytePackage();
		Descriptor firstDesc = Descriptor(Token("6B -> IMUL r32|16|64, r/m32|16|64, imm8"));
		Descriptor secondDesc = Descriptor(Token("89 -> MOV r/m32|16|64, r32|16|64"));

		FullRedirection redirect;

		redirect.Get(0) = std::make_shared<Instruction>(secondDesc, 123);
		redirect.Get(1) = std::make_shared<Instruction>(firstDesc, 280);
		redirect.Get(2) = std::make_shared<Instruction>(secondDesc, 280);
		redirect.Get(3) = std::make_shared<Instruction>(firstDesc, 280);
		redirect.Get(4) = std::make_shared<Instruction>(secondDesc, 123);
		redirect.Get(8) = std::make_shared<Instruction>(firstDesc, 123);

		BytePackage package = redirect.GetPackage(0);

		core.AddChild(package);

		Assert::AreEqual<uint32_t>(package.GetCount(), 257);
		Assert::AreEqual<uint32_t>(core.GetCount(), 258);

		std::vector<BytePackage::Raw> raw = core.GetRaw();

		Assert::AreEqual<size_t>(raw.size(), 257);

		Assert::AreEqual(raw[0].m_Type, ByteEntry::PackageType::FullRedirection);
		Assert::AreEqual<uint32_t>(raw[0].m_FullRedirection.m_BaseIndex, 0);

		Assert::AreEqual(raw[1].m_Type, ByteEntry::PackageType::Instruction);
		Assert::AreEqual<uint32_t>(raw[1].m_Instruction.m_Type, 123);

		MatchOperands(raw[1].m_Instruction, secondDesc);

		Assert::AreEqual(raw[2].m_Type, ByteEntry::PackageType::Instruction);
		Assert::AreEqual<uint32_t>(raw[2].m_Instruction.m_Type, 280);

		MatchOperands(raw[2].m_Instruction, firstDesc);

		Assert::AreEqual(raw[3].m_Type, ByteEntry::PackageType::Instruction);
		Assert::AreEqual<uint32_t>(raw[3].m_Instruction.m_Type, 280);

		MatchOperands(raw[3].m_Instruction, secondDesc);

		Assert::AreEqual(raw[4].m_Type, ByteEntry::PackageType::Instruction);
		Assert::AreEqual<uint32_t>(raw[4].m_Instruction.m_Type, 280);

		MatchOperands(raw[4].m_Instruction, firstDesc);

		Assert::AreEqual(raw[5].m_Type, ByteEntry::PackageType::Instruction);
		Assert::AreEqual<uint32_t>(raw[5].m_Instruction.m_Type, 123);

		MatchOperands(raw[5].m_Instruction, secondDesc);

		Assert::AreEqual(raw[6].m_Type, ByteEntry::PackageType::Invalid);
		Assert::AreEqual(raw[7].m_Type, ByteEntry::PackageType::Invalid);
		Assert::AreEqual(raw[8].m_Type, ByteEntry::PackageType::Invalid);

		Assert::AreEqual(raw[9].m_Type, ByteEntry::PackageType::Instruction);
		Assert::AreEqual<uint32_t>(raw[9].m_Instruction.m_Type, 123);

		MatchOperands(raw[9].m_Instruction, firstDesc);

		for (uint32_t i = 9; i < 256; i++)
		{
			Assert::AreEqual(raw[i + 1].m_Type, ByteEntry::PackageType::Invalid);
		}
	}
};