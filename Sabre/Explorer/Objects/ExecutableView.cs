using Sabre.ListItems;
using Sabre.Native.Managed;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace Sabre.Explorer.Objects
{
	public struct ExecutableView
	{
		public IntPtr m_BaseAddress; // image base address
		public IntPtr m_DataAddress; // actual mapped address

		public ExecutableViewSection[] m_Sections;

		public ByteViewItem[] ToListElements()
		{
			List<ByteViewItem> elements = new List<ByteViewItem>();

			foreach (ExecutableViewSection section in m_Sections)
			{
				elements.AddRange(section.ToListElements(this));
			}

			return elements.ToArray();
		}
	}
}
