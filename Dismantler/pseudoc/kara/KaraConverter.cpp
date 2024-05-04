#include "KaraConverter.hpp"
#include <vector>
#include <format>
#include <map>

#define CHUNK_SIZE 0x200 

KaraConverter::KaraConverter(const PEBuffer& buffer)
{
	m_Buffer = &buffer;
}

KaraBlob KaraConverter::Convert(uintptr_t function) const
{
	constexpr uint64_t masks[] = { 0, (1ull << 8) - 1, (1ull << 16) - 1, (1ull << 32) - 1, ~0 };

	if (m_Buffer->GetFileHeader().Machine != IMAGE_FILE_MACHINE_AMD64)
	{
		return {};
	}

	const IMAGE_OPTIONAL_HEADER64& optional = m_Buffer->GetOptionalHeader64();

	std::vector<ILInstruction> instructions;

	std::vector<State> stack;

	KaraBlob blob;

	State state = {};

	state.m_Blob = &blob;

	state.m_Cursor = reinterpret_cast<const void*>(reinterpret_cast<uintptr_t>(m_Buffer->GetBuffer()) + function - optional.ImageBase);

	state.m_ImageBase = optional.ImageBase;
	state.m_ImageSize = optional.SizeOfImage;
	state.m_CursorBase = reinterpret_cast<uintptr_t>(m_Buffer->GetBuffer());

	state.m_General[REG_RSP].m_Value = state.m_ImageBase + (1ull << 40) - 8;
	state.m_StackBase = state.m_General[REG_RSP].m_Value;

	state.m_General[REG_RCX].m_Type = SpaceType::Argument0;
	state.m_General[REG_RDX].m_Type = SpaceType::Argument1;
	state.m_General[REG_R8].m_Type = SpaceType::Argument2;
	state.m_General[REG_R9].m_Type = SpaceType::Argument3;

	std::vector<const void*> invalid;
	std::map<const void*, Block> blocks;
	while (true)
	{
		bool breakout = false;

		uint32_t chunk = (1 << 12) - (reinterpret_cast<uintptr_t>(state.m_Cursor) & ((1 << 12) - 1));
		if (chunk > CHUNK_SIZE ||
			chunk == 0)
		{
			chunk = CHUNK_SIZE;
		}

		instructions.clear();

		m_Disassembler.Disassemble(state.m_Cursor, chunk, instructions);
		for (const ILInstruction& ins : instructions)
		{
			bool reset = false;
			bool jump = false;

			if (blocks.contains(state.m_Cursor))
			{
				if (stack.size() != 0)
				{
					state = stack[stack.size() - 1];
					stack.pop_back();

					reset = true;
					invalid.clear();
					break;
				}

				breakout = true;
				break;
			}

			const void* address = state.m_Cursor;
			Block& block = blocks[address];

			state.m_Cursor = reinterpret_cast<const uint8_t*>(state.m_Cursor) + ins.m_Size;

			switch (ins.m_Type)
			{
			case InsType_jmp:
			{
				const ILOperand& lhs = ins.m_Operands[0];

				if (state.m_StackBase == state.m_General[REG_RSP].m_Value ||
					lhs.m_Type != ILOperandType_ValueRelative)
				{
					KaraInstruction pseudo = {};

					pseudo.m_Type = KaraInstruction::Type::Invoke;

					if (ReadOperand(state, lhs, pseudo.m_Operands[0]))
					{
						block.m_Type = BlockType::Instruction;
						block.m_Instruction = pseudo;

						state.m_LastValid = address;
					}
					
					if (stack.size() != 0)
					{
						state = stack[stack.size() - 1];
						stack.pop_back();

						reset = true;
						break;
					}

					breakout = true;
					break;
				}

				KaraInstruction pseudo = {};

				pseudo.m_Type = KaraInstruction::Type::Goto;

				if (ReadOperand(state, lhs, pseudo.m_Operands[0]))
				{
					block.m_Type = BlockType::Instruction;
					block.m_Instruction = pseudo;

					state.m_LastValid = address;
				}

				state.m_Cursor = reinterpret_cast<const uint8_t*>(state.m_Cursor) + lhs.m_Relative.m_Value;
				jump = true;
			} break;
			case InsType_jne:
			{
				const ILOperand& lhs = ins.m_Operands[0];

				if (lhs.m_Type != ILOperandType_ValueRelative)
				{
					if (stack.size() != 0)
					{
						state = stack[stack.size() - 1];
						stack.pop_back();

						reset = true;
						break;
					}

					breakout = true;
					break;
				}

				KaraInstruction pseudo = {};

				pseudo.m_Type = KaraInstruction::Type::GotoNE;

				if (ReadOperand(state, lhs, pseudo.m_Operands[0]))
				{
					block.m_Type = BlockType::Instruction;
					block.m_Instruction = pseudo;
				}

				stack.push_back(state);

				state.m_Cursor = reinterpret_cast<const uint8_t*>(state.m_Cursor) + lhs.m_Relative.m_Value;
				jump = true;
			} break;
			case InsType_je:
			{
				const ILOperand& lhs = ins.m_Operands[0];

				if (lhs.m_Type != ILOperandType_ValueRelative)
				{
					if (stack.size() != 0)
					{
						state = stack[stack.size() - 1];
						stack.pop_back();

						reset = true;
						break;
					}

					breakout = true;
					break;
				}

				KaraInstruction pseudo = {};

				pseudo.m_Type = KaraInstruction::Type::GotoE;

				if (ReadOperand(state, lhs, pseudo.m_Operands[0]))
				{
					block.m_Type = BlockType::Instruction;
					block.m_Instruction = pseudo;
				}

				stack.push_back(state);

				state.m_Cursor = reinterpret_cast<const uint8_t*>(state.m_Cursor) + lhs.m_Relative.m_Value;
				jump = true;
			} break;
			case InsType_ja:
			{
				const ILOperand& lhs = ins.m_Operands[0];

				if (lhs.m_Type != ILOperandType_ValueRelative)
				{
					if (stack.size() != 0)
					{
						state = stack[stack.size() - 1];
						stack.pop_back();

						reset = true;
						break;
					}

					breakout = true;
					break;
				}

				KaraInstruction pseudo = {};

				pseudo.m_Type = KaraInstruction::Type::GotoA;

				if (ReadOperand(state, lhs, pseudo.m_Operands[0]))
				{
					block.m_Type = BlockType::Instruction;
					block.m_Instruction = pseudo;
				}

				stack.push_back(state);

				state.m_Cursor = reinterpret_cast<const uint8_t*>(state.m_Cursor) + lhs.m_Relative.m_Value;
				jump = true;
			} break;
			case InsType_jae:
			{
				const ILOperand& lhs = ins.m_Operands[0];

				if (lhs.m_Type != ILOperandType_ValueRelative)
				{
					if (stack.size() != 0)
					{
						state = stack[stack.size() - 1];
						stack.pop_back();

						reset = true;
						break;
					}

					breakout = true;
					break;
				}

				KaraInstruction pseudo = {};

				pseudo.m_Type = KaraInstruction::Type::GotoAE;

				if (ReadOperand(state, lhs, pseudo.m_Operands[0]))
				{
					block.m_Type = BlockType::Instruction;
					block.m_Instruction = pseudo;
				}

				stack.push_back(state);

				state.m_Cursor = reinterpret_cast<const uint8_t*>(state.m_Cursor) + lhs.m_Relative.m_Value;
				jump = true;
			} break;
			case InsType_jb:
			{
				const ILOperand& lhs = ins.m_Operands[0];

				if (lhs.m_Type != ILOperandType_ValueRelative)
				{
					if (stack.size() != 0)
					{
						state = stack[stack.size() - 1];
						stack.pop_back();

						reset = true;
						break;
					}

					breakout = true;
					break;
				}

				KaraInstruction pseudo = {};

				pseudo.m_Type = KaraInstruction::Type::GotoB;

				if (ReadOperand(state, lhs, pseudo.m_Operands[0]))
				{
					block.m_Type = BlockType::Instruction;
					block.m_Instruction = pseudo;
				}

				stack.push_back(state);

				state.m_Cursor = reinterpret_cast<const uint8_t*>(state.m_Cursor) + lhs.m_Relative.m_Value;
				jump = true;
			} break;
			case InsType_jbe:
			{
				const ILOperand& lhs = ins.m_Operands[0];

				if (lhs.m_Type != ILOperandType_ValueRelative)
				{
					if (stack.size() != 0)
					{
						state = stack[stack.size() - 1];
						stack.pop_back();

						reset = true;
						break;
					}

					breakout = true;
					break;
				}

				KaraInstruction pseudo = {};

				pseudo.m_Type = KaraInstruction::Type::GotoBE;

				if (ReadOperand(state, lhs, pseudo.m_Operands[0]))
				{
					block.m_Type = BlockType::Instruction;
					block.m_Instruction = pseudo;
				}

				stack.push_back(state);

				state.m_Cursor = reinterpret_cast<const uint8_t*>(state.m_Cursor) + lhs.m_Relative.m_Value;
				jump = true;
			} break;
			case InsType_jg:
			{
				const ILOperand& lhs = ins.m_Operands[0];

				if (lhs.m_Type != ILOperandType_ValueRelative)
				{
					if (stack.size() != 0)
					{
						state = stack[stack.size() - 1];
						stack.pop_back();

						reset = true;
						break;
					}

					breakout = true;
					break;
				}

				KaraInstruction pseudo = {};

				pseudo.m_Type = KaraInstruction::Type::GotoG;

				if (ReadOperand(state, lhs, pseudo.m_Operands[0]))
				{
					block.m_Type = BlockType::Instruction;
					block.m_Instruction = pseudo;
				}

				stack.push_back(state);

				state.m_Cursor = reinterpret_cast<const uint8_t*>(state.m_Cursor) + lhs.m_Relative.m_Value;
				jump = true;
			} break;
			case InsType_jge:
			{
				const ILOperand& lhs = ins.m_Operands[0];

				if (lhs.m_Type != ILOperandType_ValueRelative)
				{
					if (stack.size() != 0)
					{
						state = stack[stack.size() - 1];
						stack.pop_back();

						reset = true;
						break;
					}

					breakout = true;
					break;
				}

				KaraInstruction pseudo = {};

				pseudo.m_Type = KaraInstruction::Type::GotoGE;

				if (ReadOperand(state, lhs, pseudo.m_Operands[0]))
				{
					block.m_Type = BlockType::Instruction;
					block.m_Instruction = pseudo;
				}

				stack.push_back(state);

				state.m_Cursor = reinterpret_cast<const uint8_t*>(state.m_Cursor) + lhs.m_Relative.m_Value;
				jump = true;
			} break;
			case InsType_jl:
			{
				const ILOperand& lhs = ins.m_Operands[0];

				if (lhs.m_Type != ILOperandType_ValueRelative)
				{
					if (stack.size() != 0)
					{
						state = stack[stack.size() - 1];
						stack.pop_back();

						reset = true;
						break;
					}

					breakout = true;
					break;
				}

				KaraInstruction pseudo = {};

				pseudo.m_Type = KaraInstruction::Type::GotoL;

				if (ReadOperand(state, lhs, pseudo.m_Operands[0]))
				{
					block.m_Type = BlockType::Instruction;
					block.m_Instruction = pseudo;
				}

				stack.push_back(state);

				state.m_Cursor = reinterpret_cast<const uint8_t*>(state.m_Cursor) + lhs.m_Relative.m_Value;
				jump = true;
			} break;
			case InsType_jle:
			{
				const ILOperand& lhs = ins.m_Operands[0];

				if (lhs.m_Type != ILOperandType_ValueRelative)
				{
					if (stack.size() != 0)
					{
						state = stack[stack.size() - 1];
						stack.pop_back();

						reset = true;
						break;
					}

					breakout = true;
					break;
				}

				KaraInstruction pseudo = {};

				pseudo.m_Type = KaraInstruction::Type::GotoLE;

				if (ReadOperand(state, lhs, pseudo.m_Operands[0]))
				{
					block.m_Type = BlockType::Instruction;
					block.m_Instruction = pseudo;
				}

				stack.push_back(state);

				state.m_Cursor = reinterpret_cast<const uint8_t*>(state.m_Cursor) + lhs.m_Relative.m_Value;
				jump = true;
			} break;
			case InsType_jno:
			{
				const ILOperand& lhs = ins.m_Operands[0];

				if (lhs.m_Type != ILOperandType_ValueRelative)
				{
					if (stack.size() != 0)
					{
						state = stack[stack.size() - 1];
						stack.pop_back();

						reset = true;
						break;
					}

					breakout = true;
					break;
				}

				KaraInstruction pseudo = {};

				pseudo.m_Type = KaraInstruction::Type::GotoNO;

				if (ReadOperand(state, lhs, pseudo.m_Operands[0]))
				{
					block.m_Type = BlockType::Instruction;
					block.m_Instruction = pseudo;
				}

				stack.push_back(state);

				state.m_Cursor = reinterpret_cast<const uint8_t*>(state.m_Cursor) + lhs.m_Relative.m_Value;
				jump = true;
			} break;
			case InsType_jns:
			{
				const ILOperand& lhs = ins.m_Operands[0];

				if (lhs.m_Type != ILOperandType_ValueRelative)
				{
					if (stack.size() != 0)
					{
						state = stack[stack.size() - 1];
						stack.pop_back();

						reset = true;
						break;
					}

					breakout = true;
					break;
				}

				KaraInstruction pseudo = {};

				pseudo.m_Type = KaraInstruction::Type::GotoNS;

				if (ReadOperand(state, lhs, pseudo.m_Operands[0]))
				{
					block.m_Type = BlockType::Instruction;
					block.m_Instruction = pseudo;
				}

				stack.push_back(state);

				state.m_Cursor = reinterpret_cast<const uint8_t*>(state.m_Cursor) + lhs.m_Relative.m_Value;
				jump = true;
			} break;
			case InsType_jo:
			{
				const ILOperand& lhs = ins.m_Operands[0];

				if (lhs.m_Type != ILOperandType_ValueRelative)
				{
					if (stack.size() != 0)
					{
						state = stack[stack.size() - 1];
						stack.pop_back();

						reset = true;
						break;
					}

					breakout = true;
					break;
				}

				KaraInstruction pseudo = {};

				pseudo.m_Type = KaraInstruction::Type::GotoO;

				if (ReadOperand(state, lhs, pseudo.m_Operands[0]))
				{
					block.m_Type = BlockType::Instruction;
					block.m_Instruction = pseudo;
				}

				stack.push_back(state);

				state.m_Cursor = reinterpret_cast<const uint8_t*>(state.m_Cursor) + lhs.m_Relative.m_Value;
				jump = true;
			} break;
			case InsType_jpe:
			{
				const ILOperand& lhs = ins.m_Operands[0];

				if (lhs.m_Type != ILOperandType_ValueRelative)
				{
					if (stack.size() != 0)
					{
						state = stack[stack.size() - 1];
						stack.pop_back();

						reset = true;
						break;
					}

					breakout = true;
					break;
				}

				KaraInstruction pseudo = {};

				pseudo.m_Type = KaraInstruction::Type::GotoPE;

				if (ReadOperand(state, lhs, pseudo.m_Operands[0]))
				{
					block.m_Type = BlockType::Instruction;
					block.m_Instruction = pseudo;
				}

				stack.push_back(state);

				state.m_Cursor = reinterpret_cast<const uint8_t*>(state.m_Cursor) + lhs.m_Relative.m_Value;
				jump = true;
			} break;
			case InsType_jpo:
			{
				const ILOperand& lhs = ins.m_Operands[0];

				if (lhs.m_Type != ILOperandType_ValueRelative)
				{
					if (stack.size() != 0)
					{
						state = stack[stack.size() - 1];
						stack.pop_back();

						reset = true;
						break;
					}

					breakout = true;
					break;
				}

				KaraInstruction pseudo = {};

				pseudo.m_Type = KaraInstruction::Type::GotoPO;

				if (ReadOperand(state, lhs, pseudo.m_Operands[0]))
				{
					block.m_Type = BlockType::Instruction;
					block.m_Instruction = pseudo;
				}

				stack.push_back(state);

				state.m_Cursor = reinterpret_cast<const uint8_t*>(state.m_Cursor) + lhs.m_Relative.m_Value;
				jump = true;
			} break;
			case InsType_jrcxz:
			{
				const ILOperand& lhs = ins.m_Operands[0];

				if (lhs.m_Type != ILOperandType_ValueRelative)
				{
					if (stack.size() != 0)
					{
						state = stack[stack.size() - 1];
						stack.pop_back();

						reset = true;
						break;
					}

					breakout = true;
					break;
				}

				KaraInstruction pseudo = {};

				pseudo.m_Type = KaraInstruction::Type::GotoCXZ;

				if (ReadOperand(state, lhs, pseudo.m_Operands[0]))
				{
					block.m_Type = BlockType::Instruction;
					block.m_Instruction = pseudo;
				}

				stack.push_back(state);

				state.m_Cursor = reinterpret_cast<const uint8_t*>(state.m_Cursor) + lhs.m_Relative.m_Value;
				jump = true;
			} break;
			case InsType_js:
			{
				const ILOperand& lhs = ins.m_Operands[0];

				if (lhs.m_Type != ILOperandType_ValueRelative)
				{
					if (stack.size() != 0)
					{
						state = stack[stack.size() - 1];
						stack.pop_back();

						reset = true;
						break;
					}

					breakout = true;
					break;
				}

				KaraInstruction pseudo = {};

				pseudo.m_Type = KaraInstruction::Type::GotoS;

				if (ReadOperand(state, lhs, pseudo.m_Operands[0]))
				{
					block.m_Type = BlockType::Instruction;
					block.m_Instruction = pseudo;
				}

				stack.push_back(state);

				state.m_Cursor = reinterpret_cast<const uint8_t*>(state.m_Cursor) + lhs.m_Relative.m_Value;
				jump = true;
			} break;
			case InsType_push:
			{
				const ILOperand& lhs = ins.m_Operands[0];

				uint64_t value;
				if (ExecReadOperand(lhs, state, value))
				{
					state.m_General[REG_RSP].m_Value -= 8;
					state.m_Memory[state.m_General[REG_RSP].m_Value].m_Value = value & masks[lhs.m_Scale];
				}
			} break;
			case InsType_pop:
			{
				const ILOperand& lhs = ins.m_Operands[0];

				uint64_t value;
				if (ExecReadOperand(lhs, state, value))
				{
					state.m_Memory[state.m_General[REG_RSP].m_Value].m_Value = value & masks[lhs.m_Scale];
					state.m_General[REG_RSP].m_Value += 8;
				}
			} break;
			case InsType_cmp:
			{
				const ILOperand& lhs = ins.m_Operands[0];
				const ILOperand& rhs = ins.m_Operands[1];

				KaraInstruction pseudo = {};

				pseudo.m_Type = KaraInstruction::Type::Compare;

				if (!ReadOperand(state, rhs, pseudo.m_Operands[1]))
				{
					ClearOperand(state, lhs);
					ClearOperand(state, rhs);
					break;
				}

				if (!ReadOperand(state, lhs, pseudo.m_Operands[0]))
				{
					ClearOperand(state, lhs);
					ClearOperand(state, rhs);
					break;
				}

				block.m_Type = BlockType::Instruction;
				block.m_Instruction = pseudo;

				state.m_LastValid = address;
			} break;
			case InsType_cbw:
			{
				ILOperand lhs;
				ILOperand rhs;

				lhs.m_Type = ILOperandType_Register;
				rhs.m_Type = ILOperandType_Register;

				lhs.m_Scale = ILOperandScale_16;
				rhs.m_Scale = ILOperandScale_8;

				lhs.m_Register.m_Type = Register::general;
				rhs.m_Register.m_Type = Register::general;

				lhs.m_Register.m_Base = REG_RAX;
				rhs.m_Register.m_Base = REG_RAX;

				lhs.m_Register.m_BaseHigh = false;
				rhs.m_Register.m_BaseHigh = false;

				uint64_t value;
				if (ExecReadOperand(rhs, state, value))
				{
					ExecWriteOperand(lhs, state, value);
				}

				KaraInstruction pseudo = {};

				pseudo.m_Type = KaraInstruction::Type::Assign;

				if (!ReadOperand(state, rhs, pseudo.m_Operands[1]))
				{
					ClearOperand(state, lhs);
					ClearOperand(state, rhs);
					break;
				}

				if (!WriteOperand(state, lhs, pseudo.m_Operands[0]))
				{
					ClearOperand(state, lhs);
					ClearOperand(state, rhs);
					break;
				}

				block.m_Type = BlockType::Instruction;
				block.m_Instruction = pseudo;

				state.m_LastValid = address;
			} break;
			case InsType_cwde:
			{
				ILOperand lhs;
				ILOperand rhs;

				lhs.m_Type = ILOperandType_Register;
				rhs.m_Type = ILOperandType_Register;

				lhs.m_Scale = ILOperandScale_32;
				rhs.m_Scale = ILOperandScale_16;

				lhs.m_Register.m_Type = Register::general;
				rhs.m_Register.m_Type = Register::general;

				lhs.m_Register.m_Base = REG_RAX;
				rhs.m_Register.m_Base = REG_RAX;

				lhs.m_Register.m_BaseHigh = false;
				rhs.m_Register.m_BaseHigh = false;

				uint64_t value;
				if (ExecReadOperand(rhs, state, value))
				{
					ExecWriteOperand(lhs, state, value);
				}

				KaraInstruction pseudo = {};

				pseudo.m_Type = KaraInstruction::Type::Assign;

				if (!ReadOperand(state, rhs, pseudo.m_Operands[1]))
				{
					ClearOperand(state, lhs);
					ClearOperand(state, rhs);
					break;
				}

				if (!WriteOperand(state, lhs, pseudo.m_Operands[0]))
				{
					ClearOperand(state, lhs);
					ClearOperand(state, rhs);
					break;
				}

				block.m_Type = BlockType::Instruction;
				block.m_Instruction = pseudo;

				state.m_LastValid = address;
			} break;
			case InsType_cdqe:
			{
				ILOperand lhs;
				ILOperand rhs;

				lhs.m_Type = ILOperandType_Register;
				rhs.m_Type = ILOperandType_Register;

				lhs.m_Scale = ILOperandScale_64;
				rhs.m_Scale = ILOperandScale_32;

				lhs.m_Register.m_Type = Register::general;
				rhs.m_Register.m_Type = Register::general;

				lhs.m_Register.m_Base = REG_RAX;
				rhs.m_Register.m_Base = REG_RAX;

				lhs.m_Register.m_BaseHigh = false;
				rhs.m_Register.m_BaseHigh = false;

				uint64_t value;
				if (ExecReadOperand(rhs, state, value))
				{
					ExecWriteOperand(lhs, state, value);
				}

				KaraInstruction pseudo = {};

				pseudo.m_Type = KaraInstruction::Type::Assign;

				if (!ReadOperand(state, rhs, pseudo.m_Operands[1]))
				{
					ClearOperand(state, lhs);
					ClearOperand(state, rhs);
					break;
				}

				if (!WriteOperand(state, lhs, pseudo.m_Operands[0]))
				{
					ClearOperand(state, lhs);
					ClearOperand(state, rhs);
					break;
				}

				block.m_Type = BlockType::Instruction;
				block.m_Instruction = pseudo;

				state.m_LastValid = address;
			} break;
			case InsType_test:
			{
				const ILOperand& lhs = ins.m_Operands[0];
				const ILOperand& rhs = ins.m_Operands[1];

				KaraInstruction pseudo = {};

				pseudo.m_Type = KaraInstruction::Type::AndCompare;

				if (!ReadOperand(state, rhs, pseudo.m_Operands[1]))
				{
					ClearOperand(state, lhs);
					ClearOperand(state, rhs);
					break;
				}

				if (!ReadOperand(state, lhs, pseudo.m_Operands[0]))
				{
					ClearOperand(state, lhs);
					ClearOperand(state, rhs);
					break;
				}

				block.m_Type = BlockType::Instruction;
				block.m_Instruction = pseudo;

				state.m_LastValid = address;
			} break;
			case InsType_dec:
			{
				const ILOperand& lhs = ins.m_Operands[0];

				uint64_t value;
				if (ExecReadOperand(lhs, state, value))
				{
					ExecWriteOperand(lhs, state, value - 1);
				}

				KaraInstruction pseudo = {};

				pseudo.m_Type = KaraInstruction::Type::Subtract;

				if (!ReadOperand(state, lhs, pseudo.m_Operands[0]))
				{
					ClearOperand(state, lhs);
					break;
				}

				pseudo.m_Operands[1].m_Literal = 1;
				pseudo.m_Operands[1].m_Scale = lhs.m_Scale;
				pseudo.m_Operands[1].m_Type = KaraOperand::Type::Literal;

				block.m_Type = BlockType::Instruction;
				block.m_Instruction = pseudo;

				state.m_LastValid = address;
			} break;
			case InsType_inc:
			{
				const ILOperand& lhs = ins.m_Operands[0];

				uint64_t value;
				if (ExecReadOperand(lhs, state, value))
				{
					ExecWriteOperand(lhs, state, value + 1);
				}

				KaraInstruction pseudo = {};

				pseudo.m_Type = KaraInstruction::Type::Addition;

				if (!ReadOperand(state, lhs, pseudo.m_Operands[0]))
				{
					ClearOperand(state, lhs);
					break;
				}

				pseudo.m_Operands[1].m_Literal = 1;
				pseudo.m_Operands[1].m_Scale = lhs.m_Scale;
				pseudo.m_Operands[1].m_Type = KaraOperand::Type::Literal;

				block.m_Type = BlockType::Instruction;
				block.m_Instruction = pseudo;

				state.m_LastValid = address;
			} break;
			case InsType_sub:
			{
				const ILOperand& lhs = ins.m_Operands[0];
				const ILOperand& rhs = ins.m_Operands[1];

				uint64_t lhsValue;
				uint64_t rhsValue;
				if (ExecReadOperand(lhs, state, lhsValue) &&
					ExecReadOperand(rhs, state, rhsValue))
				{
					ExecWriteOperand(lhs, state, lhsValue - rhsValue);
				}

				KaraInstruction pseudo = {};

				pseudo.m_Type = KaraInstruction::Type::Subtract;

				if (!ReadOperand(state, rhs, pseudo.m_Operands[1]))
				{
					ClearOperand(state, lhs);
					ClearOperand(state, rhs);
					break;
				}

				if (!ReadOperand(state, lhs, pseudo.m_Operands[0]))
				{
					ClearOperand(state, lhs);
					ClearOperand(state, rhs);
					break;
				}

				block.m_Type = BlockType::Instruction;
				block.m_Instruction = pseudo;

				state.m_LastValid = address;
			} break;
			case InsType_add:
			{
				const ILOperand& lhs = ins.m_Operands[0];
				const ILOperand& rhs = ins.m_Operands[1];

				uint64_t lhsValue;
				uint64_t rhsValue;
				if (ExecReadOperand(lhs, state, lhsValue) &&
					ExecReadOperand(rhs, state, rhsValue))
				{
					ExecWriteOperand(lhs, state, lhsValue + rhsValue);
				}

				KaraInstruction pseudo = {};

				pseudo.m_Type = KaraInstruction::Type::Addition;

				if (!ReadOperand(state, rhs, pseudo.m_Operands[1]))
				{
					ClearOperand(state, lhs);
					ClearOperand(state, rhs);
					break;
				}

				if (!ReadOperand(state, lhs, pseudo.m_Operands[0]))
				{
					ClearOperand(state, lhs);
					ClearOperand(state, rhs);
					break;
				}

				block.m_Type = BlockType::Instruction;
				block.m_Instruction = pseudo;

				state.m_LastValid = address;
			} break;
			case InsType_and:
			{
				const ILOperand& lhs = ins.m_Operands[0];
				const ILOperand& rhs = ins.m_Operands[1];

				uint64_t lhsValue;
				uint64_t rhsValue;
				if (ExecReadOperand(lhs, state, lhsValue) &&
					ExecReadOperand(rhs, state, rhsValue))
				{
					ExecWriteOperand(lhs, state, lhsValue & rhsValue);
				}

				KaraInstruction pseudo = {};

				pseudo.m_Type = KaraInstruction::Type::And;

				if (!ReadOperand(state, rhs, pseudo.m_Operands[1]))
				{
					ClearOperand(state, lhs);
					ClearOperand(state, rhs);
					break;
				}

				if (!ReadOperand(state, lhs, pseudo.m_Operands[0]))
				{
					ClearOperand(state, lhs);
					ClearOperand(state, rhs);
					break;
				}

				block.m_Type = BlockType::Instruction;
				block.m_Instruction = pseudo;

				state.m_LastValid = address;
			} break;
			case InsType_or:
			{
				const ILOperand& lhs = ins.m_Operands[0];
				const ILOperand& rhs = ins.m_Operands[1];

				uint64_t lhsValue;
				uint64_t rhsValue;
				if (ExecReadOperand(lhs, state, lhsValue) &&
					ExecReadOperand(rhs, state, rhsValue))
				{
					ExecWriteOperand(lhs, state, lhsValue | rhsValue);
				}

				KaraInstruction pseudo = {};

				pseudo.m_Type = KaraInstruction::Type::Or;

				if (!ReadOperand(state, rhs, pseudo.m_Operands[1]))
				{
					ClearOperand(state, lhs);
					ClearOperand(state, rhs);
					break;
				}

				if (!ReadOperand(state, lhs, pseudo.m_Operands[0]))
				{
					ClearOperand(state, lhs);
					ClearOperand(state, rhs);
					break;
				}

				block.m_Type = BlockType::Instruction;
				block.m_Instruction = pseudo;

				state.m_LastValid = address;
			} break;
			case InsType_xor:
			{
				const ILOperand& lhs = ins.m_Operands[0];
				const ILOperand& rhs = ins.m_Operands[1];

				uint64_t lhsValue;
				uint64_t rhsValue;
				if (ExecReadOperand(lhs, state, lhsValue) &&
					ExecReadOperand(rhs, state, rhsValue))
				{
					ExecWriteOperand(lhs, state, lhsValue ^ rhsValue);
				}

				if (lhs.m_Type == ILOperandType_Register &&
					rhs.m_Type == ILOperandType_Register &&
					lhs.m_Register.m_Base == rhs.m_Register.m_Base &&
					lhs.m_Register.m_BaseHigh == rhs.m_Register.m_BaseHigh)
				{
					KaraInstruction pseudo = {};

					pseudo.m_Type = KaraInstruction::Type::Assign;

					ILOperand newRhs;

					newRhs.m_Type = ILOperandType_Value;
					newRhs.m_Scale = lhs.m_Scale;
					newRhs.m_Value = 0;

					if (!ReadOperand(state, newRhs, pseudo.m_Operands[1]))
					{
						ClearOperand(state, lhs);
						ClearOperand(state, rhs);
						break;
					}

					if (!WriteOperand(state, lhs, pseudo.m_Operands[0]))
					{
						ClearOperand(state, lhs);
						ClearOperand(state, rhs);
						break;
					}

					block.m_Type = BlockType::Instruction;
					block.m_Instruction = pseudo;

					state.m_LastValid = address;
					break;
				}

				KaraInstruction pseudo = {};

				pseudo.m_Type = KaraInstruction::Type::Xor;

				if (!ReadOperand(state, rhs, pseudo.m_Operands[1]))
				{
					ClearOperand(state, lhs);
					ClearOperand(state, rhs);
					break;
				}

				if (!ReadOperand(state, lhs, pseudo.m_Operands[0]))
				{
					ClearOperand(state, lhs);
					ClearOperand(state, rhs);
					break;
				}

				block.m_Type = BlockType::Instruction;
				block.m_Instruction = pseudo;

				state.m_LastValid = address;
			} break;
			case InsType_not:
			{
				const ILOperand& lhs = ins.m_Operands[0];

				uint64_t value;
				if (ExecReadOperand(lhs, state, value))
				{
					ExecWriteOperand(lhs, state, ~value);
				}

				KaraInstruction pseudo = {};

				pseudo.m_Type = KaraInstruction::Type::Not;

				if (!ReadOperand(state, lhs, pseudo.m_Operands[0]))
				{
					ClearOperand(state, lhs);
					break;
				}

				block.m_Type = BlockType::Instruction;
				block.m_Instruction = pseudo;

				state.m_LastValid = address;
			} break;
			case InsType_lea:
			{
				const ILOperand& lhs = ins.m_Operands[0];
				const ILOperand& rhs = ins.m_Operands[1];

				uint64_t value;
				if (ExecLoadOperand(rhs, state, value))
				{
					ExecWriteOperand(lhs, state, value);
				}

				if (lhs.m_Type == ILOperandType_Register &&
					rhs.m_Type == ILOperandType_Memory)
				{
					if (lhs.m_Register.m_Base == REG_RBP &&
						(rhs.m_Memory.m_Base == REG_RSP ||
						rhs.m_Memory.m_Index == REG_RSP))
					{
						break;
					}
				}

				KaraInstruction pseudo = {};

				pseudo.m_Type = KaraInstruction::Type::Assign;

				if (!LoadOperand(state, rhs, pseudo.m_Operands[1]))
				{
					ClearOperand(state, lhs);
					ClearOperand(state, rhs);
					break;
				}

				if (!WriteOperand(state, lhs, pseudo.m_Operands[0]))
				{
					ClearOperand(state, lhs);
					ClearOperand(state, rhs);
					break;
				}

				block.m_Type = BlockType::Instruction;
				block.m_Instruction = pseudo;

				state.m_LastValid = address;
			} break;
			case InsType_mov:
			case InsType_movzx:
			{
				const ILOperand& lhs = ins.m_Operands[0];
				const ILOperand& rhs = ins.m_Operands[1];

				uint64_t value;
				if (ExecReadOperand(rhs, state, value))
				{
					ExecWriteOperand(lhs, state, value);
				}

				KaraInstruction pseudo = {};

				pseudo.m_Type = KaraInstruction::Type::Assign;

				if (!ReadOperand(state, rhs, pseudo.m_Operands[1]))
				{
					ClearOperand(state, lhs);
					ClearOperand(state, rhs);
					break;
				}

				if (!WriteOperand(state, lhs, pseudo.m_Operands[0]))
				{
					ClearOperand(state, lhs);
					ClearOperand(state, rhs);
					break;
				}

				block.m_Type = BlockType::Instruction;
				block.m_Instruction = pseudo;

				state.m_LastValid = address;
			} break;
			case InsType_call:
			{
				const ILOperand& lhs = ins.m_Operands[0];

				state.m_General[REG_RAX].m_Type = SpaceType::Result;
				state.m_General[REG_RAX].m_VariableHigh = 0;
				state.m_General[REG_RAX].m_VariableIndex = 0;

				state.m_General[REG_RCX].m_Type = SpaceType::Normal;
				state.m_General[REG_RCX].m_VariableHigh = 0;
				state.m_General[REG_RCX].m_VariableIndex = 0;

				state.m_General[REG_RDX].m_Type = SpaceType::Normal;
				state.m_General[REG_RDX].m_VariableHigh = 0;
				state.m_General[REG_RDX].m_VariableIndex = 0;

				state.m_General[REG_R8].m_Type = SpaceType::Normal;
				state.m_General[REG_R8].m_VariableHigh = 0;
				state.m_General[REG_R8].m_VariableIndex = 0;

				state.m_General[REG_R9].m_Type = SpaceType::Normal;
				state.m_General[REG_R9].m_VariableHigh = 0;
				state.m_General[REG_R9].m_VariableIndex = 0;

				KaraInstruction pseudo = {};

				pseudo.m_Type = KaraInstruction::Type::Invoke;

				if (!ReadOperand(state, lhs, pseudo.m_Operands[0]))
				{
					ClearOperand(state, lhs);
					break;
				}

				block.m_Type = BlockType::Instruction;
				block.m_Instruction = pseudo;

				state.m_LastValid = address;
			} break;
			case InsType_ret:
			{
				KaraInstruction pseudo = {};

				pseudo.m_Type = KaraInstruction::Type::Return;

				block.m_Type = BlockType::Instruction;
				block.m_Instruction = pseudo;

				state.m_LastValid = address;

				if (stack.size() != 0)
				{
					state = stack[stack.size() - 1];
					stack.pop_back();

					reset = true;
					break;
				}

				breakout = true;
				break;
			} break;
			case InsType_int3:
			case InsType_ud0:
			case InsType_ud1:
			case InsType_ud2:
			case InsType_invalid:
			{
				if (stack.size() != 0)
				{
					state = stack[stack.size() - 1];
					stack.pop_back();

					reset = true;
					break;
				}

				breakout = true;
				break;
			} break;
			}

			if (block.m_Type == BlockType::Instruction)
			{
				for (const void* invalid : invalid)
				{
					blocks[invalid].m_Type = BlockType::Pointer;
					blocks[invalid].m_Pointer = state.m_LastValid;
				}

				invalid.clear();
			}
			else if (state.m_LastValid != nullptr)
			{
				block.m_Type = BlockType::Pointer;
				block.m_Pointer = state.m_LastValid;
			}

			if (jump)
			{
				state.m_LastValid = nullptr;
				invalid.clear();
				break;
			}

			if (reset || breakout)
			{
				invalid.clear();
				break;
			}

			if (block.m_Type == BlockType::None)
			{
				invalid.push_back(address);
			}
		}

		if (breakout)
		{
			break;
		}
	}

	uint32_t index = 0;
	for (auto& pair : blocks)
	{
		pair.second.m_Index = index;
		if (pair.second.m_Type == BlockType::Instruction)
		{
			blob.m_Instructions.push_back(pair.second.m_Instruction);
			index++;
		}
	}

	for (KaraInstruction& instruction : blob.m_Instructions)
	{
		if (instruction.m_Type < KaraInstruction::Type::Goto_Start ||
			instruction.m_Type > KaraInstruction::Type::Goto_End)
		{
			continue;
		}

		const void* address = reinterpret_cast<const void*>(instruction.m_Operands[0].m_Literal - state.m_ImageBase + state.m_CursorBase);

		const Block& block = blocks[address];
		if (block.m_Type == BlockType::None)
		{
			continue;
		}

		if (block.m_Type == BlockType::Pointer)
		{
			instruction.m_Operands[0].m_Literal = blocks[block.m_Pointer].m_Index;
			continue;
		}

		instruction.m_Operands[0].m_Literal = block.m_Index;
	}

	return blob;
}

