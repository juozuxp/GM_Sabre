using Microsoft.VisualStudio.TestTools.UnitTesting;
using Sabre.ListItems;
using System;
using System.Globalization;

namespace Saber_Unit.ListItems
{
	[TestClass]
	public class ByteViewItem_Unit
	{
		[TestMethod]
		public void Construct()
		{
			ByteViewItem item = new ByteViewItem(new IntPtr(0x1000), ByteViewItem.Type.Data, "40 80", "some info", new IntPtr(0x18100));

			Assert.AreEqual(item.m_Type, ByteViewItem.Type.Data);
			Assert.AreEqual(item.m_Address, new IntPtr(0x1000));
			Assert.AreEqual(item.m_Jumpable, new IntPtr(0x18100));

			Assert.AreEqual(item.m_Info, "some info");
			Assert.AreEqual(item.m_Bytes, "40 80");
			Assert.AreEqual(long.Parse(item.m_DisplayAddress, NumberStyles.HexNumber), 0x1000);
		}
	}
}
