using Sabre.Explorer.Objects;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Sabre.ListItems
{
	internal class CrossRefViewItem
	{
		public CrossRefViewItem(ExecutableCrossReference crossReference)
		{
			m_Address = crossReference.m_Address;

			m_Origin = crossReference.m_Origin;
			m_AddressName = crossReference.m_Address.ToString("X16");
		}

		public readonly IntPtr m_Address;

		public string m_AddressName { get; private set; }
		public ExecutableCrossReference.Origin m_Origin { get; private set; }
	}
}
