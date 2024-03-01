using Sabre.Native.Managed;
using Sabre.Native.PE;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace Sabre.Explorer.PE
{
	internal struct PEImportTable
	{
		public string m_Name;

		public PEImportEntry[] m_Entries;
		public IMAGE_IMPORT_DESCRIPTOR m_Descriptor;
	}
}
