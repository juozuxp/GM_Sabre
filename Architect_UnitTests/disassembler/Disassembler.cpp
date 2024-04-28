#include "pch.h"
#include "utility/Setup.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

TEST_CLASS(Disassembler_Unit)
{
public:
	TEST_METHOD(Disassemble_mov_0)
	{
		constexpr uint8_t code[] = { 0x65, 0x4F, 0x8B, 0x84, 0xC0, 0x08, 0x00, 0x00, 0x00 }; // mov r8, qword ptr gs:[r8 + r8 * 8 + 8]

		std::vector<ILInstruction> instructions;
		Disassembler dissasembler;

		dissasembler.Disassemble(code, sizeof(code), instructions);

		Assert::AreEqual<size_t>(instructions.size(), 1);

		const ILInstruction& instruction = instructions[0];

		Assert::AreEqual(instruction.m_Type, InsType_mov);
		Assert::AreEqual<uint8_t>(instruction.m_Size, sizeof(code));

		Assert::AreEqual(instruction.m_Operands[0].m_Scale, ILOperandScale_64);
		Assert::AreEqual(instruction.m_Operands[0].m_Type, ILOperandType_Register);

		Assert::AreEqual(instruction.m_Operands[0].m_Register.m_Type, Register::general);
		Assert::AreEqual<uint8_t>(instruction.m_Operands[0].m_Register.m_Base, REG_R8);
		Assert::AreEqual<uint8_t>(instruction.m_Operands[0].m_Register.m_BaseHigh, false);

		Assert::AreEqual(instruction.m_Operands[1].m_Scale, ILOperandScale_64);
		Assert::AreEqual(instruction.m_Operands[1].m_Type, ILOperandType_Memory);

		Assert::AreEqual<uint8_t>(instruction.m_Operands[1].m_Memory.m_Base, REG_R8);
		Assert::AreEqual<uint8_t>(instruction.m_Operands[1].m_Memory.m_Index, REG_R8);
		Assert::AreEqual(instruction.m_Operands[1].m_Memory.m_Scale, ILMemoryScaler_8);
		Assert::AreEqual<uint8_t>(instruction.m_Operands[1].m_Memory.m_Segment, REG_GS);
		Assert::AreEqual<int32_t>(instruction.m_Operands[1].m_Memory.m_Offset, 8);

		Assert::AreEqual(instruction.m_Operands[2].m_Type, ILOperandType_None);
	}

	TEST_METHOD(Disassemble_mov_1)
	{
		constexpr uint8_t code[] = { 0x48, 0x8B, 0x05, 0x0A, 0x00, 0x00, 0x00 }; // mov rax, qword ptr [rip + 10]

		std::vector<ILInstruction> instructions;
		Disassembler dissasembler;

		dissasembler.Disassemble(code, sizeof(code), instructions);

		Assert::AreEqual<size_t>(instructions.size(), 1);

		const ILInstruction& instruction = instructions[0];

		Assert::AreEqual(instruction.m_Type, InsType_mov);
		Assert::AreEqual<uint8_t>(instruction.m_Size, sizeof(code));

		Assert::AreEqual(instruction.m_Operands[0].m_Scale, ILOperandScale_64);
		Assert::AreEqual(instruction.m_Operands[0].m_Type, ILOperandType_Register);

		Assert::AreEqual(instruction.m_Operands[0].m_Register.m_Type, Register::general);
		Assert::AreEqual<uint8_t>(instruction.m_Operands[0].m_Register.m_Base, REG_RAX);
		Assert::AreEqual<uint8_t>(instruction.m_Operands[0].m_Register.m_BaseHigh, false);

		Assert::AreEqual(instruction.m_Operands[1].m_Scale, ILOperandScale_64);
		Assert::AreEqual(instruction.m_Operands[1].m_Type, ILOperandType_MemoryRelative);

		Assert::AreEqual<uint8_t>(instruction.m_Operands[1].m_Relative.m_Segment, IL_INVALID_REGISTER);
		Assert::AreEqual<int32_t>(instruction.m_Operands[1].m_Relative.m_Value, 10);

		Assert::AreEqual(instruction.m_Operands[2].m_Type, ILOperandType_None);
	}

	TEST_METHOD(Disassemble_mov_2)
	{
		constexpr uint8_t code[] = { 0x48, 0x8B, 0x04, 0x24 }; // mov rax, [rsp]

		std::vector<ILInstruction> instructions;
		Disassembler dissasembler;

		dissasembler.Disassemble(code, sizeof(code), instructions);

		Assert::AreEqual<size_t>(instructions.size(), 1);

		const ILInstruction& instruction = instructions[0];

		Assert::AreEqual(instruction.m_Type, InsType_mov);
		Assert::AreEqual<uint8_t>(instruction.m_Size, sizeof(code));

		Assert::AreEqual(instruction.m_Operands[0].m_Scale, ILOperandScale_64);
		Assert::AreEqual(instruction.m_Operands[0].m_Type, ILOperandType_Register);

		Assert::AreEqual(instruction.m_Operands[0].m_Register.m_Type, Register::general);
		Assert::AreEqual<uint8_t>(instruction.m_Operands[0].m_Register.m_Base, REG_RAX);
		Assert::AreEqual<uint8_t>(instruction.m_Operands[0].m_Register.m_BaseHigh, false);

		Assert::AreEqual(instruction.m_Operands[1].m_Scale, ILOperandScale_64);
		Assert::AreEqual(instruction.m_Operands[1].m_Type, ILOperandType_Memory);

		Assert::AreEqual<uint8_t>(instruction.m_Operands[1].m_Memory.m_Segment, IL_INVALID_REGISTER);
		Assert::AreEqual(instruction.m_Operands[1].m_Memory.m_Scale, ILMemoryScaler_1);
		Assert::AreEqual<uint8_t>(instruction.m_Operands[1].m_Memory.m_Base, REG_RSP);
		Assert::AreEqual<uint8_t>(instruction.m_Operands[1].m_Memory.m_Index, IL_INVALID_REGISTER);
		Assert::AreEqual<int32_t>(instruction.m_Operands[1].m_Memory.m_Offset, 0);

		Assert::AreEqual(instruction.m_Operands[2].m_Type, ILOperandType_None);
	}

	TEST_METHOD(Disassemble_mov_3)
	{
		constexpr uint8_t code[] = { 0x88, 0xE0 }; // mov al, ah

		std::vector<ILInstruction> instructions;
		Disassembler dissasembler;

		dissasembler.Disassemble(code, sizeof(code), instructions);

		Assert::AreEqual<size_t>(instructions.size(), 1);

		const ILInstruction& instruction = instructions[0];

		Assert::AreEqual(instruction.m_Type, InsType_mov);
		Assert::AreEqual<uint8_t>(instruction.m_Size, sizeof(code));

		Assert::AreEqual(instruction.m_Operands[0].m_Scale, ILOperandScale_8);
		Assert::AreEqual(instruction.m_Operands[0].m_Type, ILOperandType_Register);

		Assert::AreEqual(instruction.m_Operands[0].m_Register.m_Type, Register::general);
		Assert::AreEqual<uint8_t>(instruction.m_Operands[0].m_Register.m_Base, REG_AL);
		Assert::AreEqual<uint8_t>(instruction.m_Operands[0].m_Register.m_BaseHigh, false);

		Assert::AreEqual(instruction.m_Operands[1].m_Scale, ILOperandScale_8);
		Assert::AreEqual(instruction.m_Operands[1].m_Type, ILOperandType_Register);

		Assert::AreEqual(instruction.m_Operands[1].m_Register.m_Type, Register::general);
		Assert::AreEqual<uint8_t>(instruction.m_Operands[1].m_Register.m_Base, REG_AH);
		Assert::AreEqual<uint8_t>(instruction.m_Operands[1].m_Register.m_BaseHigh, true);

		Assert::AreEqual(instruction.m_Operands[2].m_Type, ILOperandType_None);
	}

	TEST_METHOD(Disassemble_mov_4)
	{
		constexpr uint8_t code[] = { 0x8B, 0x04, 0x25, 0x08, 0x00, 0x00, 0x00 }; // mov eax, dword ptr [8]

		std::vector<ILInstruction> instructions;
		Disassembler dissasembler;

		dissasembler.Disassemble(code, sizeof(code), instructions);

		Assert::IsTrue(instructions.size() > 0);

		Assert::AreEqual<size_t>(instructions.size(), 1);

		const ILInstruction& instruction = instructions[0];

		Assert::AreEqual(instruction.m_Type, InsType_mov);
		Assert::AreEqual<uint8_t>(instruction.m_Size, sizeof(code));

		Assert::AreEqual(instruction.m_Operands[0].m_Scale, ILOperandScale_32);
		Assert::AreEqual(instruction.m_Operands[0].m_Type, ILOperandType_Register);

		Assert::AreEqual(instruction.m_Operands[0].m_Register.m_Type, Register::general);
		Assert::AreEqual<uint8_t>(instruction.m_Operands[0].m_Register.m_Base, REG_EAX);
		Assert::AreEqual<uint8_t>(instruction.m_Operands[0].m_Register.m_BaseHigh, false);

		Assert::AreEqual(instruction.m_Operands[1].m_Scale, ILOperandScale_32);
		Assert::AreEqual(instruction.m_Operands[1].m_Type, ILOperandType_MemoryAbsolute);

		Assert::AreEqual<int64_t>(instruction.m_Operands[1].m_MemoryValue.m_Value, 8);
		Assert::AreEqual<uint8_t>(instruction.m_Operands[1].m_MemoryValue.m_Segment, IL_INVALID_REGISTER);

		Assert::AreEqual(instruction.m_Operands[2].m_Type, ILOperandType_None);
	}

	TEST_METHOD(Disassemble_mov_5)
	{
		constexpr uint8_t code[] = { 0x8B, 0x04, 0x05, 0x08, 0x00, 0x00, 0x00 }; // mov eax, dword ptr [rax + 8]

		std::vector<ILInstruction> instructions;
		Disassembler dissasembler;

		dissasembler.Disassemble(code, sizeof(code), instructions);

		Assert::IsTrue(instructions.size() > 0);

		Assert::AreEqual<size_t>(instructions.size(), 1);

		const ILInstruction& instruction = instructions[0];

		Assert::AreEqual(instruction.m_Type, InsType_mov);
		Assert::AreEqual<uint8_t>(instruction.m_Size, sizeof(code));

		Assert::AreEqual(instruction.m_Operands[0].m_Scale, ILOperandScale_32);
		Assert::AreEqual(instruction.m_Operands[0].m_Type, ILOperandType_Register);

		Assert::AreEqual(instruction.m_Operands[0].m_Register.m_Type, Register::general);
		Assert::AreEqual<uint8_t>(instruction.m_Operands[0].m_Register.m_Base, REG_EAX);
		Assert::AreEqual<uint8_t>(instruction.m_Operands[0].m_Register.m_BaseHigh, false);

		Assert::AreEqual(instruction.m_Operands[1].m_Scale, ILOperandScale_32);
		Assert::AreEqual(instruction.m_Operands[1].m_Type, ILOperandType_Memory);

		Assert::AreEqual<uint8_t>(instruction.m_Operands[1].m_Memory.m_Segment, IL_INVALID_REGISTER);
		Assert::AreEqual(instruction.m_Operands[1].m_Memory.m_Scale, ILMemoryScaler_1);
		Assert::AreEqual<uint8_t>(instruction.m_Operands[1].m_Memory.m_Base, IL_INVALID_REGISTER);
		Assert::AreEqual<uint8_t>(instruction.m_Operands[1].m_Memory.m_Index, REG_RAX);
		Assert::AreEqual<int32_t>(instruction.m_Operands[1].m_Memory.m_Offset, 8);

		Assert::AreEqual(instruction.m_Operands[2].m_Type, ILOperandType_None);
	}

	TEST_METHOD(Disassemble_mov_6)
	{
		constexpr uint8_t code[] = { 0x8B, 0x40, 0x08 }; // mov eax, dword ptr [rax + 8]

		std::vector<ILInstruction> instructions;
		Disassembler dissasembler;

		dissasembler.Disassemble(code, sizeof(code), instructions);

		Assert::IsTrue(instructions.size() > 0);

		Assert::AreEqual<size_t>(instructions.size(), 1);

		const ILInstruction& instruction = instructions[0];

		Assert::AreEqual(instruction.m_Type, InsType_mov);
		Assert::AreEqual<uint8_t>(instruction.m_Size, sizeof(code));

		Assert::AreEqual(instruction.m_Operands[0].m_Scale, ILOperandScale_32);
		Assert::AreEqual(instruction.m_Operands[0].m_Type, ILOperandType_Register);

		Assert::AreEqual(instruction.m_Operands[0].m_Register.m_Type, Register::general);
		Assert::AreEqual<uint8_t>(instruction.m_Operands[0].m_Register.m_Base, REG_EAX);
		Assert::AreEqual<uint8_t>(instruction.m_Operands[0].m_Register.m_BaseHigh, false);

		Assert::AreEqual(instruction.m_Operands[1].m_Scale, ILOperandScale_32);
		Assert::AreEqual(instruction.m_Operands[1].m_Type, ILOperandType_Memory);

		Assert::AreEqual<uint8_t>(instruction.m_Operands[1].m_Memory.m_Segment, IL_INVALID_REGISTER);
		Assert::AreEqual(instruction.m_Operands[1].m_Memory.m_Scale, ILMemoryScaler_1);
		Assert::AreEqual<uint8_t>(instruction.m_Operands[1].m_Memory.m_Base, REG_RAX);
		Assert::AreEqual<uint8_t>(instruction.m_Operands[1].m_Memory.m_Index, IL_INVALID_REGISTER);
		Assert::AreEqual<int32_t>(instruction.m_Operands[1].m_Memory.m_Offset, 8);

		Assert::AreEqual(instruction.m_Operands[2].m_Type, ILOperandType_None);
	}

	TEST_METHOD(Disassemble_mov_7)
	{
		constexpr uint8_t code[] = { 0x8B, 0x80, 0x00, 0x01, 0x00, 0x00 }; // mov eax, dword ptr [rax + 8]

		std::vector<ILInstruction> instructions;
		Disassembler dissasembler;

		dissasembler.Disassemble(code, sizeof(code), instructions);

		Assert::IsTrue(instructions.size() > 0);

		Assert::AreEqual<size_t>(instructions.size(), 1);

		const ILInstruction& instruction = instructions[0];

		Assert::AreEqual(instruction.m_Type, InsType_mov);
		Assert::AreEqual<uint8_t>(instruction.m_Size, sizeof(code));

		Assert::AreEqual(instruction.m_Operands[0].m_Scale, ILOperandScale_32);
		Assert::AreEqual(instruction.m_Operands[0].m_Type, ILOperandType_Register);

		Assert::AreEqual(instruction.m_Operands[0].m_Register.m_Type, Register::general);
		Assert::AreEqual<uint8_t>(instruction.m_Operands[0].m_Register.m_Base, REG_EAX);
		Assert::AreEqual<uint8_t>(instruction.m_Operands[0].m_Register.m_BaseHigh, false);

		Assert::AreEqual(instruction.m_Operands[1].m_Scale, ILOperandScale_32);
		Assert::AreEqual(instruction.m_Operands[1].m_Type, ILOperandType_Memory);

		Assert::AreEqual<uint8_t>(instruction.m_Operands[1].m_Memory.m_Segment, IL_INVALID_REGISTER);
		Assert::AreEqual(instruction.m_Operands[1].m_Memory.m_Scale, ILMemoryScaler_1);
		Assert::AreEqual<uint8_t>(instruction.m_Operands[1].m_Memory.m_Base, REG_RAX);
		Assert::AreEqual<uint8_t>(instruction.m_Operands[1].m_Memory.m_Index, IL_INVALID_REGISTER);
		Assert::AreEqual<int32_t>(instruction.m_Operands[1].m_Memory.m_Offset, 256);

		Assert::AreEqual(instruction.m_Operands[2].m_Type, ILOperandType_None);
	}

	TEST_METHOD(Disassemble_mov_8)
	{
		constexpr uint8_t code[] = { 0xA0, 0xEF, 0xCD, 0xAB, 0x89, 0x67, 0x45, 0x23, 0x01 }; // mov al, byte ptr [0123456789ABCDEF]

		std::vector<ILInstruction> instructions;
		Disassembler dissasembler;

		dissasembler.Disassemble(code, sizeof(code), instructions);

		Assert::IsTrue(instructions.size() > 0);

		Assert::AreEqual<size_t>(instructions.size(), 1);

		const ILInstruction& instruction = instructions[0];

		Assert::AreEqual(instruction.m_Type, InsType_mov);
		Assert::AreEqual<uint8_t>(instruction.m_Size, sizeof(code));

		Assert::AreEqual(instruction.m_Operands[0].m_Scale, ILOperandScale_8);
		Assert::AreEqual(instruction.m_Operands[0].m_Type, ILOperandType_Register);

		Assert::AreEqual(instruction.m_Operands[0].m_Register.m_Type, Register::general);
		Assert::AreEqual<uint8_t>(instruction.m_Operands[0].m_Register.m_Base, REG_AL);
		Assert::AreEqual<uint8_t>(instruction.m_Operands[0].m_Register.m_BaseHigh, false);

		Assert::AreEqual(instruction.m_Operands[1].m_Scale, ILOperandScale_8);
		Assert::AreEqual(instruction.m_Operands[1].m_Type, ILOperandType_MemoryAbsolute);

		Assert::AreEqual<uint8_t>(instruction.m_Operands[1].m_MemoryValue.m_Segment, IL_INVALID_REGISTER);
		Assert::AreEqual<int64_t>(instruction.m_Operands[1].m_MemoryValue.m_Value, 0x0123456789ABCDEF);

		Assert::AreEqual(instruction.m_Operands[2].m_Type, ILOperandType_None);
	}

	TEST_METHOD(Disassemble_mov_9)
	{
		constexpr uint8_t code[] = { 0x48, 0xB8, 0xEF, 0xCD, 0xAB, 0x89, 0x67, 0x45, 0x23, 0x01 }; // mov rax, 0x0123456789ABCDEF

		std::vector<ILInstruction> instructions;
		Disassembler dissasembler;

		dissasembler.Disassemble(code, sizeof(code), instructions);

		Assert::IsTrue(instructions.size() > 0);

		Assert::AreEqual<size_t>(instructions.size(), 1);

		const ILInstruction& instruction = instructions[0];

		Assert::AreEqual(instruction.m_Type, InsType_mov);
		Assert::AreEqual<uint8_t>(instruction.m_Size, sizeof(code));

		Assert::AreEqual(instruction.m_Operands[0].m_Scale, ILOperandScale_64);
		Assert::AreEqual(instruction.m_Operands[0].m_Type, ILOperandType_Register);

		Assert::AreEqual(instruction.m_Operands[0].m_Register.m_Type, Register::general);
		Assert::AreEqual<uint8_t>(instruction.m_Operands[0].m_Register.m_Base, REG_RAX);
		Assert::AreEqual<uint8_t>(instruction.m_Operands[0].m_Register.m_BaseHigh, false);

		Assert::AreEqual(instruction.m_Operands[1].m_Scale, ILOperandScale_64);
		Assert::AreEqual(instruction.m_Operands[1].m_Type, ILOperandType_Value);

		Assert::AreEqual<int64_t>(instruction.m_Operands[1].m_Value, 0x0123456789ABCDEF);

		Assert::AreEqual(instruction.m_Operands[2].m_Type, ILOperandType_None);
	}

	TEST_METHOD(Disassemble_fwait_0)
	{
		constexpr uint8_t code[] = { 0x9B, 0x01, 0xC0 }; // fwait, add eax, eax

		std::vector<ILInstruction> instructions;
		Disassembler dissasembler;

		dissasembler.Disassemble(code, sizeof(code), instructions);

		Assert::AreEqual<size_t>(instructions.size(), 2);

		const ILInstruction& first = instructions[0];

		Assert::AreEqual(first.m_Type, InsType_fwait);
		Assert::AreEqual<uint8_t>(first.m_Size, 1);

		Assert::AreEqual(first.m_Operands[0].m_Type, ILOperandType_None);

		const ILInstruction& second = instructions[1];

		Assert::AreEqual(second.m_Type, InsType_add);
		Assert::AreEqual<uint8_t>(second.m_Size, 2);

		Assert::AreEqual(second.m_Operands[0].m_Scale, ILOperandScale_32);
		Assert::AreEqual(second.m_Operands[0].m_Type, ILOperandType_Register);

		Assert::AreEqual(second.m_Operands[0].m_Register.m_Type, Register::general);
		Assert::AreEqual<uint8_t>(second.m_Operands[0].m_Register.m_Base, REG_EAX);
		Assert::AreEqual<uint8_t>(second.m_Operands[0].m_Register.m_BaseHigh, false);

		Assert::AreEqual(second.m_Operands[1].m_Scale, ILOperandScale_32);
		Assert::AreEqual(second.m_Operands[1].m_Type, ILOperandType_Register);

		Assert::AreEqual(second.m_Operands[1].m_Register.m_Type, Register::general);
		Assert::AreEqual<uint8_t>(second.m_Operands[1].m_Register.m_Base, REG_EAX);
		Assert::AreEqual<uint8_t>(second.m_Operands[1].m_Register.m_BaseHigh, false);

		Assert::AreEqual(second.m_Operands[2].m_Type, ILOperandType_None);
	}

	TEST_METHOD(Disassemble_fwait_1)
	{
		constexpr uint8_t code[] = { 0x9B, 0x9B, 0x01, 0xC0 }; // fwait, fwait, add eax, eax

		std::vector<ILInstruction> instructions;
		Disassembler dissasembler;

		dissasembler.Disassemble(code, sizeof(code), instructions);

		Assert::AreEqual<size_t>(instructions.size(), 3);

		const ILInstruction& first = instructions[0];

		Assert::AreEqual(first.m_Type, InsType_fwait);
		Assert::AreEqual<uint8_t>(first.m_Size, 1);

		Assert::AreEqual(first.m_Operands[0].m_Type, ILOperandType_None);

		const ILInstruction& second = instructions[1];

		Assert::AreEqual(second.m_Type, InsType_fwait);
		Assert::AreEqual<uint8_t>(second.m_Size, 1);

		Assert::AreEqual(second.m_Operands[0].m_Type, ILOperandType_None);

		const ILInstruction& third = instructions[2];

		Assert::AreEqual(third.m_Type, InsType_add);
		Assert::AreEqual<uint8_t>(third.m_Size, 2);

		Assert::AreEqual(third.m_Operands[0].m_Scale, ILOperandScale_32);
		Assert::AreEqual(third.m_Operands[0].m_Type, ILOperandType_Register);

		Assert::AreEqual(third.m_Operands[0].m_Register.m_Type, Register::general);
		Assert::AreEqual<uint8_t>(third.m_Operands[0].m_Register.m_Base, REG_EAX);
		Assert::AreEqual<uint8_t>(third.m_Operands[0].m_Register.m_BaseHigh, false);

		Assert::AreEqual(third.m_Operands[1].m_Scale, ILOperandScale_32);
		Assert::AreEqual(third.m_Operands[1].m_Type, ILOperandType_Register);

		Assert::AreEqual(third.m_Operands[1].m_Register.m_Type, Register::general);
		Assert::AreEqual<uint8_t>(third.m_Operands[1].m_Register.m_Base, REG_EAX);
		Assert::AreEqual<uint8_t>(third.m_Operands[1].m_Register.m_BaseHigh, false);

		Assert::AreEqual(third.m_Operands[2].m_Type, ILOperandType_None);
	}

	TEST_METHOD(Disassemble_pcmpestri)
	{
		constexpr uint8_t code[] = { 0x66, 0x46, 0x0F, 0x3A, 0x61, 0x7C, 0x00, 0x0A, 0x08 }; // pcmpestri xmm15, oword ptr [rax + r8 + 10], 8

		std::vector<ILInstruction> instructions;
		Disassembler dissasembler;

		dissasembler.Disassemble(code, sizeof(code), instructions);

		Assert::AreEqual<size_t>(instructions.size(), 1);

		const ILInstruction& instruction = instructions[0];

		Assert::AreEqual(instruction.m_Type, InsType_pcmpestri);
		Assert::AreEqual<uint8_t>(instruction.m_Size, sizeof(code));

		Assert::AreEqual(instruction.m_Operands[0].m_Scale, ILOperandScale_128);
		Assert::AreEqual(instruction.m_Operands[0].m_Type, ILOperandType_Register);

		Assert::AreEqual(instruction.m_Operands[0].m_Register.m_Type, Register::xmm);
		Assert::AreEqual<uint8_t>(instruction.m_Operands[0].m_Register.m_Base, REG_XMM15);
		Assert::AreEqual<uint8_t>(instruction.m_Operands[0].m_Register.m_BaseHigh, false);

		Assert::AreEqual(instruction.m_Operands[1].m_Scale, ILOperandScale_128);
		Assert::AreEqual(instruction.m_Operands[1].m_Type, ILOperandType_Memory);

		Assert::AreEqual<uint8_t>(instruction.m_Operands[1].m_Memory.m_Segment, IL_INVALID_REGISTER);
		Assert::AreEqual(instruction.m_Operands[1].m_Memory.m_Scale, ILMemoryScaler_1);
		Assert::AreEqual<uint8_t>(instruction.m_Operands[1].m_Memory.m_Base, REG_RAX);
		Assert::AreEqual<uint8_t>(instruction.m_Operands[1].m_Memory.m_Index, REG_R8);
		Assert::AreEqual<int32_t>(instruction.m_Operands[1].m_Memory.m_Offset, 10);

		Assert::AreEqual(instruction.m_Operands[2].m_Scale, ILOperandScale_8);
		Assert::AreEqual(instruction.m_Operands[2].m_Type, ILOperandType_Value);

		Assert::AreEqual<int64_t>(instruction.m_Operands[2].m_Value, 8);

		Assert::AreEqual(instruction.m_Operands[3].m_Type, ILOperandType_None);
	}

	TEST_METHOD(Disassemble_crc32)
	{
		constexpr uint8_t code[] = { 0x66, 0xF2, 0x0F, 0x38, 0xF1, 0xC0 }; // crc32 eax, ax

		std::vector<ILInstruction> instructions;
		Disassembler dissasembler;

		dissasembler.Disassemble(code, sizeof(code), instructions);

		Assert::AreEqual<size_t>(instructions.size(), 1);

		const ILInstruction& instruction = instructions[0];

		Assert::AreEqual(instruction.m_Type, InsType_crc32);
		Assert::AreEqual<uint8_t>(instruction.m_Size, sizeof(code));

		Assert::AreEqual(instruction.m_Operands[0].m_Scale, ILOperandScale_32);
		Assert::AreEqual(instruction.m_Operands[0].m_Type, ILOperandType_Register);

		Assert::AreEqual(instruction.m_Operands[0].m_Register.m_Type, Register::general);
		Assert::AreEqual<uint8_t>(instruction.m_Operands[0].m_Register.m_Base, REG_EAX);
		Assert::AreEqual<uint8_t>(instruction.m_Operands[0].m_Register.m_BaseHigh, false);

		Assert::AreEqual(instruction.m_Operands[1].m_Scale, ILOperandScale_16);
		Assert::AreEqual(instruction.m_Operands[1].m_Type, ILOperandType_Register);

		Assert::AreEqual(instruction.m_Operands[1].m_Register.m_Type, Register::general);
		Assert::AreEqual<uint8_t>(instruction.m_Operands[1].m_Register.m_Base, REG_AX);
		Assert::AreEqual<uint8_t>(instruction.m_Operands[1].m_Register.m_BaseHigh, false);

		Assert::AreEqual(instruction.m_Operands[2].m_Type, ILOperandType_None);
	}

	TEST_METHOD(Disassemble_psubq)
	{
		constexpr uint8_t code[] = { 0x0F, 0xFB, 0x3C, 0x00 }; // psubq mm7, qword ptr [rax + rax]

		std::vector<ILInstruction> instructions;
		Disassembler dissasembler;

		dissasembler.Disassemble(code, sizeof(code), instructions);

		Assert::AreEqual<size_t>(instructions.size(), 1);

		const ILInstruction& instruction = instructions[0];

		Assert::AreEqual(instruction.m_Type, InsType_psubq);
		Assert::AreEqual<uint8_t>(instruction.m_Size, sizeof(code));

		Assert::AreEqual(instruction.m_Operands[0].m_Scale, ILOperandScale_64);
		Assert::AreEqual(instruction.m_Operands[0].m_Type, ILOperandType_Register);

		Assert::AreEqual(instruction.m_Operands[0].m_Register.m_Type, Register::mm);
		Assert::AreEqual<uint8_t>(instruction.m_Operands[0].m_Register.m_Base, REG_MM7);
		Assert::AreEqual<uint8_t>(instruction.m_Operands[0].m_Register.m_BaseHigh, false);

		Assert::AreEqual(instruction.m_Operands[1].m_Scale, ILOperandScale_64);
		Assert::AreEqual(instruction.m_Operands[1].m_Type, ILOperandType_Memory);

		Assert::AreEqual<uint8_t>(instruction.m_Operands[1].m_Memory.m_Segment, IL_INVALID_REGISTER);
		Assert::AreEqual(instruction.m_Operands[1].m_Memory.m_Scale, ILMemoryScaler_1);
		Assert::AreEqual<uint8_t>(instruction.m_Operands[1].m_Memory.m_Base, REG_RAX);
		Assert::AreEqual<uint8_t>(instruction.m_Operands[1].m_Memory.m_Index, REG_RAX);
		Assert::AreEqual<int32_t>(instruction.m_Operands[1].m_Memory.m_Offset, 0);

		Assert::AreEqual(instruction.m_Operands[2].m_Type, ILOperandType_None);
	}

	TEST_METHOD(Disassemble_invalid_0)
	{
		constexpr uint8_t code[] = { 0x0F, 0x38, 0x37, 0x00 }; // invalid

		std::vector<ILInstruction> instructions;
		Disassembler dissasembler;

		dissasembler.Disassemble(code, sizeof(code), instructions);

		Assert::IsTrue(instructions.size() > 0);

		const ILInstruction& instruction = instructions[0];

		Assert::AreEqual(instruction.m_Type, InsType_invalid);
		Assert::AreEqual<uint8_t>(instruction.m_Size, 1);

		Assert::AreEqual(instruction.m_Operands[0].m_Type, ILOperandType_None);
	}

	TEST_METHOD(Disassemble_invalid_1)
	{
		constexpr uint8_t code[] = { 0x0F, 0x3A, 0x00 }; // invalid

		std::vector<ILInstruction> instructions;
		Disassembler dissasembler;

		dissasembler.Disassemble(code, sizeof(code), instructions);

		Assert::IsTrue(instructions.size() > 0);

		const ILInstruction& instruction = instructions[0];

		Assert::AreEqual(instruction.m_Type, InsType_invalid);
		Assert::AreEqual<uint8_t>(instruction.m_Size, 1);

		Assert::AreEqual(instruction.m_Operands[0].m_Type, ILOperandType_None);
	}

	TEST_METHOD(Disassemble_invalid_2)
	{
		constexpr uint8_t code[] = { 0x0F, 0x3A, 0xFF }; // invalid

		std::vector<ILInstruction> instructions;
		Disassembler dissasembler;

		dissasembler.Disassemble(code, sizeof(code), instructions);

		Assert::IsTrue(instructions.size() > 0);

		const ILInstruction& instruction = instructions[0];

		Assert::AreEqual(instruction.m_Type, InsType_invalid);
		Assert::AreEqual<uint8_t>(instruction.m_Size, 1);

		Assert::AreEqual(instruction.m_Operands[0].m_Type, ILOperandType_None);
	}

	TEST_METHOD(Disassemble_invalid_3)
	{
		constexpr uint8_t code[] = { 0xD9, 0xD8 }; // invalid

		std::vector<ILInstruction> instructions;
		Disassembler dissasembler;

		dissasembler.Disassemble(code, sizeof(code), instructions);

		Assert::IsTrue(instructions.size() > 0);

		const ILInstruction& instruction = instructions[0];

		Assert::AreEqual(instruction.m_Type, InsType_invalid);
		Assert::AreEqual<uint8_t>(instruction.m_Size, 1);

		Assert::AreEqual(instruction.m_Operands[0].m_Type, ILOperandType_None);
	}

	TEST_METHOD(Disassemble_invalid_4)
	{
		constexpr uint8_t code[] = { 0xD9, 0xD1 }; // invalid

		std::vector<ILInstruction> instructions;
		Disassembler dissasembler;

		dissasembler.Disassemble(code, sizeof(code), instructions);

		Assert::IsTrue(instructions.size() > 0);

		const ILInstruction& instruction = instructions[0];

		Assert::AreEqual(instruction.m_Type, InsType_invalid);
		Assert::AreEqual<uint8_t>(instruction.m_Size, 1);

		Assert::AreEqual(instruction.m_Operands[0].m_Type, ILOperandType_None);
	}

	TEST_METHOD(Disassemble_invalid_5)
	{
		constexpr uint8_t code[] = { 0x8D, 0xC0 }; // invalid

		std::vector<ILInstruction> instructions;
		Disassembler dissasembler;

		dissasembler.Disassemble(code, sizeof(code), instructions);

		Assert::IsTrue(instructions.size() > 0);

		const ILInstruction& instruction = instructions[0];

		Assert::AreEqual(instruction.m_Type, InsType_invalid);
		Assert::AreEqual<uint8_t>(instruction.m_Size, 1);

		Assert::AreEqual(instruction.m_Operands[0].m_Type, ILOperandType_None);
	}

	TEST_METHOD(Disassemble_invalid_6)
	{
		constexpr uint8_t code[] = { 0x8C, 0x38 }; // invalid

		std::vector<ILInstruction> instructions;
		Disassembler dissasembler;

		dissasembler.Disassemble(code, sizeof(code), instructions);

		Assert::IsTrue(instructions.size() > 0);

		const ILInstruction& instruction = instructions[0];

		Assert::AreEqual(instruction.m_Type, InsType_invalid);
		Assert::AreEqual<uint8_t>(instruction.m_Size, 1);

		Assert::AreEqual(instruction.m_Operands[0].m_Type, ILOperandType_None);
	}

	TEST_METHOD(Disassemble_invalid_7)
	{
		constexpr uint8_t code[] = { 0x66, 0x0F, 0x1A, 0xC7 }; // invalid

		std::vector<ILInstruction> instructions;
		Disassembler dissasembler;

		dissasembler.Disassemble(code, sizeof(code), instructions);

		Assert::IsTrue(instructions.size() > 0);

		const ILInstruction& instruction = instructions[0];

		Assert::AreEqual(instruction.m_Type, InsType_invalid);
		Assert::AreEqual<uint8_t>(instruction.m_Size, 1);

		Assert::AreEqual(instruction.m_Operands[0].m_Type, ILOperandType_None);
	}

	TEST_METHOD(Disassemble_invalid_8)
	{
		constexpr uint8_t code[] = { 0x48, 0x66, 0x89, 0xC4 }; // invalid rex.w .66 mov rsp, rax

		std::vector<ILInstruction> instructions;
		Disassembler dissasembler;

		dissasembler.Disassemble(code, sizeof(code), instructions);

		Assert::IsTrue(instructions.size() > 0);

		const ILInstruction& instruction = instructions[0];

		Assert::AreEqual(instruction.m_Type, InsType_invalid);
		Assert::AreEqual<uint8_t>(instruction.m_Size, 1);

		Assert::AreEqual(instruction.m_Operands[0].m_Type, ILOperandType_None);
	}

	TEST_METHOD(Disassemble_fxam)
	{
		constexpr uint8_t code[] = { 0xD9, 0xE5 }; // fxam

		std::vector<ILInstruction> instructions;
		Disassembler dissasembler;

		dissasembler.Disassemble(code, sizeof(code), instructions);

		Assert::AreEqual<size_t>(instructions.size(), 1);

		const ILInstruction& instruction = instructions[0];

		Assert::AreEqual(instruction.m_Type, InsType_fxam);
		Assert::AreEqual<uint8_t>(instruction.m_Size, sizeof(code));

		Assert::AreEqual(instruction.m_Operands[0].m_Type, ILOperandType_None);
	}

	TEST_METHOD(Disassemble_idiv)
	{
		constexpr uint8_t code[] = { 0x49, 0xF7, 0xFF }; // idiv r15

		std::vector<ILInstruction> instructions;
		Disassembler dissasembler;

		dissasembler.Disassemble(code, sizeof(code), instructions);

		Assert::AreEqual<size_t>(instructions.size(), 1);

		const ILInstruction& instruction = instructions[0];

		Assert::AreEqual(instruction.m_Type, InsType_idiv);
		Assert::AreEqual<uint8_t>(instruction.m_Size, sizeof(code));

		Assert::AreEqual(instruction.m_Operands[0].m_Scale, ILOperandScale_64);
		Assert::AreEqual(instruction.m_Operands[0].m_Type, ILOperandType_Register);

		Assert::AreEqual(instruction.m_Operands[0].m_Register.m_Type, Register::general);
		Assert::AreEqual<uint8_t>(instruction.m_Operands[0].m_Register.m_Base, REG_R15);
		Assert::AreEqual<uint8_t>(instruction.m_Operands[0].m_Register.m_BaseHigh, false);

		Assert::AreEqual(instruction.m_Operands[1].m_Type, ILOperandType_None);
	}

	TEST_METHOD(Disassemble_fmul)
	{
		constexpr uint8_t code[] = { 0xD8, 0xCF }; // fmul st(0), st(7)

		std::vector<ILInstruction> instructions;
		Disassembler dissasembler;

		dissasembler.Disassemble(code, sizeof(code), instructions);

		Assert::AreEqual<size_t>(instructions.size(), 1);

		const ILInstruction& instruction = instructions[0];

		Assert::AreEqual(instruction.m_Type, InsType_fmul);
		Assert::AreEqual<uint8_t>(instruction.m_Size, sizeof(code));

		Assert::AreEqual(instruction.m_Operands[0].m_Scale, ILOperandScale_80);
		Assert::AreEqual(instruction.m_Operands[0].m_Type, ILOperandType_Register);

		Assert::AreEqual(instruction.m_Operands[0].m_Register.m_Type, Register::st);
		Assert::AreEqual<uint8_t>(instruction.m_Operands[0].m_Register.m_Base, REG_ST0);
		Assert::AreEqual<uint8_t>(instruction.m_Operands[0].m_Register.m_BaseHigh, false);

		Assert::AreEqual(instruction.m_Operands[1].m_Scale, ILOperandScale_80);
		Assert::AreEqual(instruction.m_Operands[1].m_Type, ILOperandType_Register);

		Assert::AreEqual(instruction.m_Operands[1].m_Register.m_Type, Register::st);
		Assert::AreEqual<uint8_t>(instruction.m_Operands[1].m_Register.m_Base, REG_ST7);
		Assert::AreEqual<uint8_t>(instruction.m_Operands[1].m_Register.m_BaseHigh, false);

		Assert::AreEqual(instruction.m_Operands[2].m_Type, ILOperandType_None);
	}

	TEST_METHOD(Disassemble_cmp)
	{
		constexpr uint8_t code[] = { 0x3C, 0x09 }; // cmp al, 9

		std::vector<ILInstruction> instructions;
		Disassembler dissasembler;

		dissasembler.Disassemble(code, sizeof(code), instructions);

		Assert::AreEqual<size_t>(instructions.size(), 1);

		const ILInstruction& instruction = instructions[0];

		Assert::AreEqual(instruction.m_Type, InsType_cmp);
		Assert::AreEqual<uint8_t>(instruction.m_Size, sizeof(code));

		Assert::AreEqual(instruction.m_Operands[0].m_Scale, ILOperandScale_8);
		Assert::AreEqual(instruction.m_Operands[0].m_Type, ILOperandType_Register);

		Assert::AreEqual(instruction.m_Operands[0].m_Register.m_Type, Register::general);
		Assert::AreEqual<uint8_t>(instruction.m_Operands[0].m_Register.m_Base, REG_AL);
		Assert::AreEqual<uint8_t>(instruction.m_Operands[0].m_Register.m_BaseHigh, false);

		Assert::AreEqual(instruction.m_Operands[1].m_Scale, ILOperandScale_8);
		Assert::AreEqual(instruction.m_Operands[1].m_Type, ILOperandType_Value);

		Assert::AreEqual<int64_t>(instruction.m_Operands[1].m_Value, 9);

		Assert::AreEqual(instruction.m_Operands[2].m_Type, ILOperandType_None);
	}

	TEST_METHOD(Disassemble_bswap)
	{
		constexpr uint8_t code[] = { 0x4F, 0x0F, 0xC8 }; // bswap r8

		std::vector<ILInstruction> instructions;
		Disassembler dissasembler;

		dissasembler.Disassemble(code, sizeof(code), instructions);

		Assert::AreEqual<size_t>(instructions.size(), 1);

		const ILInstruction& instruction = instructions[0];

		Assert::AreEqual(instruction.m_Type, InsType_bswap);
		Assert::AreEqual<uint8_t>(instruction.m_Size, sizeof(code));

		Assert::AreEqual(instruction.m_Operands[0].m_Scale, ILOperandScale_64);
		Assert::AreEqual(instruction.m_Operands[0].m_Type, ILOperandType_Register);

		Assert::AreEqual(instruction.m_Operands[0].m_Register.m_Type, Register::general);
		Assert::AreEqual<uint8_t>(instruction.m_Operands[0].m_Register.m_Base, REG_R8);
		Assert::AreEqual<uint8_t>(instruction.m_Operands[0].m_Register.m_BaseHigh, false);

		Assert::AreEqual(instruction.m_Operands[1].m_Type, ILOperandType_None);
	}

	TEST_METHOD(Disassemble_rol)
	{
		constexpr uint8_t code[] = { 0x41, 0xD1, 0xC1 }; // rol r9d, 1

		std::vector<ILInstruction> instructions;
		Disassembler dissasembler;

		dissasembler.Disassemble(code, sizeof(code), instructions);

		Assert::AreEqual<size_t>(instructions.size(), 1);

		const ILInstruction& instruction = instructions[0];

		Assert::AreEqual(instruction.m_Type, InsType_rol);
		Assert::AreEqual<uint8_t>(instruction.m_Size, sizeof(code));

		Assert::AreEqual(instruction.m_Operands[0].m_Scale, ILOperandScale_32);
		Assert::AreEqual(instruction.m_Operands[0].m_Type, ILOperandType_Register);

		Assert::AreEqual(instruction.m_Operands[0].m_Register.m_Type, Register::general);
		Assert::AreEqual<uint8_t>(instruction.m_Operands[0].m_Register.m_Base, REG_R9);
		Assert::AreEqual<uint8_t>(instruction.m_Operands[0].m_Register.m_BaseHigh, false);

		Assert::AreEqual(instruction.m_Operands[1].m_Scale, ILOperandScale_8);
		Assert::AreEqual(instruction.m_Operands[1].m_Type, ILOperandType_Value);

		Assert::AreEqual<int64_t>(instruction.m_Operands[1].m_Value, 1);

		Assert::AreEqual(instruction.m_Operands[2].m_Type, ILOperandType_None);
	}

	TEST_METHOD(Disassemble_ret)
	{
		constexpr uint8_t code[] = { 0xC2, 0xBB, 0xAA }; // ret 0xAABB

		std::vector<ILInstruction> instructions;
		Disassembler dissasembler;

		dissasembler.Disassemble(code, sizeof(code), instructions);

		Assert::AreEqual<size_t>(instructions.size(), 1);

		const ILInstruction& instruction = instructions[0];

		Assert::AreEqual(instruction.m_Type, InsType_ret);
		Assert::AreEqual<uint8_t>(instruction.m_Size, sizeof(code));

		Assert::AreEqual(instruction.m_Operands[0].m_Scale, ILOperandScale_16);
		Assert::AreEqual(instruction.m_Operands[0].m_Type, ILOperandType_Value);

		Assert::AreEqual<int64_t>(instruction.m_Operands[0].m_Value, static_cast<int16_t>(0xAABB));

		Assert::AreEqual(instruction.m_Operands[1].m_Type, ILOperandType_None);
	}

	TEST_METHOD(Disassemble_sbb)
	{
		constexpr uint8_t code[] = { 0x66, 0x1D, 0x23, 0x01 }; // sbb ax, 0x0123

		std::vector<ILInstruction> instructions;
		Disassembler dissasembler;

		dissasembler.Disassemble(code, sizeof(code), instructions);

		Assert::AreEqual<size_t>(instructions.size(), 1);

		const ILInstruction& instruction = instructions[0];

		Assert::AreEqual(instruction.m_Type, InsType_sbb);
		Assert::AreEqual<uint8_t>(instruction.m_Size, sizeof(code));

		Assert::AreEqual(instruction.m_Operands[0].m_Scale, ILOperandScale_16);
		Assert::AreEqual(instruction.m_Operands[0].m_Type, ILOperandType_Register);

		Assert::AreEqual(instruction.m_Operands[0].m_Register.m_Type, Register::general);
		Assert::AreEqual<uint8_t>(instruction.m_Operands[0].m_Register.m_Base, REG_AX);
		Assert::AreEqual<uint8_t>(instruction.m_Operands[0].m_Register.m_BaseHigh, false);

		Assert::AreEqual(instruction.m_Operands[1].m_Scale, ILOperandScale_16);
		Assert::AreEqual(instruction.m_Operands[1].m_Type, ILOperandType_Value);

		Assert::AreEqual<int64_t>(instruction.m_Operands[1].m_Value, 0x0123);

		Assert::AreEqual(instruction.m_Operands[2].m_Type, ILOperandType_None);
	}

	TEST_METHOD(Disassemble_xor)
	{
		constexpr uint8_t code[] = { 0x41, 0x81, 0xF5, 0x67, 0x45, 0x23, 0x01 }; // xor r13d, 0x01234567

		std::vector<ILInstruction> instructions;
		Disassembler dissasembler;

		dissasembler.Disassemble(code, sizeof(code), instructions);

		Assert::AreEqual<size_t>(instructions.size(), 1);

		const ILInstruction& instruction = instructions[0];

		Assert::AreEqual(instruction.m_Type, InsType_xor);
		Assert::AreEqual<uint8_t>(instruction.m_Size, sizeof(code));

		Assert::AreEqual(instruction.m_Operands[0].m_Scale, ILOperandScale_32);
		Assert::AreEqual(instruction.m_Operands[0].m_Type, ILOperandType_Register);

		Assert::AreEqual(instruction.m_Operands[0].m_Register.m_Type, Register::general);
		Assert::AreEqual<uint8_t>(instruction.m_Operands[0].m_Register.m_Base, REG_R13D);
		Assert::AreEqual<uint8_t>(instruction.m_Operands[0].m_Register.m_BaseHigh, false);

		Assert::AreEqual(instruction.m_Operands[1].m_Scale, ILOperandScale_32);
		Assert::AreEqual(instruction.m_Operands[1].m_Type, ILOperandType_Value);

		Assert::AreEqual<int64_t>(instruction.m_Operands[1].m_Value, 0x01234567);

		Assert::AreEqual(instruction.m_Operands[2].m_Type, ILOperandType_None);
	}

	TEST_METHOD(Disassemble_vmread)
	{
		constexpr uint8_t code[] = { 0x45, 0x0F, 0x78, 0x5C, 0x24, 0x0A }; // vmread qword ptr [r12 + 10], r11

		std::vector<ILInstruction> instructions;
		Disassembler dissasembler;

		dissasembler.Disassemble(code, sizeof(code), instructions);

		Assert::AreEqual<size_t>(instructions.size(), 1);

		const ILInstruction& instruction = instructions[0];

		Assert::AreEqual(instruction.m_Type, InsType_vmread);
		Assert::AreEqual<uint8_t>(instruction.m_Size, sizeof(code));

		Assert::AreEqual(instruction.m_Operands[0].m_Scale, ILOperandScale_64);
		Assert::AreEqual(instruction.m_Operands[0].m_Type, ILOperandType_Memory);

		Assert::AreEqual<uint8_t>(instruction.m_Operands[0].m_Memory.m_Segment, IL_INVALID_REGISTER);
		Assert::AreEqual(instruction.m_Operands[0].m_Memory.m_Scale, ILMemoryScaler_1);
		Assert::AreEqual<uint8_t>(instruction.m_Operands[0].m_Memory.m_Base, REG_R12);
		Assert::AreEqual<uint8_t>(instruction.m_Operands[0].m_Memory.m_Index, IL_INVALID_REGISTER);
		Assert::AreEqual<int32_t>(instruction.m_Operands[0].m_Memory.m_Offset, 10);

		Assert::AreEqual(instruction.m_Operands[1].m_Scale, ILOperandScale_64);
		Assert::AreEqual(instruction.m_Operands[1].m_Type, ILOperandType_Register);

		Assert::AreEqual(instruction.m_Operands[1].m_Register.m_Type, Register::general);
		Assert::AreEqual<uint8_t>(instruction.m_Operands[1].m_Register.m_Base, REG_R11);
		Assert::AreEqual<uint8_t>(instruction.m_Operands[1].m_Register.m_BaseHigh, false);

		Assert::AreEqual(instruction.m_Operands[2].m_Type, ILOperandType_None);
	}

	TEST_METHOD(Disassemble_jo)
	{
		constexpr uint8_t code[] = { 0x0F, 0x80, 0x44, 0x33, 0x22, 0x11 }; // jo 0x11223344

		std::vector<ILInstruction> instructions;
		Disassembler dissasembler;

		dissasembler.Disassemble(code, sizeof(code), instructions);

		Assert::AreEqual<size_t>(instructions.size(), 1);

		const ILInstruction& instruction = instructions[0];

		Assert::AreEqual(instruction.m_Type, InsType_jo);
		Assert::AreEqual<uint8_t>(instruction.m_Size, sizeof(code));

		Assert::AreEqual(instruction.m_Operands[0].m_Scale, ILOperandScale_32);
		Assert::AreEqual(instruction.m_Operands[0].m_Type, ILOperandType_ValueRelative);

		Assert::AreEqual<uint8_t>(instruction.m_Operands[0].m_Relative.m_Segment, IL_INVALID_REGISTER);
		Assert::AreEqual<int64_t>(instruction.m_Operands[0].m_Relative.m_Value, 0x11223344);

		Assert::AreEqual(instruction.m_Operands[1].m_Type, ILOperandType_None);
	}

	TEST_METHOD(Disassemble_fstp)
	{
		constexpr uint8_t code[] = { 0x41, 0xDB, 0x7A, 0x2D }; // fstp tbyte ptr [r10 + 45]

		std::vector<ILInstruction> instructions;
		Disassembler dissasembler;

		dissasembler.Disassemble(code, sizeof(code), instructions);

		Assert::AreEqual<size_t>(instructions.size(), 1);

		const ILInstruction& instruction = instructions[0];

		Assert::AreEqual(instruction.m_Type, InsType_fstp);
		Assert::AreEqual<uint8_t>(instruction.m_Size, sizeof(code));

		Assert::AreEqual(instruction.m_Operands[0].m_Scale, ILOperandScale_80);
		Assert::AreEqual(instruction.m_Operands[0].m_Type, ILOperandType_Memory);

		Assert::AreEqual<uint8_t>(instruction.m_Operands[0].m_Memory.m_Segment, IL_INVALID_REGISTER);
		Assert::AreEqual(instruction.m_Operands[0].m_Memory.m_Scale, ILMemoryScaler_1);
		Assert::AreEqual<uint8_t>(instruction.m_Operands[0].m_Memory.m_Base, REG_R10);
		Assert::AreEqual<uint8_t>(instruction.m_Operands[0].m_Memory.m_Index, IL_INVALID_REGISTER);
		Assert::AreEqual<int32_t>(instruction.m_Operands[0].m_Memory.m_Offset, 45);
	}
};