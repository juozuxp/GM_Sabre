﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Sabre.Explorer.Objects
{
	public struct ExecutableString
    {
        public bool m_IsWide;
        public IntPtr m_Base;

        public string m_String;
        public ExecutableXRef[] m_XRefs;
    }
}
