using System;
using System.Globalization;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using Sabre.Controllers;
using Sabre.Explorer;
using Sabre.ListItems;
using Sabre.Explorer.Objects;
using Sabre.Containers;
using System.IO.Packaging;
using System.Reflection;
using Sabre.Utility;
using System.Windows.Media;

namespace Sabre.Views.ByteView
{
	public partial class ByteViewControl : UserControl
	{
		private RollingStack<int> m_JumpHistory = new RollingStack<int>(100);

		public ByteViewControl()
		{
			this.InitializeComponent();

			SabreController.AddOnLoadEvent(OnExecutableLoad);
			SabreController.AddOnJumpToEvent(JumpToAddress);
		}

		private void OnExecutableLoad()
		{
			ExecutableView? view = SabreController.GetExecutableView();

			m_JumpHistory.Reset();
			m_AddressField.Clear();

			if (view == null)
			{
				m_DismView.ItemsSource = null;
				return;
			}

			m_DismView.ItemsSource = view.Value.ToListElements();
		}

		private void JumpToAddress(IntPtr address)
		{
			if (m_DismView.Items.Count == 0)
			{
				return;
			}

			if (m_DismView.SelectedItem != null)
			{
				m_JumpHistory.Push(m_DismView.SelectedIndex);
			}

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

		private void DismView_SelectionChanged(object sender, SelectionChangedEventArgs e)
		{
			if (e.AddedItems.Count == 0)
			{
				return;
			}

			ByteViewItem obj = e.AddedItems[0] as ByteViewItem;

			m_AddressField.Text = obj.m_DisplayAddress;
		}

		private void AddressField_KeyDown(object sender, KeyEventArgs e)
		{
			if (e.Key == Key.Enter)
			{
				long address;
				if (!long.TryParse(m_AddressField.Text, NumberStyles.HexNumber, NumberFormatInfo.CurrentInfo, out address))
				{
					return;
				}

				JumpToAddress(new IntPtr(address));
			}
		}

		private void DismView_KeyDown(object sender, KeyEventArgs e)
		{
			if (e.Key == Key.C)
			{
				ByteViewItem selected = m_DismView.SelectedItem as ByteViewItem;
				if (selected == null || selected.m_Type != ByteViewItem.Type.Code)
				{
					return;
				}

				SabreController.SetPseudoCFunction(selected.m_Address);
			}
			else if (e.Key == Key.X)
			{
				ByteViewItem selected = m_DismView.SelectedItem as ByteViewItem;
				if (selected == null)
				{
					return;
				}

				SabreController.SetXrefAddress(selected.m_Address);
			}	
			else if (e.Key == Key.Escape || e.Key == Key.Back)
            {
				int index;
                if (!m_JumpHistory.TryPop(out index))
				{
					return;
				}

				m_DismView.SelectedIndex = index;
				m_DismView.ScrollIntoView(m_DismView.SelectedItem);
            }
        }

		private void DismView_MouseDoubleClick(object sender, MouseButtonEventArgs e)
		{
			if (e.ChangedButton != MouseButton.Left)
			{
				return;
			}

			ListView listView = sender as ListView;
			if (listView.SelectedItem == null)
			{
				return;
			}

			ByteViewItem item = listView.SelectedItem as ByteViewItem;
			if (item.m_Jumpable == IntPtr.Zero)
			{
				return;
			}

			JumpToAddress(item.m_Jumpable);
		}
	}
}