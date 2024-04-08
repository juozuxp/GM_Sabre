using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Sabre.ListItems
{
	internal class ByteViewItem
	{
		public ByteViewItem(IntPtr address, string bytes, string info, IntPtr jumpable)
		{
			m_Address = address;

			m_Info = info;
			m_Bytes = bytes;
			m_DisplayAddress = address.ToString("X16");
			m_Jumpable = jumpable;
		}

		public readonly IntPtr m_Jumpable;
		public readonly IntPtr m_Address;

		public string m_DisplayAddress { get; private set; }
		public string m_Bytes { get; private set; }
		public string m_Info { get; private set; }
	}
}