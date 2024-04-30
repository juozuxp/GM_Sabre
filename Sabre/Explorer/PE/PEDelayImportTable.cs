using Sabre.Native.Managed;
using Sabre.Native.PE;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Sabre.Explorer.PE
{
	public struct PEDelayImportTable
	{
		public IMAGE_DELAYLOAD_DESCRIPTOR m_Descriptor;

		public string m_Name;
		public PEImportEntry[] m_Entries;
	}
}
