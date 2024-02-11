using System;
using System.Diagnostics.CodeAnalysis;
using System.Windows;
using System.Windows.Controls;
using Sabre.Native;
using Sabre.Dismantler;
using System.Windows.Documents;
using System.Text.RegularExpressions;
using Sabre.Utility;
using Sabre.Dismantler.Visuals;
using System.Windows.Media;
using Sabre.Dismantler;
using Sabre.Controller;
using Sabre.Explorer;
using System.Runtime.InteropServices;
using System.Linq;
using System.Text;

namespace Sabre.Views.Disassembly
{
	public partial class DisassemblyControl : UserControl
	{
		public DisassemblyControl()
		{
			this.InitializeComponent();

			SabreController.AddOnLoadEvent(OnExecutableLoad);

			//Disassembler disassembler = new Disassembler();
			//NativeVisual[] visuals = disassembler.Disassemble(rawData, IntPtr.Zero);

			//Visualizer.Inlines inlines = new Visualizer.Inlines();
			//Visualizer.Options options = new Visualizer.Options();

			//inlines.m_Bytes = m_DismBytes.Inlines;
			//inlines.m_Addresses = m_DismAddresses.Inlines;
			//inlines.m_Instructions = m_DismInstructions.Inlines;

			//options.m_Bytes = rawData;

			//options.m_AddressBrush = new SolidColorBrush(Color.FromArgb(0xFF, 0x6B, 0xFB, 0x60));
			//options.m_OffsetBrush = options.m_AddressBrush;
			//options.m_ValueBrush = options.m_AddressBrush;

			//options.m_RegisterBrush = new SolidColorBrush(Color.FromArgb(0xFF, 0xFF, 0x00, 0xFF));

			//options.m_InstructionBrush = new SolidColorBrush(Color.FromArgb(0xFF, 0xFF, 0xAF, 0x00));

			//options.m_Reference = IntPtr.Zero;

			//Visualizer.ToInlineElements(visuals, inlines, options);
		}

		private void OnExecutableLoad()
		{
			ExecutableView? view = SabreController.GetExecutableView();
			if (view == null)
			{
				return;
			}

			byte[] data = new byte[view.Value.m_SizeOfView];

			Marshal.Copy(view.Value.m_DataAddress, data, 0, (int)view.Value.m_SizeOfView);

			StringBuilder builder = new StringBuilder();
			IntPtr address = view.Value.m_BaseAddress;

			object[] source = new object[data.Length / 8];
			for (int i = 0; i < data.Length; i += 8, address += 8)
			{
				if (data.Length - i < 8)
				{
					for (int j = 0; j < data.Length - i; j++)
					{
						if (j != 0)
						{
							builder.Append(' ');
						}
						else
						{
							builder.Append(' ');
						}

						builder.Append(data[i + j].ToString("X2"));
					}

					break;
				}

				source[i / 8] = new { m_Address = address.ToString("X16"), m_Bytes = $"{data[i].ToString("X2")} {data[i + 1].ToString("X2")} {data[i + 2].ToString("X2")} {data[i + 3].ToString("X2")} {data[i + 4].ToString("X2")} {data[i + 5].ToString("X2")} {data[i + 6].ToString("X2")} {data[i + 7].ToString("X2")}", m_Info = "yes" };
			}

			m_DismView.ItemsSource = source;
		}
	}
}