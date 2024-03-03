using Sabre.Controller;
using System.Diagnostics.CodeAnalysis;
using System.Windows;
using System.Windows.Controls;
using Sabre.Explorer.Objects;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using Sabre.Utility;

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

			ExecutableFunction[] functions = SabreController.GetExecutableFunctions().OrderBy(x => x.m_Base.ToInt64()).ToArray();
			for (int i = 0; i < functions.Length; i++)
			{
				m_FunctionView.Items.Add(new { m_Name = $"f_{functions[i].m_Base.ToString("X16")}", m_Address = functions[i].m_Base.ToString("X16"), m_Size = functions[i].m_Size.ToString("X8") });
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