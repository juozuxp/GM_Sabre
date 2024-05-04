using Microsoft.IO;
using Sabre.Controllers;
using Sabre.Explorer.Objects;
using Sabre.ListItems;
using System.Diagnostics.CodeAnalysis;
using System.Linq;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;

namespace Sabre.Views
{
    public partial class StringViewControl : UserControl
    {
		private StringViewItem[] m_StringItems;

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

			m_StringItems = items.OrderBy(x => x.m_String).ToArray();
			m_StringView.ItemsSource = m_StringItems;
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
			if (e.AddedItems.Count == 0)
			{
				return;
			}

			StringViewItem item = e.AddedItems[0] as StringViewItem;

			m_XRefView.ItemsSource = item.m_XRefs;
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

		private void XRefView_MouseDoubleClick(object sender, MouseButtonEventArgs e)
		{
			if (e.ChangedButton != MouseButton.Left)
			{
				return;
			}

			ListView listView = sender as ListView;
			XRefViewItem item = listView.SelectedItem as XRefViewItem;

			SabreController.SetJumpToAddress(item.m_Address);
		}

		private void SearchField_TextChanged(object sender, TextChangedEventArgs e)
		{
			if (m_StringItems == null)
			{
				return;
			}

			TextBox textBox = sender as TextBox;
			if (textBox.Text.Length <= 0)
			{
				m_StringView.ItemsSource = m_StringItems;
				return;
			}

			m_StringView.ItemsSource = m_StringItems.Where(x => x.m_String.ToLower().Contains(textBox.Text.ToLower()));
		}
	}
}