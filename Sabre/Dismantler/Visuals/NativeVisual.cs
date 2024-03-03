using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace Sabre.Dismantler.Visuals
{
	[StructLayout(LayoutKind.Explicit, Pack = 1)]
	internal struct NativeVisual
	{
		internal enum Type : byte
		{
			Instruction,
			OperandMemory,
			OperandMemoryValue,
			OperandAddressValue,
			OperandValue,
			OperandRegister,
		}

		public const byte c_InvalidRegister = 255;

		[FieldOffset(0)]
		public Type m_Type;

		[FieldOffset(1)]
		public byte m_Register;

		[FieldOffset(1)]
		public Instruction m_Instruction;

		[FieldOffset(1)]
		public Memory m_Memory;

		[FieldOffset(1)]
		public Value m_Value;
	}
}
