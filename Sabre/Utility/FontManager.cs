using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Media;

namespace Sabre.Utility
{
	internal class FontManager
	{
		private static Dictionary<string, FontFamily> m_Fonts = new Dictionary<string, FontFamily>();

		public static FontFamily GetFont(string name)
		{
			lock (m_Fonts)
			{
				FontFamily font;
				if (m_Fonts.TryGetValue(name, out font))
				{
					return font;
				}

				string ttf = Path.Combine(Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location), "Fonts", name + ".ttf") + "#Ubuntu Mono";
				//if (!File.Exists(ttf))
				//{
				//	return null;
				//}

				font = new FontFamily(ttf);
				m_Fonts.Add(name, font);
				return font;
			}
		}
	}
}
