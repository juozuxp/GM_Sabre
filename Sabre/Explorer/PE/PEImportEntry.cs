using Sabre.Native.Managed;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Sabre.Explorer.PE
{
	public struct PEImportEntry
	{
		public ushort m_Ordinal;
		public uint m_FunctionAddressBase;

		public string m_Name;
	}
}
