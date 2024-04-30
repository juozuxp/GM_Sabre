using System;
using System.Collections.Generic;
using System.Windows.Media;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Documents;
using Microsoft.Win32;
using EnvDTE80;
using Sabre.ListItems;
using Sabre.Dismantler.Visuals;

namespace Sabre.Dismantler
{
	public static class Visualizer
	{
		public struct Options
		{
			public byte[] m_Bytes;

			public IntPtr m_Reference;
		}

		private static readonly string[] c_InstructionName = new string[]
		{
			"invalid", "adc", "adcx", "add", "addpd", "addps", "addsd", "addss", "addsubpd", "addsubps", "adox", "aesdec", "aesdec128kl", "aesdec256kl", "aesdeclast", "aesdecwide128kl",
			"aesdecwide256kl", "aesenc", "aesenc128kl", "aesenc256kl", "aesenclast", "aesencwide128kl", "aesencwide256kl", "aesimc", "aeskeygenassist", "and", "andnpd", "andnps", "andpd", "andps", "blendpd", "blendps",
			"blendvpd", "blendvps", "bndcl", "bndcn", "bndcu", "bndldx", "bndmk", "bndmov", "bndstx", "bsf", "bsr", "bswap", "bt", "btc", "btr", "bts",
			"call", "cbw", "cdq", "cdqe", "clac", "clc", "cld", "cldemote", "clflush", "clflushopt", "cli", "clrssbsy", "clts", "clui", "clwb", "cmc",
			"cmova", "cmovae", "cmovb", "cmovbe", "cmove", "cmovg", "cmovge", "cmovl", "cmovle", "cmovne", "cmovno", "cmovnp", "cmovns", "cmovo", "cmovp", "cmovs",
			"cmp", "cmppd", "cmpps", "cmpsb", "cmpsd", "cmpsq", "cmpss", "cmpsw", "cmpxchg", "cmpxchg16b", "cmpxchg8b", "comisd", "comiss", "cpuid", "cqo", "crc32",
			"cvtdq2pd", "cvtdq2ps", "cvtpd2dq", "cvtpd2pi", "cvtpd2ps", "cvtpi2pd", "cvtpi2ps", "cvtps2dq", "cvtps2pd", "cvtps2pi", "cvtsd2si", "cvtsd2ss", "cvtsi2sd", "cvtsi2ss", "cvtss2sd", "cvtss2si",
			"cvttpd2dq", "cvttpd2pi", "cvttps2dq", "cvttps2pi", "cvttsd2si", "cvttss2si", "cwd", "cwde", "dec", "div", "divpd", "divps", "divsd", "divss", "dppd", "dpps",
			"emms", "encls", "enclu", "enclv", "encodekey128", "encodekey256", "endbr32", "endbr64", "enqcmd", "enqcmds", "enter", "extractps", "f2xm1", "fabs", "fadd", "faddp",
			"fbld", "fbstp", "fchs", "fclex", "fcmovb", "fcmovbe", "fcmove", "fcmovnb", "fcmovnbe", "fcmovne", "fcmovnu", "fcmovu", "fcom", "fcomi", "fcomip", "fcomp",
			"fcos", "fdecstp", "fdiv", "fdivp", "fdivr", "fdivrp", "ffree", "fiadd", "ficom", "ficomp", "fidiv", "fidivr", "fild", "fimul", "fincstp", "finit",
			"fist", "fistp", "fisttp", "fisub", "fisubr", "fld", "fld1", "fldcw", "fldenv", "fldl2e", "fldl2t", "fldlg2", "fldln2", "fldpi", "fldz", "fmul",
			"fmulp", "fnclex", "fninit", "fnop", "fnsave", "fnstcw", "fnstenv", "fnstsw", "fpatan", "fprem", "fprem1", "fptan", "frndint", "frstor", "fsave", "fscale",
			"fsin", "fsincos", "fsqrt", "fst", "fstcw", "fstenv", "fstp", "fstsw", "fsub", "fsubp", "fsubr", "fsubrp", "ftst", "fucom", "fucomi", "fucomip",
			"fucomp", "fucompp", "fwait", "fxam", "fxch", "fxrstor", "fxrstor64", "fxsave", "fxsave64", "fxtract", "fyl2x", "fyl2xp1", "getsec", "gf2p8affineinvqb", "gf2p8affineqb", "gf2p8mulb",
			"haddpd", "haddps", "hlt", "hreset", "hsubpd", "hsubps", "idiv", "imul", "in", "inc", "incsspd", "insb", "insd", "insertps", "insw", "int",
			"int1", "int3", "into", "invd", "invept", "invlpg", "invpcid", "invvpid", "iret", "iretd", "iretq", "ja", "jae", "jb", "jbe", "je",
			"jg", "jge", "jl", "jle", "jmp", "jne", "jno", "jns", "jo", "jpe", "jpo", "jrcxz", "js", "lar", "lddqu", "ldmxcsr",
			"lea", "leave", "lfence", "lfs", "lgdt", "lgs", "lidt", "lldt", "lmsw", "loadiwkey", "lodsb", "lodsd", "lodsq", "lodsw", "loop", "loope",
			"loopne", "lsl", "lss", "ltr", "lzcnt", "maskmovdqu", "maskmovq", "maxpd", "maxps", "maxsd", "maxss", "mfence", "minpd", "minps", "minsd", "minss",
			"monitor", "monitorx", "mov", "movapd", "movaps", "movbe", "movddup", "movdir64b", "movdiri", "movdq2q", "movdqa", "movdqu", "movhlps", "movhpd", "movhps", "movlhps",
			"movlpd", "movlps", "movmskpd", "movmskps", "movntdq", "movntdqa", "movnti", "movntpd", "movntps", "movntq", "movq", "movq2dq", "movsb", "movsd", "movshdup", "movsldup",
			"movsq", "movss", "movsw", "movsx", "movsxd", "movupd", "movups", "movzx", "mpsadbw", "mul", "mulpd", "mulps", "mulsd", "mulss", "mwait", "mwaitx",
			"neg", "nop", "not", "or", "orpd", "orps", "out", "outsb", "outsd", "outsw", "pabsb", "pabsd", "pabsw", "packssdw", "packsswb", "packusdw",
			"packuswb", "paddb", "paddd", "paddq", "paddsb", "paddsw", "paddusb", "paddusw", "paddw", "palignr", "pand", "pandn", "pause", "pavgb", "pavgw", "pblendvb",
			"pblendw", "pclmulqdq", "pcmpeqb", "pcmpeqd", "pcmpeqq", "pcmpeqw", "pcmpestri", "pcmpestrm", "pcmpgtb", "pcmpgtd", "pcmpgtq", "pcmpgtw", "pcmpistri", "pcmpistrm", "pconfig", "pextr",
			"pextrb", "pextrw", "phaddd", "phaddsw", "phaddw", "phminposuw", "phsubd", "phsubsw", "phsubw", "pinsr", "pinsrb", "pinsrw", "pmaddubsw", "pmaddwd", "pmaxsb", "pmaxsd",
			"pmaxsw", "pmaxub", "pmaxud", "pmaxuw", "pminsb", "pminsd", "pminsw", "pminub", "pminud", "pminuw", "pmovmskb", "pmovsxbd", "pmovsxbq", "pmovsxbw", "pmovsxdq", "pmovsxwd",
			"pmovsxwq", "pmovzxbd", "pmovzxbq", "pmovzxbw", "pmovzxdq", "pmovzxwd", "pmovzxwq", "pmuldq", "pmulhrsw", "pmulhuw", "pmulhw", "pmulld", "pmullw", "pmuludq", "pop", "popcnt",
			"popf", "por", "prefetchnta", "prefetcht0", "prefetcht1", "prefetcht2", "prefetchw", "prefetchwt1", "psadbw", "pshufb", "pshufd", "pshufhw", "pshuflw", "pshufw", "psignb", "psignd",
			"psignw", "pslld", "pslldq", "psllq", "psllw", "psrad", "psraw", "psrld", "psrldq", "psrlq", "psrlw", "psubb", "psubd", "psubq", "psubsb", "psubsw",
			"psubusb", "psubusw", "psubw", "ptest", "ptwrite", "punpckhbw", "punpckhdq", "punpckhqdq", "punpckhwd", "punpcklbw", "punpckldq", "punpcklqdq", "punpcklwd", "push", "pushf", "pxor",
			"rcl", "rcpps", "rcpss", "rcr", "rdfsbase", "rdgsbase", "rdmsr", "rdpid", "rdpkru", "rdpmc", "rdrand", "rdseed", "rdsspd", "rdtsc", "rdtscp", "ret",
			"rol", "ror", "roundpd", "roundps", "roundsd", "roundss", "rsm", "rsqrtps", "rsqrtss", "rstorssp", "sahf", "sar", "saveprevssp", "sbb", "scasb", "scasd",
			"scasq", "scasw", "senduipi", "serialize", "seta", "setae", "setb", "setbe", "sete", "setg", "setge", "setl", "setle", "setne", "setno", "setns",
			"seto", "setpe", "setpo", "sets", "setssbsy", "sfence", "sgdt", "sha1msg1", "sha1msg2", "sha1nexte", "sha1rnds4", "sha256msg1", "sha256msg2", "sha256rnds2", "shl", "shld",
			"shr", "shrd", "shufpd", "shufps", "sidt", "sldt", "smsw", "sqrtpd", "sqrtps", "sqrtsd", "sqrtss", "stac", "stc", "std", "sti", "stmxcsr",
			"stosb", "stosd", "stosq", "stosw", "str", "stui", "sub", "subpd", "subps", "subsd", "subss", "swapgs", "syscall", "sysenter", "sysexit", "sysret",
			"test", "testui", "tpause", "tzcnt", "ucomisd", "ucomiss", "ud0", "ud1", "ud2", "uiret", "umonitor", "umwait", "unpckhpd", "unpckhps", "unpcklpd", "unpcklps",
			"verr", "verw", "vmcall", "vmclear", "vmfunc", "vmlaunch", "vmptrld", "vmptrst", "vmread", "vmresume", "vmwrite", "vmxoff", "vmxon", "wbinvd", "wbnoinvd", "wrfsbase",
			"wrgsbase", "wrmsr", "wrpkru", "wrss", "wruss", "xabort", "xadd", "xbegin", "xchg", "xend", "xgetbv", "xlatb", "xor", "xorpd", "xorps", "xresldtrk",
			"xrstor", "xrstor64", "xrstors", "xrstors64", "xsave", "xsave64", "xsavec", "xsavec64", "xsaveopt", "xsaveopt64", "xsaves", "xsaves64", "xsetbv", "xsusldtrk", "xtest"
		};

