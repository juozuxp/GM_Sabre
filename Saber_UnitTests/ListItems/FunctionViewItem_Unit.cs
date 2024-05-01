using Microsoft.VisualStudio.TestTools.UnitTesting;
using Sabre.Explorer.Objects;
using Sabre.ListItems;
using System;
using System.Globalization;

namespace Saber_Unit.ListItems
{
	[TestClass]
	public class FunctionViewItem_Unit
	{
		[TestMethod]
		public void Construct()
		{
			ExecutableFunction function = new ExecutableFunction();

			function.m_Size = 0x1000;
			function.m_Base = new IntPtr(0x10000);
			function.m_Name = "neato name";

			function.m_XRefs = new ExecutableXRef[2];

			function.m_XRefs[0] = new ExecutableXRef();
			function.m_XRefs[1] = new ExecutableXRef();

			FunctionViewItem item = new FunctionViewItem(function);

			Assert.AreEqual(item.m_Size, function.m_Size);
			Assert.AreEqual(item.m_Base, function.m_Base);
			Assert.AreEqual(item.m_XRefs.Length, function.m_XRefs.Length);

			Assert.AreEqual(new IntPtr(long.Parse(item.m_BaseName, NumberStyles.HexNumber)), function.m_Base);
			Assert.AreEqual(uint.Parse(item.m_SizeName, NumberStyles.HexNumber), function.m_Size);

			Assert.AreEqual(item.m_Name, function.m_Name);
		}
	}
}
