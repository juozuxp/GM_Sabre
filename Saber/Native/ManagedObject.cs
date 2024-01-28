﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace Saber.Native
{
	[StructLayout(LayoutKind.Sequential)]
	internal readonly struct ManagedObject : IDisposable
	{
		[DllImport("Dismantler.dll")]
		private static extern void Managed_Destroy(IntPtr handle);

		public static implicit operator IntPtr(ManagedObject obj)
		{
			return obj.m_Handle;
		}

		public void Dispose()
		{
			Managed_Destroy(m_Handle);
		}

		private readonly IntPtr m_Vtable;
		private readonly IntPtr m_Handle;
	}
}
