using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace Sabre.Explorer.Objects
{
	[StructLayout(LayoutKind.Sequential)]
	internal struct ExecutableCrossReference
	{
		public enum Origin : byte
		{
			None,
			Data,
			Code
		}

		public readonly IntPtr m_Address;
		public readonly Origin m_Origin;
	}
}