bool KaraConverter::ClearOperand(State& state, const ILOperand& operand) const
{
	switch (operand.m_Type)
	{
	case ILOperandType_Register:
	{
		if (operand.m_Register.m_BaseHigh)
		{
			RegSpace& reg = state.m_General[operand.m_Memory.m_Base];

			reg.m_Type = SpaceType::Normal;
			reg.m_VariableHigh = 0;
		}
		else
		{
			RegSpace& reg = state.m_General[operand.m_Memory.m_Base];

			reg.m_Type = SpaceType::Normal;
			reg.m_VariableIndex = 0;
		}
	} break;
	}

	return false;
}

bool KaraConverter::LoadOperand(State& state, const ILOperand& asmOperand, KaraOperand& pcOperand) const
{
	constexpr uint8_t multiplier[] = { 1, 2, 4, 8 };

	pcOperand.m_Scale = asmOperand.m_Scale;
	switch (asmOperand.m_Type)
	{
	case ILOperandType_Memory:
	{
		uint64_t address = 0;

		uint32_t base = ~0;
		if (asmOperand.m_Memory.m_Base != IL_INVALID_REGISTER)
		{
			RegSpace& reg = state.m_General[asmOperand.m_Memory.m_Base];
			if (reg.m_VariableIndex == 0)
			{
				if (reg.m_Type >= SpaceType::Argument_Start &&
					reg.m_Type <= SpaceType::Argument_End)
				{
					reg.m_VariableIndex = state.m_Blob->m_Variables.size() + 1;

					KaraVariable variable = {};

					variable.m_Size = asmOperand.m_Scale;
					variable.m_Type = KaraVariable::Type::Argument;
					variable.m_Index = static_cast<uint32_t>(reg.m_Type) - static_cast<uint32_t>(SpaceType::Argument_Start);

					state.m_Blob->m_Variables.push_back(variable);
				}
				else if (reg.m_Type == SpaceType::Result)
				{
					reg.m_VariableIndex = state.m_Blob->m_Variables.size() + 1;

					KaraVariable variable = {};

					variable.m_Size = asmOperand.m_Scale;
					variable.m_Type = KaraVariable::Type::Result;

					state.m_Blob->m_Variables.push_back(variable);
				}
			}

			base = reg.m_VariableIndex;
			address = reg.m_Value;
		}

		uint32_t index = ~0;
		if (asmOperand.m_Memory.m_Index != IL_INVALID_REGISTER)
		{
			RegSpace& reg = state.m_General[asmOperand.m_Memory.m_Index];
			if (reg.m_VariableIndex == 0)
			{
				if (reg.m_Type >= SpaceType::Argument_Start &&
					reg.m_Type <= SpaceType::Argument_End)
				{
					reg.m_VariableIndex = state.m_Blob->m_Variables.size() + 1;

					KaraVariable variable = {};

					variable.m_Size = asmOperand.m_Scale;
					variable.m_Type = KaraVariable::Type::Argument;
					variable.m_Index = static_cast<uint32_t>(reg.m_Type) - static_cast<uint32_t>(SpaceType::Argument_Start);

					state.m_Blob->m_Variables.push_back(variable);
				}
				else if (reg.m_Type == SpaceType::Result)
				{
					reg.m_VariableIndex = state.m_Blob->m_Variables.size() + 1;

					KaraVariable variable = {};

					variable.m_Size = asmOperand.m_Scale;
					variable.m_Type = KaraVariable::Type::Result;

					state.m_Blob->m_Variables.push_back(variable);
				}
			}

			index = reg.m_VariableIndex;
			address += reg.m_Value * multiplier[asmOperand.m_Memory.m_Scale];
		}

		address += asmOperand.m_Memory.m_Offset;

		if (base != 0 && index != 0)
		{
			pcOperand.m_Type = KaraOperand::Type::Expression;

			pcOperand.m_Expression.m_BaseVariable = (base == ~0) ? 0 : base;
			pcOperand.m_Expression.m_IndexVariable = (index == ~0) ? 0 : index;
			pcOperand.m_Expression.m_Offset = asmOperand.m_Memory.m_Offset;
			pcOperand.m_Expression.m_Multiplier = multiplier[asmOperand.m_Memory.m_Scale];

			return true;
		}

		pcOperand.m_Type = KaraOperand::Type::Reference;

		MemSpace& memory = state.m_Memory[address];

		memory.m_VariableIndex = state.m_Blob->m_Variables.size() + 1;

		KaraVariable variable = {};

		variable.m_Size = asmOperand.m_Scale;
		variable.m_Type = KaraVariable::Type::Local;

		state.m_Blob->m_Variables.push_back(variable);

		pcOperand.m_Variable.m_Index = memory.m_VariableIndex;
		return true;
	} break;
	case ILOperandType_MemoryRelative:
	{
		uint64_t address = reinterpret_cast<uintptr_t>(state.m_Cursor) + asmOperand.m_Relative.m_Value - state.m_CursorBase + state.m_ImageBase;

		pcOperand.m_Type = KaraOperand::Type::Reference;

		MemSpace& space = state.m_Memory[address];
		if (space.m_VariableIndex == 0)
		{
			space.m_VariableIndex = state.m_Blob->m_Variables.size() + 1;

			KaraVariable variable = {};

			variable.m_Size = asmOperand.m_Scale;
			variable.m_Type = KaraVariable::Type::Static;
			variable.m_Address = address;

			state.m_Blob->m_Variables.push_back(variable);
		}

		pcOperand.m_Variable.m_Index = space.m_VariableIndex;
		return true;
	} break;
	case ILOperandType_MemoryAbsolute:
	{
		uint64_t address = asmOperand.m_MemoryValue.m_Value;

		pcOperand.m_Type = KaraOperand::Type::Reference;

		MemSpace& space = state.m_Memory[address];
		if (space.m_VariableIndex == 0)
		{
			space.m_VariableIndex = state.m_Blob->m_Variables.size() + 1;

			KaraVariable variable = {};

			variable.m_Size = asmOperand.m_Scale;
			variable.m_Type = KaraVariable::Type::Static;
			variable.m_Address = address;

			state.m_Blob->m_Variables.push_back(variable);
		}

		pcOperand.m_Variable.m_Index = space.m_VariableIndex;
		return true;
	} break;
	}

	return false;
}

