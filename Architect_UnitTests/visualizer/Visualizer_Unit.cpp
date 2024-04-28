#include "pch.h"
#include "utility/Setup.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

TEST_CLASS(Visualizer_Unit)
{
private:
	enum VisRegisterBase
	{
		VisReg_General_64 = 0,
		VisReg_General_32 = 16,
		VisReg_General_16 = 32,
		VisReg_General_8l = 48,
		VisReg_General_8h = 64,
		VisReg_SSE_128 = 68,
		VisReg_MMX_64 = 84,
		VisReg_MPX_64 = 100,
		VisReg_FPU_80 = 104,
		VisReg_CR_64 = 112,
		VisReg_DR_64 = 128,
		VisReg_Sreg_64 = 144,
	};

public:
	TEST_METHOD(Visualize_Memory_Value_Register_ValueRelative)
	{
		std::vector<ILInstruction> instructions;

		ILInstruction instruction = {};

		instruction.m_Size = 0x50;
		instruction.m_Type = InsType_add;

		instruction.m_Operands[0].m_Type = ILOperandType_Memory;
		instruction.m_Operands[0].m_Scale = ILOperandScale_32;

		instruction.m_Operands[0].m_Memory.m_Segment = REG_GS;
		instruction.m_Operands[0].m_Memory.m_Scale = ILMemoryScaler_4;

		instruction.m_Operands[0].m_Memory.m_Base = REG_RAX;
		instruction.m_Operands[0].m_Memory.m_Index = REG_R8;

		instruction.m_Operands[0].m_Memory.m_Offset = 0x10;

		instruction.m_Operands[1].m_Type = ILOperandType_Value;
		instruction.m_Operands[1].m_Scale = ILOperandScale_8;

		instruction.m_Operands[1].m_Value = 0x32;

		instruction.m_Operands[2].m_Type = ILOperandType_Register;
		instruction.m_Operands[2].m_Scale = ILOperandScale_128;

		instruction.m_Operands[2].m_Register.m_Base = REG_XMM1;
		instruction.m_Operands[2].m_Register.m_BaseHigh = false;

		instruction.m_Operands[2].m_Register.m_Type = Register::xmm;

		instruction.m_Operands[3].m_Type = ILOperandType_ValueRelative;
		instruction.m_Operands[3].m_Scale = ILOperandScale_8;

		instruction.m_Operands[3].m_Relative.m_Value = 0x10;
		instruction.m_Operands[3].m_Relative.m_Segment = IL_INVALID_REGISTER;

		instructions.push_back(instruction);

		Visualizer visualizer;

		std::vector<Visualizer::Visual> visuals = visualizer.ToVisuals(instructions, reinterpret_cast<const void*>(1ull << 63));

		Assert::AreEqual<size_t>(visuals.size(), 5);

		const Visualizer::Visual& first = visuals[0];

		Assert::AreEqual(first.m_Type, Visualizer::VisType::Instruction);

		Assert::AreEqual(first.m_Instruction.m_Type, instruction.m_Type);
		Assert::AreEqual<uint8_t>(first.m_Instruction.m_Size, instruction.m_Size);

		const Visualizer::Visual& second = visuals[1];

		Assert::AreEqual(second.m_Type, Visualizer::VisType::OperandMemory);

		Assert::AreEqual(second.m_Memory.m_Size, ILOperandScale_32);
		Assert::AreEqual<uint8_t>(second.m_Memory.m_Base, VisReg_General_64 + REG_RAX);
		Assert::AreEqual<uint8_t>(second.m_Memory.m_Index, VisReg_General_64 + REG_R8);
		Assert::AreEqual<uint8_t>(second.m_Memory.m_Segment, VisReg_Sreg_64 + REG_GS);
		Assert::AreEqual<uint8_t>(second.m_Memory.m_Multiplier, 4);
		Assert::AreEqual<int32_t>(second.m_Memory.m_Offset, 0x10);

		const Visualizer::Visual& thrid = visuals[2];

		Assert::AreEqual(thrid.m_Type, Visualizer::VisType::OperandValue);

		Assert::AreEqual(thrid.m_Value.m_Size, ILOperandScale_8);
		Assert::AreEqual<uint8_t>(thrid.m_Value.m_Segment, VIS_INVALID_REGISTER);
		Assert::AreEqual<int64_t>(thrid.m_Value.m_Value, 0x32);

		const Visualizer::Visual& fourth = visuals[3];

		Assert::AreEqual(fourth.m_Type, Visualizer::VisType::OperandRegister);

		Assert::AreEqual<uint8_t>(fourth.m_Register, VisReg_SSE_128 + REG_XMM1);

		const Visualizer::Visual& fifth = visuals[4];

		Assert::AreEqual(fifth.m_Type, Visualizer::VisType::OperandAddressValue);

		Assert::AreEqual(fifth.m_Value.m_Size, ILOperandScale_8);
		Assert::AreEqual<uint8_t>(fifth.m_Value.m_Segment, VIS_INVALID_REGISTER);
		Assert::AreEqual<int64_t>(fifth.m_Value.m_Value, (1ull << 63) + 0x10 + instruction.m_Size);
	}

	TEST_METHOD(Visualize_MemoryRelative_MemoryAbsolute)
	{
		std::vector<ILInstruction> instructions;

		ILInstruction instruction = {};

		instruction.m_Size = 0x10;
		instruction.m_Type = InsType_vmcall;

		instruction.m_Operands[0].m_Type = ILOperandType_MemoryRelative;
		instruction.m_Operands[0].m_Scale = ILOperandScale_64;

		instruction.m_Operands[0].m_Relative.m_Segment = REG_FS;
		instruction.m_Operands[0].m_Relative.m_Value = 0x1000;

		instruction.m_Operands[1].m_Type = ILOperandType_MemoryAbsolute;
		instruction.m_Operands[1].m_Scale = ILOperandScale_128;

		instruction.m_Operands[1].m_MemoryValue.m_Segment = REG_GS;
		instruction.m_Operands[1].m_MemoryValue.m_Value = 0x1000000;

		instructions.push_back(instruction);

		Visualizer visualizer;

		std::vector<Visualizer::Visual> visuals = visualizer.ToVisuals(instructions, reinterpret_cast<const void*>(1ull << 63));

		Assert::AreEqual<size_t>(visuals.size(), 3);

		const Visualizer::Visual& first = visuals[0];

		Assert::AreEqual(first.m_Type, Visualizer::VisType::Instruction);

		Assert::AreEqual(first.m_Instruction.m_Type, instruction.m_Type);
		Assert::AreEqual<uint8_t>(first.m_Instruction.m_Size, instruction.m_Size);

		const Visualizer::Visual& second = visuals[1];

		Assert::AreEqual(second.m_Type, Visualizer::VisType::OperandMemoryValue);

		Assert::AreEqual(second.m_Value.m_Size, ILOperandScale_64);
		Assert::AreEqual<uint8_t>(second.m_Value.m_Segment, VisReg_Sreg_64 + REG_FS);
		Assert::AreEqual<int64_t>(second.m_Value.m_Value, (1ull << 63) + 0x1000 + instruction.m_Size);

		const Visualizer::Visual& thrid = visuals[2];

		Assert::AreEqual(thrid.m_Type, Visualizer::VisType::OperandMemoryValue);

		Assert::AreEqual(thrid.m_Value.m_Size, ILOperandScale_128);
		Assert::AreEqual<uint8_t>(thrid.m_Value.m_Segment, VisReg_Sreg_64 + REG_GS);
		Assert::AreEqual<int64_t>(thrid.m_Value.m_Value, 0x1000000);
	}

	TEST_METHOD(Visualize_Invalid)
	{
		std::vector<ILInstruction> instructions;

		ILInstruction instruction = {};

		instruction.m_Size = 1;
		instruction.m_Type = InsType_invalid;

		instructions.push_back(instruction);

		Visualizer visualizer;

		std::vector<Visualizer::Visual> visuals = visualizer.ToVisuals(instructions, reinterpret_cast<const void*>(1ull << 63));

		Assert::AreEqual<size_t>(visuals.size(), 1);

		const Visualizer::Visual& first = visuals[0];

		Assert::AreEqual(first.m_Type, Visualizer::VisType::Instruction);

		Assert::AreEqual(first.m_Instruction.m_Type, instruction.m_Type);
		Assert::AreEqual<uint8_t>(first.m_Instruction.m_Size, instruction.m_Size);
	}

	TEST_METHOD(Visualize_Operands_0)
	{
		std::vector<ILInstruction> instructions;

		ILInstruction instruction = {};

		instruction.m_Size = 1;
		instruction.m_Type = InsType_loop;

		instruction.m_Operands[0].m_Scale = ILOperandScale_80;
		instruction.m_Operands[0].m_Type = ILOperandType_Memory;

		instruction.m_Operands[0].m_Memory.m_Segment = IL_INVALID_REGISTER;
		instruction.m_Operands[0].m_Memory.m_Scale = ILMemoryScaler_2;

		instruction.m_Operands[0].m_Memory.m_Base = IL_INVALID_REGISTER;
		instruction.m_Operands[0].m_Memory.m_Index = REG_RDX;

		instruction.m_Operands[0].m_Memory.m_Offset = 0x100;

		instruction.m_Operands[1].m_Scale = ILOperandScale_32;
		instruction.m_Operands[1].m_Type = ILOperandType_Memory;

		instruction.m_Operands[1].m_Memory.m_Segment = IL_INVALID_REGISTER;
		instruction.m_Operands[1].m_Memory.m_Scale = ILMemoryScaler_8;

		instruction.m_Operands[1].m_Memory.m_Base = REG_RCX;
		instruction.m_Operands[1].m_Memory.m_Index = REG_RDX;

		instruction.m_Operands[1].m_Memory.m_Offset = 0;

		instruction.m_Operands[2].m_Scale = ILOperandScale_64;
		instruction.m_Operands[2].m_Type = ILOperandType_Memory;

		instruction.m_Operands[2].m_Memory.m_Segment = REG_GS;
		instruction.m_Operands[2].m_Memory.m_Scale = ILMemoryScaler_1;

		instruction.m_Operands[2].m_Memory.m_Base = REG_RDI;
		instruction.m_Operands[2].m_Memory.m_Index = IL_INVALID_REGISTER;

		instruction.m_Operands[2].m_Memory.m_Offset = 0x1000;

		instruction.m_Operands[3].m_Scale = ILOperandScale_8;
		instruction.m_Operands[3].m_Type = ILOperandType_Register;

		instruction.m_Operands[3].m_Register.m_Type = Register::general;
		instruction.m_Operands[3].m_Register.m_Base = REG_BL;
		instruction.m_Operands[3].m_Register.m_BaseHigh = false;

		instructions.push_back(instruction);

		Visualizer visualizer;

		std::vector<Visualizer::Visual> visuals = visualizer.ToVisuals(instructions, reinterpret_cast<const void*>(1ull << 63));

		Assert::AreEqual<size_t>(visuals.size(), 5);

		const Visualizer::Visual& first = visuals[0];

		Assert::AreEqual(first.m_Type, Visualizer::VisType::Instruction);

		Assert::AreEqual(first.m_Instruction.m_Type, instruction.m_Type);
		Assert::AreEqual<uint8_t>(first.m_Instruction.m_Size, instruction.m_Size);

		const Visualizer::Visual& second = visuals[1];

		Assert::AreEqual(second.m_Type, Visualizer::VisType::OperandMemory);

		Assert::AreEqual(second.m_Memory.m_Size, ILOperandScale_80);
		Assert::AreEqual<uint8_t>(second.m_Memory.m_Base, VIS_INVALID_REGISTER);
		Assert::AreEqual<uint8_t>(second.m_Memory.m_Index, VisReg_General_64 + REG_RDX);
		Assert::AreEqual<uint8_t>(second.m_Memory.m_Segment, VIS_INVALID_REGISTER);
		Assert::AreEqual<uint8_t>(second.m_Memory.m_Multiplier, 2);
		Assert::AreEqual<int32_t>(second.m_Memory.m_Offset, 0x100);

		const Visualizer::Visual& third = visuals[2];

		Assert::AreEqual(third.m_Type, Visualizer::VisType::OperandMemory);

		Assert::AreEqual(third.m_Memory.m_Size, ILOperandScale_32);
		Assert::AreEqual<uint8_t>(third.m_Memory.m_Base, VisReg_General_64 + REG_RCX);
		Assert::AreEqual<uint8_t>(third.m_Memory.m_Index, VisReg_General_64 + REG_RDX);
		Assert::AreEqual<uint8_t>(third.m_Memory.m_Segment, VIS_INVALID_REGISTER);
		Assert::AreEqual<uint8_t>(third.m_Memory.m_Multiplier, 8);
		Assert::AreEqual<int32_t>(third.m_Memory.m_Offset, 0);

		const Visualizer::Visual& fourth = visuals[3];

		Assert::AreEqual(fourth.m_Type, Visualizer::VisType::OperandMemory);

		Assert::AreEqual(fourth.m_Memory.m_Size, ILOperandScale_64);
		Assert::AreEqual<uint8_t>(fourth.m_Memory.m_Base, VisReg_General_64 + REG_RDI);
		Assert::AreEqual<uint8_t>(fourth.m_Memory.m_Index, VIS_INVALID_REGISTER);
		Assert::AreEqual<uint8_t>(fourth.m_Memory.m_Segment, VisReg_Sreg_64 + REG_GS);
		Assert::AreEqual<uint8_t>(fourth.m_Memory.m_Multiplier, 1);
		Assert::AreEqual<int32_t>(fourth.m_Memory.m_Offset, 0x1000);

		const Visualizer::Visual& fifth = visuals[4];

		Assert::AreEqual(fifth.m_Type, Visualizer::VisType::OperandRegister);

		Assert::AreEqual<uint8_t>(fifth.m_Register, VisReg_General_8l + REG_BL);
	}

	TEST_METHOD(Visualize_Operands_1)
	{
		std::vector<ILInstruction> instructions;

		ILInstruction instruction = {};

		instruction.m_Size = 45;
		instruction.m_Type = InsType_xor;

		instruction.m_Operands[0].m_Scale = ILOperandScale_8;
		instruction.m_Operands[0].m_Type = ILOperandType_Register;

		instruction.m_Operands[0].m_Register.m_Type = Register::general;
		instruction.m_Operands[0].m_Register.m_Base = REG_CH;
		instruction.m_Operands[0].m_Register.m_BaseHigh = true;

		instruction.m_Operands[1].m_Scale = ILOperandScale_16;
		instruction.m_Operands[1].m_Type = ILOperandType_Register;

		instruction.m_Operands[1].m_Register.m_Type = Register::general;
		instruction.m_Operands[1].m_Register.m_Base = REG_AX;
		instruction.m_Operands[1].m_Register.m_BaseHigh = false;

		instruction.m_Operands[2].m_Scale = ILOperandScale_32;
		instruction.m_Operands[2].m_Type = ILOperandType_Register;

		instruction.m_Operands[2].m_Register.m_Type = Register::general;
		instruction.m_Operands[2].m_Register.m_Base = REG_R8D;
		instruction.m_Operands[2].m_Register.m_BaseHigh = false;

		instruction.m_Operands[3].m_Scale = ILOperandScale_64;
		instruction.m_Operands[3].m_Type = ILOperandType_Register;

		instruction.m_Operands[3].m_Register.m_Type = Register::general;
		instruction.m_Operands[3].m_Register.m_Base = REG_R10;
		instruction.m_Operands[3].m_Register.m_BaseHigh = false;

		instructions.push_back(instruction);

		Visualizer visualizer;

		std::vector<Visualizer::Visual> visuals = visualizer.ToVisuals(instructions, reinterpret_cast<const void*>(1ull << 63));

		Assert::AreEqual<size_t>(visuals.size(), 5);

		const Visualizer::Visual& first = visuals[0];

		Assert::AreEqual(first.m_Type, Visualizer::VisType::Instruction);

		Assert::AreEqual(first.m_Instruction.m_Type, instruction.m_Type);
		Assert::AreEqual<uint8_t>(first.m_Instruction.m_Size, instruction.m_Size);

		const Visualizer::Visual& second = visuals[1];

		Assert::AreEqual(second.m_Type, Visualizer::VisType::OperandRegister);

		Assert::AreEqual<uint8_t>(second.m_Register, VisReg_General_8h + REG_CH);

		const Visualizer::Visual& third = visuals[2];

		Assert::AreEqual(third.m_Type, Visualizer::VisType::OperandRegister);

		Assert::AreEqual<uint8_t>(third.m_Register, VisReg_General_16 + REG_AX);

		const Visualizer::Visual& fourth = visuals[3];

		Assert::AreEqual(fourth.m_Type, Visualizer::VisType::OperandRegister);

		Assert::AreEqual<uint8_t>(fourth.m_Register, VisReg_General_32 + REG_R8D);

		const Visualizer::Visual& fifth = visuals[4];

		Assert::AreEqual(fifth.m_Type, Visualizer::VisType::OperandRegister);

		Assert::AreEqual<uint8_t>(fifth.m_Register, VisReg_General_64 + REG_R10);
	}

	TEST_METHOD(Visualize_Operands_2)
	{
		std::vector<ILInstruction> instructions;

		ILInstruction instruction = {};

		instruction.m_Size = 78;
		instruction.m_Type = InsType_or;

		instruction.m_Operands[0].m_Scale = ILOperandScale_64;
		instruction.m_Operands[0].m_Type = ILOperandType_Register;

		instruction.m_Operands[0].m_Register.m_Type = Register::mm;
		instruction.m_Operands[0].m_Register.m_Base = REG_MM7;
		instruction.m_Operands[0].m_Register.m_BaseHigh = false;

		instruction.m_Operands[1].m_Scale = ILOperandScale_64;
		instruction.m_Operands[1].m_Type = ILOperandType_Register;

		instruction.m_Operands[1].m_Register.m_Type = Register::bnd;
		instruction.m_Operands[1].m_Register.m_Base = REG_BND0;
		instruction.m_Operands[1].m_Register.m_BaseHigh = false;

		instruction.m_Operands[2].m_Scale = ILOperandScale_80;
		instruction.m_Operands[2].m_Type = ILOperandType_Register;

		instruction.m_Operands[2].m_Register.m_Type = Register::st;
		instruction.m_Operands[2].m_Register.m_Base = REG_ST5;
		instruction.m_Operands[2].m_Register.m_BaseHigh = false;

		instruction.m_Operands[3].m_Scale = ILOperandScale_64;
		instruction.m_Operands[3].m_Type = ILOperandType_Register;

		instruction.m_Operands[3].m_Register.m_Type = Register::sreg;
		instruction.m_Operands[3].m_Register.m_Base = REG_FS;
		instruction.m_Operands[3].m_Register.m_BaseHigh = false;

		instructions.push_back(instruction);

		Visualizer visualizer;

		std::vector<Visualizer::Visual> visuals = visualizer.ToVisuals(instructions, reinterpret_cast<const void*>(1ull << 63));

		Assert::AreEqual<size_t>(visuals.size(), 5);

		const Visualizer::Visual& first = visuals[0];

		Assert::AreEqual(first.m_Type, Visualizer::VisType::Instruction);

		Assert::AreEqual(first.m_Instruction.m_Type, instruction.m_Type);
		Assert::AreEqual<uint8_t>(first.m_Instruction.m_Size, instruction.m_Size);

		const Visualizer::Visual& second = visuals[1];

		Assert::AreEqual(second.m_Type, Visualizer::VisType::OperandRegister);

		Assert::AreEqual<uint8_t>(second.m_Register, VisReg_MMX_64 + REG_MM7);

		const Visualizer::Visual& third = visuals[2];

		Assert::AreEqual(third.m_Type, Visualizer::VisType::OperandRegister);

		Assert::AreEqual<uint8_t>(third.m_Register, VisReg_MPX_64 + REG_BND0);

		const Visualizer::Visual& fourth = visuals[3];

		Assert::AreEqual(fourth.m_Type, Visualizer::VisType::OperandRegister);

		Assert::AreEqual<uint8_t>(fourth.m_Register, VisReg_FPU_80 + REG_ST5);

		const Visualizer::Visual& fifth = visuals[4];

		Assert::AreEqual(fifth.m_Type, Visualizer::VisType::OperandRegister);

		Assert::AreEqual<uint8_t>(fifth.m_Register, VisReg_Sreg_64 + REG_FS);
	}

	TEST_METHOD(Visualize_Operands_3)
	{
		std::vector<ILInstruction> instructions;

		ILInstruction instruction = {};

		instruction.m_Size = 23;
		instruction.m_Type = InsType_vmread;

		instruction.m_Operands[0].m_Scale = ILOperandScale_64;
		instruction.m_Operands[0].m_Type = ILOperandType_Register;

		instruction.m_Operands[0].m_Register.m_Type = Register::cr;
		instruction.m_Operands[0].m_Register.m_Base = REG_CR15;
		instruction.m_Operands[0].m_Register.m_BaseHigh = false;

		instruction.m_Operands[1].m_Scale = ILOperandScale_64;
		instruction.m_Operands[1].m_Type = ILOperandType_Register;

		instruction.m_Operands[1].m_Register.m_Type = Register::dr;
		instruction.m_Operands[1].m_Register.m_Base = REG_DR8;
		instruction.m_Operands[1].m_Register.m_BaseHigh = false;

		instruction.m_Operands[2].m_Scale = ILOperandScale_128;
		instruction.m_Operands[2].m_Type = ILOperandType_MemoryRelative;

		instruction.m_Operands[2].m_Relative.m_Value = 0x5156;
		instruction.m_Operands[2].m_Relative.m_Segment = IL_INVALID_REGISTER;

		instruction.m_Operands[3].m_Scale = ILOperandScale_80;
		instruction.m_Operands[3].m_Type = ILOperandType_MemoryAbsolute;

		instruction.m_Operands[3].m_MemoryValue.m_Value = 0x4545641656;
		instruction.m_Operands[3].m_MemoryValue.m_Segment = IL_INVALID_REGISTER;

		instructions.push_back(instruction);

		Visualizer visualizer;

		std::vector<Visualizer::Visual> visuals = visualizer.ToVisuals(instructions, reinterpret_cast<const void*>(1ull << 63));

		Assert::AreEqual<size_t>(visuals.size(), 5);

		const Visualizer::Visual& first = visuals[0];

		Assert::AreEqual(first.m_Type, Visualizer::VisType::Instruction);

		Assert::AreEqual(first.m_Instruction.m_Type, instruction.m_Type);
		Assert::AreEqual<uint8_t>(first.m_Instruction.m_Size, instruction.m_Size);

		const Visualizer::Visual& second = visuals[1];

		Assert::AreEqual(second.m_Type, Visualizer::VisType::OperandRegister);

		Assert::AreEqual<uint8_t>(second.m_Register, VisReg_CR_64 + REG_CR15);

		const Visualizer::Visual& third = visuals[2];

		Assert::AreEqual(third.m_Type, Visualizer::VisType::OperandRegister);

		Assert::AreEqual<uint8_t>(third.m_Register, VisReg_DR_64 + REG_DR8);

		const Visualizer::Visual& fourth = visuals[3];

		Assert::AreEqual(fourth.m_Type, Visualizer::VisType::OperandMemoryValue);

		Assert::AreEqual(fourth.m_Value.m_Size, ILOperandScale_128);
		Assert::AreEqual<uint8_t>(fourth.m_Value.m_Segment, VIS_INVALID_REGISTER);
		Assert::AreEqual<int64_t>(fourth.m_Value.m_Value, (1ull << 63) + 0x5156 + instruction.m_Size);

		const Visualizer::Visual& fifth = visuals[4];

		Assert::AreEqual(fifth.m_Type, Visualizer::VisType::OperandMemoryValue);

		Assert::AreEqual(fifth.m_Value.m_Size, ILOperandScale_80);
		Assert::AreEqual<uint8_t>(fifth.m_Value.m_Segment, VIS_INVALID_REGISTER);
		Assert::AreEqual<int64_t>(fifth.m_Value.m_Value, 0x4545641656);
	}
};