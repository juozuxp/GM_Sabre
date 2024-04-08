using Sabre.Controller;
using System.Diagnostics.CodeAnalysis;
using System.Windows;
using System.Windows.Controls;
using Sabre.Explorer.Objects;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using Sabre.Utility;
using Sabre.ListItems;

namespace Sabre.Views.FunctionView
{
	/// <summary>
	/// Interaction logic for FunctionsControl.
	/// </summary>
	public partial class FunctionsControl : UserControl
	{
		public FunctionsControl()
		{
			this.InitializeComponent();

			SabreController.AddOnLoadEvent(OnExecutableLoad);
		}

		private void OnExecutableLoad()
		{
			m_FunctionView.Items.Clear();

			foreach (ExecutableFunction function in SabreController.GetExecutableFunctions().OrderBy(x => x.m_Base.ToInt64()))
			{
				m_FunctionView.Items.Add(new FunctionViewItem(function));
			}
		}

		private void FunctionView_SizeChanged(object sender, SizeChangedEventArgs e)
		{
			ListView listView = sender as ListView;
			GridView gridView = listView.View as GridView;

			double width = listView.ActualWidth - SystemParameters.VerticalScrollBarWidth;
			for (int i = 0; i < (gridView.Columns.Count - 1); i++)
			{
				width = width - gridView.Columns[i].ActualWidth;
			}

			if (width < 0)
			{
				return;
			}

			gridView.Columns[gridView.Columns.Count - 1].Width = width;
		}
	}
}