bool KaraConverter::WriteOperand(State& state, const ILOperand& asmOperand, KaraOperand& pcOperand) const
{
	constexpr uint8_t multiplier[] = { 1, 2, 4, 8 };

	pcOperand.m_Scale = asmOperand.m_Scale;
	switch (asmOperand.m_Type)
	{
	case ILOperandType_Memory:
	{
		uint64_t address = 0;

		uint32_t base = ~0;
		if (asmOperand.m_Memory.m_Base != IL_INVALID_REGISTER)
		{
			RegSpace& reg = state.m_General[asmOperand.m_Memory.m_Base];
			if (reg.m_VariableIndex == 0)
			{
				if (reg.m_Type >= SpaceType::Argument_Start &&
					reg.m_Type <= SpaceType::Argument_End)
				{
					reg.m_VariableIndex = state.m_Blob->m_Variables.size() + 1;

					KaraVariable variable = {};

					variable.m_Size = asmOperand.m_Scale;
					variable.m_Type = KaraVariable::Type::Argument;
					variable.m_Index = static_cast<uint32_t>(reg.m_Type) - static_cast<uint32_t>(SpaceType::Argument_Start);

					state.m_Blob->m_Variables.push_back(variable);
				}
				else if (reg.m_Type == SpaceType::Result)
				{
					reg.m_VariableIndex = state.m_Blob->m_Variables.size() + 1;

					KaraVariable variable = {};

					variable.m_Size = asmOperand.m_Scale;
					variable.m_Type = KaraVariable::Type::Result;

					state.m_Blob->m_Variables.push_back(variable);
				}
			}

			base = reg.m_VariableIndex;
			address = reg.m_Value;
		}

		uint32_t index = ~0;
		if (asmOperand.m_Memory.m_Index != IL_INVALID_REGISTER)
		{
			RegSpace& reg = state.m_General[asmOperand.m_Memory.m_Index];
			if (reg.m_VariableIndex == 0)
			{
				if (reg.m_Type >= SpaceType::Argument_Start &&
					reg.m_Type <= SpaceType::Argument_End)
				{
					reg.m_VariableIndex = state.m_Blob->m_Variables.size() + 1;

					KaraVariable variable = {};

					variable.m_Size = asmOperand.m_Scale;
					variable.m_Type = KaraVariable::Type::Argument;
					variable.m_Index = static_cast<uint32_t>(reg.m_Type) - static_cast<uint32_t>(SpaceType::Argument_Start);

					state.m_Blob->m_Variables.push_back(variable);
				}
				else if (reg.m_Type == SpaceType::Result)
				{
					reg.m_VariableIndex = state.m_Blob->m_Variables.size() + 1;

					KaraVariable variable = {};

					variable.m_Size = asmOperand.m_Scale;
					variable.m_Type = KaraVariable::Type::Result;

					state.m_Blob->m_Variables.push_back(variable);
				}
			}

			index = reg.m_VariableIndex;
			address += reg.m_Value * multiplier[asmOperand.m_Memory.m_Scale];
		}

		address += asmOperand.m_Memory.m_Offset;

		if (base != 0 && index != 0)
		{
			pcOperand.m_Type = KaraOperand::Type::Dereference;

			pcOperand.m_Expression.m_BaseVariable = (base == ~0) ? 0 : base;
			pcOperand.m_Expression.m_IndexVariable = (index == ~0) ? 0 : index;
			pcOperand.m_Expression.m_Offset = asmOperand.m_Memory.m_Offset;
			pcOperand.m_Expression.m_Multiplier = multiplier[asmOperand.m_Memory.m_Scale];

			return true;
		}

		pcOperand.m_Type = KaraOperand::Type::Variable;

		MemSpace& memory = state.m_Memory[address];

		memory.m_VariableIndex = state.m_Blob->m_Variables.size() + 1;

		KaraVariable variable = {};

		variable.m_Size = asmOperand.m_Scale;
		variable.m_Type = KaraVariable::Type::Local;

		state.m_Blob->m_Variables.push_back(variable);

		pcOperand.m_Variable.m_Index = memory.m_VariableIndex;
		return true;
	} break;
	case ILOperandType_Register:
	{
		if (asmOperand.m_Register.m_Type != Register::general)
		{
			return false;
		}

		if (asmOperand.m_Register.m_Base == REG_RSP)
		{
			return false;
		}

		pcOperand.m_Type = KaraOperand::Type::Variable;

		uint32_t base = 0;
		if (asmOperand.m_Register.m_BaseHigh)
		{
			RegSpace& reg = state.m_General[asmOperand.m_Register.m_Base];

			base = state.m_Blob->m_Variables.size() + 1;

			KaraVariable variable = {};

			variable.m_Size = asmOperand.m_Scale;
			variable.m_Type = KaraVariable::Type::Local;

			switch (asmOperand.m_Register.m_Base)
			{
			case REG_RAX:
			{
				variable.m_Bind = 1;
			} break;
			case REG_RCX:
			{
				variable.m_Bind = 2;
			} break;
			case REG_RDX:
			{
				variable.m_Bind = 3;
			} break;
			case REG_R8:
			{
				variable.m_Bind = 4;
			} break;
			case REG_R9:
			{
				variable.m_Bind = 5;
			} break;
			}

			state.m_Blob->m_Variables.push_back(variable);

			reg.m_VariableHigh = base;
			reg.m_Type = SpaceType::Normal;
		}
		else
		{
			RegSpace& reg = state.m_General[asmOperand.m_Register.m_Base];

			base = state.m_Blob->m_Variables.size() + 1;

			KaraVariable variable = {};

			variable.m_Size = asmOperand.m_Scale;
			variable.m_Type = KaraVariable::Type::Local;

			switch (asmOperand.m_Register.m_Base)
			{
			case REG_RAX:
			{
				variable.m_Bind = 1;
			} break;
			case REG_RCX:
			{
				variable.m_Bind = 2;
			} break;
			case REG_RDX:
			{
				variable.m_Bind = 3;
			} break;
			case REG_R8:
			{
				variable.m_Bind = 4;
			} break;
			case REG_R9:
			{
				variable.m_Bind = 5;
			} break;
			}

			state.m_Blob->m_Variables.push_back(variable);

			reg.m_VariableIndex = base;
			reg.m_Type = SpaceType::Normal;
		}

		pcOperand.m_Variable.m_Index = base;
		return true;
	} break;
	case ILOperandType_MemoryRelative:
	{
		uint64_t address = reinterpret_cast<uintptr_t>(state.m_Cursor) + asmOperand.m_Relative.m_Value - state.m_CursorBase + state.m_ImageBase;

		pcOperand.m_Type = KaraOperand::Type::Variable;

		MemSpace& space = state.m_Memory[address];
		if (space.m_VariableIndex == 0)
		{
			space.m_VariableIndex = state.m_Blob->m_Variables.size() + 1;

			KaraVariable variable = {};

			variable.m_Size = asmOperand.m_Scale;
			variable.m_Type = KaraVariable::Type::Static;
			variable.m_Address = address;

			state.m_Blob->m_Variables.push_back(variable);
		}

		pcOperand.m_Variable.m_Index = space.m_VariableIndex;
		return true;
	} break;
	case ILOperandType_MemoryAbsolute:
	{
		uint64_t address = asmOperand.m_MemoryValue.m_Value;

		pcOperand.m_Type = KaraOperand::Type::Variable;

		MemSpace& space = state.m_Memory[address];
		if (space.m_VariableIndex == 0)
		{
			space.m_VariableIndex = state.m_Blob->m_Variables.size() + 1;

			KaraVariable variable = {};

			variable.m_Size = asmOperand.m_Scale;
			variable.m_Type = KaraVariable::Type::Static;
			variable.m_Address = address;

			state.m_Blob->m_Variables.push_back(variable);
		}

		pcOperand.m_Variable.m_Index = space.m_VariableIndex;
		return true;
	} break;
	}

	return false;
}

