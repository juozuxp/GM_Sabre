using Sabre.Explorer.Objects;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Sabre.ListItems
{
	internal class XRefViewItem
	{
		public XRefViewItem(ExecutableXRef xref)
		{
			m_Address = xref.m_Address;

			m_Origin = xref.m_Origin;
			m_AddressName = xref.m_Address.ToString("X16");
		}

		public readonly IntPtr m_Address;

		public string m_AddressName { get; private set; }
		public ExecutableXRef.Origin m_Origin { get; private set; }
	}
}
