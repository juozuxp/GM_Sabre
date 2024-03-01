using Sabre.Explorer;
using Sabre.Explorer.PE;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Sabre.Controller
{
	internal class SabreController
	{
		public delegate void OnLoad();

		private static SabreController s_InstanceBacking = null;
		private static object s_InstanceLock = new object();

		private static SabreController s_Instance
		{
			get
			{
				if (s_InstanceBacking != null)
				{
					return s_InstanceBacking;
				}

				lock (s_InstanceLock)
				{
					if (s_InstanceBacking != null)
					{
						return s_InstanceBacking;
					}

					s_InstanceBacking = new SabreController();
				}

				return s_InstanceBacking;
			}
		}

		private ExecutableExplorer m_Explorer;
		private OnLoad m_OnLoadEvents;

		private string m_LoadedPath;

		public static void LoadExecutable(string path)
		{
			s_Instance.m_LoadedPath = path;
			s_Instance.m_Explorer = new ExecutableExplorer(path);

			if (s_Instance.m_OnLoadEvents != null)
			{
				s_Instance.m_OnLoadEvents();
			}
		}

		public static PEHeaders? GetHeaders()
		{
			if (s_Instance.m_Explorer == null)
			{
				return null;
			}

			return s_Instance.m_Explorer.GetHeaders();
		}

		public static ExecutableView? GetExecutableView()
		{
			if (s_Instance.m_Explorer == null)
			{
				return null;
			}

			return s_Instance.m_Explorer.GetExecutableView();
		}

		public static void AddOnLoadEvent(OnLoad function)
		{
			if (s_Instance.m_LoadedPath != null)
			{
				function();
			}

			s_Instance.m_OnLoadEvents += function;
		}
	}
}
