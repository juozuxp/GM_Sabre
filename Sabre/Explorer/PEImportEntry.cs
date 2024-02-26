using Sabre.Native.Managed;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Sabre.Explorer
{
	internal struct PEImportEntry
	{
		public ushort m_Ordinal;
		public uint m_FirstThunk;
		public uint m_OriginalFirstThunk;

		public string m_Name;
	}
}