		private static readonly string[] c_Registers = new string[]
		{
			"rax", "rcx", "rdx", "rbx", "rsp", "rbp", "rsi", "rdi", "r8", "r9", "r10", "r11", "r12", "r13", "r14", "r15", // General 64
			"eax", "ecx", "edx", "ebx", "esp", "ebp", "esi", "edi", "r8d", "r9d", "r10d", "r11d", "r12d", "r13d", "r14d", "r15d", // General 32
			"ax", "cx", "dx", "bx", "sp", "bp", "si", "di", "r8w", "r9w", "r10w", "r11w", "r12w", "r13w", "r14w", "r15w", // General 16
			"al", "cl", "dl", "bl", "spl", "bpl", "sil", "dil", "r8l", "r9l", "r10l", "r11l", "r12l", "r13l", "r14l", "r15l", // General 8 lower
			"ah", "ch", "dh", "bh", // General 8 upper
			"xmm0", "xmm1", "xmm2", "xmm3", "xmm4", "xmm5", "xmm6", "xmm7", "xmm8", "xmm9", "xmm10", "xmm11", "xmm12", "xmm13", "xmm14", "xmm15", // SSE 128 bit
			"mm0", "mm1", "mm2", "mm3", "mm4", "mm5", "mm6", "mm7", "mm8", "mm9", "mm10", "mm11", "mm12", "mm13", "mm14", "mm15", // MMX 64 bit
			"bnd0", "bnd1", "bnd2", "bnd3", // MPX 64 bit
			"st(0)", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)", // FPU 80 bit
			"cr0", "cr1", "cr2", "cr3", "cr4", "cr5", "cr6", "cr7", "cr8", "cr9", "cr10", "cr11", "cr12", "cr13", "cr14", "cr15", // Control registers
			"dr0", "dr1", "dr2", "dr3", "dr4", "dr5", "dr6", "dr7", "dr8", "dr9", "dr10", "dr11", "dr12", "dr13", "dr14", "dr15", // Debug registers
			"es", "cs", "ss", "fs", "ds", "gs" // Segments
		};

