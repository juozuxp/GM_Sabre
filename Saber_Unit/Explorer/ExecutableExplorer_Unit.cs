﻿using Microsoft.VisualStudio.TestTools.UnitTesting;
using Sabre.Explorer;
using Sabre.Explorer.Objects;
using Sabre.Native.Managed;
using System;
using System.Reflection;

namespace Saber_Unit.Explorer
{
	[TestClass]
	public class ExecutableExplorer_Unit
	{
		private readonly FieldInfo m_Instance;
		private readonly FieldInfo m_ManagedObjectHandle;

		public ExecutableExplorer_Unit()
		{
			m_Instance = typeof(ExecutableExplorer).GetField("m_Instance", BindingFlags.Instance | BindingFlags.NonPublic);
			m_ManagedObjectHandle = typeof(ManagedObject).GetField("m_Handle", BindingFlags.Instance | BindingFlags.NonPublic);
		}

		[TestMethod]
		public void Construct()
		{
			ExecutableExplorer explorer = new ExecutableExplorer("kernel32.dll");

			ManagedObject instance = (ManagedObject)m_Instance.GetValue(explorer);

			Assert.AreNotEqual(m_ManagedObjectHandle.GetValue(instance), IntPtr.Zero);
		}

		[TestMethod]
		public void GetHeaders()
		{
			ExecutableExplorer explorer = new ExecutableExplorer("kernel32.dll");

			Assert.IsNotNull(explorer.GetHeaders());

			explorer = new ExecutableExplorer("kernel32_32bit.dll");

			Assert.IsNotNull(explorer.GetHeaders());

			explorer = new ExecutableExplorer("NotExe.txt");

			Assert.IsNull(explorer.GetHeaders());

			explorer = new ExecutableExplorer("NotAFile.dll");

			Assert.IsNull(explorer.GetHeaders());
		}

		[TestMethod]
		public void GetExecutableView()
		{
			ExecutableExplorer explorer = new ExecutableExplorer("kernel32.dll");

			Assert.IsNotNull(explorer.GetExecutableView());

			explorer = new ExecutableExplorer("kernel32_32bit.dll");

			Assert.IsNotNull(explorer.GetExecutableView());

			explorer = new ExecutableExplorer("NotExe.txt");

			Assert.IsNull(explorer.GetExecutableView());

			explorer = new ExecutableExplorer("NotAFile.dll");

			Assert.IsNull(explorer.GetExecutableView());
		}

		[TestMethod]
		public void GetExecutableFunctions()
		{
			ExecutableExplorer explorer = new ExecutableExplorer("kernel32.dll");

			Assert.AreNotEqual(explorer.GetExecutableFunctions().Length, 0);

			explorer = new ExecutableExplorer("NotExe.txt");

			Assert.AreEqual(explorer.GetExecutableFunctions().Length, 0);

			explorer = new ExecutableExplorer("NotAFile.dll");

			Assert.AreEqual(explorer.GetExecutableFunctions().Length, 0);
		}

		[TestMethod]
		public void GetPCFunction()
		{
			ExecutableExplorer explorer = new ExecutableExplorer("kernel32.dll");

			foreach (ExecutableFunction function in explorer.GetExecutableFunctions())
			{
				Assert.AreNotEqual(explorer.GetPCFunction(function.m_Base).Length, 0);
			}

			explorer = new ExecutableExplorer("NotExe.txt");

			Assert.AreEqual(explorer.GetPCFunction(IntPtr.Zero).Length, 0);

			explorer = new ExecutableExplorer("NotAFile.dll");

			Assert.AreEqual(explorer.GetPCFunction(IntPtr.Zero).Length, 0);
		}

		[TestMethod]
		public void GetExecutableStrings()
		{
			ExecutableExplorer explorer = new ExecutableExplorer("kernel32.dll");

			Assert.AreNotEqual(explorer.GetExecutableStrings().Length, 0);

			explorer = new ExecutableExplorer("kernel32_32bit.dll");

			Assert.AreNotEqual(explorer.GetExecutableStrings().Length, 0);

			explorer = new ExecutableExplorer("NotExe.txt");

			Assert.AreEqual(explorer.GetExecutableStrings().Length, 0);

			explorer = new ExecutableExplorer("NotAFile.dll");

			Assert.AreEqual(explorer.GetExecutableStrings().Length, 0);
		}

		[TestMethod]
		public void GetExecutableXRefs()
		{
			ExecutableExplorer explorer = new ExecutableExplorer("kernel32.dll");

			ExecutableFunction? choice = null;
			foreach (ExecutableFunction function in explorer.GetExecutableFunctions())
			{
				if (function.m_XRefs.Length != 0)
				{
					choice = function;
					break;
				}
			}

			if (choice != null)
			{
				ExecutableXRef[] refs = explorer.GetExecutableXRefs(choice.Value.m_Base);

				Assert.AreEqual(refs.Length, choice.Value.m_XRefs.Length);

				for (int i = 0; i < refs.Length; i++)
				{
					Assert.AreEqual(refs[i].m_Origin, choice.Value.m_XRefs[i].m_Origin);
					Assert.AreEqual(refs[i].m_Address, choice.Value.m_XRefs[i].m_Address);
				}
			}

			explorer = new ExecutableExplorer("NotExe.txt");

			Assert.AreEqual(explorer.GetExecutableXRefs(IntPtr.Zero).Length, 0);

			explorer = new ExecutableExplorer("NotAFile.dll");

			Assert.AreEqual(explorer.GetExecutableXRefs(IntPtr.Zero).Length, 0);
		}
	}
}