bool KaraConverter::ReadOperand(State& state, const ILOperand& asmOperand, KaraOperand& pcOperand) const
{
	constexpr uint8_t multiplier[] = { 1, 2, 4, 8 };

	pcOperand.m_Scale = asmOperand.m_Scale;
	switch (asmOperand.m_Type)
	{
	case ILOperandType_Memory:
	{
		uint64_t address = 0;

		uint32_t base = ~0;
		if (asmOperand.m_Memory.m_Base != IL_INVALID_REGISTER)
		{
			RegSpace& reg = state.m_General[asmOperand.m_Memory.m_Base];
			if (reg.m_VariableIndex == 0)
			{
				if (reg.m_Type >= SpaceType::Argument_Start &&
					reg.m_Type <= SpaceType::Argument_End)
				{
					reg.m_VariableIndex = state.m_Blob->m_Variables.size() + 1;

					KaraVariable variable = {};

					variable.m_Size = asmOperand.m_Scale;
					variable.m_Type = KaraVariable::Type::Argument;
					variable.m_Index = static_cast<uint32_t>(reg.m_Type) - static_cast<uint32_t>(SpaceType::Argument_Start);

					state.m_Blob->m_Variables.push_back(variable);
				}
				else if (reg.m_Type == SpaceType::Result)
				{
					reg.m_VariableIndex = state.m_Blob->m_Variables.size() + 1;

					KaraVariable variable = {};

					variable.m_Size = asmOperand.m_Scale;
					variable.m_Type = KaraVariable::Type::Result;

					state.m_Blob->m_Variables.push_back(variable);
				}
			}

			base = reg.m_VariableIndex;
			address = reg.m_Value;
		}

		uint32_t index = ~0;
		if (asmOperand.m_Memory.m_Index != IL_INVALID_REGISTER)
		{
			RegSpace& reg = state.m_General[asmOperand.m_Memory.m_Index];
			if (reg.m_VariableIndex == 0)
			{
				if (reg.m_Type >= SpaceType::Argument_Start &&
					reg.m_Type <= SpaceType::Argument_End)
				{
					reg.m_VariableIndex = state.m_Blob->m_Variables.size() + 1;

					KaraVariable variable = {};

					variable.m_Size = asmOperand.m_Scale;
					variable.m_Type = KaraVariable::Type::Argument;
					variable.m_Index = static_cast<uint32_t>(reg.m_Type) - static_cast<uint32_t>(SpaceType::Argument_Start);

					state.m_Blob->m_Variables.push_back(variable);
				}
				else if (reg.m_Type == SpaceType::Result)
				{
					reg.m_VariableIndex = state.m_Blob->m_Variables.size() + 1;

					KaraVariable variable = {};

					variable.m_Size = asmOperand.m_Scale;
					variable.m_Type = KaraVariable::Type::Result;

					state.m_Blob->m_Variables.push_back(variable);
				}
			}

			index = reg.m_VariableIndex;
			address += reg.m_Value * multiplier[asmOperand.m_Memory.m_Scale];
		}

		address += asmOperand.m_Memory.m_Offset;

		if (base != 0 && index != 0)
		{
			pcOperand.m_Type = KaraOperand::Type::Dereference;

			pcOperand.m_Expression.m_BaseVariable = (base == ~0) ? 0 : base;
			pcOperand.m_Expression.m_IndexVariable = (index == ~0) ? 0 : index;
			pcOperand.m_Expression.m_Offset = asmOperand.m_Memory.m_Offset;
			pcOperand.m_Expression.m_Multiplier = multiplier[asmOperand.m_Memory.m_Scale];

			return true;
		}
		
		auto memory = state.m_Memory.find(address);
		if (memory == state.m_Memory.end())
		{
			return false;
		}

		if (memory->second.m_VariableIndex == 0)
		{
			return false;
		}

		pcOperand.m_Type = KaraOperand::Type::Variable;
		pcOperand.m_Variable.m_Index = memory->second.m_VariableIndex;
		return true;
	} break;
	case ILOperandType_Register:
	{
		if (asmOperand.m_Register.m_Type != Register::general)
		{
			return false;
		}

		pcOperand.m_Type = KaraOperand::Type::Variable;

		uint32_t base = 0;
		if (asmOperand.m_Register.m_BaseHigh)
		{
			RegSpace& reg = state.m_General[asmOperand.m_Register.m_Base];
			if (reg.m_VariableHigh == 0)
			{
				if (reg.m_Type >= SpaceType::Argument_Start &&
					reg.m_Type <= SpaceType::Argument_End)
				{
					reg.m_VariableHigh = state.m_Blob->m_Variables.size() + 1;

					KaraVariable variable = {};

					variable.m_Size = asmOperand.m_Scale;
					variable.m_Type = KaraVariable::Type::Argument;
					variable.m_Index = static_cast<uint32_t>(reg.m_Type) - static_cast<uint32_t>(SpaceType::Argument_Start);

					state.m_Blob->m_Variables.push_back(variable);
				}
				else if (reg.m_Type == SpaceType::Result)
				{
					reg.m_VariableHigh = state.m_Blob->m_Variables.size() + 1;

					KaraVariable variable = {};

					variable.m_Size = asmOperand.m_Scale;
					variable.m_Type = KaraVariable::Type::Result;

					state.m_Blob->m_Variables.push_back(variable);
				}
				else
				{
					return false;
				}
			}

			base = reg.m_VariableHigh;
		}
		else
		{
			RegSpace& reg = state.m_General[asmOperand.m_Register.m_Base];
			if (reg.m_VariableIndex == 0)
			{
				if (reg.m_Type >= SpaceType::Argument_Start &&
					reg.m_Type <= SpaceType::Argument_End)
				{
					reg.m_VariableIndex = state.m_Blob->m_Variables.size() + 1;

					KaraVariable variable = {};

					variable.m_Size = asmOperand.m_Scale;
					variable.m_Type = KaraVariable::Type::Argument;
					variable.m_Index = static_cast<uint32_t>(reg.m_Type) - static_cast<uint32_t>(SpaceType::Argument_Start);

					state.m_Blob->m_Variables.push_back(variable);
				}
				else if (reg.m_Type == SpaceType::Result)
				{
					reg.m_VariableIndex = state.m_Blob->m_Variables.size() + 1;

					KaraVariable variable = {};

					variable.m_Size = asmOperand.m_Scale;
					variable.m_Type = KaraVariable::Type::Result;

					state.m_Blob->m_Variables.push_back(variable);
				}
				else
				{
					return false;
				}
			}

			base = reg.m_VariableIndex;
		}

		pcOperand.m_Variable.m_Index = base;
		return true;
	} break;
	case ILOperandType_Value:
	{
		pcOperand.m_Type = KaraOperand::Type::Literal;
		pcOperand.m_Literal = asmOperand.m_Value;
		return true;
	} break;
	case ILOperandType_ValueRelative:
	{
		pcOperand.m_Type = KaraOperand::Type::Literal;
		pcOperand.m_Scale = ILOperandScale::ILOperandScale_64;
		pcOperand.m_Literal = reinterpret_cast<uintptr_t>(state.m_Cursor) + asmOperand.m_Relative.m_Value - state.m_CursorBase + state.m_ImageBase;
		return true;
	} break;
	case ILOperandType_MemoryRelative:
	{
		uint64_t address = reinterpret_cast<uintptr_t>(state.m_Cursor) + asmOperand.m_Relative.m_Value - state.m_CursorBase + state.m_ImageBase;

		pcOperand.m_Type = KaraOperand::Type::Variable;

		MemSpace& space = state.m_Memory[address];
		if (space.m_VariableIndex == 0)
		{
			space.m_VariableIndex = state.m_Blob->m_Variables.size() + 1;

			KaraVariable variable = {};

			variable.m_Size = asmOperand.m_Scale;
			variable.m_Type = KaraVariable::Type::Static;
			variable.m_Address = address;

			state.m_Blob->m_Variables.push_back(variable);
		}

		pcOperand.m_Variable.m_Index = space.m_VariableIndex;
		return true;
	} break;
	case ILOperandType_MemoryAbsolute:
	{
		uint64_t address = asmOperand.m_MemoryValue.m_Value;

		pcOperand.m_Type = KaraOperand::Type::Variable;

		MemSpace& space = state.m_Memory[address];
		if (space.m_VariableIndex == 0)
		{
			space.m_VariableIndex = state.m_Blob->m_Variables.size() + 1;

			KaraVariable variable = {};

			variable.m_Size = asmOperand.m_Scale;
			variable.m_Type = KaraVariable::Type::Static;
			variable.m_Address = address;

			state.m_Blob->m_Variables.push_back(variable);
		}

		pcOperand.m_Variable.m_Index = space.m_VariableIndex;
		return true;
	} break;
	}

	return false;
}

