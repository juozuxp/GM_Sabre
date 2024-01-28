﻿using System;
using System.Diagnostics.CodeAnalysis;
using System.Windows;
using System.Windows.Controls;
using Saber.Native;
using Saber.Dismantler;
using System.Windows.Documents;
using System.Text.RegularExpressions;
using Saber.Utility;
using Saber.Dismantler.Visuals;
using System.Windows.Media;

namespace Saber.DissasemblyView
{
	/// <summary>
	/// Interaction logic for DissasemblerControl.
	/// </summary>
	public partial class DissasemblerControl : UserControl
	{
		TextBlock m_DismBytes;
		TextBlock m_DismAddresses;
		TextBlock m_DismInstructions;

		public DissasemblerControl()
		{

			byte[] rawData = {
				0x48, 0x8D, 0x0D, 0x19, 0xCC, 0x24, 0x00, 0xE9, 0xC4, 0xCB, 0x23, 0x00,
				0xCC, 0xCC, 0xCC, 0xCC, 0x48, 0x83, 0xEC, 0x38, 0x48, 0xC7, 0x44, 0x24,
				0x20, 0xFE, 0xFF, 0xFF, 0xFF, 0x33, 0xD2, 0x48, 0x8D, 0x0D, 0x42, 0xDF,
				0x3D, 0x00, 0xFF, 0x15, 0x14, 0x20, 0x25, 0x00, 0x90, 0x48, 0x8D, 0x05,
				0x44, 0x9A, 0x25, 0x00, 0x48, 0x89, 0x05, 0x2D, 0xDF, 0x3D, 0x00, 0x48,
				0xC7, 0x05, 0x32, 0xDF, 0x3D, 0x00, 0x00, 0x00, 0x00, 0x00, 0x33, 0xD2,
				0x48, 0x8D, 0x0D, 0x19, 0xDF, 0x3D, 0x00, 0xE8, 0x0C, 0x46, 0x08, 0x00,
				0x90, 0x48, 0x8D, 0x0D, 0xD4, 0xCB, 0x24, 0x00, 0x48, 0x83, 0xC4, 0x38,
				0xE9, 0x6B, 0xCB, 0x23, 0x00, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC,
				0xCC, 0xCC, 0xCC, 0xCC, 0x48, 0x8B, 0x05, 0x69, 0x1E, 0x25, 0x00, 0x48,
				0x8D, 0x0D, 0xC2, 0xCB, 0x24, 0x00, 0x48, 0x89, 0x05, 0xDB, 0xDE, 0x3D,
				0x00, 0xE9, 0x46, 0xCB, 0x23, 0x00, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC,
				0x48, 0x8B, 0x05, 0x41, 0x1E, 0x25, 0x00, 0x48, 0x8D, 0x0D, 0xE2, 0xCB,
				0x24, 0x00, 0x48, 0x89, 0x05, 0x9B, 0xDE, 0x3D, 0x00, 0xE9, 0x26, 0xCB,
				0x23, 0x00, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0x48, 0x83, 0xEC, 0x18,
				0x33, 0xC9, 0x48, 0x8D, 0x05, 0xC3, 0xC0, 0x00, 0x00, 0x48, 0x89, 0x04,
				0x24, 0x48, 0x8D, 0x05, 0x28, 0xC0, 0x00, 0x00, 0x89, 0x4C, 0x24, 0x08,
				0x0F, 0x10, 0x04, 0x24, 0x48, 0x89, 0x04, 0x24, 0x48, 0x8D, 0x05, 0xF5,
				0xC7, 0x00, 0x00, 0x89, 0x4C, 0x24, 0x08, 0x0F, 0x29, 0x05, 0x2A, 0x1F,
				0x3D, 0x00, 0x0F, 0x10, 0x04, 0x24, 0x48, 0x89, 0x04, 0x24, 0x89, 0x4C,
				0x24, 0x08, 0x0F, 0x29, 0x05, 0x37, 0x1F, 0x3D, 0x00, 0x0F, 0x10, 0x04,
				0x24, 0x0F, 0x29, 0x05, 0x4C, 0x1F, 0x3D, 0x00, 0x48, 0x83, 0xC4, 0x18,
				0xC3, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0x48, 0x8D, 0x0D, 0xB9,
				0xCB, 0x24, 0x00, 0xE9, 0xB4, 0xCA, 0x23, 0x00, 0xCC, 0xCC, 0xCC, 0xCC,
				0x48, 0x8D, 0x0D, 0xB9, 0xCB, 0x24, 0x00, 0xE9, 0xA4, 0xCA, 0x23, 0x00,
				0xCC, 0xCC, 0xCC, 0xCC, 0x48, 0x8D, 0x0D, 0xA9, 0xCC, 0x24, 0x00, 0xE9,
				0x94, 0xCA, 0x23, 0x00, 0xCC, 0xCC, 0xCC, 0xCC, 0x48, 0x8D, 0x0D, 0xA9,
				0xCC, 0x24, 0x00, 0xE9, 0x84, 0xCA, 0x23, 0x00, 0xCC, 0xCC, 0xCC, 0xCC,
				0x48, 0x83, 0xEC, 0x28, 0x41, 0xB1, 0x01, 0x4C, 0x8D, 0x05, 0x32, 0xE8,
				0x01, 0x00, 0x48, 0x8D, 0x15, 0xF3, 0x40, 0x27, 0x00, 0x48, 0x8D, 0x0D,
				0x14, 0xF7, 0x3D, 0x00, 0xE8, 0x9F, 0x85, 0x0F, 0x00, 0x48, 0x8D, 0x0D,
				0x88, 0xCC, 0x24, 0x00, 0x48, 0x83, 0xC4, 0x28, 0xE9, 0x4F, 0xCA, 0x23,
				0x00, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC,
				0xCC, 0xCC, 0xCC, 0xCC, 0x48, 0x8D, 0x0D, 0xA9, 0xCC, 0x24, 0x00, 0xE9,
				0x34, 0xCA, 0x23, 0x00, 0xCC, 0xCC, 0xCC, 0xCC, 0x48, 0x83, 0xEC, 0x38,
				0x4C, 0x8D, 0x0D, 0x65, 0x61, 0x03, 0x00, 0xC6, 0x44, 0x24, 0x20, 0x01,
				0x4C, 0x8D, 0x05, 0x69, 0x61, 0x03, 0x00, 0x48, 0x8D, 0x15, 0xEA, 0xB8,
				0x26, 0x00, 0x48, 0x8D, 0x0D, 0xE3, 0x02, 0x3E, 0x00, 0xE8, 0xE6, 0x85,
				0x0F, 0x00, 0x48, 0x8D, 0x0D, 0x2F, 0xCD, 0x24, 0x00, 0x48, 0x83, 0xC4,
				0x38, 0xE9, 0xF6, 0xC9, 0x23, 0x00, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC,
				0x48, 0x8D, 0x0D, 0x59, 0xCD, 0x24, 0x00, 0xE9, 0xE4, 0xC9, 0x23, 0x00,
				0xCC, 0xCC, 0xCC, 0xCC, 0x48, 0x8D, 0x0D, 0x59, 0xCD, 0x24, 0x00, 0xE9,
				0xD4, 0xC9, 0x23, 0x00, 0xCC, 0xCC, 0xCC, 0xCC
			};


			this.InitializeComponent();

			m_DismBytes = FindName("dism_bytes") as TextBlock;
			m_DismAddresses = FindName("dism_addresses") as TextBlock;
			m_DismInstructions = FindName("dism_instructions") as TextBlock;

			Disassembler disassembler = new Disassembler();
			NativeVisual[] visuals = disassembler.Disassemble(rawData, IntPtr.Zero);

			Visualizer.Inlines inlines = new Visualizer.Inlines();
			Visualizer.Options options = new Visualizer.Options();

			inlines.m_Bytes = m_DismBytes.Inlines;
			inlines.m_Addresses = m_DismAddresses.Inlines;
			inlines.m_Instructions = m_DismInstructions.Inlines;

			options.m_Bytes = rawData;

			options.m_AddressBrush = new SolidColorBrush(Color.FromArgb(0xFF, 0x6B, 0xFB, 0x60));
			options.m_OffsetBrush = options.m_AddressBrush;
			options.m_ValueBrush = options.m_AddressBrush;

			options.m_RegisterBrush = new SolidColorBrush(Color.FromArgb(0xFF, 0xFF, 0x00, 0xFF));

			options.m_Reference = IntPtr.Zero;

			Visualizer.ToInlineElements(visuals, inlines, options);
		}
	}
}