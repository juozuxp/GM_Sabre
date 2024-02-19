using System;
using System.Windows;
using System.Windows.Controls;
using Sabre.Controller;
using Sabre.Explorer;

namespace Sabre.Views.ByteView
{
	public partial class ByteViewControl : UserControl
	{
		public ByteViewControl()
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

			if (width < 0)
			{
				return;
			}

			gridView.Columns[gridView.Columns.Count - 1].Width = width;
		}
	}
}