using Sabre.Native.Managed;
using Sabre.Native.PE;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Sabre.Explorer.PE
{
	internal struct PEExportTable
	{
		public IMAGE_EXPORT_DIRECTORY m_Descriptor;

		public string m_Name;
		public PEExportEntry[] m_Entries;
	}
}
