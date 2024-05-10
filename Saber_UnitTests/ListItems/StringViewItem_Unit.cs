using Microsoft.VisualStudio.TestTools.UnitTesting;
using Sabre.Explorer.Objects;
using Sabre.ListItems;
using System;
using System.Globalization;

namespace Saber_Unit.ListItems
{
	[TestClass]
	public class StringViewItem_Unit
	{
		[TestMethod]
		public void Construct_unicode()
		{
			ExecutableString exec = new ExecutableString();

			exec.m_IsWide = true;
			exec.m_String = "hello world";
			exec.m_Base = new IntPtr(0x1000);

			exec.m_XRefs = new ExecutableXRef[1];
			exec.m_XRefs[0] = new ExecutableXRef();

			StringViewItem item = new StringViewItem(exec);

			Assert.AreEqual(item.m_Base, exec.m_Base);
			Assert.AreEqual(item.m_String, exec.m_String);
			Assert.AreEqual(item.m_XRefs.Length, exec.m_XRefs.Length);

			Assert.AreEqual(new IntPtr(long.Parse(item.m_BaseName, NumberStyles.HexNumber)), exec.m_Base);

			Assert.AreEqual(item.m_Type, "UTF-16");
		}

		[TestMethod]
		public void Construct_ascii()
		{
			ExecutableString exec = new ExecutableString();

			exec.m_IsWide = false;
			exec.m_String = "hello world";
			exec.m_Base = new IntPtr(0x1000);

			exec.m_XRefs = new ExecutableXRef[1];
			exec.m_XRefs[0] = new ExecutableXRef();

			StringViewItem item = new StringViewItem(exec);

			Assert.AreEqual(item.m_Base, exec.m_Base);
			Assert.AreEqual(item.m_String, exec.m_String);
			Assert.AreEqual(item.m_XRefs.Length, exec.m_XRefs.Length);

			Assert.AreEqual(new IntPtr(long.Parse(item.m_BaseName, NumberStyles.HexNumber)), exec.m_Base);

			Assert.AreEqual(item.m_Type, "ASCII");
		}
	}
}