bool KaraConverter::ExecWriteOperand(const ILOperand& operand, State& state, uint64_t value) const
{
	constexpr uint64_t masks[] = { 0, (1ull << 8) - 1, (1ull << 16) - 1, (1ull << 32) - 1, ~0 };
	constexpr uint8_t multiplier[] = { 1, 2, 4, 8 };

	switch (operand.m_Type)
	{
	case ILOperandType_Register:
	{
		if (operand.m_Register.m_Type != Register::general)
		{
			return false;
		}

		if (operand.m_Scale == ILOperandScale_8)
		{
			if (operand.m_Register.m_BaseHigh)
			{
				state.m_General[operand.m_Register.m_Base].m_Value = (state.m_General[operand.m_Register.m_Base].m_Value & ~(((1 << 8) - 1) << 8)) | ((value & ((1 << 8) - 1)) << 8);
			}
			else
			{
				state.m_General[operand.m_Register.m_Base].m_Value = (state.m_General[operand.m_Register.m_Base].m_Value & ~((1 << 8) - 1)) | (value & ((1 << 8) - 1));
			}

			return true;
		}

		if (operand.m_Scale == ILOperandScale_16)
		{
			state.m_General[operand.m_Register.m_Base].m_Value = (state.m_General[operand.m_Register.m_Base].m_Value & ~((1 << 16) - 1)) | (value & ((1 << 16) - 1));
			return true;
		}

		state.m_General[operand.m_Register.m_Base].m_Value = value & masks[operand.m_Scale];
		return true;
	} break;
	case ILOperandType_Memory:
	{
		uint64_t address = 0;
		if (operand.m_Memory.m_Base != IL_INVALID_REGISTER)
		{
			address = state.m_General[operand.m_Memory.m_Base].m_Value;
		}

		if (operand.m_Memory.m_Index != IL_INVALID_REGISTER)
		{
			address += state.m_General[operand.m_Memory.m_Index].m_Value * multiplier[operand.m_Memory.m_Scale];
		}

		address += operand.m_Memory.m_Offset;

		if (address >= state.m_ImageBase && address < (state.m_ImageBase + state.m_ImageSize))
		{
			switch (operand.m_Scale)
			{
			case ILOperandScale_8:
			{
				*reinterpret_cast<uint8_t*>(address - state.m_ImageBase + state.m_CursorBase) = value;
			} break;
			case ILOperandScale_16:
			{
				*reinterpret_cast<uint16_t*>(address - state.m_ImageBase + state.m_CursorBase) = value;
			} break;
			case ILOperandScale_32:
			{
				*reinterpret_cast<uint32_t*>(address - state.m_ImageBase + state.m_CursorBase) = value;
			} break;
			case ILOperandScale_64:
			{
				*reinterpret_cast<uint64_t*>(address - state.m_ImageBase + state.m_CursorBase) = value;
			} break;
			}
		}
		else
		{
			state.m_Memory[address].m_Value = value & masks[operand.m_Scale];
		}

		return true;
	} break;
	case ILOperandType_MemoryAbsolute:
	{
		uint64_t address = operand.m_MemoryValue.m_Value;
		if (address >= state.m_ImageBase && address < (state.m_ImageBase + state.m_ImageSize))
		{
			switch (operand.m_Scale)
			{
			case ILOperandScale_8:
			{
				*reinterpret_cast<uint8_t*>(address - state.m_ImageBase + state.m_CursorBase) = value;
			} break;
			case ILOperandScale_16:
			{
				*reinterpret_cast<uint16_t*>(address - state.m_ImageBase + state.m_CursorBase) = value;
			} break;
			case ILOperandScale_32:
			{
				*reinterpret_cast<uint32_t*>(address - state.m_ImageBase + state.m_CursorBase) = value;
			} break;
			case ILOperandScale_64:
			{
				*reinterpret_cast<uint64_t*>(address - state.m_ImageBase + state.m_CursorBase) = value;
			} break;
			}
		}
		else
		{
			state.m_Memory[address].m_Value = value & masks[operand.m_Scale];
		}

		return true;
	} break;
	case ILOperandType_MemoryRelative:
	{
		uint64_t address = reinterpret_cast<uint64_t>(state.m_Cursor) - state.m_CursorBase + state.m_ImageBase + operand.m_Relative.m_Value;
		if (address >= state.m_ImageBase && address < (state.m_ImageBase + state.m_ImageSize))
		{
			switch (operand.m_Scale)
			{
			case ILOperandScale_8:
			{
				*reinterpret_cast<uint8_t*>(address - state.m_ImageBase + state.m_CursorBase) = value;
			} break;
			case ILOperandScale_16:
			{
				*reinterpret_cast<uint16_t*>(address - state.m_ImageBase + state.m_CursorBase) = value;
			} break;
			case ILOperandScale_32:
			{
				*reinterpret_cast<uint32_t*>(address - state.m_ImageBase + state.m_CursorBase) = value;
			} break;
			case ILOperandScale_64:
			{
				*reinterpret_cast<uint64_t*>(address - state.m_ImageBase + state.m_CursorBase) = value;
			} break;
			}
		}
		else
		{
			state.m_Memory[address].m_Value = value & masks[operand.m_Scale];
		}

		return true;
	} break;
	}

	return false;
}

