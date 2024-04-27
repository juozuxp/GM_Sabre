using Sabre.Controller;
using Sabre.Explorer.Objects;
using Sabre.ListItems;
using System;
using System.Diagnostics.CodeAnalysis;
using System.Globalization;
using System.Net;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;

namespace Sabre.Views.XRefView
{
	public partial class XRefViewControl : UserControl
	{
		public XRefViewControl()
		{
			this.InitializeComponent();

			SabreController.AddOnGatherXRefsEvent(GatherXRefs);
		}

		private void GatherXRefs(IntPtr address)
		{
			m_AddressField.Text = address.ToString("X16");

			ExecutableXRef[] xrefs = SabreController.GetExecutableXRefs(address);
			XRefViewItem[] items = new XRefViewItem[xrefs.Length];

			for (int i = 0; i < xrefs.Length; i++)
			{
				items[i] = new XRefViewItem(xrefs[i]);
			}

			m_XRefView.ItemsSource = items;
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

				GatherXRefs(new IntPtr(address));
			}
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
	}
}