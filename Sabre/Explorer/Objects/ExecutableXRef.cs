using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace Sabre.Explorer.Objects
{
	[StructLayout(LayoutKind.Sequential)]
	internal struct ExecutableXRef
	{
		public enum Origin : byte
		{
			None,
			Code,
			Data
		}

		public readonly Origin m_Origin;
		public readonly IntPtr m_Address;
	}
}
