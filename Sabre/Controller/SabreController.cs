using Sabre.Explorer;
using Sabre.Explorer.PE;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Sabre.Explorer.Objects;

namespace Sabre.Controller
{
	internal class SabreController
	{
		public delegate void OnLoad();
		public delegate void OnConvertPseudoC(IntPtr function);

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

		private OnConvertPseudoC m_ConvertPseudoCEvents;
		private OnLoad m_OnLoadEvents;

		private ExecutableExplorer m_Explorer;

		private string m_LoadedPath;

		private IntPtr m_PseudoCFunction;

		public static void LoadExecutable(string path)
		{
			s_Instance.m_LoadedPath = path;
			s_Instance.m_Explorer = new ExecutableExplorer(path);

			if (s_Instance.m_OnLoadEvents != null)
			{
				s_Instance.m_OnLoadEvents();
			}
		}

		public static void SetPseudoCFunction(IntPtr function)
		{
			s_Instance.m_PseudoCFunction = function;

			if (s_Instance.m_ConvertPseudoCEvents != null)
			{
				s_Instance.m_ConvertPseudoCEvents(function);
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

		public static ExecutableFunction[] GetExecutableFunctions()
		{
			if (s_Instance.m_Explorer == null)
			{
				return Array.Empty<ExecutableFunction>();
			}

			return s_Instance.m_Explorer.GetExecutableFunctions();
		}

		public static string GetPseudoC(IntPtr function)
		{
			if (s_Instance.m_Explorer == null)
			{
				return string.Empty;
			}

			return s_Instance.m_Explorer.GetPCFunction(function);
		}

		public static void AddOnLoadEvent(OnLoad function)
		{
			if (s_Instance.m_LoadedPath != null)
			{
				function();
			}

			s_Instance.m_OnLoadEvents += function;
		}

		public static void AddOnConvertCEvent(OnConvertPseudoC function)
		{
			if (s_Instance.m_PseudoCFunction != IntPtr.Zero)
			{
				function(s_Instance.m_PseudoCFunction);
			}

			s_Instance.m_ConvertPseudoCEvents += function;
		}
	}
}
