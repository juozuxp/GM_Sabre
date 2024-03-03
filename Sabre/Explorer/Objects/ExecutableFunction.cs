using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace Sabre.Explorer.Objects
{
	[StructLayout(LayoutKind.Sequential)]
	internal struct ExecutableFunction
	{
		public IntPtr m_Base;
		public uint m_Size;
	}
}
