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
using System.Windows.Input;

namespace Sabre.Views.FunctionView
{
	/// <summary>
	/// Interaction logic for FunctionsControl.
	/// </summary>
	public partial class FunctionsControl : UserControl
	{
		private FunctionViewItem[] m_FunctionItems;

		public FunctionsControl()
		{
			this.InitializeComponent();

			SabreController.AddOnLoadEvent(OnExecutableLoad);
		}

		private void OnExecutableLoad()
		{
			List<FunctionViewItem> items = new List<FunctionViewItem>();
			foreach (ExecutableFunction function in SabreController.GetExecutableFunctions().OrderBy(x => x.m_Base.ToInt64()))
			{
				items.Add(new FunctionViewItem(function));
			}

			m_FunctionItems = items.ToArray();
			m_FunctionView.ItemsSource = m_FunctionItems;
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

		private void SearchField_TextChanged(object sender, TextChangedEventArgs e)
		{
			TextBox textBox = sender as TextBox;

			if (textBox.Text.Length <= 0)
			{
				m_FunctionView.ItemsSource = m_FunctionItems;
				return;
			}

			m_FunctionView.ItemsSource = m_FunctionItems.Where(x => x.m_Name.Contains(textBox.Text));
		}

		private void FunctionView_MouseDoubleClick(object sender, MouseButtonEventArgs e)
		{
			if (e.ChangedButton != MouseButton.Left)
			{
				return;
			}

			ListView listView = sender as ListView;
			FunctionViewItem item = listView.SelectedItem as FunctionViewItem;

			SabreController.SetJumpToAddress(item.m_Base);
		}
	}
}