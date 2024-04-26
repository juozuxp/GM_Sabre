using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Sabre.ListItems
{
	internal class CrossRefViewItem
	{
		public enum Type
		{
			None,
			Data,
			Code
		}

		public CrossRefViewItem(Type type, IntPtr address)
		{
			m_Address = address;

			m_Type = type;
			m_AddressName = address.ToString("X16");
		}

		public readonly IntPtr m_Address;

		public Type m_Type { get; private set; }
		public string m_AddressName { get; private set; }
	}
}
