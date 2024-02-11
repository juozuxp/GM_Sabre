using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace Sabre.Explorer
{
	internal struct ExecutableView
	{
		public IntPtr m_DataAddress; // actual mapped address

		public uint m_SizeOfView; // size of the data
		public IntPtr m_BaseAddress; // image base address
	}
}
