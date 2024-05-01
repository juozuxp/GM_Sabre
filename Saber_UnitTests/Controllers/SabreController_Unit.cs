using Microsoft.VisualStudio.TestTools.UnitTesting;
using Sabre.Controllers;
using Sabre.Explorer.Objects;
using System;
using System.Reflection;
using System.Windows.Documents;

namespace Saber_Unit.Controllers
{
	[TestClass]
	public class SabreController_Unit
	{
		private readonly FieldInfo m_Instance;

		public SabreController_Unit()
		{
			Type type = typeof(SabreController);

			m_Instance = type.GetField("s_InstanceBacking", BindingFlags.NonPublic | BindingFlags.Static);
		}

		[TestMethod]
		public void LoadExecutable()
		{
			SabreController.LoadExecutable("kernel32.dll");
		}

		[TestMethod]
		public void SetPseudoCFunction()
		{
			SabreController.SetPseudoCFunction(new IntPtr(0x1000));
		}

		[TestMethod]
		public void SetJumpToAddress()
		{
			SabreController.SetJumpToAddress(new IntPtr(0x1000));
		}

		[TestMethod]
		public void SetXrefAddress()
		{
			SabreController.SetXrefAddress(new IntPtr(0x1000));
		}

		[TestMethod]
		public void GetHeaders()
		{
			m_Instance.SetValue(null, null);

			Assert.IsNull(SabreController.GetHeaders());

			SabreController.LoadExecutable("kernel32.dll");

			Assert.IsNotNull(SabreController.GetHeaders());
		}

		[TestMethod]
		public void GetExecutableView()
		{
			m_Instance.SetValue(null, null);

			Assert.IsNull(SabreController.GetExecutableView());

			SabreController.LoadExecutable("kernel32.dll");

			Assert.IsNotNull(SabreController.GetExecutableView());
		}

		[TestMethod]
		public void GetExecutableFunctions()
		{
			m_Instance.SetValue(null, null);

			Assert.AreEqual(SabreController.GetExecutableFunctions().Length, 0);

			SabreController.LoadExecutable("kernel32.dll");

			Assert.AreNotEqual(SabreController.GetExecutableFunctions().Length, 0);
		}

		[TestMethod]
		public void GetExecutableStrings()
		{
			m_Instance.SetValue(null, null);

			Assert.AreEqual(SabreController.GetExecutableStrings().Length, 0);

			SabreController.LoadExecutable("kernel32.dll");

			Assert.AreNotEqual(SabreController.GetExecutableStrings().Length, 0);
		}

		[TestMethod]
		public void GetPseudoC()
		{
			m_Instance.SetValue(null, null);

			Assert.AreEqual(SabreController.GetPseudoC(new IntPtr(0x1000)).Length, 0);

			SabreController.LoadExecutable("kernel32.dll");

			ExecutableFunction function = SabreController.GetExecutableFunctions()[0];

			Assert.AreNotEqual(SabreController.GetPseudoC(function.m_Base).Length, 0);
		}

		[TestMethod]
		public void GetExecutableXRefs()
		{
			m_Instance.SetValue(null, null);

			Assert.AreEqual(SabreController.GetExecutableXRefs(new IntPtr(0x1000)).Length, 0);

			SabreController.LoadExecutable("kernel32.dll");

			ExecutableFunction? choice = null;
			foreach (ExecutableFunction function in SabreController.GetExecutableFunctions())
			{
				if (function.m_XRefs.Length != 0)
				{
					choice = function;
					break;
				}
			}

			if (choice == null)
			{
				return;
			}

			Assert.AreNotEqual(SabreController.GetPseudoC(choice.Value.m_Base).Length, 0);
		}

		[TestMethod]
		public void AddOnLoadEvent()
		{
			m_Instance.SetValue(null, null);

			bool call0 = false;
			bool call1 = false;

			SabreController.AddOnLoadEvent(() => { call0 = true; });
			SabreController.AddOnLoadEvent(() => { call1 = true; });

			Assert.IsFalse(call0);
			Assert.IsFalse(call1);

			SabreController.LoadExecutable("kernel32.dll");

			Assert.IsTrue(call0);
			Assert.IsTrue(call1);

			bool callLate = false;

			SabreController.AddOnLoadEvent(() => { callLate = true; });

			Assert.IsTrue(callLate);
		}

		[TestMethod]
		public void AddOnConvertCEvent()
		{
			IntPtr address0 = IntPtr.Zero;
			IntPtr address1 = IntPtr.Zero;

			SabreController.AddOnConvertCEvent((IntPtr address) => { address0 = address; });
			SabreController.AddOnConvertCEvent((IntPtr address) => { address1 = address; });

			Assert.AreEqual(address0, IntPtr.Zero);
			Assert.AreEqual(address0, IntPtr.Zero);

			SabreController.SetPseudoCFunction(new IntPtr(0x1000));

			Assert.AreEqual(address0, new IntPtr(0x1000));
			Assert.AreEqual(address0, new IntPtr(0x1000));

			IntPtr addressLate = IntPtr.Zero;

			SabreController.AddOnConvertCEvent((IntPtr address) => { addressLate = address; });

			Assert.AreEqual(addressLate, new IntPtr(0x1000));
		}

		[TestMethod]
		public void AddOnJumpToEvent()
		{
			IntPtr address0 = IntPtr.Zero;
			IntPtr address1 = IntPtr.Zero;

			SabreController.AddOnJumpToEvent((IntPtr address) => { address0 = address; });
			SabreController.AddOnJumpToEvent((IntPtr address) => { address1 = address; });

			Assert.AreEqual(address0, IntPtr.Zero);
			Assert.AreEqual(address0, IntPtr.Zero);

			SabreController.SetJumpToAddress(new IntPtr(0x1000));

			Assert.AreEqual(address0, new IntPtr(0x1000));
			Assert.AreEqual(address0, new IntPtr(0x1000));

			IntPtr addressLate = IntPtr.Zero;

			SabreController.AddOnJumpToEvent((IntPtr address) => { addressLate = address; });

			Assert.AreEqual(addressLate, new IntPtr(0x1000));
		}

		[TestMethod]
		public void AddOnGatherXRefsEvent()
		{
			IntPtr address0 = IntPtr.Zero;
			IntPtr address1 = IntPtr.Zero;

			SabreController.AddOnGatherXRefsEvent((IntPtr address) => { address0 = address; });
			SabreController.AddOnGatherXRefsEvent((IntPtr address) => { address1 = address; });

			Assert.AreEqual(address0, IntPtr.Zero);
			Assert.AreEqual(address0, IntPtr.Zero);

			SabreController.SetXrefAddress(new IntPtr(0x1000));

			Assert.AreEqual(address0, new IntPtr(0x1000));
			Assert.AreEqual(address0, new IntPtr(0x1000));

			IntPtr addressLate = IntPtr.Zero;

			SabreController.AddOnGatherXRefsEvent((IntPtr address) => { addressLate = address; });

			Assert.AreEqual(addressLate, new IntPtr(0x1000));
		}
	}
}
