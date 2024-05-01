using Microsoft.VisualStudio.TestTools.UnitTesting;
using Sabre.Explorer.Objects;
using Sabre.ListItems;
using System;
using System.Globalization;
using System.Reflection;
using static Sabre.Explorer.Objects.ExecutableXRef;

namespace Saber_Unit.ListItems
{
	[TestClass]
	public class XRefViewItem_Unit
	{
		[TestMethod]
		public void Construct()
		{
			ExecutableXRef xref = new ExecutableXRef();
			XRefViewItem item = new XRefViewItem(xref);

			Assert.AreEqual(item.m_Origin, xref.m_Origin);
			Assert.AreEqual(item.m_Address, xref.m_Address);

			Assert.AreEqual(new IntPtr(long.Parse(item.m_AddressName, NumberStyles.HexNumber)), xref.m_Address);
		}
	}
}
