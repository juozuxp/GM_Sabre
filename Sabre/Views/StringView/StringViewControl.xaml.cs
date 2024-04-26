using Sabre.Controller;
using Sabre.Explorer.Objects;
using Sabre.ListItems;
using System.Diagnostics.CodeAnalysis;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;

namespace Sabre.Views
{
    public partial class StringViewControl : UserControl
    {
        public StringViewControl()
        {
            this.InitializeComponent();
            SabreController.AddOnLoadEvent(OnLoad);
        }

        private void OnLoad()
        {
            ExecutableString[] strings = SabreController.GetExecutableStrings();
            StringViewItem[] items = new StringViewItem[strings.Length];

            for (int i = 0; i < strings.Length; i++)
            {
                items[i] = new StringViewItem(strings[i]);
            }

            m_StringView.ItemsSource = items;
		}

		private void ListView_SizeChanged(object sender, SizeChangedEventArgs e)
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

		private void StringView_SelectionChanged(object sender, SelectionChangedEventArgs e)
		{
			ListView listView = sender as ListView;
			StringViewItem item = listView.SelectedItem as StringViewItem;

			m_CrossRefView.ItemsSource = item.m_CrossReferences;
		}

		private void StringView_MouseDoubleClick(object sender, MouseButtonEventArgs e)
		{
			if (e.ChangedButton != MouseButton.Left)
			{
				return;
			}

			ListView listView = sender as ListView;
			StringViewItem item = listView.SelectedItem as StringViewItem;

			SabreController.SetJumpToAddress(item.m_Base);
		}

		private void CrossRefView_MouseDoubleClick(object sender, MouseButtonEventArgs e)
		{
			if (e.ChangedButton != MouseButton.Left)
			{
				return;
			}

			ListView listView = sender as ListView;
			CrossRefViewItem item = listView.SelectedItem as CrossRefViewItem;

			SabreController.SetJumpToAddress(item.m_Address);
		}
	}
}