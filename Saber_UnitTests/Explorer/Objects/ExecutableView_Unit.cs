using Microsoft.VisualStudio.TestTools.UnitTesting;
using Sabre.Dismantler.Visuals;
using Sabre.Explorer.Objects;
using Sabre.ListItems;
using System;
using System.Runtime.InteropServices;
using System.Text;

namespace Saber_Unit.Explorer.Objects
{
	[TestClass]
	public class ExecutableView_Unit
	{
		[TestMethod]
		public void ToListElements()
		{
			ExecutableView view = new ExecutableView();

			ExecutableViewSection dataSection = new ExecutableViewSection();

			view.m_BaseAddress = new IntPtr(0x18000000);
			view.m_DataAddress = Marshal.AllocCoTaskMem(0x1000);

			dataSection.m_Type = ExecutableViewSection.Type.Bytes;
			dataSection.m_Start = 0x0;
			dataSection.m_Size = 0x100;

			ExecutableViewSection codeSection = new ExecutableViewSection();

			codeSection.m_Type = ExecutableViewSection.Type.Code;
			codeSection.m_Start = 0x0;
			codeSection.m_Size = 0x100;

			codeSection.m_Visuals = new NativeVisual[4];

			codeSection.m_Visuals[0].m_Type = NativeVisual.Type.Instruction;

			codeSection.m_Visuals[0].m_Instruction.m_Size = 0x10;
			codeSection.m_Visuals[0].m_Instruction.m_Type = 0x85;

			codeSection.m_Visuals[1].m_Type = NativeVisual.Type.Instruction;

			codeSection.m_Visuals[1].m_Instruction.m_Size = 0x12;
			codeSection.m_Visuals[1].m_Instruction.m_Type = 0x12;

			codeSection.m_Visuals[2].m_Type = NativeVisual.Type.Instruction;

			codeSection.m_Visuals[2].m_Instruction.m_Size = 0x22;
			codeSection.m_Visuals[2].m_Instruction.m_Type = 0x45;

			codeSection.m_Visuals[3].m_Type = NativeVisual.Type.Instruction;

			codeSection.m_Visuals[3].m_Instruction.m_Size = 0x8;
			codeSection.m_Visuals[3].m_Instruction.m_Type = 0x33;

			view.m_Sections = new ExecutableViewSection[2];

			view.m_Sections[0] = dataSection;
			view.m_Sections[1] = codeSection;

			ByteViewItem[] items = view.ToListElements();

			Assert.AreEqual(items.Length, 0x24);

			for (int i = 0; i < 0x20; i++)
			{
				Assert.IsNotNull(items[i]);
				Assert.AreEqual(items[i].m_Type, ByteViewItem.Type.Data);
			}

			for (int i = 0x20; i < 0x24; i++)
			{
				Assert.IsNotNull(items[i]);
				Assert.AreEqual(items[i].m_Type, ByteViewItem.Type.Code);
			}
		}
	}
}
