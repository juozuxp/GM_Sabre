using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Documents;
using System.Windows;
using System.Windows.Controls;
using System.Collections.Specialized;
using System.Globalization;
using System.Collections.Concurrent;
using System.Windows.Media;
using Microsoft.VisualStudio.Shell;

namespace Sabre.WPF
{
	public class BetterTextBlock : TextBlock
	{
		public string DynamicText
		{
			get { return GetValue(m_DynamicText) as string; }
			set { SetValue(m_DynamicText, value); }
		}

		private static readonly ConcurrentDictionary<uint, SolidColorBrush> m_Brushes = new ConcurrentDictionary<uint, SolidColorBrush>();
		public static readonly DependencyProperty m_DynamicText = DependencyProperty.Register("DynamicText", typeof(string), typeof(BetterTextBlock), new UIPropertyMetadata(null, OnPropertyChanged));

		//public BetterTextBlock()
		//{
		//	IsVisibleChanged += Visible;
		//}

		//void Visible(object sender, DependencyPropertyChangedEventArgs e)
		//{
		//	if (!(bool)e.NewValue)
		//	{
		//		return;
		//	}
		//}

		private static uint GetColor(string number, int index)
		{
			uint value = 0;
			for (int i = 0; i < 8; i++)
			{
				char letter = number[index + i];
				if (letter >= '0' && letter <= '9')
				{
					value |= (uint)(letter - '0') << ((7 - i) * 4);
				}
				else
				{
					value |= ((uint)(letter - 'A') + 10) << ((7 - i) * 4);
				}
			}

			return value;
		}

		private static void OnPropertyChanged(DependencyObject sender, DependencyPropertyChangedEventArgs e)
		{
			BetterTextBlock block = sender as BetterTextBlock;

			string value = e.NewValue as string;
			if (value == null)
			{
				block.Text = null;
				block.Inlines.Clear();
				return;
			}

			int index = value.IndexOf('\0');
			if (index == -1)
			{
				block.Text = value;
				return;
			}

			int closer = 0;
			while (index != -1)
			{
				if ((closer + 1) < index)
				{
					block.Inlines.Add(new Run() { Text = value.Substring(closer + 1, index - (closer + 1)) });
				}

				uint color = GetColor(value, index + 1);

				SolidColorBrush brush;
				if (!m_Brushes.TryGetValue(color, out brush))
				{
					brush = m_Brushes.GetOrAdd(color, new SolidColorBrush(Color.FromArgb((byte)(color >> 24), (byte)(color >> 16), (byte)(color >> 8), (byte)(color >> 0))));
				}

				closer = value.IndexOf('\x1', index + 1 + 8);
				if (closer != index + 1 + 8)
				{
					block.Inlines.Add(new Run() { Text = value.Substring(index + 1 + 8, closer - (index + 1 + 8)), Foreground = brush });
				}
				
				index = value.IndexOf('\0', closer + 1);
			}

			if (closer + 1 < value.Length)
			{
				block.Inlines.Add(new Run() { Text = value.Substring(closer + 1) });
			}

			//IEnumerable<Inline> list = e.NewValue as ObservableCollection<Inline>;
			//

			//block.Inlines.Clear();
			//foreach (Inline inline in (e.NewValue as IEnumerable<Inline>))
			//{
			//	block.Inlines.Add(inline);
			//}
		}
	}
}
