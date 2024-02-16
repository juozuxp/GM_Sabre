﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Markup;
using Sabre.Dismantler.Visuals;

namespace Sabre.Explorer
{
	internal struct ExecutableViewSection
	{
		public enum Type
		{
			Bytes,
			Code
		}

		public Type m_Type;

		public uint m_Start;
		public uint m_Size;

		public NativeVisual[] m_Visuals;

		public object[] ToListElements(ExecutableView view)
		{
			switch (m_Type)
			{
				case Type.Bytes:

					byte[] bytes = new byte[m_Size];
					Marshal.Copy(view.m_DataAddress + (int)m_Start, bytes, 0, (int)m_Size);

					StringBuilder builder = new StringBuilder();

					IntPtr address = view.m_BaseAddress + (int)m_Start;

					object[] list = new object[m_Size / 8 + ((m_Size % 8) != 0 ? 1 : 0) + ((m_Start % 8) != 0 ? 1 : 0)];

					int listIndex = 0;
					if (m_Start % 8 != 0)
					{
						for (int i = 0; i < m_Start % 8; i++)
						{
							if (i != 0)
							{
								builder.Append(' ');
							}
							else
							{
								builder.Append(' ');
							}

							builder.Append(bytes[i].ToString("X2"));
						}

						list[listIndex++] = new { m_Address = address, m_Bytes = builder.ToString(), m_Info = string.Empty };
					}

					for (uint i = m_Start % 8; i < m_Size; i += 8, address += 8)
					{
						if (m_Size - i < 8)
						{
							for (int j = 0; j < bytes.Length - i; j++)
							{
								if (j != 0)
								{
									builder.Append(' ');
								}
								else
								{
									builder.Append(' ');
								}

								builder.Append(bytes[i + j].ToString("X2"));
							}

							break;
						}

						list[listIndex++] = new { m_Address = address.ToString("X16"), m_Bytes = $"{bytes[i].ToString("X2")} {bytes[i + 1].ToString("X2")} {bytes[i + 2].ToString("X2")} {bytes[i + 3].ToString("X2")} {bytes[i + 4].ToString("X2")} {bytes[i + 5].ToString("X2")} {bytes[i + 6].ToString("X2")} {bytes[i + 7].ToString("X2")}", m_Info = string.Empty };
					}

					return list;

				case Type.Code:

					Visualizer.Options options = new Visualizer.Options();

					options.m_Bytes = new byte[m_Size];
					options.m_Reference = view.m_BaseAddress + (int)m_Start;

					Marshal.Copy(view.m_DataAddress + (int)m_Start, options.m_Bytes, 0, (int)m_Size);

					return Visualizer.ToListElements(m_Visuals, options);
			}

			return null;
		}
	}
}
