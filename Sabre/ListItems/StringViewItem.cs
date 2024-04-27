using Sabre.Explorer.Objects;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Sabre.ListItems
{
	internal class StringViewItem
	{
		public StringViewItem(ExecutableString executableString)
		{
			m_Base = executableString.m_Base;
			
			m_String = executableString.m_String;
			m_BaseName = executableString.m_Base.ToString("X16");
			m_Type = executableString.m_IsWide ? "UTF-16" : "ASCII";

			m_XRefs = new XRefViewItem[executableString.m_XRefs.Length];
			for (int i = 0; i < m_XRefs.Length; i++)
			{
				m_XRefs[i] = new XRefViewItem(executableString.m_XRefs[i]);
			}
		}

		public readonly IntPtr m_Base;

		public readonly XRefViewItem[] m_XRefs;

		public string m_String { get; private set; }
		public string m_BaseName { get; private set; }
		public string m_Type { get; private set; }
	}
}