		private static readonly string[] c_MemoryScale = new string[]
		{
			"",
			"byte ptr ",
			"word ptr ",
			"dword ptr ",
			"qword ptr ",
			"tbyte ptr ",
			"oword ptr "
		};

		private static readonly string c_NumberColor = "FFFF00FF";
		private static readonly string c_RegisterColor = "FF80FF80";
		private static readonly string c_MemorySizeColor = "FFADD8E6";
		private static readonly string c_InstructionColor = "FFFFFFFF";

		public static ByteViewItem[] ToListElements(NativeVisual[] visuals, Options options)
		{
			int offset = 0;
			IntPtr address = options.m_Reference;
			IntPtr jumpable = IntPtr.Zero;

			bool operand = false;

			List<ByteViewItem> elements = new List<ByteViewItem>();

			string bytesStr = null;
			IntPtr currentAddress = IntPtr.Zero;

			StringBuilder builder = new StringBuilder();
			foreach (NativeVisual visual in visuals)
			{
				if (visual.m_Type != NativeVisual.Type.Instruction)
				{
					if (operand)
					{
						builder.Append(", ");
					}
					else
					{
						builder.Append(' ');
					}

					operand = true;
				}
				else
				{
					operand = false;
				}

				switch (visual.m_Type)
				{
					case NativeVisual.Type.Instruction:

						if (builder.Length != 0)
						{
							elements.Add(new ByteViewItem(currentAddress, ByteViewItem.Type.Code, bytesStr, builder.ToString(), jumpable));
						}

						jumpable = IntPtr.Zero;
						currentAddress = address;
						address += visual.m_Instruction.m_Size;

						builder.Clear();

						for (int i = 0; i < visual.m_Instruction.m_Size; i++)
						{
							if (i != 0)
							{
								builder.Append(' ');
							}

							builder.Append(options.m_Bytes[i + offset].ToString("X2"));
						}

						bytesStr = builder.ToString();
						offset += visual.m_Instruction.m_Size;

						builder.Clear();

						builder.Append($"\0{c_InstructionColor}{c_InstructionName[visual.m_Instruction.m_Type]}\x1");
						break;

					case NativeVisual.Type.OperandMemory:

						builder.Append($"\0{c_MemorySizeColor}{c_MemoryScale[(byte)visual.m_Memory.m_Size]}\x1");

						if (visual.m_Memory.m_Segment != NativeVisual.c_InvalidRegister)
						{
							builder.Append($"\0{c_RegisterColor}{c_Registers[visual.m_Memory.m_Segment]}\x1:[");
						}
						else
						{
							builder.Append('[');
						}

						if (visual.m_Memory.m_Base != NativeVisual.c_InvalidRegister)
						{
							builder.Append($"\0{c_RegisterColor}{c_Registers[visual.m_Memory.m_Base]}\x1");
						}

						if (visual.m_Memory.m_Index != NativeVisual.c_InvalidRegister)
						{
							if (visual.m_Memory.m_Base != NativeVisual.c_InvalidRegister)
							{
								builder.Append(" + ");
							}

							builder.Append($"\0{c_RegisterColor}{c_Registers[visual.m_Memory.m_Index]}\x1");

							if (visual.m_Memory.m_Multiplier != 1)
							{
								builder.Append($" * \0{c_NumberColor}{visual.m_Memory.m_Multiplier}\x1");
							}
						}

						if (visual.m_Memory.m_Offset != 0)
						{
							bool sign = visual.m_Memory.m_Offset < 0;
							int value = visual.m_Memory.m_Offset;

							if (visual.m_Memory.m_Base != NativeVisual.c_InvalidRegister ||
								visual.m_Memory.m_Index != NativeVisual.c_InvalidRegister)
							{
								value = sign ? -value : value;

								builder.Append(sign ? " - " : " + ");
							}

							builder.Append($"\0{c_NumberColor}{value.ToString("X8")}\x1");
						}

						builder.Append(']');
						break;

					case NativeVisual.Type.OperandRegister:

						builder.Append($"\0{c_RegisterColor}{c_Registers[visual.m_Register]}\x1");
						break;

					case NativeVisual.Type.OperandMemoryValue:

						builder.Append($"\0{c_MemorySizeColor}{c_MemoryScale[(byte)visual.m_Value.m_Size]}\x1");
						if (visual.m_Value.m_Segment != NativeVisual.c_InvalidRegister)
						{
							builder.Append($"\0{c_RegisterColor}{c_Registers[visual.m_Value.m_Segment]}\x1:");
						}
						else
						{
							jumpable = new IntPtr(visual.m_Value.m_Value);
						}

						builder.Append($"\0{c_NumberColor}{visual.m_Value.m_Value.ToString("X16")}\x1]");
						break;

					case NativeVisual.Type.OperandAddressValue:

						jumpable = new IntPtr(visual.m_Value.m_Value);
						builder.Append($"\0{c_NumberColor}{visual.m_Value.m_Value.ToString("X16")}\x1");
						break;

					case NativeVisual.Type.OperandValue:

						string size = "X";
						long local = visual.m_Value.m_Value;

						switch (visual.m_Value.m_Size)
						{
							case Scale.Scale_8:
								size = "X2";
								local &= 0xFF;
								break;

							case Scale.Scale_16:
								size = "X4";
								local &= 0xFFFF;
								break;

							case Scale.Scale_32:
								size = "X8";
								local &= 0xFFFFFFFF;
								break;

							case Scale.Scale_64:
								size = "X16";
								break;
						}

						builder.Append($"\0{c_NumberColor}{local.ToString(size)}\x1");
						break;
				}
			}

			if (builder.Length != 0)
			{
				elements.Add(new ByteViewItem(currentAddress, ByteViewItem.Type.Code, bytesStr, builder.ToString(), jumpable));
			}

			return elements.ToArray();
		}
	}
}
