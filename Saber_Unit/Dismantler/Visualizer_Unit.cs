using Microsoft.VisualStudio.TestTools.UnitTesting;
using Sabre.Dismantler;
using Sabre.Dismantler.Visuals;
using Sabre.ListItems;
using System;
using System.Collections.Generic;
using System.Reflection;

namespace Saber_Unit.Dismantler
{
	[TestClass]
	public class Visualizer_Unit
	{
		enum Base
		{
			General_64 = 0,
			General_32 = 16,
			General_16 = 32,
			General_8l = 48,
			General_8h = 64,
			SSE_128 = 68,
			MMX_64 = 84,
			MPX_64 = 100,
			FPU_80 = 104,
			CR_64 = 112,
			DR_64 = 128,
			Sreg_64 = 144,
		};

		[TestMethod]
		public void ToListElements()
		{
			List<NativeVisual> visuals = new List<NativeVisual>();

			NativeVisual visual = new NativeVisual();

			visual.m_Type = NativeVisual.Type.Instruction;

			visual.m_Instruction.m_Type = 10;
			visual.m_Instruction.m_Size = 0x10;

			visuals.Add(visual);

			visual = new NativeVisual();

			visual.m_Type = NativeVisual.Type.OperandMemory;

			visual.m_Memory.m_Size = Scale.Scale_128;

			visual.m_Memory.m_Base = (int)Base.General_64 + 1;
			visual.m_Memory.m_Index = (int)Base.General_64 + 2;
			visual.m_Memory.m_Segment = ((int)Base.Sreg_64) + 2;

			visual.m_Memory.m_Offset = 0x80;
			visual.m_Memory.m_Multiplier = 8;

			visuals.Add(visual);

			visual = new NativeVisual();

			visual.m_Type = NativeVisual.Type.OperandMemory;

			visual.m_Memory.m_Size = Scale.Scale_80;

			visual.m_Memory.m_Base = (int)Base.General_64 + 1;
			visual.m_Memory.m_Index = NativeVisual.c_InvalidRegister;
			visual.m_Memory.m_Segment = NativeVisual.c_InvalidRegister;

			visual.m_Memory.m_Offset = 0;
			visual.m_Memory.m_Multiplier = 1;

			visuals.Add(visual);

			visual = new NativeVisual();

			visual.m_Type = NativeVisual.Type.OperandMemory;

			visual.m_Memory.m_Size = Scale.Scale_32;

			visual.m_Memory.m_Base = NativeVisual.c_InvalidRegister;
			visual.m_Memory.m_Index = (int)Base.General_64 + 0;
			visual.m_Memory.m_Segment = NativeVisual.c_InvalidRegister;

			visual.m_Memory.m_Offset = -0x10;
			visual.m_Memory.m_Multiplier = 1;

			visuals.Add(visual);

			visual = new NativeVisual();

			visual.m_Type = NativeVisual.Type.OperandMemory;

			visual.m_Memory.m_Size = Scale.Scale_32;

			visual.m_Memory.m_Base = NativeVisual.c_InvalidRegister;
			visual.m_Memory.m_Index = NativeVisual.c_InvalidRegister;
			visual.m_Memory.m_Segment = NativeVisual.c_InvalidRegister;

			visual.m_Memory.m_Offset = 0;
			visual.m_Memory.m_Multiplier = 1;

			visuals.Add(visual);

			visual = new NativeVisual();

			visual.m_Type = NativeVisual.Type.OperandMemoryValue;

			visual.m_Value.m_Size = Scale.Scale_32;
			visual.m_Value.m_Segment = ((int)Base.Sreg_64) + 5;
			visual.m_Value.m_Value = 0x100000;

			visuals.Add(visual);

			visual = new NativeVisual();

			visual.m_Type = NativeVisual.Type.OperandAddressValue;

			visual.m_Value.m_Size = Scale.Scale_32;
			visual.m_Value.m_Value = 0x1000000;

			visuals.Add(visual);

			visual = new NativeVisual();

			visual.m_Type = NativeVisual.Type.OperandValue;

			visual.m_Value.m_Size = Scale.Scale_8;
			visual.m_Value.m_Value = 0x1000;

			visuals.Add(visual);

			visual = new NativeVisual();

			visual.m_Type = NativeVisual.Type.OperandValue;

			visual.m_Value.m_Size = Scale.Scale_16;
			visual.m_Value.m_Value = 0x1020;

			visuals.Add(visual);

			visual = new NativeVisual();

			visual.m_Type = NativeVisual.Type.OperandValue;

			visual.m_Value.m_Size = Scale.Scale_32;
			visual.m_Value.m_Value = 0x1005000;

			visuals.Add(visual);

			visual = new NativeVisual();

			visual.m_Type = NativeVisual.Type.OperandValue;

			visual.m_Value.m_Size = Scale.Scale_64;
			visual.m_Value.m_Value = 0x10050000;

			visuals.Add(visual);

			visual = new NativeVisual();

			visual.m_Type = NativeVisual.Type.OperandRegister;

			visual.m_Register = ((int)Base.General_32) + 2;

			visuals.Add(visual);

			visual = new NativeVisual();

			visual.m_Type = NativeVisual.Type.Instruction;

			visual.m_Instruction.m_Type = 20;
			visual.m_Instruction.m_Size = 0x20;

			visuals.Add(visual);

			visual = new NativeVisual();

			visual.m_Type = NativeVisual.Type.OperandRegister;

			visual.m_Register = ((int)Base.General_32) + 8;

			visuals.Add(visual);

			visual = new NativeVisual();

			visual.m_Type = NativeVisual.Type.Instruction;

			visual.m_Instruction.m_Type = 20;
			visual.m_Instruction.m_Size = 0x40;

			visuals.Add(visual);

			visual = new NativeVisual();

			visual.m_Type = NativeVisual.Type.OperandAddressValue;

			visual.m_Value.m_Size = Scale.Scale_32;
			visual.m_Value.m_Value = 0x800;

			visuals.Add(visual);

			visual = new NativeVisual();

			visual.m_Type = NativeVisual.Type.Instruction;

			visual.m_Instruction.m_Type = 20;
			visual.m_Instruction.m_Size = 0x10;

			visuals.Add(visual);

			visual = new NativeVisual();

			visual.m_Type = NativeVisual.Type.OperandMemoryValue;

			visual.m_Value.m_Size = Scale.Scale_32;
			visual.m_Value.m_Segment = ((int)Base.Sreg_64) + 5;
			visual.m_Value.m_Value = 0x100000;

			visuals.Add(visual);

			visual = new NativeVisual();

			visual.m_Type = NativeVisual.Type.Instruction;

			visual.m_Instruction.m_Type = 20;
			visual.m_Instruction.m_Size = 0x10;

			visuals.Add(visual);

			visual = new NativeVisual();

			visual.m_Type = NativeVisual.Type.OperandMemoryValue;

			visual.m_Value.m_Size = Scale.Scale_64;
			visual.m_Value.m_Segment = NativeVisual.c_InvalidRegister;
			visual.m_Value.m_Value = 0x100000;

			visuals.Add(visual);

			Visualizer.Options options;

			options.m_Bytes = new byte[0x1000];
			options.m_Reference = new IntPtr(0x1000000);

			ByteViewItem[] items = Visualizer.ToListElements(visuals.ToArray(), options);

			Assert.AreEqual(items.Length, 5);

			foreach (ByteViewItem item in items)
			{
				Assert.AreEqual(item.m_Type, ByteViewItem.Type.Code);

				int index = item.m_Info.IndexOf('\0');
				while (index != -1)
				{
					int closer = item.m_Info.IndexOf('\x1', index + 1);
					
					Assert.AreNotEqual(closer, -1);
					Assert.IsTrue(closer - (index + 1) >= 8);
					for (int i = 0; i < 8; i++)
					{
						char number = item.m_Info[index + 1 + i];
						Assert.IsTrue((number >= '0' && number <= '9') || (number >= 'A' && number <= 'F'));
					}

					index = item.m_Info.IndexOf('\0', index + 1 + 8);
				}
			}

			Assert.AreEqual(items[0].m_Address, options.m_Reference);
			Assert.AreEqual(items[1].m_Address, options.m_Reference + 0x10);
			Assert.AreEqual(items[2].m_Address, options.m_Reference + 0x10 + 0x20);
			Assert.AreEqual(items[3].m_Address, options.m_Reference + 0x10 + 0x20 + 0x40);
			Assert.AreEqual(items[4].m_Address, options.m_Reference + 0x10 + 0x20 + 0x40 + 0x10);
		}
	}
}
