using Sabre.Native.Managed;
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
		public IntPtr m_BaseAddress; // image base address
		public IntPtr m_DataAddress; // actual mapped address

		public ExecutableViewSection[] m_Sections;

		public object[] ToListElements()
		{
			List<object> elements = new List<object>();

			foreach (ExecutableViewSection section in m_Sections)
			{
				elements.AddRange(section.ToListElements(this));
			}

			return elements.ToArray();
		}
	}
}
