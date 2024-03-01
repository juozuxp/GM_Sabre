using Sabre.Native.Managed;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Sabre.Explorer.PE
{
	internal struct PEExportEntry
	{
		public ushort m_Ordinal;
		public uint m_Function;

		public string m_Name;
		public string m_Forwarder;
	}
}
