﻿using System;
using System.Windows;
using System.Windows.Controls;
using Sabre.Controller;
using Sabre.Explorer;

namespace Sabre.Views.Disassembly
{
	public partial class DisassemblyControl : UserControl
	{
		public DisassemblyControl()
		{
			this.InitializeComponent();

			SabreController.AddOnLoadEvent(OnExecutableLoad);
		}

		private void OnExecutableLoad()
		{
			ExecutableView? view = SabreController.GetExecutableView();
			if (view == null)
			{
				return;
			}

			m_DismView.ItemsSource = view.Value.ToListElements();
		}

		private void DismView_SizeChanged(object sender, SizeChangedEventArgs e)
		{
			ListView listView = sender as ListView;
			GridView gridView = listView.View as GridView;

			double width = listView.ActualWidth - SystemParameters.VerticalScrollBarWidth;
			for (int i = 0; i < (gridView.Columns.Count - 1); i++)
			{
				width = width - gridView.Columns[i].ActualWidth;
			}

			gridView.Columns[gridView.Columns.Count - 1].Width = width;
		}
	}
}