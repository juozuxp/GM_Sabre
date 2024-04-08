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
			m_BaseName = function.m_Base.ToString("X16");
			m_SizeName = function.m_Size.ToString("X8");

			m_Base = function.m_Base;
			m_Size = function.m_Size;
			m_Name = function.m_Name;
		}

		public IntPtr m_Base { get; private set; }
		public uint m_Size { get; private set; }

		public string m_BaseName { get; private set; }
		public string m_SizeName { get; private set; }
		public string m_Name { get; private set; }
	}
}
