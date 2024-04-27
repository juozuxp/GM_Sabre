using Sabre.Explorer.Objects;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Sabre.ListItems
{
	internal class FunctionViewItem
	{
		public FunctionViewItem(ExecutableFunction function)
		{
			m_Base = function.m_Base;
			m_Size = function.m_Size;
			m_Name = function.m_Name;

			m_SizeName = function.m_Size.ToString("X8");
			m_BaseName = function.m_Base.ToString("X16");

			m_XRefs = new XRefViewItem[function.m_XRefs.Length];
			for (int i = 0; i < m_XRefs.Length; i++)
			{
				m_XRefs[i] = new XRefViewItem(function.m_XRefs[i]);
			}
		}

		public readonly uint m_Size;
		public readonly IntPtr m_Base;
		public readonly XRefViewItem[] m_XRefs;

		public string m_BaseName { get; private set; }
		public string m_SizeName { get; private set; }
		public string m_Name { get; private set; }
	}
}
