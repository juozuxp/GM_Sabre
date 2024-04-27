using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Documents;

namespace Sabre.ListItems
{
	internal class ByteViewItem
	{
		public enum Type
		{
			None,
			Code,
			Data
		}

		public ByteViewItem(IntPtr address, Type type, string bytes, string info, IntPtr jumpable)
		{
			m_Type = type;
			m_Address = address;
			m_Jumpable = jumpable;

			m_Info = info;
			m_Bytes = bytes;
			m_DisplayAddress = address.ToString("X16");
		}

		public readonly Type m_Type;
		public readonly IntPtr m_Address;
		public readonly IntPtr m_Jumpable;

		public string m_Info { get; private set; }
		public string m_Bytes { get; private set; }
		public string m_DisplayAddress { get; private set; }
	}
}