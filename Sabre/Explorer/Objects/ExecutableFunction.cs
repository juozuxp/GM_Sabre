using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace Sabre.Explorer.Objects
{
	public struct ExecutableFunction
	{
		public uint m_Size;
		public IntPtr m_Base;
		public string m_Name;

		public ExecutableXRef[] m_XRefs;
	}
}
