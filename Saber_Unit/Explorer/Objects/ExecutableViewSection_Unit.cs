using Microsoft.VisualStudio.TestTools.UnitTesting;
using Sabre.Dismantler.Visuals;
using Sabre.Explorer.Objects;
using Sabre.ListItems;
using System;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;

namespace Saber_Unit.Explorer.Objects
{
	[TestClass]
	public class ExecutableViewSection_Unit
	{
		[TestMethod]
		public void ToListElements()
		{
			ExecutableView view = new ExecutableView();
			ExecutableViewSection viewSection = new ExecutableViewSection();

			view.m_BaseAddress = new IntPtr(0x18000000);
			view.m_DataAddress = Marshal.AllocCoTaskMem(0x1000);

			string text = "hello world :)";

			Marshal.Copy(Encoding.ASCII.GetBytes(text), 0, view.m_DataAddress + 0x2, text.Length);

			viewSection.m_Type = ExecutableViewSection.Type.Bytes;
			viewSection.m_Start = 0x2;
			viewSection.m_Size = 0x100;

			view.m_Sections = new ExecutableViewSection[1];
			view.m_Sections[0] = viewSection;

			ByteViewItem[] items = viewSection.ToListElements(view);

			Assert.AreEqual(items.Length, (viewSection.m_Size - (8 - ((viewSection.m_Start % 8) == 0 ? 8 : (viewSection.m_Start % 8)))) / 8 + ((viewSection.m_Start % 8) == 0 ? 0 : 1) + (((viewSection.m_Size - (8 - ((viewSection.m_Start % 8) == 0 ? 8 : (viewSection.m_Start % 8)))) % 8) == 0 ? 0 : 1));

			foreach (ByteViewItem item in items)
			{
				Assert.IsNotNull(item);

				Assert.AreEqual(item.m_Jumpable, IntPtr.Zero);
				Assert.AreEqual(item.m_Type, ByteViewItem.Type.Data);
			}

			Assert.AreEqual(items[0].m_Info, "h e l l o .");
			Assert.AreEqual(items[1].m_Info, "w o r l d . : )");

			Assert.AreEqual(items[0].m_Address, view.m_BaseAddress + (int)viewSection.m_Start);

			IntPtr address = view.m_BaseAddress + IntPtr.Size;
			foreach (ByteViewItem item in items.Skip(1))
			{
				Assert.AreEqual(item.m_Address, address);
				address += IntPtr.Size;
			}

			viewSection.m_Type = ExecutableViewSection.Type.Code;
			viewSection.m_Start = 0x0;
			viewSection.m_Size = 0x100;

			viewSection.m_Visuals = new NativeVisual[4];

			viewSection.m_Visuals[0].m_Type = NativeVisual.Type.Instruction;

			viewSection.m_Visuals[0].m_Instruction.m_Size = 0x10;
			viewSection.m_Visuals[0].m_Instruction.m_Type = 0x85;

			viewSection.m_Visuals[1].m_Type = NativeVisual.Type.Instruction;

			viewSection.m_Visuals[1].m_Instruction.m_Size = 0x12;
			viewSection.m_Visuals[1].m_Instruction.m_Type = 0x12;

			viewSection.m_Visuals[2].m_Type = NativeVisual.Type.Instruction;

			viewSection.m_Visuals[2].m_Instruction.m_Size = 0x22;
			viewSection.m_Visuals[2].m_Instruction.m_Type = 0x45;

			viewSection.m_Visuals[3].m_Type = NativeVisual.Type.Instruction;

			viewSection.m_Visuals[3].m_Instruction.m_Size = 0x8;
			viewSection.m_Visuals[3].m_Instruction.m_Type = 0x33;

			view.m_Sections[0] = viewSection;

			items = viewSection.ToListElements(view);

			Assert.AreEqual(items.Length, 4);

			foreach (ByteViewItem item in items)
			{
				Assert.IsNotNull(item);

				Assert.AreEqual(item.m_Type, ByteViewItem.Type.Code);
			}

			viewSection.m_Type = ExecutableViewSection.Type.Bytes;
			viewSection.m_Start = 0x0;
			viewSection.m_Size = 0x100;

			view.m_Sections[0] = viewSection;

			items = viewSection.ToListElements(view);

			Assert.AreEqual(items.Length, (viewSection.m_Size - (8 - ((viewSection.m_Start % 8) == 0 ? 8 : (viewSection.m_Start % 8)))) / 8 + ((viewSection.m_Start % 8) == 0 ? 0 : 1) + (((viewSection.m_Size - (8 - ((viewSection.m_Start % 8) == 0 ? 8 : (viewSection.m_Start % 8)))) % 8) == 0 ? 0 : 1));

			address = view.m_BaseAddress;
			foreach (ByteViewItem item in items)
			{
				Assert.IsNotNull(item);

				Assert.AreEqual(item.m_Jumpable, IntPtr.Zero);
				Assert.AreEqual(item.m_Type, ByteViewItem.Type.Data);

				Assert.AreEqual(item.m_Address, address);
				address += IntPtr.Size;
			}

			viewSection.m_Type = ExecutableViewSection.Type.Bytes;
			viewSection.m_Start = 0x0;
			viewSection.m_Size = 0x9;

			view.m_Sections[0] = viewSection;

			items = viewSection.ToListElements(view);

			Assert.AreEqual(items.Length, (viewSection.m_Size - (8 - ((viewSection.m_Start % 8) == 0 ? 8 : (viewSection.m_Start % 8)))) / 8 + ((viewSection.m_Start % 8) == 0 ? 0 : 1) + (((viewSection.m_Size - (8 - ((viewSection.m_Start % 8) == 0 ? 8 : (viewSection.m_Start % 8)))) % 8) == 0 ? 0 : 1));

			address = view.m_BaseAddress;
			foreach (ByteViewItem item in items)
			{
				Assert.IsNotNull(item);

				Assert.AreEqual(item.m_Jumpable, IntPtr.Zero);
				Assert.AreEqual(item.m_Type, ByteViewItem.Type.Data);

				Assert.AreEqual(item.m_Address, address);
				address += IntPtr.Size;
			}

			Marshal.FreeCoTaskMem(view.m_DataAddress);
		}
	}
}