bool KaraConverter::ExecReadOperand(const ILOperand& operand, const State& state, uint64_t& value) const
{
	constexpr uint64_t masks[] = { 0, (1ull << 8) - 1, (1ull << 16) - 1, (1ull << 32) - 1, ~0 };
	constexpr uint8_t multiplier[] = { 1, 2, 4, 8 };

	switch (operand.m_Type)
	{
	case ILOperandType_Memory:
	{
		uint64_t address = 0;
		if (operand.m_Memory.m_Base != IL_INVALID_REGISTER)
		{
			address = state.m_General[operand.m_Memory.m_Base].m_Value;
		}

		if (operand.m_Memory.m_Index != IL_INVALID_REGISTER)
		{
			address += state.m_General[operand.m_Memory.m_Index].m_Value * multiplier[operand.m_Memory.m_Scale];
		}

		address += operand.m_Memory.m_Offset;

		if (address >= state.m_ImageBase && address < (state.m_ImageBase + state.m_ImageSize))
		{
			switch (operand.m_Scale)
			{
			case ILOperandScale_8:
			{
				value = *reinterpret_cast<const uint8_t*>(address - state.m_ImageBase + state.m_CursorBase);
			} break;
			case ILOperandScale_16:
			{
				value = *reinterpret_cast<const uint16_t*>(address - state.m_ImageBase + state.m_CursorBase);
			} break;
			case ILOperandScale_32:
			{
				value = *reinterpret_cast<const uint32_t*>(address - state.m_ImageBase + state.m_CursorBase);
			} break;
			case ILOperandScale_64:
			{
				value = *reinterpret_cast<const uint64_t*>(address - state.m_ImageBase + state.m_CursorBase);
			} break;
			}
		}
		else
		{
			const auto& iterator = state.m_Memory.find(address);
			if (iterator == state.m_Memory.end())
			{
				return false;
			}

			value = iterator->second.m_Value & masks[operand.m_Scale];
		}

		return true;
	} break;
	case ILOperandType_Register:
	{
		if (operand.m_Register.m_Type != Register::general)
		{
			return false;
		}

		if (operand.m_Scale == ILOperandScale_8 &&
			operand.m_Register.m_BaseHigh)
		{
			value = (state.m_General[operand.m_Register.m_Base].m_Value >> 8) & ((1 << 8) - 1);
			return true;
		}

		value = state.m_General[operand.m_Register.m_Base].m_Value & masks[operand.m_Scale];
		return true;
	} break;
	case ILOperandType_Value:
	{
		value = operand.m_Value;
		return true;
	} break;
	case ILOperandType_ValueRelative:
	{
		value = (reinterpret_cast<uintptr_t>(state.m_Cursor) - state.m_CursorBase + state.m_ImageBase + operand.m_Relative.m_Value) & masks[operand.m_Scale];
		return true;
	} break;
	case ILOperandType_MemoryAbsolute:
	{
		uint64_t address = operand.m_MemoryValue.m_Value;
		if (address >= state.m_ImageBase && address < (state.m_ImageBase + state.m_ImageSize))
		{
			switch (operand.m_Scale)
			{
			case ILOperandScale_8:
			{
				value = *reinterpret_cast<const uint8_t*>(address - state.m_ImageBase + state.m_CursorBase);
			} break;
			case ILOperandScale_16:
			{
				value = *reinterpret_cast<const uint16_t*>(address - state.m_ImageBase + state.m_CursorBase);
			} break;
			case ILOperandScale_32:
			{
				value = *reinterpret_cast<const uint32_t*>(address - state.m_ImageBase + state.m_CursorBase);
			} break;
			case ILOperandScale_64:
			{
				value = *reinterpret_cast<const uint64_t*>(address - state.m_ImageBase + state.m_CursorBase);
			} break;
			}
		}
		else
		{
			const auto& iterator = state.m_Memory.find(address);
			if (iterator == state.m_Memory.end())
			{
				return false;
			}

			value = iterator->second.m_Value & masks[operand.m_Scale];
		}

		return true;
	} break;
	case ILOperandType_MemoryRelative:
	{
		uint64_t address = reinterpret_cast<uint64_t>(state.m_Cursor) - state.m_CursorBase + state.m_ImageBase + operand.m_Relative.m_Value;
		if (address >= state.m_ImageBase && address < (state.m_ImageBase + state.m_ImageSize))
		{
			switch (operand.m_Scale)
			{
			case ILOperandScale_8:
			{
				value = *reinterpret_cast<const uint8_t*>(address - state.m_ImageBase + state.m_CursorBase);
			} break;
			case ILOperandScale_16:
			{
				value = *reinterpret_cast<const uint16_t*>(address - state.m_ImageBase + state.m_CursorBase);
			} break;
			case ILOperandScale_32:
			{
				value = *reinterpret_cast<const uint32_t*>(address - state.m_ImageBase + state.m_CursorBase);
			} break;
			case ILOperandScale_64:
			{
				value = *reinterpret_cast<const uint64_t*>(address - state.m_ImageBase + state.m_CursorBase);
			} break;
			}
		}
		else
		{
			const auto& iterator = state.m_Memory.find(address);
			if (iterator == state.m_Memory.end())
			{
				return false;
			}

			value = iterator->second.m_Value & masks[operand.m_Scale];
		}

		return true;
	} break;
	}

	return false;
}

bool KaraConverter::ExecLoadOperand(const ILOperand& operand, const State& state, uint64_t& value) const
{
	constexpr uint8_t multiplier[] = { 1, 2, 4, 8 };

	switch (operand.m_Type)
	{
	case ILOperandType_Memory:
	{
		value = 0;
		if (operand.m_Memory.m_Base != IL_INVALID_REGISTER)
		{
			value = state.m_General[operand.m_Memory.m_Base].m_Value;
		}

		if (operand.m_Memory.m_Index != IL_INVALID_REGISTER)
		{
			value += state.m_General[operand.m_Memory.m_Index].m_Value * multiplier[operand.m_Memory.m_Scale];
		}

		value += operand.m_Memory.m_Offset;
		return true;
	} break;
	case ILOperandType_MemoryRelative:
	{
		value = reinterpret_cast<uintptr_t>(state.m_Cursor) - state.m_CursorBase + state.m_ImageBase + operand.m_Relative.m_Value;
		return true;
	} break;
	case ILOperandType_MemoryAbsolute:
	{
		value = operand.m_MemoryValue.m_Value;
		return true;
	} break;
	}

	return false;
}