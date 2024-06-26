﻿using Sabre.Controllers;
using System;
using System.Diagnostics.CodeAnalysis;
using System.Windows;
using System.Windows.Controls;

namespace Sabre.Views.PseudoC
{
	/// <summary>
	/// Interaction logic for PCControl.
	/// </summary>
	public partial class PCControl : UserControl
	{
		public PCControl()
		{
			this.InitializeComponent();

			SabreController.AddOnLoadEvent(OnLoad);
			SabreController.AddOnConvertCEvent(LoadPseudoC);
		}

		private void OnLoad()
		{
			m_Display.Clear();
		}

		public void LoadPseudoC(IntPtr function)
		{
			m_Display.Text = SabreController.GetPseudoC(function);
		}
	}
}