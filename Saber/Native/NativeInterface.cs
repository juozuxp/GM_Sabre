using Microsoft.VisualStudio.Text.Differencing;
using Microsoft.Win32;
using Newtonsoft.Json.Linq;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace Saber.Native
{
    internal class NativeInterface
    {
        private class Functions
        {
            [DllImport("Dismantler.dll")]
            public static extern void Test(ref InstructionArray instruction);

            [DllImport("Dismantler.dll")]
            public static extern void ManagedArray_Free(IntPtr array);
        }

        const byte IL_INVALID_REGISTER = (1 << 5) - 1;

        enum InsType : ushort
        {
            _invalid,
            _adc,
            _adcx,
            _add,
            _addpd,
            _addps,
            _addsd,
            _addss,
            _addsubpd,
            _addsubps,
            _adox,
            _aesdec,
            _aesdec128kl,
            _aesdec256kl,
            _aesdeclast,
            _aesdecwide128kl,
            _aesdecwide256kl,
            _aesenc,
            _aesenc128kl,
            _aesenc256kl,
            _aesenclast,
            _aesencwide128kl,
            _aesencwide256kl,
            _aesimc,
            _aeskeygenassist,
            _and,
            _andnpd,
            _andnps,
            _andpd,
            _andps,
            _blendpd,
            _blendps,
            _blendvpd,
            _blendvps,
            _bndcl,
            _bndcn,
            _bndcu,
            _bndldx,
            _bndmk,
            _bndmov,
            _bndstx,
            _bsf,
            _bsr,
            _bswap,
            _bt,
            _btc,
            _btr,
            _bts,
            _call,
            _cbw,
            _cdq,
            _cdqe,
            _clac,
            _clc,
            _cld,
            _cldemote,
            _clflush,
            _clflushopt,
            _cli,
            _clrssbsy,
            _clts,
            _clui,
            _clwb,
            _cmc,
            _cmova,
            _cmovae,
            _cmovb,
            _cmovbe,
            _cmove,
            _cmovg,
            _cmovge,
            _cmovl,
            _cmovle,
            _cmovne,
            _cmovno,
            _cmovnp,
            _cmovns,
            _cmovo,
            _cmovp,
            _cmovs,
            _cmp,
            _cmppd,
            _cmpps,
            _cmpsb,
            _cmpsd,
            _cmpsq,
            _cmpss,
            _cmpsw,
            _cmpxchg,
            _cmpxchg16b,
            _cmpxchg8b,
            _comisd,
            _comiss,
            _cpuid,
            _cqo,
            _crc32,
            _cvtdq2pd,
            _cvtdq2ps,
            _cvtpd2dq,
            _cvtpd2pi,
            _cvtpd2ps,
            _cvtpi2pd,
            _cvtpi2ps,
            _cvtps2dq,
            _cvtps2pd,
            _cvtps2pi,
            _cvtsd2si,
            _cvtsd2ss,
            _cvtsi2sd,
            _cvtsi2ss,
            _cvtss2sd,
            _cvtss2si,
            _cvttpd2dq,
            _cvttpd2pi,
            _cvttps2dq,
            _cvttps2pi,
            _cvttsd2si,
            _cvttss2si,
            _cwd,
            _cwde,
            _dec,
            _div,
            _divpd,
            _divps,
            _divsd,
            _divss,
            _dppd,
            _dpps,
            _emms,
            _encls,
            _enclu,
            _enclv,
            _encodekey128,
            _encodekey256,
            _endbr32,
            _endbr64,
            _enqcmd,
            _enqcmds,
            _enter,
            _extractps,
            _f2xm1,
            _fabs,
            _fadd,
            _faddp,
            _fbld,
            _fbstp,
            _fchs,
            _fclex,
            _fcmovb,
            _fcmovbe,
            _fcmove,
            _fcmovnb,
            _fcmovnbe,
            _fcmovne,
            _fcmovnu,
            _fcmovu,
            _fcom,
            _fcomi,
            _fcomip,
            _fcomp,
            _fcos,
            _fdecstp,
            _fdiv,
            _fdivp,
            _fdivr,
            _fdivrp,
            _ffree,
            _fiadd,
            _ficom,
            _ficomp,
            _fidiv,
            _fidivr,
            _fild,
            _fimul,
            _fincstp,
            _finit,
            _fist,
            _fistp,
            _fisttp,
            _fisub,
            _fisubr,
            _fld,
            _fld1,
            _fldcw,
            _fldenv,
            _fldl2e,
            _fldl2t,
            _fldlg2,
            _fldln2,
            _fldpi,
            _fldz,
            _fmul,
            _fmulp,
            _fnclex,
            _fninit,
            _fnop,
            _fnsave,
            _fnstcw,
            _fnstenv,
            _fnstsw,
            _fpatan,
            _fprem,
            _fprem1,
            _fptan,
            _frndint,
            _frstor,
            _fsave,
            _fscale,
            _fsin,
            _fsincos,
            _fsqrt,
            _fst,
            _fstcw,
            _fstenv,
            _fstp,
            _fstsw,
            _fsub,
            _fsubp,
            _fsubr,
            _fsubrp,
            _ftst,
            _fucom,
            _fucomi,
            _fucomip,
            _fucomp,
            _fucompp,
            _fwait,
            _fxam,
            _fxch,
            _fxrstor,
            _fxrstor64,
            _fxsave,
            _fxsave64,
            _fxtract,
            _fyl2x,
            _fyl2xp1,
            _getsec,
            _gf2p8affineinvqb,
            _gf2p8affineqb,
            _gf2p8mulb,
            _haddpd,
            _haddps,
            _hlt,
            _hreset,
            _hsubpd,
            _hsubps,
            _idiv,
            _imul,
            _in,
            _inc,
            _incsspd,
            _insb,
            _insd,
            _insertps,
            _insw,
            _int,
            _int1,
            _int3,
            _into,
            _invd,
            _invept,
            _invlpg,
            _invpcid,
            _invvpid,
            _iret,
            _iretd,
            _iretq,
            _ja,
            _jae,
            _jb,
            _jbe,
            _je,
            _jg,
            _jge,
            _jl,
            _jle,
            _jmp,
            _jne,
            _jno,
            _jns,
            _jo,
            _jpe,
            _jpo,
            _jrcxz,
            _js,
            _lar,
            _lddqu,
            _ldmxcsr,
            _lea,
            _leave,
            _lfence,
            _lfs,
            _lgdt,
            _lgs,
            _lidt,
            _lldt,
            _lmsw,
            _loadiwkey,
            _lodsb,
            _lodsd,
            _lodsq,
            _lodsw,
            _loop,
            _loope,
            _loopne,
            _lsl,
            _lss,
            _ltr,
            _lzcnt,
            _maskmovdqu,
            _maskmovq,
            _maxpd,
            _maxps,
            _maxsd,
            _maxss,
            _mfence,
            _minpd,
            _minps,
            _minsd,
            _minss,
            _monitor,
            _monitorx,
            _mov,
            _movapd,
            _movaps,
            _movbe,
            _movddup,
            _movdir64b,
            _movdiri,
            _movdq2q,
            _movdqa,
            _movdqu,
            _movhlps,
            _movhpd,
            _movhps,
            _movlhps,
            _movlpd,
            _movlps,
            _movmskpd,
            _movmskps,
            _movntdq,
            _movntdqa,
            _movnti,
            _movntpd,
            _movntps,
            _movntq,
            _movq,
            _movq2dq,
            _movsb,
            _movsd,
            _movshdup,
            _movsldup,
            _movsq,
            _movss,
            _movsw,
            _movsx,
            _movsxd,
            _movupd,
            _movups,
            _movzx,
            _mpsadbw,
            _mul,
            _mulpd,
            _mulps,
            _mulsd,
            _mulss,
            _mwait,
            _mwaitx,
            _neg,
            _nop,
            _not,
            _or,
            _orpd,
            _orps,
            _out,
            _outsb,
            _outsd,
            _outsw,
            _pabsb,
            _pabsd,
            _pabsw,
            _packssdw,
            _packsswb,
            _packusdw,
            _packuswb,
            _paddb,
            _paddd,
            _paddq,
            _paddsb,
            _paddsw,
            _paddusb,
            _paddusw,
            _paddw,
            _palignr,
            _pand,
            _pandn,
            _pause,
            _pavgb,
            _pavgw,
            _pblendvb,
            _pblendw,
            _pclmulqdq,
            _pcmpeqb,
            _pcmpeqd,
            _pcmpeqq,
            _pcmpeqw,
            _pcmpestri,
            _pcmpestrm,
            _pcmpgtb,
            _pcmpgtd,
            _pcmpgtq,
            _pcmpgtw,
            _pcmpistri,
            _pcmpistrm,
            _pconfig,
            _pextr,
            _pextrb,
            _pextrw,
            _phaddd,
            _phaddsw,
            _phaddw,
            _phminposuw,
            _phsubd,
            _phsubsw,
            _phsubw,
            _pinsr,
            _pinsrb,
            _pinsrw,
            _pmaddubsw,
            _pmaddwd,
            _pmaxsb,
            _pmaxsd,
            _pmaxsw,
            _pmaxub,
            _pmaxud,
            _pmaxuw,
            _pminsb,
            _pminsd,
            _pminsw,
            _pminub,
            _pminud,
            _pminuw,
            _pmovmskb,
            _pmovsxbd,
            _pmovsxbq,
            _pmovsxbw,
            _pmovsxdq,
            _pmovsxwd,
            _pmovsxwq,
            _pmovzxbd,
            _pmovzxbq,
            _pmovzxbw,
            _pmovzxdq,
            _pmovzxwd,
            _pmovzxwq,
            _pmuldq,
            _pmulhrsw,
            _pmulhuw,
            _pmulhw,
            _pmulld,
            _pmullw,
            _pmuludq,
            _pop,
            _popcnt,
            _popf,
            _por,
            _prefetchnta,
            _prefetcht0,
            _prefetcht1,
            _prefetcht2,
            _prefetchw,
            _prefetchwt1,
            _psadbw,
            _pshufb,
            _pshufd,
            _pshufhw,
            _pshuflw,
            _pshufw,
            _psignb,
            _psignd,
            _psignw,
            _pslld,
            _pslldq,
            _psllq,
            _psllw,
            _psrad,
            _psraw,
            _psrld,
            _psrldq,
            _psrlq,
            _psrlw,
            _psubb,
            _psubd,
            _psubq,
            _psubsb,
            _psubsw,
            _psubusb,
            _psubusw,
            _psubw,
            _ptest,
            _ptwrite,
            _punpckhbw,
            _punpckhdq,
            _punpckhqdq,
            _punpckhwd,
            _punpcklbw,
            _punpckldq,
            _punpcklqdq,
            _punpcklwd,
            _push,
            _pushf,
            _pxor,
            _rcl,
            _rcpps,
            _rcpss,
            _rcr,
            _rdfsbase,
            _rdgsbase,
            _rdmsr,
            _rdpid,
            _rdpkru,
            _rdpmc,
            _rdrand,
            _rdseed,
            _rdsspd,
            _rdtsc,
            _rdtscp,
            _ret,
            _rol,
            _ror,
            _roundpd,
            _roundps,
            _roundsd,
            _roundss,
            _rsm,
            _rsqrtps,
            _rsqrtss,
            _rstorssp,
            _sahf,
            _sar,
            _saveprevssp,
            _sbb,
            _scasb,
            _scasd,
            _scasq,
            _scasw,
            _senduipi,
            _serialize,
            _seta,
            _setae,
            _setb,
            _setbe,
            _sete,
            _setg,
            _setge,
            _setl,
            _setle,
            _setne,
            _setno,
            _setns,
            _seto,
            _setpe,
            _setpo,
            _sets,
            _setssbsy,
            _sfence,
            _sgdt,
            _sha1msg1,
            _sha1msg2,
            _sha1nexte,
            _sha1rnds4,
            _sha256msg1,
            _sha256msg2,
            _sha256rnds2,
            _shl,
            _shld,
            _shr,
            _shrd,
            _shufpd,
            _shufps,
            _sidt,
            _sldt,
            _smsw,
            _sqrtpd,
            _sqrtps,
            _sqrtsd,
            _sqrtss,
            _stac,
            _stc,
            _std,
            _sti,
            _stmxcsr,
            _stosb,
            _stosd,
            _stosq,
            _stosw,
            _str,
            _stui,
            _sub,
            _subpd,
            _subps,
            _subsd,
            _subss,
            _swapgs,
            _syscall,
            _sysenter,
            _sysexit,
            _sysret,
            _test,
            _testui,
            _tpause,
            _tzcnt,
            _ucomisd,
            _ucomiss,
            _ud0,
            _ud1,
            _ud2,
            _uiret,
            _umonitor,
            _umwait,
            _unpckhpd,
            _unpckhps,
            _unpcklpd,
            _unpcklps,
            _verr,
            _verw,
            _vmcall,
            _vmclear,
            _vmfunc,
            _vmlaunch,
            _vmptrld,
            _vmptrst,
            _vmread,
            _vmresume,
            _vmwrite,
            _vmxoff,
            _vmxon,
            _wbinvd,
            _wbnoinvd,
            _wrfsbase,
            _wrgsbase,
            _wrmsr,
            _wrpkru,
            _wrss,
            _wruss,
            _xabort,
            _xadd,
            _xbegin,
            _xchg,
            _xend,
            _xgetbv,
            _xlatb,
            _xor,
            _xorpd,
            _xorps,
            _xresldtrk,
            _xrstor,
            _xrstor64,
            _xrstors,
            _xrstors64,
            _xsave,
            _xsave64,
            _xsavec,
            _xsavec64,
            _xsaveopt,
            _xsaveopt64,
            _xsaves,
            _xsaves64,
            _xsetbv,
            _xsusldtrk,
            _xtest,
            _ARRAY_MAX
        };

        enum OperandType : byte
        {
            None,
            Memory,
            Register,
            Value,
            ValueRelative,
            MemoryRelative,
            MemoryAbsolute
        };

        enum MemoryScaler : byte
        {
            Scale1,
            Scale2,
            Scale4,
            Scale8
        };

        enum OperandScale : byte
        {
            Undefined,
            Scale8,
            Scale16,
            Scale32,
            Scale64,
            Scale80,
            Scale128,
            Scale256,
            Scale512
        };

        enum Register : byte
        {
            general,
            xmm,
            mm,
            bnd,
            st,
            sreg,
            cr,
            dr
        };

        [StructLayout(LayoutKind.Sequential)]
        struct InstructionArray : IDisposable
        {
            public void Dispose()
            {
                Functions.ManagedArray_Free(m_Array);
            }

            public Instruction[] ToArray()
            {
                Instruction[] array = new Instruction[m_Size];

                IntPtr address = m_Array;
                for (ulong i = 0; i < m_Size; i++, address += c_StructSize)
                {
                    array[i] = Marshal.PtrToStructure<Instruction>(address);
                }

                return array;
            }

            private readonly IntPtr m_Array;
            private readonly ulong m_Size;

            private static readonly int c_StructSize = Marshal.SizeOf(typeof(Instruction));
        }

        [StructLayout(LayoutKind.Sequential, Pack = 1)]
        struct OperandMemory
        {
            public readonly byte m_Segment; // segment index, IL_INVALID_REGISTER - reserved for none

            public readonly MemoryScaler m_Scale;

            public readonly byte m_Base; // register index, IL_INVALID_REGISTER - reserved for none
            public readonly byte m_Index; // register index, IL_INVALID_REGISTER - reserved for none

            public readonly int m_Offset;
        }

        [StructLayout(LayoutKind.Sequential, Pack = 1)]
        struct OperandRegister
        {
            private readonly byte m_BaseBaseHigh;

            public bool m_BaseHigh
            {
                get
                {
                    return (m_BaseBaseHigh >> 7) == 1;
                }
            }

            public byte m_Base
            {
                get
                {
                    return (byte)(m_BaseBaseHigh & ((1 << 7) - 1));
                }
            }

            public readonly Register m_Type;
        }

        [StructLayout(LayoutKind.Sequential, Pack = 1)]
        struct OperandMemoryValue
        {
            public readonly byte m_Segment; // segment index, IL_INVALID_REGISTER - reserved for none
            public readonly long m_Value;
        }

        [StructLayout(LayoutKind.Sequential, Pack = 1)]
        struct OperandRelative
        {
            public readonly byte m_Segment; // segment index, IL_INVALID_REGISTER - reserved for none
            public readonly int m_Value;
        }

        [StructLayout(LayoutKind.Explicit, Pack = 1)]
        struct Operand
        {
            [FieldOffset(0)]
            private readonly byte m_TypeScale;

            public OperandType m_Type 
            { 
                get 
                {
                    return (OperandType)(m_TypeScale & ((1 << 4) - 1));
                } 
            }

            public OperandScale m_Scale
            {
                get
                {
                    return (OperandScale)(m_TypeScale >> 4);
                }
            }

            [FieldOffset(1)]
            public readonly OperandMemory m_Memory; // ILOperandType_Memory

            [FieldOffset(1)]
            public readonly OperandRegister m_Register; // ILOperandType_Register

            [FieldOffset(1)]
            public readonly OperandMemoryValue m_MemoryValue; // [m_Value] if ILOperandType_MemoryAbsolute specified

            [FieldOffset(1)]
            public readonly OperandRelative m_Relative;

            [FieldOffset(1)]
            public readonly long m_Value; // ILOperandType_Value
        }

        [StructLayout(LayoutKind.Sequential, Pack = 1)]
        struct Instruction
        {
            public InsType m_Type;
            public byte m_Size;

            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 4)]
            public Operand[] m_Operands;
        }


        public static void Testing()
        {
            InstructionArray ins = new InstructionArray();

            Functions.Test(ref ins);

            Instruction[] array = ins.ToArray();

            ins.Dispose();
        }
    }
}
