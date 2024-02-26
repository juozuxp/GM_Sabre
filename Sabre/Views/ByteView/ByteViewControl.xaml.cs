using System;
using System.Globalization;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using Sabre.Controller;
using Sabre.Explorer;
using Sabre.ListItems;

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

		private void JumpToAddress(IntPtr address)
		{
			for (int i = 0; i < m_DismView.Items.Count; i++)
			{
				ByteViewItem item = m_DismView.Items[i] as ByteViewItem;

				if (((ulong)address.ToInt64()) <= ((ulong)item.m_Address.ToInt64()))
				{
					if (i == 0 ||
						address == item.m_Address)
					{
						m_DismView.ScrollIntoView(item);
						m_DismView.SelectedItem = item;
					}
					else
					{
						item = m_DismView.Items[i - 1] as ByteViewItem;

						m_DismView.ScrollIntoView(item);
						m_DismView.SelectedItem = item;
					}

					return;
				}
			}

			object lastItem = m_DismView.Items[m_DismView.Items.Count - 1];

			m_DismView.ScrollIntoView(lastItem);
			m_DismView.SelectedItem = lastItem;
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

		private void m_DismView_SelectionChanged(object sender, SelectionChangedEventArgs e)
		{
			ByteViewItem obj = e.AddedItems[0] as ByteViewItem;

			m_AddressField.Text = obj.m_DisplayAddress;
		}

		private void AddressField_KeyDown(object sender, KeyEventArgs e)
		{
			if (e.Key == Key.Enter)
			{
				IntPtr address = new IntPtr(long.Parse(m_AddressField.Text, NumberStyles.HexNumber));

				JumpToAddress(address);
			}
		}
	}
}