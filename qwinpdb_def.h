// copyright (c) 2020 hors<horsicq@gmail.com>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
#ifndef QWINPDB_DEF
#define QWINPDB_DEF

#include <QString>
//#include "../../cpp/_lib/DIA_SDK/include/dia2.h"
#include "dia2.h"
#include "diacreate.h"
#include "cvconst.h"

#define QWINPDB_UNKNOWN "<Unknown>"

typedef struct MapIa64Reg
{
    CV_HREG_e  iCvReg;
    const char* szRegName;
} MapIa64Reg;

const char * const rgBaseType[] =
{
    "<NoType>",                         // btNoType = 0,
    "void",                             // btVoid = 1,
    "char",                             // btChar = 2,
    "wchar_t",                          // btWChar = 3,
    "signed char",
    "unsigned char",
    "int",                              // btInt = 6,
    "unsigned int",                     // btUInt = 7,
    "float",                            // btFloat = 8,
    "<BCD>",                            // btBCD = 9,
    "bool",                             // btBool = 10,
    "short",
    "unsigned short",
    "long",                             // btLong = 13,
    "unsigned long",                    // btULong = 14,
    "__int8",
    "__int16",
    "__int32",
    "__int64",
    "__int128",
    "unsigned __int8",
    "unsigned __int16",
    "unsigned __int32",
    "unsigned __int64",
    "unsigned __int128",
    "<currency>",                       // btCurrency = 25,
    "<date>",                           // btDate = 26,
    "VARIANT",                          // btVariant = 27,
    "<complex>",                        // btComplex = 28,
    "<bit>",                            // btBit = 29,
    "BSTR",                             // btBSTR = 30,
    "HRESULT"                           // btHresult = 31
};

const char * const rgTags[] =
{
    "(SymTagNull)",                     // SymTagNull
    "Executable",                       // SymTagExe
    "Compiland",                        // SymTagCompiland
    "CompilandDetails",                 // SymTagCompilandDetails
    "CompilandEnv",                     // SymTagCompilandEnv
    "Function",                         // SymTagFunction
    "Block",                            // SymTagBlock
    "Data",                             // SymTagData
    "Annotation",                       // SymTagAnnotation
    "Label",                            // SymTagLabel
    "PublicSymbol",                     // SymTagPublicSymbol
    "UserDefinedType",                  // SymTagUDT
    "Enum",                             // SymTagEnum
    "FunctionType",                     // SymTagFunctionType
    "PointerType",                      // SymTagPointerType
    "ArrayType",                        // SymTagArrayType
    "BaseType",                         // SymTagBaseType
    "Typedef",                          // SymTagTypedef
    "BaseClass",                        // SymTagBaseClass
    "Friend",                           // SymTagFriend
    "FunctionArgType",                  // SymTagFunctionArgType
    "FuncDebugStart",                   // SymTagFuncDebugStart
    "FuncDebugEnd",                     // SymTagFuncDebugEnd
    "UsingNamespace",                   // SymTagUsingNamespace
    "VTableShape",                      // SymTagVTableShape
    "VTable",                           // SymTagVTable
    "Custom",                           // SymTagCustom
    "Thunk",                            // SymTagThunk
    "CustomType",                       // SymTagCustomType
    "ManagedType",                      // SymTagManagedType
    "Dimension",                        // SymTagDimension
    "CallSite",                         // SymTagCallSite
    "InlineSite",                       // SymTagInlineSite
    "BaseInterface",                    // SymTagBaseInterface
    "VectorType",                       // SymTagVectorType
    "MatrixType",                       // SymTagMatrixType
    "HLSLType",                         // SymTagHLSLType
    "Caller",                           // SymTagCaller,
    "Callee"                            // SymTagCallee,
};

const char * const rgCallingConvention[] =
{
    "CV_CALL_NEAR_C      ",
    "CV_CALL_FAR_C       ",
    "CV_CALL_NEAR_PASCAL ",
    "CV_CALL_FAR_PASCAL  ",
    "CV_CALL_NEAR_FAST   ",
    "CV_CALL_FAR_FAST    ",
    "CV_CALL_SKIPPED     ",
    "CV_CALL_NEAR_STD    ",
    "CV_CALL_FAR_STD     ",
    "CV_CALL_NEAR_SYS    ",
    "CV_CALL_FAR_SYS     ",
    "CV_CALL_THISCALL    ",
    "CV_CALL_MIPSCALL    ",
    "CV_CALL_GENERIC     ",
    "CV_CALL_ALPHACALL   ",
    "CV_CALL_PPCCALL     ",
    "CV_CALL_SHCALL      ",
    "CV_CALL_ARMCALL     ",
    "CV_CALL_AM33CALL    ",
    "CV_CALL_TRICALL     ",
    "CV_CALL_SH5CALL     ",
    "CV_CALL_M32RCALL    ",
    "CV_ALWAYS_INLINED   ",
    "CV_CALL_NEAR_VECTOR ",
    "CV_CALL_RESERVED    "
};

const char * const rgLanguage[] =
{
    "C",                                // CV_CFL_C
    "C++",                              // CV_CFL_CXX
    "FORTRAN",                          // CV_CFL_FORTRAN
    "MASM",                             // CV_CFL_MASM
    "Pascal",                           // CV_CFL_PASCAL
    "Basic",                            // CV_CFL_BASIC
    "COBOL",                            // CV_CFL_COBOL
    "LINK",                             // CV_CFL_LINK
    "CVTRES",                           // CV_CFL_CVTRES
    "CVTPGD",                           // CV_CFL_CVTPGD
    "C#",                               // CV_CFL_CSHARP
    "Visual Basic",                     // CV_CFL_VB
    "ILASM",                            // CV_CFL_ILASM
    "Java",                             // CV_CFL_JAVA
    "JScript",                          // CV_CFL_JSCRIPT
    "MSIL",                             // CV_CFL_MSIL
    "HLSL",                             // CV_CFL_HLSL
};

const char * const rgProcessorStrings[] =
{
    "8080",                             //  CV_CFL_8080
    "8086",                             //  CV_CFL_8086
    "80286",                            //  CV_CFL_80286
    "80386",                            //  CV_CFL_80386
    "80486",                            //  CV_CFL_80486
    "Pentium",                          //  CV_CFL_PENTIUM
    "Pentium Pro/Pentium II",           //  CV_CFL_PENTIUMII/CV_CFL_PENTIUMPRO
    "Pentium III",                      //  CV_CFL_PENTIUMIII
    "???",
    "???",
    "???",
    "???",
    "???",
    "???",
    "???",
    "???",
    "MIPS (Generic)",                   //  CV_CFL_MIPSR4000
    "MIPS16",                           //  CV_CFL_MIPS16
    "MIPS32",                           //  CV_CFL_MIPS32
    "MIPS64",                           //  CV_CFL_MIPS64
    "MIPS I",                           //  CV_CFL_MIPSI
    "MIPS II",                          //  CV_CFL_MIPSII
    "MIPS III",                         //  CV_CFL_MIPSIII
    "MIPS IV",                          //  CV_CFL_MIPSIV
    "MIPS V",                           //  CV_CFL_MIPSV
    "???",
    "???",
    "???",
    "???",
    "???",
    "???",
    "???",
    "M68000",                           //  CV_CFL_M68000
    "M68010",                           //  CV_CFL_M68010
    "M68020",                           //  CV_CFL_M68020
    "M68030",                           //  CV_CFL_M68030
    "M68040",                           //  CV_CFL_M68040
    "???",
    "???",
    "???",
    "???",
    "???",
    "???",
    "???",
    "???",
    "???",
    "???",
    "???",
    "Alpha 21064",                      // CV_CFL_ALPHA, CV_CFL_ALPHA_21064
    "Alpha 21164",                      // CV_CFL_ALPHA_21164
    "Alpha 21164A",                     // CV_CFL_ALPHA_21164A
    "Alpha 21264",                      // CV_CFL_ALPHA_21264
    "Alpha 21364",                      // CV_CFL_ALPHA_21364
    "???",
    "???",
    "???",
    "???",
    "???",
    "???",
    "???",
    "???",
    "???",
    "???",
    "???",
    "PPC 601",                          // CV_CFL_PPC601
    "PPC 603",                          // CV_CFL_PPC603
    "PPC 604",                          // CV_CFL_PPC604
    "PPC 620",                          // CV_CFL_PPC620
    "PPC w/FP",                         // CV_CFL_PPCFP
    "PPC (Big Endian)",                 // CV_CFL_PPCBE
    "???",
    "???",
    "???",
    "???",
    "???",
    "???",
    "???",
    "???",
    "???",
    "???",
    "SH3",                              // CV_CFL_SH3
    "SH3E",                             // CV_CFL_SH3E
    "SH3DSP",                           // CV_CFL_SH3DSP
    "SH4",                              // CV_CFL_SH4
    "SHmedia",                          // CV_CFL_SHMEDIA
    "???",
    "???",
    "???",
    "???",
    "???",
    "???",
    "???",
    "???",
    "???",
    "???",
    "???",
    "ARM3",                             // CV_CFL_ARM3
    "ARM4",                             // CV_CFL_ARM4
    "ARM4T",                            // CV_CFL_ARM4T
    "ARM5",                             // CV_CFL_ARM5
    "ARM5T",                            // CV_CFL_ARM5T
    "ARM6",                             // CV_CFL_ARM6
    "ARM (XMAC)",                       // CV_CFL_ARM_XMAC
    "ARM (WMMX)",                       // CV_CFL_ARM_WMMX
    "???",
    "???",
    "???",
    "???",
    "???",
    "???",
    "???",
    "???",
    "Omni",                             // CV_CFL_OMNI
    "???",
    "???",
    "???",
    "???",
    "???",
    "???",
    "???",
    "???",
    "???",
    "???",
    "???",
    "???",
    "???",
    "???",
    "???",
    "Itanium",                          // CV_CFL_IA64, CV_CFL_IA64_1
    "Itanium (McKinley)",               // CV_CFL_IA64_2
    "???",
    "???",
    "???",
    "???",
    "???",
    "???",
    "???",
    "???",
    "???",
    "???",
    "???",
    "???",
    "???",
    "???",
    "CEE",                              // CV_CFL_CEE
    "???",
    "???",
    "???",
    "???",
    "???",
    "???",
    "???",
    "???",
    "???",
    "???",
    "???",
    "???",
    "???",
    "???",
    "???",
    "AM33",                             // CV_CFL_AM33
    "???",
    "???",
    "???",
    "???",
    "???",
    "???",
    "???",
    "???",
    "???",
    "???",
    "???",
    "???",
    "???",
    "???",
    "???",
    "M32R",                             // CV_CFL_M32R
    "???",
    "???",
    "???",
    "???",
    "???",
    "???",
    "???",
    "???",
    "???",
    "???",
    "???",
    "???",
    "???",
    "???",
    "???",
    "TriCore",                          // CV_CFL_TRICORE
    "???",
    "???",
    "???",
    "???",
    "???",
    "???",
    "???",
    "???",
    "???",
    "???",
    "???",
    "???",
    "???",
    "???",
    "???",
    "x64",                              // CV_CFL_X64
    "???",
    "???",
    "???",
    "???",
    "???",
    "???",
    "???",
    "???",
    "???",
    "???",
    "???",
    "???",
    "???",
    "???",
    "???",
    "EBC",                              // CV_CFL_EBC
    "???",
    "???",
    "???",
    "???",
    "???",
    "???",
    "???",
    "???",
    "???",
    "???",
    "???",
    "???",
    "???",
    "???",
    "???",
    "Thumb (CE)",                       // CV_CFL_THUMB
    "???",
    "???",
    "???",
    "ARM",                              // CV_CFL_ARMNT
    "???",
    "???",
    "???",
    "???",
    "???",
    "???",
    "???",
    "???",
    "???",
    "???",
    "???",
    "D3D11_SHADE",                      // CV_CFL_D3D11_SHADER
};

const char * const rgDataKind[] =
{
    "Unknown",
    "Local",
    "Static Local",
    "Param",
    "Object Ptr",
    "File Static",
    "Global",
    "Member",
    "Static Member",
    "Constant",
};

const char * const rgLocationTypeString[] =
{
    "NULL",
    "static",
    "TLS",
    "RegRel",
    "ThisRel",
    "Enregistered",
    "BitField",
    "Slot",
    "IL Relative",
    "In MetaData",
    "Constant"
};

const char * const rgRegX86[] =
{
    "None",         // 0   CV_REG_NONE
    "al",           // 1   CV_REG_AL
    "cl",           // 2   CV_REG_CL
    "dl",           // 3   CV_REG_DL
    "bl",           // 4   CV_REG_BL
    "ah",           // 5   CV_REG_AH
    "ch",           // 6   CV_REG_CH
    "dh",           // 7   CV_REG_DH
    "bh",           // 8   CV_REG_BH
    "ax",           // 9   CV_REG_AX
    "cx",           // 10  CV_REG_CX
    "dx",           // 11  CV_REG_DX
    "bx",           // 12  CV_REG_BX
    "sp",           // 13  CV_REG_SP
    "bp",           // 14  CV_REG_BP
    "si",           // 15  CV_REG_SI
    "di",           // 16  CV_REG_DI
    "eax",          // 17  CV_REG_EAX
    "ecx",          // 18  CV_REG_ECX
    "edx",          // 19  CV_REG_EDX
    "ebx",          // 20  CV_REG_EBX
    "esp",          // 21  CV_REG_ESP
    "ebp",          // 22  CV_REG_EBP
    "esi",          // 23  CV_REG_ESI
    "edi",          // 24  CV_REG_EDI
    "es",           // 25  CV_REG_ES
    "cs",           // 26  CV_REG_CS
    "ss",           // 27  CV_REG_SS
    "ds",           // 28  CV_REG_DS
    "fs",           // 29  CV_REG_FS
    "gs",           // 30  CV_REG_GS
    "IP",           // 31  CV_REG_IP
    "FLAGS",        // 32  CV_REG_FLAGS
    "EIP",          // 33  CV_REG_EIP
    "EFLAGS",       // 34  CV_REG_EFLAG
    "???",          // 35
    "???",          // 36
    "???",          // 37
    "???",          // 38
    "???",          // 39
    "TEMP",         // 40  CV_REG_TEMP
    "TEMPH"         // 41  CV_REG_TEMPH
    "QUOTE",        // 42  CV_REG_QUOTE
    "PCDR3",        // 43  CV_REG_PCDR3
    "PCDR4",        // 44  CV_REG_PCDR4
    "PCDR5",        // 45  CV_REG_PCDR5
    "PCDR6",        // 46  CV_REG_PCDR6
    "PCDR7",        // 47  CV_REG_PCDR7
    "???",          // 48
    "???",          // 49
    "???",          // 50
    "???",          // 51
    "???",          // 52
    "???",          // 53
    "???",          // 54
    "???",          // 55
    "???",          // 56
    "???",          // 57
    "???",          // 58
    "???",          // 59
    "???",          // 60
    "???",          // 61
    "???",          // 62
    "???",          // 63
    "???",          // 64
    "???",          // 65
    "???",          // 66
    "???",          // 67
    "???",          // 68
    "???",          // 69
    "???",          // 70
    "???",          // 71
    "???",          // 72
    "???",          // 73
    "???",          // 74
    "???",          // 75
    "???",          // 76
    "???",          // 77
    "???",          // 78
    "???",          // 79
    "cr0",          // 80  CV_REG_CR0
    "cr1",          // 81  CV_REG_CR1
    "cr2",          // 82  CV_REG_CR2
    "cr3",          // 83  CV_REG_CR3
    "cr4",          // 84  CV_REG_CR4
    "???",          // 85
    "???",          // 86
    "???",          // 87
    "???",          // 88
    "???",          // 89
    "dr0",          // 90  CV_REG_DR0
    "dr1",          // 91  CV_REG_DR1
    "dr2",          // 92  CV_REG_DR2
    "dr3",          // 93  CV_REG_DR3
    "dr4",          // 94  CV_REG_DR4
    "dr5",          // 95  CV_REG_DR5
    "dr6",          // 96  CV_REG_DR6
    "dr7",          // 97  CV_REG_DR7
    "???",          // 98
    "???",          // 99
    "???",          // 10
    "???",          // 101
    "???",          // 102
    "???",          // 103
    "???",          // 104
    "???",          // 105
    "???",          // 106
    "???",          // 107
    "???",          // 108
    "???",          // 109
    "GDTR",         // 110 CV_REG_GDTR
    "GDTL",         // 111 CV_REG_GDTL
    "IDTR",         // 112 CV_REG_IDTR
    "IDTL",         // 113 CV_REG_IDTL
    "LDTR",         // 114 CV_REG_LDTR
    "TR",           // 115 CV_REG_TR
    "???",          // 116
    "???",          // 117
    "???",          // 118
    "???",          // 119
    "???",          // 120
    "???",          // 121
    "???",          // 122
    "???",          // 123
    "???",          // 124
    "???",          // 125
    "???",          // 126
    "???",          // 127
    "st(0)",        // 128 CV_REG_ST0
    "st(1)",        // 129 CV_REG_ST1
    "st(2)",        // 130 CV_REG_ST2
    "st(3)",        // 131 CV_REG_ST3
    "st(4)",        // 132 CV_REG_ST4
    "st(5)",        // 133 CV_REG_ST5
    "st(6)",        // 134 CV_REG_ST6
    "st(7)",        // 135 CV_REG_ST7
    "CTRL",         // 136 CV_REG_CTRL
    "STAT",         // 137 CV_REG_STAT
    "TAG",          // 138 CV_REG_TAG
    "FPIP",         // 139 CV_REG_FPIP
    "FPCS",         // 140 CV_REG_FPCS
    "FPDO",         // 141 CV_REG_FPDO
    "FPDS",         // 142 CV_REG_FPDS
    "ISEM",         // 143 CV_REG_ISEM
    "FPEIP",        // 144 CV_REG_FPEIP
    "FPED0"         // 145 CV_REG_FPEDO
};

const char * const rgRegAMD64[] =
{
    "None",         // 0   CV_REG_NONE
    "al",           // 1   CV_AMD64_AL
    "cl",           // 2   CV_AMD64_CL
    "dl",           // 3   CV_AMD64_DL
    "bl",           // 4   CV_AMD64_BL
    "ah",           // 5   CV_AMD64_AH
    "ch",           // 6   CV_AMD64_CH
    "dh",           // 7   CV_AMD64_DH
    "bh",           // 8   CV_AMD64_BH
    "ax",           // 9   CV_AMD64_AX
    "cx",           // 10  CV_AMD64_CX
    "dx",           // 11  CV_AMD64_DX
    "bx",           // 12  CV_AMD64_BX
    "sp",           // 13  CV_AMD64_SP
    "bp",           // 14  CV_AMD64_BP
    "si",           // 15  CV_AMD64_SI
    "di",           // 16  CV_AMD64_DI
    "eax",          // 17  CV_AMD64_EAX
    "ecx",          // 18  CV_AMD64_ECX
    "edx",          // 19  CV_AMD64_EDX
    "ebx",          // 20  CV_AMD64_EBX
    "esp",          // 21  CV_AMD64_ESP
    "ebp",          // 22  CV_AMD64_EBP
    "esi",          // 23  CV_AMD64_ESI
    "edi",          // 24  CV_AMD64_EDI
    "es",           // 25  CV_AMD64_ES
    "cs",           // 26  CV_AMD64_CS
    "ss",           // 27  CV_AMD64_SS
    "ds",           // 28  CV_AMD64_DS
    "fs",           // 29  CV_AMD64_FS
    "gs",           // 30  CV_AMD64_GS
    "???",          // 31  Not filled up
    "flags",        // 32  CV_AMD64_FLAGS
    "rip",          // 33  CV_AMD64_RIP
    "eflags",       // 34  CV_AMD64_EFLAGS
    "???",          // 35
    "???",          // 36
    "???",          // 37
    "???",          // 38
    "???",          // 39
    "???",          // 40
    "???",          // 41
    "???",          // 42
    "???",          // 43
    "???",          // 44
    "???",          // 45
    "???",          // 46
    "???",          // 47
    "???",          // 48
    "???",          // 49
    "???",          // 50
    "???",          // 51
    "???",          // 52
    "???",          // 53
    "???",          // 54
    "???",          // 55
    "???",          // 56
    "???",          // 57
    "???",          // 58
    "???",          // 59
    "???",          // 60
    "???",          // 61
    "???",          // 62
    "???",          // 63
    "???",          // 64
    "???",          // 65
    "???",          // 66
    "???",          // 67
    "???",          // 68
    "???",          // 69
    "???",          // 70
    "???",          // 71
    "???",          // 72
    "???",          // 73
    "???",          // 74
    "???",          // 75
    "???",          // 76
    "???",          // 77
    "???",          // 78
    "???",          // 79
    "cr0",          // 80  CV_AMD64_CR0
    "cr1",          // 81  CV_AMD64_CR1
    "cr2",          // 82  CV_AMD64_CR2
    "cr3",          // 83  CV_AMD64_CR3
    "cr4",          // 84  CV_AMD64_CR4
    "???",          // 85
    "???",          // 86
    "???",          // 87
    "cr8",          // 88  CV_AMD64_CR8
    "???",          // 89
    "dr0",          // 90  CV_AMD64_DR0
    "dr1",          // 91  CV_AMD64_DR1
    "dr2",          // 92  CV_AMD64_DR2
    "dr3",          // 93  CV_AMD64_DR3
    "dr4",          // 94  CV_AMD64_DR4
    "dr5",          // 95  CV_AMD64_DR5
    "dr6",          // 96  CV_AMD64_DR6
    "dr7",          // 97  CV_AMD64_DR7
    "dr8",          // 98  CV_AMD64_DR8
    "dr9",          // 99  CV_AMD64_DR9
    "dr10",         // 100 CV_AMD64_DR10
    "dr11",         // 101 CV_AMD64_DR11
    "dr12",         // 102 CV_AMD64_DR12
    "dr13",         // 103 CV_AMD64_DR13
    "dr14",         // 104 CV_AMD64_DR14
    "dr15",         // 105 CV_AMD64_DR15
    "???",          // 106
    "???",          // 107
    "???",          // 108
    "???",          // 109
    "gdtr",         // 110 CV_AMD64_GDTR
    "gdt",         // 111 CV_AMD64_GDTL
    "idtr",         // 112 CV_AMD64_IDTR
    "idt",         // 113 CV_AMD64_IDTL
    "ldtr",         // 114 CV_AMD64_LDTR
    "tr",           // 115 CV_AMD64_TR
    "???",          // 116
    "???",          // 117
    "???",          // 118
    "???",          // 119
    "???",          // 120
    "???",          // 121
    "???",          // 122
    "???",          // 123
    "???",          // 124
    "???",          // 125
    "???",          // 126
    "???",          // 127
    "st(0)",        // 128 CV_AMD64_ST0
    "st(1)",        // 129 CV_AMD64_ST1
    "st(2)",        // 130 CV_AMD64_ST2
    "st(3)",        // 131 CV_AMD64_ST3
    "st(4)",        // 132 CV_AMD64_ST4
    "st(5)",        // 133 CV_AMD64_ST5
    "st(6)",        // 134 CV_AMD64_ST6
    "st(7)",        // 135 CV_AMD64_ST7
    "ctr",         // 136 CV_AMD64_CTRL
    "stat",         // 137 CV_AMD64_STAT
    "tag",          // 138 CV_AMD64_TAG
    "fpip",         // 139 CV_AMD64_FPIP
    "fpcs",         // 140 CV_AMD64_FPCS
    "fpdo",         // 141 CV_AMD64_FPDO
    "fpds",         // 142 CV_AMD64_FPDS
    "isem",         // 143 CV_AMD64_ISEM
    "fpeip",        // 144 CV_AMD64_FPEIP
    "fped0",        // 145 CV_AMD64_FPEDO
    "mm0",          // 146 CV_AMD64_MM0
    "mm1",          // 147 CV_AMD64_MM1
    "mm2",          // 148 CV_AMD64_MM2
    "mm3",          // 149 CV_AMD64_MM3
    "mm4",          // 150 CV_AMD64_MM4
    "mm5",          // 151 CV_AMD64_MM5
    "mm6",          // 152 CV_AMD64_MM6
    "mm7",          // 153 CV_AMD64_MM7
    "xmm0",         // 154 CV_AMD64_XMM0
    "xmm1",         // 155 CV_AMD64_XMM1
    "xmm2",         // 156 CV_AMD64_XMM2
    "xmm3",         // 157 CV_AMD64_XMM3
    "xmm4",         // 158 CV_AMD64_XMM4
    "xmm5",         // 159 CV_AMD64_XMM5
    "xmm6",         // 160 CV_AMD64_XMM6
    "xmm7",         // 161 CV_AMD64_XMM7
    "xmm0_0",       // 162 CV_AMD64_XMM0_0
    "xmm0_1",       // 163 CV_AMD64_XMM0_1
    "xmm0_2",       // 164 CV_AMD64_XMM0_2
    "xmm0_3",       // 165 CV_AMD64_XMM0_3
    "xmm1_0",       // 166 CV_AMD64_XMM1_0
    "xmm1_1",       // 167 CV_AMD64_XMM1_1
    "xmm1_2",       // 168 CV_AMD64_XMM1_2
    "xmm1_3",       // 169 CV_AMD64_XMM1_3
    "xmm2_0",       // 170 CV_AMD64_XMM2_0
    "xmm2_1",       // 171 CV_AMD64_XMM2_1
    "xmm2_2",       // 172 CV_AMD64_XMM2_2
    "xmm2_3",       // 173 CV_AMD64_XMM2_3
    "xmm3_0",       // 174 CV_AMD64_XMM3_0
    "xmm3_1",       // 175 CV_AMD64_XMM3_1
    "xmm3_2",       // 176 CV_AMD64_XMM3_2
    "xmm3_3",       // 177 CV_AMD64_XMM3_3
    "xmm4_0",       // 178 CV_AMD64_XMM4_0
    "xmm4_1",       // 179 CV_AMD64_XMM4_1
    "xmm4_2",       // 180 CV_AMD64_XMM4_2
    "xmm4_3",       // 181 CV_AMD64_XMM4_3
    "xmm5_0",       // 182 CV_AMD64_XMM5_0
    "xmm5_1",       // 183 CV_AMD64_XMM5_1
    "xmm5_2",       // 184 CV_AMD64_XMM5_2
    "xmm5_3",       // 185 CV_AMD64_XMM5_3
    "xmm6_0",       // 186 CV_AMD64_XMM6_0
    "xmm6_1",       // 187 CV_AMD64_XMM6_1
    "xmm6_2",       // 188 CV_AMD64_XMM6_2
    "xmm6_3",       // 189 CV_AMD64_XMM6_3
    "xmm7_0",       // 190 CV_AMD64_XMM7_0
    "xmm7_1",       // 191 CV_AMD64_XMM7_1
    "xmm7_2",       // 192 CV_AMD64_XMM7_2
    "xmm7_3",       // 193 CV_AMD64_XMM7_3
    "xmm0",        // 194 CV_AMD64_XMM0L
    "xmm1",        // 195 CV_AMD64_XMM1L
    "xmm2",        // 196 CV_AMD64_XMM2L
    "xmm3",        // 197 CV_AMD64_XMM3L
    "xmm4",        // 198 CV_AMD64_XMM4L
    "xmm5",        // 199 CV_AMD64_XMM5L
    "xmm6",        // 200 CV_AMD64_XMM6L
    "xmm7",        // 201 CV_AMD64_XMM7L
    "xmm0h",        // 202 CV_AMD64_XMM0H
    "xmm1h",        // 203 CV_AMD64_XMM1H
    "xmm2h",        // 204 CV_AMD64_XMM2H
    "xmm3h",        // 205 CV_AMD64_XMM3H
    "xmm4h",        // 206 CV_AMD64_XMM4H
    "xmm5h",        // 207 CV_AMD64_XMM5H
    "xmm6h",        // 208 CV_AMD64_XMM6H
    "xmm7h",        // 209 CV_AMD64_XMM7H
    "???",          // 210
    "mxcsr",        // 211 CV_AMD64_MXCSR
    "???",          // 212
    "???",          // 213
    "???",          // 214
    "???",          // 215
    "???",          // 216
    "???",          // 217
    "???",          // 218
    "???",          // 219
    "emm0",        // 220 CV_AMD64_EMM0L
    "emm1",        // 221 CV_AMD64_EMM1L
    "emm2",        // 222 CV_AMD64_EMM2L
    "emm3",        // 223 CV_AMD64_EMM3L
    "emm4",        // 224 CV_AMD64_EMM4L
    "emm5",        // 225 CV_AMD64_EMM5L
    "emm6",        // 226 CV_AMD64_EMM6L
    "emm7",        // 227 CV_AMD64_EMM7L
    "emm0h",        // 228 CV_AMD64_EMM0H
    "emm1h",        // 229 CV_AMD64_EMM1H
    "emm2h",        // 230 CV_AMD64_EMM2H
    "emm3h",        // 231 CV_AMD64_EMM3H
    "emm4h",        // 232 CV_AMD64_EMM4H
    "emm5h",        // 233 CV_AMD64_EMM5H
    "emm6h",        // 234 CV_AMD64_EMM6H
    "emm7h",        // 235 CV_AMD64_EMM7H
    "mm00",         // 236 CV_AMD64_MM00
    "mm01",         // 237 CV_AMD64_MM01
    "mm10",         // 238 CV_AMD64_MM10
    "mm11",         // 239 CV_AMD64_MM11
    "mm20",         // 240 CV_AMD64_MM20
    "mm21",         // 241 CV_AMD64_MM21
    "mm30",         // 242 CV_AMD64_MM30
    "mm31",         // 243 CV_AMD64_MM31
    "mm40",         // 244 CV_AMD64_MM40
    "mm41",         // 245 CV_AMD64_MM41
    "mm50",         // 246 CV_AMD64_MM50
    "mm51",         // 247 CV_AMD64_MM51
    "mm60",         // 248 CV_AMD64_MM60
    "mm61",         // 249 CV_AMD64_MM61
    "mm70",         // 250 CV_AMD64_MM70
    "mm71",         // 251 CV_AMD64_MM71
    "xmm8",         // 252 CV_AMD64_XMM8
    "xmm9",         // 253 CV_AMD64_XMM9
    "xmm10",        // 254 CV_AMD64_XMM10
    "xmm11",        // 255 CV_AMD64_XMM11
    "xmm12",        // 256 CV_AMD64_XMM12
    "xmm13",        // 257 CV_AMD64_XMM13
    "xmm14",        // 258 CV_AMD64_XMM14
    "xmm15",        // 259 CV_AMD64_XMM15
    "xmm8_0",       // 260 CV_AMD64_XMM8_0
    "xmm8_1",       // 261 CV_AMD64_XMM8_1
    "xmm8_2",       // 262 CV_AMD64_XMM8_2
    "xmm8_3",       // 263 CV_AMD64_XMM8_3
    "xmm9_0",       // 264 CV_AMD64_XMM9_0
    "xmm9_1",       // 265 CV_AMD64_XMM9_1
    "xmm9_2",       // 266 CV_AMD64_XMM9_2
    "xmm9_3",       // 267 CV_AMD64_XMM9_3
    "xmm10_0",      // 268 CV_AMD64_XMM10_0
    "xmm10_1",      // 269 CV_AMD64_XMM10_1
    "xmm10_2",      // 270 CV_AMD64_XMM10_2
    "xmm10_3",      // 271 CV_AMD64_XMM10_3
    "xmm11_0",      // 272 CV_AMD64_XMM11_0
    "xmm11_1",      // 273 CV_AMD64_XMM11_1
    "xmm11_2",      // 274 CV_AMD64_XMM11_2
    "xmm11_3",      // 275 CV_AMD64_XMM11_3
    "xmm12_0",      // 276 CV_AMD64_XMM12_0
    "xmm12_1",      // 277 CV_AMD64_XMM12_1
    "xmm12_2",      // 278 CV_AMD64_XMM12_2
    "xmm12_3",      // 279 CV_AMD64_XMM12_3
    "xmm13_0",      // 280 CV_AMD64_XMM13_0
    "xmm13_1",      // 281 CV_AMD64_XMM13_1
    "xmm13_2",      // 282 CV_AMD64_XMM13_2
    "xmm13_3",      // 283 CV_AMD64_XMM13_3
    "xmm14_0",      // 284 CV_AMD64_XMM14_0
    "xmm14_1",      // 285 CV_AMD64_XMM14_1
    "xmm14_2",      // 286 CV_AMD64_XMM14_2
    "xmm14_3",      // 287 CV_AMD64_XMM14_3
    "xmm15_0",      // 288 CV_AMD64_XMM15_0
    "xmm15_1",      // 289 CV_AMD64_XMM15_1
    "xmm15_2",      // 290 CV_AMD64_XMM15_2
    "xmm15_3",      // 291 CV_AMD64_XMM15_3
    "xmm8",        // 292 CV_AMD64_XMM8L
    "xmm9",        // 293 CV_AMD64_XMM9L
    "xmm10",       // 294 CV_AMD64_XMM10L
    "xmm11",       // 295 CV_AMD64_XMM11L
    "xmm12",       // 296 CV_AMD64_XMM12L
    "xmm13",       // 297 CV_AMD64_XMM13L
    "xmm14",       // 298 CV_AMD64_XMM14L
    "xmm15",       // 299 CV_AMD64_XMM15L
    "xmm8h",        // 300 CV_AMD64_XMM8H
    "xmm9h",        // 301 CV_AMD64_XMM9H
    "xmm10h",       // 302 CV_AMD64_XMM10H
    "xmm11h",       // 303 CV_AMD64_XMM11H
    "xmm12h",       // 304 CV_AMD64_XMM12H
    "xmm13h",       // 305 CV_AMD64_XMM13H
    "xmm14h",       // 306 CV_AMD64_XMM14H
    "xmm15h",       // 307 CV_AMD64_XMM15H
    "emm8",        // 308 CV_AMD64_EMM8L
    "emm9",        // 309 CV_AMD64_EMM9L
    "emm10",       // 310 CV_AMD64_EMM10L
    "emm11",       // 311 CV_AMD64_EMM11L
    "emm12",       // 312 CV_AMD64_EMM12L
    "emm13",       // 313 CV_AMD64_EMM13L
    "emm14",       // 314 CV_AMD64_EMM14L
    "emm15",       // 315 CV_AMD64_EMM15L
    "emm8h",        // 316 CV_AMD64_EMM8H
    "emm9h",        // 317 CV_AMD64_EMM9H
    "emm10h",       // 318 CV_AMD64_EMM10H
    "emm11h",       // 319 CV_AMD64_EMM11H
    "emm12h",       // 320 CV_AMD64_EMM12H
    "emm13h",       // 321 CV_AMD64_EMM13H
    "emm14h",       // 322 CV_AMD64_EMM14H
    "emm15h",       // 323 CV_AMD64_EMM15H
    "si",          // 324 CV_AMD64_SIL
    "di",          // 325 CV_AMD64_DIL
    "bp",          // 326 CV_AMD64_BPL
    "sp",          // 327 CV_AMD64_SPL
    "rax",          // 328 CV_AMD64_RAX
    "rbx",          // 329 CV_AMD64_RBX
    "rcx",          // 330 CV_AMD64_RCX
    "rdx",          // 331 CV_AMD64_RDX
    "rsi",          // 332 CV_AMD64_RSI
    "rdi",          // 333 CV_AMD64_RDI
    "rbp",          // 334 CV_AMD64_RBP
    "rsp",          // 335 CV_AMD64_RSP
    "r8",           // 336 CV_AMD64_R8
    "r9",           // 337 CV_AMD64_R9
    "r10",          // 338 CV_AMD64_R10
    "r11",          // 339 CV_AMD64_R11
    "r12",          // 340 CV_AMD64_R12
    "r13",          // 341 CV_AMD64_R13
    "r14",          // 342 CV_AMD64_R14
    "r15",          // 343 CV_AMD64_R15
    "r8b",          // 344 CV_AMD64_R8B
    "r9b",          // 345 CV_AMD64_R9B
    "r10b",         // 346 CV_AMD64_R10B
    "r11b",         // 347 CV_AMD64_R11B
    "r12b",         // 348 CV_AMD64_R12B
    "r13b",         // 349 CV_AMD64_R13B
    "r14b",         // 350 CV_AMD64_R14B
    "r15b",         // 351 CV_AMD64_R15B
    "r8w",          // 352 CV_AMD64_R8W
    "r9w",          // 353 CV_AMD64_R9W
    "r10w",         // 354 CV_AMD64_R10W
    "r11w",         // 355 CV_AMD64_R11W
    "r12w",         // 356 CV_AMD64_R12W
    "r13w",         // 357 CV_AMD64_R13W
    "r14w",         // 358 CV_AMD64_R14W
    "r15w",         // 359 CV_AMD64_R15W
    "r8d",          // 360 CV_AMD64_R8D
    "r9d",          // 361 CV_AMD64_R9D
    "r10d",         // 362 CV_AMD64_R10D
    "r11d",         // 363 CV_AMD64_R11D
    "r12d",         // 364 CV_AMD64_R12D
    "r13d",         // 365 CV_AMD64_R13D
    "r14d",         // 366 CV_AMD64_R14D
    "r15d"          // 367 CV_AMD64_R15D
};

const char * const rgRegMips[] =
{
    "None",         // 0   CV_M4_NOREG
    "???",          // 1
    "???",          // 2
    "???",          // 3
    "???",          // 4
    "???",          // 5
    "???",          // 6
    "???",          // 7
    "???",          // 8
    "???",          // 9
    "zero",         // 10  CV_M4_IntZERO
    "at",           // 11  CV_M4_IntAT
    "v0",           // 12  CV_M4_IntV0
    "v1",           // 13  CV_M4_IntV1
    "a0",           // 14  CV_M4_IntA0
    "a1",           // 15  CV_M4_IntA1
    "a2",           // 16  CV_M4_IntA2
    "a3",           // 17  CV_M4_IntA3
    "t0",           // 18  CV_M4_IntT0
    "t1",           // 19  CV_M4_IntT1
    "t2",           // 20  CV_M4_IntT2
    "t3",           // 21  CV_M4_IntT3
    "t4",           // 22  CV_M4_IntT4
    "t5",           // 23  CV_M4_IntT5
    "t6",           // 24  CV_M4_IntT6
    "t7",           // 25  CV_M4_IntT7
    "s0",           // 26  CV_M4_IntS0
    "s1",           // 27  CV_M4_IntS1
    "s2",           // 28  CV_M4_IntS2
    "s3",           // 29  CV_M4_IntS3
    "s4",           // 30  CV_M4_IntS4
    "s5",           // 31  CV_M4_IntS5
    "s6",           // 32  CV_M4_IntS6
    "s7",           // 33  CV_M4_IntS7
    "t8",           // 34  CV_M4_IntT8
    "t9",           // 35  CV_M4_IntT9
    "k0",           // 36  CV_M4_IntKT0
    "k1",           // 37  CV_M4_IntKT1
    "gp",           // 38  CV_M4_IntGP
    "sp",           // 39  CV_M4_IntSP
    "s8",           // 40  CV_M4_IntS8
    "ra",           // 41  CV_M4_IntRA
    "lo",           // 42  CV_M4_IntLO
    "hi",           // 43  CV_M4_IntHI
    "???",          // 44
    "???",          // 45
    "???",          // 46
    "???",          // 47
    "???",          // 48
    "???",          // 49
    "Fir",          // 50  CV_M4_Fir
    "Psr",          // 51  CV_M4_Psr
    "???",          // 52
    "???",          // 53
    "???",          // 54
    "???",          // 55
    "???",          // 56
    "???",          // 57
    "???",          // 58
    "???",          // 59
    "$f0",          // 60  CV_M4_FltF0
    "$f1",          // 61  CV_M4_FltF1
    "$f2",          // 62  CV_M4_FltF2
    "$f3",          // 63  CV_M4_FltF3
    "$f4",          // 64  CV_M4_FltF4
    "$f5",          // 65  CV_M4_FltF5
    "$f6",          // 66  CV_M4_FltF6
    "$f7",          // 67  CV_M4_FltF7
    "$f8",          // 68  CV_M4_FltF8
    "$f9",          // 69  CV_M4_FltF9
    "$f10",         // 70  CV_M4_FltF10
    "$f11",         // 71  CV_M4_FltF11
    "$f12",         // 72  CV_M4_FltF12
    "$f13",         // 73  CV_M4_FltF13
    "$f14",         // 74  CV_M4_FltF14
    "$f15",         // 75  CV_M4_FltF15
    "$f16",         // 76  CV_M4_FltF16
    "$f17",         // 77  CV_M4_FltF17
    "$f18",         // 78  CV_M4_FltF18
    "$f19",         // 79  CV_M4_FltF19
    "$f20",         // 80  CV_M4_FltF20
    "$f21",         // 81  CV_M4_FltF21
    "$f22",         // 82  CV_M4_FltF22
    "$f23",         // 83  CV_M4_FltF23
    "$f24",         // 84  CV_M4_FltF24
    "$f25",         // 85  CV_M4_FltF25
    "$f26",         // 86  CV_M4_FltF26
    "$f27",         // 87  CV_M4_FltF27
    "$f28",         // 88  CV_M4_FltF28
    "$f29",         // 89  CV_M4_FltF29
    "$f30",         // 90  CV_M4_FltF30
    "$f31",         // 91  CV_M4_FltF31
    "Fsr"           // 92  CV_M4_FltFsr
};

const char * const rgReg68k[] =
{
    "D0",           // 0   CV_R68_D0
    "D1",           // 1   CV_R68_D1
    "D2",           // 2   CV_R68_D2
    "D3",           // 3   CV_R68_D3
    "D4",           // 4   CV_R68_D4
    "D5",           // 5   CV_R68_D5
    "D6",           // 6   CV_R68_D6
    "D7",           // 7   CV_R68_D7
    "A0",           // 8   CV_R68_A0
    "A1",           // 9   CV_R68_A1
    "A2",           // 10  CV_R68_A2
    "A3",           // 11  CV_R68_A3
    "A4",           // 12  CV_R68_A4
    "A5",           // 13  CV_R68_A5
    "A6",           // 14  CV_R68_A6
    "A7",           // 15  CV_R68_A7
    "CCR",          // 16  CV_R68_CCR
    "SR",           // 17  CV_R68_SR
    "USP",          // 18  CV_R68_USP
    "MSP",          // 19  CV_R68_MSP
    "SFC",          // 20  CV_R68_SFC
    "DFC",          // 21  CV_R68_DFC
    "CACR",         // 22  CV_R68_CACR
    "VBR",          // 23  CV_R68_VBR
    "CAAR",         // 24  CV_R68_CAAR
    "ISP",          // 25  CV_R68_ISP
    "PC",           // 26  CV_R68_PC
    "???",          // 27
    "FPCR",         // 28  CV_R68_FPCR
    "FPSR",         // 29  CV_R68_FPSR
    "FPIAR",        // 30  CV_R68_FPIAR
    "???",          // 31
    "FP0",          // 32  CV_R68_FP0
    "FP1",          // 33  CV_R68_FP1
    "FP2",          // 34  CV_R68_FP2
    "FP3",          // 35  CV_R68_FP3
    "FP4",          // 36  CV_R68_FP4
    "FP5",          // 37  CV_R68_FP5
    "FP6",          // 38  CV_R68_FP6
    "FP7",          // 39  CV_R68_FP7
    "???",          // 40
    "???",          // 41  CV_R68_MMUSR030
    "???",          // 42  CV_R68_MMUSR
    "???",          // 43  CV_R68_URP
    "???",          // 44  CV_R68_DTT0
    "???",          // 45  CV_R68_DTT1
    "???",          // 46  CV_R68_ITT0
    "???",          // 47  CV_R68_ITT1
    "???",          // 48
    "???",          // 49
    "???",          // 50
    "PSR",          // 51  CV_R68_PSR
    "PCSR",         // 52  CV_R68_PCSR
    "VAL",          // 53  CV_R68_VAL
    "CRP",          // 54  CV_R68_CRP
    "SRP",          // 55  CV_R68_SRP
    "DRP",          // 56  CV_R68_DRP
    "TC",           // 57  CV_R68_TC
    "AC",           // 58  CV_R68_AC
    "SCC",          // 59  CV_R68_SCC
    "CAL",          // 60  CV_R68_CAL
    "TT0",          // 61  CV_R68_TT0
    "TT1",          // 62  CV_R68_TT1
    "???",          // 63
    "BAD0",         // 64  CV_R68_BAD0
    "BAD1",         // 65  CV_R68_BAD1
    "BAD2",         // 66  CV_R68_BAD2
    "BAD3",         // 67  CV_R68_BAD3
    "BAD4",         // 68  CV_R68_BAD4
    "BAD5",         // 69  CV_R68_BAD5
    "BAD6",         // 70  CV_R68_BAD6
    "BAD7",         // 71  CV_R68_BAD7
    "BAC0",         // 72  CV_R68_BAC0
    "BAC1",         // 73  CV_R68_BAC1
    "BAC2",         // 74  CV_R68_BAC2
    "BAC3",         // 75  CV_R68_BAC3
    "BAC4",         // 76  CV_R68_BAC4
    "BAC5",         // 77  CV_R68_BAC5
    "BAC6",         // 78  CV_R68_BAC6
    "BAC7"          // 79  CV_R68_BAC7
};

const char * const rgRegAlpha[] =
{
    "None",         // 0   CV_ALPHA_NOREG
    "???",          // 1
    "???",          // 2
    "???",          // 3
    "???",          // 4
    "???",          // 5
    "???",          // 6
    "???",          // 7
    "???",          // 8
    "???",          // 9
    "$f0",          // 10  CV_ALPHA_FltF0
    "$f1",          // 11  CV_ALPHA_FltF1
    "$f2",          // 12  CV_ALPHA_FltF2
    "$f3",          // 13  CV_ALPHA_FltF3
    "$f4",          // 14  CV_ALPHA_FltF4
    "$f5",          // 15  CV_ALPHA_FltF5
    "$f6",          // 16  CV_ALPHA_FltF6
    "$f7",          // 17  CV_ALPHA_FltF7
    "$f8",          // 18  CV_ALPHA_FltF8
    "$f9",          // 19  CV_ALPHA_FltF9
    "$f10",         // 20  CV_ALPHA_FltF10
    "$f11",         // 21  CV_ALPHA_FltF11
    "$f12",         // 22  CV_ALPHA_FltF12
    "$f13",         // 23  CV_ALPHA_FltF13
    "$f14",         // 24  CV_ALPHA_FltF14
    "$f15",         // 25  CV_ALPHA_FltF15
    "$f16",         // 26  CV_ALPHA_FltF16
    "$f17",         // 27  CV_ALPHA_FltF17
    "$f18",         // 28  CV_ALPHA_FltF18
    "$f19",         // 29  CV_ALPHA_FltF19
    "$f20",         // 30  CV_ALPHA_FltF20
    "$f21",         // 31  CV_ALPHA_FltF21
    "$f22",         // 32  CV_ALPHA_FltF22
    "$f23",         // 33  CV_ALPHA_FltF23
    "$f24",         // 34  CV_ALPHA_FltF24
    "$f25",         // 35  CV_ALPHA_FltF25
    "$f26",         // 36  CV_ALPHA_FltF26
    "$f27",         // 37  CV_ALPHA_FltF27
    "$f28",         // 38  CV_ALPHA_FltF28
    "$f29",         // 39  CV_ALPHA_FltF29
    "$f30",         // 40  CV_ALPHA_FltF30
    "$f31",         // 41  CV_ALPHA_FltF31
    "v0",           // 42  CV_ALPHA_IntV0
    "t0",           // 43  CV_ALPHA_IntT0
    "t1",           // 44  CV_ALPHA_IntT1
    "t2",           // 45  CV_ALPHA_IntT2
    "t3",           // 46  CV_ALPHA_IntT3
    "t4",           // 47  CV_ALPHA_IntT4
    "t5",           // 48  CV_ALPHA_IntT5
    "t6",           // 49  CV_ALPHA_IntT6
    "t7",           // 50  CV_ALPHA_IntT7
    "s0",           // 51  CV_ALPHA_IntS0
    "s1",           // 52  CV_ALPHA_IntS1
    "s2",           // 53  CV_ALPHA_IntS2
    "s3",           // 54  CV_ALPHA_IntS3
    "s4",           // 55  CV_ALPHA_IntS4
    "s5",           // 56  CV_ALPHA_IntS5
    "fp",           // 57  CV_ALPHA_IntFP
    "a0",           // 58  CV_ALPHA_IntA0
    "a1",           // 59  CV_ALPHA_IntA1
    "a2",           // 60  CV_ALPHA_IntA2
    "a3",           // 61  CV_ALPHA_IntA3
    "a4",           // 62  CV_ALPHA_IntA4
    "a5",           // 63  CV_ALPHA_IntA5
    "t8",           // 64  CV_ALPHA_IntT8
    "t9",           // 65  CV_ALPHA_IntT9
    "t10",          // 66  CV_ALPHA_IntT10
    "t11",          // 67  CV_ALPHA_IntT11
    "ra",           // 68  CV_ALPHA_IntRA
    "t12",          // 69  CV_ALPHA_IntT12
    "at",           // 70  CV_ALPHA_IntAT
    "gp",           // 71  CV_ALPHA_IntGP
    "sp",           // 72  CV_ALPHA_IntSP
    "zero",         // 73  CV_ALPHA_IntZERO
    "Fpcr",         // 74  CV_ALPHA_Fpcr
    "Fir",          // 75  CV_ALPHA_Fir
    "Psr",          // 76  CV_ALPHA_Psr
    "FltFsr"        // 77  CV_ALPHA_FltFsr
};

const char * const rgRegPpc[] =
{
    "None",         // 0
    "r0",           // 1   CV_PPC_GPR0
    "r1",           // 2   CV_PPC_GPR1
    "r2",           // 3   CV_PPC_GPR2
    "r3",           // 4   CV_PPC_GPR3
    "r4",           // 5   CV_PPC_GPR4
    "r5",           // 6   CV_PPC_GPR5
    "r6",           // 7   CV_PPC_GPR6
    "r7",           // 8   CV_PPC_GPR7
    "r8",           // 9   CV_PPC_GPR8
    "r9",           // 10  CV_PPC_GPR9
    "r10",          // 11  CV_PPC_GPR10
    "r11",          // 12  CV_PPC_GPR11
    "r12",          // 13  CV_PPC_GPR12
    "r13",          // 14  CV_PPC_GPR13
    "r14",          // 15  CV_PPC_GPR14
    "r15",          // 16  CV_PPC_GPR15
    "r16",          // 17  CV_PPC_GPR16
    "r17",          // 18  CV_PPC_GPR17
    "r18",          // 19  CV_PPC_GPR18
    "r19",          // 20  CV_PPC_GPR19
    "r20",          // 21  CV_PPC_GPR20
    "r21",          // 22  CV_PPC_GPR21
    "r22",          // 23  CV_PPC_GPR22
    "r23",          // 24  CV_PPC_GPR23
    "r24",          // 25  CV_PPC_GPR24
    "r25",          // 26  CV_PPC_GPR25
    "r26",          // 27  CV_PPC_GPR26
    "r27",          // 28  CV_PPC_GPR27
    "r28",          // 29  CV_PPC_GPR28
    "r29",          // 30  CV_PPC_GPR29
    "r30",          // 31  CV_PPC_GPR30
    "r31",          // 32  CV_PPC_GPR31
    "cr",           // 33  CV_PPC_CR
    "cr0",          // 34  CV_PPC_CR0
    "cr1",          // 35  CV_PPC_CR1
    "cr2",          // 36  CV_PPC_CR2
    "cr3",          // 37  CV_PPC_CR3
    "cr4",          // 38  CV_PPC_CR4
    "cr5",          // 39  CV_PPC_CR5
    "cr6",          // 40  CV_PPC_CR6
    "cr7",          // 41  CV_PPC_CR7
    "f0",           // 42  CV_PPC_FPR0
    "f1",           // 43  CV_PPC_FPR1
    "f2",           // 44  CV_PPC_FPR2
    "f3",           // 45  CV_PPC_FPR3
    "f4",           // 46  CV_PPC_FPR4
    "f5",           // 47  CV_PPC_FPR5
    "f6",           // 48  CV_PPC_FPR6
    "f7",           // 49  CV_PPC_FPR7
    "f8",           // 50  CV_PPC_FPR8
    "f9",           // 51  CV_PPC_FPR9
    "f10",          // 52  CV_PPC_FPR10
    "f11",          // 53  CV_PPC_FPR11
    "f12",          // 54  CV_PPC_FPR12
    "f13",          // 55  CV_PPC_FPR13
    "f14",          // 56  CV_PPC_FPR14
    "f15",          // 57  CV_PPC_FPR15
    "f16",          // 58  CV_PPC_FPR16
    "f17",          // 59  CV_PPC_FPR17
    "f18",          // 60  CV_PPC_FPR18
    "f19",          // 61  CV_PPC_FPR19
    "f20",          // 62  CV_PPC_FPR20
    "f21",          // 63  CV_PPC_FPR21
    "f22",          // 64  CV_PPC_FPR22
    "f23",          // 65  CV_PPC_FPR23
    "f24",          // 66  CV_PPC_FPR24
    "f25",          // 67  CV_PPC_FPR25
    "f26",          // 68  CV_PPC_FPR26
    "f27",          // 69  CV_PPC_FPR27
    "f28",          // 70  CV_PPC_FPR28
    "f29",          // 71  CV_PPC_FPR29
    "f30",          // 72  CV_PPC_FPR30
    "f31",          // 73  CV_PPC_FPR31
    "Fpscr",        // 74  CV_PPC_FPSCR
    "Msr"           // 75  CV_PPC_MSR
};

const char * const rgRegSh[] =
{
    "None",         // 0   CV_SH3_NOREG
    "???",          // 1
    "???",          // 2
    "???",          // 3
    "???",          // 4
    "???",          // 5
    "???",          // 6
    "???",          // 7
    "???",          // 8
    "???",          // 9
    "r0",           // 10  CV_SH3_IntR0
    "r1",           // 11  CV_SH3_IntR1
    "r2",           // 12  CV_SH3_IntR2
    "r3",           // 13  CV_SH3_IntR3
    "r4",           // 14  CV_SH3_IntR4
    "r5",           // 15  CV_SH3_IntR5
    "r6",           // 16  CV_SH3_IntR6
    "r7",           // 17  CV_SH3_IntR7
    "r8",           // 18  CV_SH3_IntR8
    "r9",           // 19  CV_SH3_IntR9
    "r10",          // 20  CV_SH3_IntR10
    "r11",          // 21  CV_SH3_IntR11
    "r12",          // 22  CV_SH3_IntR12
    "r13",          // 23  CV_SH3_IntR13
    "fp",           // 24  CV_SH3_IntFp
    "sp",           // 25  CV_SH3_IntSp
    "???",          // 26
    "???",          // 27
    "???",          // 28
    "???",          // 29
    "???",          // 30
    "???",          // 31
    "???",          // 32
    "???",          // 33
    "???",          // 34
    "???",          // 35
    "???",          // 36
    "???",          // 37
    "gbr",          // 38  CV_SH3_Gbr
    "pr",           // 39  CV_SH3_Pr
    "mach",         // 40  CV_SH3_Mach
    "macl",         // 41  CV_SH3_Macl
    "???",          // 42
    "???",          // 43
    "???",          // 44
    "???",          // 45
    "???",          // 46
    "???",          // 47
    "???",          // 48
    "???",          // 49
    "pc",           // 50
    "sr",           // 51
    "???",          // 52
    "???",          // 53
    "???",          // 54
    "???",          // 55
    "???",          // 56
    "???",          // 57
    "???",          // 58
    "???",          // 59
    "bara",         // 60  CV_SH3_BarA
    "basra",        // 61  CV_SH3_BasrA
    "bamra",        // 62  CV_SH3_BamrA
    "bbra",         // 63  CV_SH3_BbrA
    "barb",         // 64  CV_SH3_BarB
    "basrb",        // 65  CV_SH3_BasrB
    "bamrb",        // 66  CV_SH3_BamrB
    "bbrb",         // 67  CV_SH3_BbrB
    "bdrb",         // 68  CV_SH3_BdrB
    "bdmrb",        // 69  CV_SH3_BdmrB
    "brcr"          // 70  CV_SH3_Brcr
};

const char * const rgRegArm[] =
{
    "None",         // 0   CV_ARM_NOREG
    "???",          // 1
    "???",          // 2
    "???",          // 3
    "???",          // 4
    "???",          // 5
    "???",          // 6
    "???",          // 7
    "???",          // 8
    "???",          // 9
    "r0",           // 10  CV_ARM_R0
    "r1",           // 11  CV_ARM_R1
    "r2",           // 12  CV_ARM_R2
    "r3",           // 13  CV_ARM_R3
    "r4",           // 14  CV_ARM_R4
    "r5",           // 15  CV_ARM_R5
    "r6",           // 16  CV_ARM_R6
    "r7",           // 17  CV_ARM_R7
    "r8",           // 18  CV_ARM_R8
    "r9",           // 19  CV_ARM_R9
    "r10",          // 20  CV_ARM_R10
    "r11",          // 21  CV_ARM_R11
    "r12",          // 22  CV_ARM_R12
    "sp",           // 23  CV_ARM_SP
    "lr",           // 24  CV_ARM_LR
    "pc",           // 25  CV_ARM_PC
    "cpsr"          // 26  CV_ARM_CPSR
};

const MapIa64Reg mpIa64regSz[] =
{
    { CV_IA64_Br0, "Br0" },
    { CV_IA64_Br1, "Br1" },
    { CV_IA64_Br2, "Br2" },
    { CV_IA64_Br3, "Br3" },
    { CV_IA64_Br4, "Br4" },
    { CV_IA64_Br5, "Br5" },
    { CV_IA64_Br6, "Br6" },
    { CV_IA64_Br7, "Br7" },
    { CV_IA64_Preds, "Preds" },
    { CV_IA64_IntH0, "IntH0" },
    { CV_IA64_IntH1, "IntH1" },
    { CV_IA64_IntH2, "IntH2" },
    { CV_IA64_IntH3, "IntH3" },
    { CV_IA64_IntH4, "IntH4" },
    { CV_IA64_IntH5, "IntH5" },
    { CV_IA64_IntH6, "IntH6" },
    { CV_IA64_IntH7, "IntH7" },
    { CV_IA64_IntH8, "IntH8" },
    { CV_IA64_IntH9, "IntH9" },
    { CV_IA64_IntH10, "IntH10" },
    { CV_IA64_IntH11, "IntH11" },
    { CV_IA64_IntH12, "IntH12" },
    { CV_IA64_IntH13, "IntH13" },
    { CV_IA64_IntH14, "IntH14" },
    { CV_IA64_IntH15, "IntH15" },
    { CV_IA64_Ip, "Ip" },
    { CV_IA64_Umask, "Umask" },
    { CV_IA64_Cfm, "Cfm" },
    { CV_IA64_Psr, "Psr" },
    { CV_IA64_Nats, "Nats" },
    { CV_IA64_Nats2, "Nats2" },
    { CV_IA64_Nats3, "Nats3" },
    { CV_IA64_IntR0, "IntR0" },
    { CV_IA64_IntR1, "IntR1" },
    { CV_IA64_IntR2, "IntR2" },
    { CV_IA64_IntR3, "IntR3" },
    { CV_IA64_IntR4, "IntR4" },
    { CV_IA64_IntR5, "IntR5" },
    { CV_IA64_IntR6, "IntR6" },
    { CV_IA64_IntR7, "IntR7" },
    { CV_IA64_IntR8, "IntR8" },
    { CV_IA64_IntR9, "IntR9" },
    { CV_IA64_IntR10, "IntR10" },
    { CV_IA64_IntR11, "IntR11" },
    { CV_IA64_IntR12, "IntR12" },
    { CV_IA64_IntR13, "IntR13" },
    { CV_IA64_IntR14, "IntR14" },
    { CV_IA64_IntR15, "IntR15" },
    { CV_IA64_IntR16, "IntR16" },
    { CV_IA64_IntR17, "IntR17" },
    { CV_IA64_IntR18, "IntR18" },
    { CV_IA64_IntR19, "IntR19" },
    { CV_IA64_IntR20, "IntR20" },
    { CV_IA64_IntR21, "IntR21" },
    { CV_IA64_IntR22, "IntR22" },
    { CV_IA64_IntR23, "IntR23" },
    { CV_IA64_IntR24, "IntR24" },
    { CV_IA64_IntR25, "IntR25" },
    { CV_IA64_IntR26, "IntR26" },
    { CV_IA64_IntR27, "IntR27" },
    { CV_IA64_IntR28, "IntR28" },
    { CV_IA64_IntR29, "IntR29" },
    { CV_IA64_IntR30, "IntR30" },
    { CV_IA64_IntR31, "IntR31" },
    { CV_IA64_IntR32, "IntR32" },
    { CV_IA64_IntR33, "IntR33" },
    { CV_IA64_IntR34, "IntR34" },
    { CV_IA64_IntR35, "IntR35" },
    { CV_IA64_IntR36, "IntR36" },
    { CV_IA64_IntR37, "IntR37" },
    { CV_IA64_IntR38, "IntR38" },
    { CV_IA64_IntR39, "IntR39" },
    { CV_IA64_IntR40, "IntR40" },
    { CV_IA64_IntR41, "IntR41" },
    { CV_IA64_IntR42, "IntR42" },
    { CV_IA64_IntR43, "IntR43" },
    { CV_IA64_IntR44, "IntR44" },
    { CV_IA64_IntR45, "IntR45" },
    { CV_IA64_IntR46, "IntR46" },
    { CV_IA64_IntR47, "IntR47" },
    { CV_IA64_IntR48, "IntR48" },
    { CV_IA64_IntR49, "IntR49" },
    { CV_IA64_IntR50, "IntR50" },
    { CV_IA64_IntR51, "IntR51" },
    { CV_IA64_IntR52, "IntR52" },
    { CV_IA64_IntR53, "IntR53" },
    { CV_IA64_IntR54, "IntR54" },
    { CV_IA64_IntR55, "IntR55" },
    { CV_IA64_IntR56, "IntR56" },
    { CV_IA64_IntR57, "IntR57" },
    { CV_IA64_IntR58, "IntR58" },
    { CV_IA64_IntR59, "IntR59" },
    { CV_IA64_IntR60, "IntR60" },
    { CV_IA64_IntR61, "IntR61" },
    { CV_IA64_IntR62, "IntR62" },
    { CV_IA64_IntR63, "IntR63" },
    { CV_IA64_IntR64, "IntR64" },
    { CV_IA64_IntR65, "IntR65" },
    { CV_IA64_IntR66, "IntR66" },
    { CV_IA64_IntR67, "IntR67" },
    { CV_IA64_IntR68, "IntR68" },
    { CV_IA64_IntR69, "IntR69" },
    { CV_IA64_IntR70, "IntR70" },
    { CV_IA64_IntR71, "IntR71" },
    { CV_IA64_IntR72, "IntR72" },
    { CV_IA64_IntR73, "IntR73" },
    { CV_IA64_IntR74, "IntR74" },
    { CV_IA64_IntR75, "IntR75" },
    { CV_IA64_IntR76, "IntR76" },
    { CV_IA64_IntR77, "IntR77" },
    { CV_IA64_IntR78, "IntR78" },
    { CV_IA64_IntR79, "IntR79" },
    { CV_IA64_IntR80, "IntR80" },
    { CV_IA64_IntR81, "IntR81" },
    { CV_IA64_IntR82, "IntR82" },
    { CV_IA64_IntR83, "IntR83" },
    { CV_IA64_IntR84, "IntR84" },
    { CV_IA64_IntR85, "IntR85" },
    { CV_IA64_IntR86, "IntR86" },
    { CV_IA64_IntR87, "IntR87" },
    { CV_IA64_IntR88, "IntR88" },
    { CV_IA64_IntR89, "IntR89" },
    { CV_IA64_IntR90, "IntR90" },
    { CV_IA64_IntR91, "IntR91" },
    { CV_IA64_IntR92, "IntR92" },
    { CV_IA64_IntR93, "IntR93" },
    { CV_IA64_IntR94, "IntR94" },
    { CV_IA64_IntR95, "IntR95" },
    { CV_IA64_IntR96, "IntR96" },
    { CV_IA64_IntR97, "IntR97" },
    { CV_IA64_IntR98, "IntR98" },
    { CV_IA64_IntR99, "IntR99" },
    { CV_IA64_IntR100, "IntR100" },
    { CV_IA64_IntR101, "IntR101" },
    { CV_IA64_IntR102, "IntR102" },
    { CV_IA64_IntR103, "IntR103" },
    { CV_IA64_IntR104, "IntR104" },
    { CV_IA64_IntR105, "IntR105" },
    { CV_IA64_IntR106, "IntR106" },
    { CV_IA64_IntR107, "IntR107" },
    { CV_IA64_IntR108, "IntR108" },
    { CV_IA64_IntR109, "IntR109" },
    { CV_IA64_IntR110, "IntR110" },
    { CV_IA64_IntR111, "IntR111" },
    { CV_IA64_IntR112, "IntR112" },
    { CV_IA64_IntR113, "IntR113" },
    { CV_IA64_IntR114, "IntR114" },
    { CV_IA64_IntR115, "IntR115" },
    { CV_IA64_IntR116, "IntR116" },
    { CV_IA64_IntR117, "IntR117" },
    { CV_IA64_IntR118, "IntR118" },
    { CV_IA64_IntR119, "IntR119" },
    { CV_IA64_IntR120, "IntR120" },
    { CV_IA64_IntR121, "IntR121" },
    { CV_IA64_IntR122, "IntR122" },
    { CV_IA64_IntR123, "IntR123" },
    { CV_IA64_IntR124, "IntR124" },
    { CV_IA64_IntR125, "IntR125" },
    { CV_IA64_IntR126, "IntR126" },
    { CV_IA64_IntR127, "IntR127" },
    { CV_IA64_FltF0, "FltF0" },
    { CV_IA64_FltF1, "FltF1" },
    { CV_IA64_FltF2, "FltF2" },
    { CV_IA64_FltF3, "FltF3" },
    { CV_IA64_FltF4, "FltF4" },
    { CV_IA64_FltF5, "FltF5" },
    { CV_IA64_FltF6, "FltF6" },
    { CV_IA64_FltF7, "FltF7" },
    { CV_IA64_FltF8, "FltF8" },
    { CV_IA64_FltF9, "FltF9" },
    { CV_IA64_FltF10, "FltF10" },
    { CV_IA64_FltF11, "FltF11" },
    { CV_IA64_FltF12, "FltF12" },
    { CV_IA64_FltF13, "FltF13" },
    { CV_IA64_FltF14, "FltF14" },
    { CV_IA64_FltF15, "FltF15" },
    { CV_IA64_FltF16, "FltF16" },
    { CV_IA64_FltF17, "FltF17" },
    { CV_IA64_FltF18, "FltF18" },
    { CV_IA64_FltF19, "FltF19" },
    { CV_IA64_FltF20, "FltF20" },
    { CV_IA64_FltF21, "FltF21" },
    { CV_IA64_FltF22, "FltF22" },
    { CV_IA64_FltF23, "FltF23" },
    { CV_IA64_FltF24, "FltF24" },
    { CV_IA64_FltF25, "FltF25" },
    { CV_IA64_FltF26, "FltF26" },
    { CV_IA64_FltF27, "FltF27" },
    { CV_IA64_FltF28, "FltF28" },
    { CV_IA64_FltF29, "FltF29" },
    { CV_IA64_FltF30, "FltF30" },
    { CV_IA64_FltF31, "FltF31" },
    { CV_IA64_FltF32, "FltF32" },
    { CV_IA64_FltF33, "FltF33" },
    { CV_IA64_FltF34, "FltF34" },
    { CV_IA64_FltF35, "FltF35" },
    { CV_IA64_FltF36, "FltF36" },
    { CV_IA64_FltF37, "FltF37" },
    { CV_IA64_FltF38, "FltF38" },
    { CV_IA64_FltF39, "FltF39" },
    { CV_IA64_FltF40, "FltF40" },
    { CV_IA64_FltF41, "FltF41" },
    { CV_IA64_FltF42, "FltF42" },
    { CV_IA64_FltF43, "FltF43" },
    { CV_IA64_FltF44, "FltF44" },
    { CV_IA64_FltF45, "FltF45" },
    { CV_IA64_FltF46, "FltF46" },
    { CV_IA64_FltF47, "FltF47" },
    { CV_IA64_FltF48, "FltF48" },
    { CV_IA64_FltF49, "FltF49" },
    { CV_IA64_FltF50, "FltF50" },
    { CV_IA64_FltF51, "FltF51" },
    { CV_IA64_FltF52, "FltF52" },
    { CV_IA64_FltF53, "FltF53" },
    { CV_IA64_FltF54, "FltF54" },
    { CV_IA64_FltF55, "FltF55" },
    { CV_IA64_FltF56, "FltF56" },
    { CV_IA64_FltF57, "FltF57" },
    { CV_IA64_FltF58, "FltF58" },
    { CV_IA64_FltF59, "FltF59" },
    { CV_IA64_FltF60, "FltF60" },
    { CV_IA64_FltF61, "FltF61" },
    { CV_IA64_FltF62, "FltF62" },
    { CV_IA64_FltF63, "FltF63" },
    { CV_IA64_FltF64, "FltF64" },
    { CV_IA64_FltF65, "FltF65" },
    { CV_IA64_FltF66, "FltF66" },
    { CV_IA64_FltF67, "FltF67" },
    { CV_IA64_FltF68, "FltF68" },
    { CV_IA64_FltF69, "FltF69" },
    { CV_IA64_FltF70, "FltF70" },
    { CV_IA64_FltF71, "FltF71" },
    { CV_IA64_FltF72, "FltF72" },
    { CV_IA64_FltF73, "FltF73" },
    { CV_IA64_FltF74, "FltF74" },
    { CV_IA64_FltF75, "FltF75" },
    { CV_IA64_FltF76, "FltF76" },
    { CV_IA64_FltF77, "FltF77" },
    { CV_IA64_FltF78, "FltF78" },
    { CV_IA64_FltF79, "FltF79" },
    { CV_IA64_FltF80, "FltF80" },
    { CV_IA64_FltF81, "FltF81" },
    { CV_IA64_FltF82, "FltF82" },
    { CV_IA64_FltF83, "FltF83" },
    { CV_IA64_FltF84, "FltF84" },
    { CV_IA64_FltF85, "FltF85" },
    { CV_IA64_FltF86, "FltF86" },
    { CV_IA64_FltF87, "FltF87" },
    { CV_IA64_FltF88, "FltF88" },
    { CV_IA64_FltF89, "FltF89" },
    { CV_IA64_FltF90, "FltF90" },
    { CV_IA64_FltF91, "FltF91" },
    { CV_IA64_FltF92, "FltF92" },
    { CV_IA64_FltF93, "FltF93" },
    { CV_IA64_FltF94, "FltF94" },
    { CV_IA64_FltF95, "FltF95" },
    { CV_IA64_FltF96, "FltF96" },
    { CV_IA64_FltF97, "FltF97" },
    { CV_IA64_FltF98, "FltF98" },
    { CV_IA64_FltF99, "FltF99" },
    { CV_IA64_FltF100, "FltF100" },
    { CV_IA64_FltF101, "FltF101" },
    { CV_IA64_FltF102, "FltF102" },
    { CV_IA64_FltF103, "FltF103" },
    { CV_IA64_FltF104, "FltF104" },
    { CV_IA64_FltF105, "FltF105" },
    { CV_IA64_FltF106, "FltF106" },
    { CV_IA64_FltF107, "FltF107" },
    { CV_IA64_FltF108, "FltF108" },
    { CV_IA64_FltF109, "FltF109" },
    { CV_IA64_FltF110, "FltF110" },
    { CV_IA64_FltF111, "FltF111" },
    { CV_IA64_FltF112, "FltF112" },
    { CV_IA64_FltF113, "FltF113" },
    { CV_IA64_FltF114, "FltF114" },
    { CV_IA64_FltF115, "FltF115" },
    { CV_IA64_FltF116, "FltF116" },
    { CV_IA64_FltF117, "FltF117" },
    { CV_IA64_FltF118, "FltF118" },
    { CV_IA64_FltF119, "FltF119" },
    { CV_IA64_FltF120, "FltF120" },
    { CV_IA64_FltF121, "FltF121" },
    { CV_IA64_FltF122, "FltF122" },
    { CV_IA64_FltF123, "FltF123" },
    { CV_IA64_FltF124, "FltF124" },
    { CV_IA64_FltF125, "FltF125" },
    { CV_IA64_FltF126, "FltF126" },
    { CV_IA64_FltF127, "FltF127" },
    { CV_IA64_ApKR0, "ApKR0" },
    { CV_IA64_ApKR1, "ApKR1" },
    { CV_IA64_ApKR2, "ApKR2" },
    { CV_IA64_ApKR3, "ApKR3" },
    { CV_IA64_ApKR4, "ApKR4" },
    { CV_IA64_ApKR5, "ApKR5" },
    { CV_IA64_ApKR6, "ApKR6" },
    { CV_IA64_ApKR7, "ApKR7" },
    { CV_IA64_AR8, "AR8" },
    { CV_IA64_AR9, "AR9" },
    { CV_IA64_AR10, "AR10" },
    { CV_IA64_AR11, "AR11" },
    { CV_IA64_AR12, "AR12" },
    { CV_IA64_AR13, "AR13" },
    { CV_IA64_AR14, "AR14" },
    { CV_IA64_AR15, "AR15" },
    { CV_IA64_RsRSC, "RsRSC" },
    { CV_IA64_RsBSP, "RsBSP" },
    { CV_IA64_RsBSPSTORE, "RsBSPSTORE" },
    { CV_IA64_RsRNAT, "RsRNAT" },
    { CV_IA64_AR20, "AR20" },
    { CV_IA64_StFCR, "StFCR" },
    { CV_IA64_AR22, "AR22" },
    { CV_IA64_AR23, "AR23" },
    { CV_IA64_EFLAG, "EFLAG" },
    { CV_IA64_CSD, "CSD" },
    { CV_IA64_SSD, "SSD" },
    { CV_IA64_CFLG, "CFLG" },
    { CV_IA64_StFSR, "StFSR" },
    { CV_IA64_StFIR, "StFIR" },
    { CV_IA64_StFDR, "StFDR" },
    { CV_IA64_AR31, "AR31" },
    { CV_IA64_ApCCV, "ApCCV" },
    { CV_IA64_AR33, "AR33" },
    { CV_IA64_AR34, "AR34" },
    { CV_IA64_AR35, "AR35" },
    { CV_IA64_ApUNAT, "ApUNAT" },
    { CV_IA64_AR37, "AR37" },
    { CV_IA64_AR38, "AR38" },
    { CV_IA64_AR39, "AR39" },
    { CV_IA64_StFPSR, "StFPSR" },
    { CV_IA64_AR41, "AR41" },
    { CV_IA64_AR42, "AR42" },
    { CV_IA64_AR43, "AR43" },
    { CV_IA64_ApITC, "ApITC" },
    { CV_IA64_AR45, "AR45" },
    { CV_IA64_AR46, "AR46" },
    { CV_IA64_AR47, "AR47" },
    { CV_IA64_AR48, "AR48" },
    { CV_IA64_AR49, "AR49" },
    { CV_IA64_AR50, "AR50" },
    { CV_IA64_AR51, "AR51" },
    { CV_IA64_AR52, "AR52" },
    { CV_IA64_AR53, "AR53" },
    { CV_IA64_AR54, "AR54" },
    { CV_IA64_AR55, "AR55" },
    { CV_IA64_AR56, "AR56" },
    { CV_IA64_AR57, "AR57" },
    { CV_IA64_AR58, "AR58" },
    { CV_IA64_AR59, "AR59" },
    { CV_IA64_AR60, "AR60" },
    { CV_IA64_AR61, "AR61" },
    { CV_IA64_AR62, "AR62" },
    { CV_IA64_AR63, "AR63" },
    { CV_IA64_RsPFS, "RsPFS" },
    { CV_IA64_ApLC, "ApLC" },
    { CV_IA64_ApEC, "ApEC" },
    { CV_IA64_AR67, "AR67" },
    { CV_IA64_AR68, "AR68" },
    { CV_IA64_AR69, "AR69" },
    { CV_IA64_AR70, "AR70" },
    { CV_IA64_AR71, "AR71" },
    { CV_IA64_AR72, "AR72" },
    { CV_IA64_AR73, "AR73" },
    { CV_IA64_AR74, "AR74" },
    { CV_IA64_AR75, "AR75" },
    { CV_IA64_AR76, "AR76" },
    { CV_IA64_AR77, "AR77" },
    { CV_IA64_AR78, "AR78" },
    { CV_IA64_AR79, "AR79" },
    { CV_IA64_AR80, "AR80" },
    { CV_IA64_AR81, "AR81" },
    { CV_IA64_AR82, "AR82" },
    { CV_IA64_AR83, "AR83" },
    { CV_IA64_AR84, "AR84" },
    { CV_IA64_AR85, "AR85" },
    { CV_IA64_AR86, "AR86" },
    { CV_IA64_AR87, "AR87" },
    { CV_IA64_AR88, "AR88" },
    { CV_IA64_AR89, "AR89" },
    { CV_IA64_AR90, "AR90" },
    { CV_IA64_AR91, "AR91" },
    { CV_IA64_AR92, "AR92" },
    { CV_IA64_AR93, "AR93" },
    { CV_IA64_AR94, "AR94" },
    { CV_IA64_AR95, "AR95" },
    { CV_IA64_AR96, "AR96" },
    { CV_IA64_AR97, "AR97" },
    { CV_IA64_AR98, "AR98" },
    { CV_IA64_AR99, "AR99" },
    { CV_IA64_AR100, "AR100" },
    { CV_IA64_AR101, "AR101" },
    { CV_IA64_AR102, "AR102" },
    { CV_IA64_AR103, "AR103" },
    { CV_IA64_AR104, "AR104" },
    { CV_IA64_AR105, "AR105" },
    { CV_IA64_AR106, "AR106" },
    { CV_IA64_AR107, "AR107" },
    { CV_IA64_AR108, "AR108" },
    { CV_IA64_AR109, "AR109" },
    { CV_IA64_AR110, "AR110" },
    { CV_IA64_AR111, "AR111" },
    { CV_IA64_AR112, "AR112" },
    { CV_IA64_AR113, "AR113" },
    { CV_IA64_AR114, "AR114" },
    { CV_IA64_AR115, "AR115" },
    { CV_IA64_AR116, "AR116" },
    { CV_IA64_AR117, "AR117" },
    { CV_IA64_AR118, "AR118" },
    { CV_IA64_AR119, "AR119" },
    { CV_IA64_AR120, "AR120" },
    { CV_IA64_AR121, "AR121" },
    { CV_IA64_AR122, "AR122" },
    { CV_IA64_AR123, "AR123" },
    { CV_IA64_AR124, "AR124" },
    { CV_IA64_AR125, "AR125" },
    { CV_IA64_AR126, "AR126" },
    { CV_IA64_AR127, "AR127" },
    { CV_IA64_ApDCR, "ApDCR" },
    { CV_IA64_ApITM, "ApITM" },
    { CV_IA64_ApIVA, "ApIVA" },
    { CV_IA64_CR3, "CR3" },
    { CV_IA64_CR4, "CR4" },
    { CV_IA64_CR5, "CR5" },
    { CV_IA64_CR6, "CR6" },
    { CV_IA64_CR7, "CR7" },
    { CV_IA64_ApPTA, "ApPTA" },
    { CV_IA64_ApGPTA, "ApGPTA" },
    { CV_IA64_CR10, "CR10" },
    { CV_IA64_CR11, "CR11" },
    { CV_IA64_CR12, "CR12" },
    { CV_IA64_CR13, "CR13" },
    { CV_IA64_CR14, "CR14" },
    { CV_IA64_CR15, "CR15" },
    { CV_IA64_StIPSR, "StIPSR" },
    { CV_IA64_StISR, "StISR" },
    { CV_IA64_CR18, "CR18" },
    { CV_IA64_StIIP, "StIIP" },
    { CV_IA64_StIFA, "StIFA" },
    { CV_IA64_StITIR, "StITIR" },
    { CV_IA64_StIIPA, "StIIPA" },
    { CV_IA64_StIFS, "StIFS" },
    { CV_IA64_StIIM, "StIIM" },
    { CV_IA64_StIHA, "StIHA" },
    { CV_IA64_CR26, "CR26" },
    { CV_IA64_CR27, "CR27" },
    { CV_IA64_CR28, "CR28" },
    { CV_IA64_CR29, "CR29" },
    { CV_IA64_CR30, "CR30" },
    { CV_IA64_CR31, "CR31" },
    { CV_IA64_CR32, "CR32" },
    { CV_IA64_CR33, "CR33" },
    { CV_IA64_CR34, "CR34" },
    { CV_IA64_CR35, "CR35" },
    { CV_IA64_CR36, "CR36" },
    { CV_IA64_CR37, "CR37" },
    { CV_IA64_CR38, "CR38" },
    { CV_IA64_CR39, "CR39" },
    { CV_IA64_CR40, "CR40" },
    { CV_IA64_CR41, "CR41" },
    { CV_IA64_CR42, "CR42" },
    { CV_IA64_CR43, "CR43" },
    { CV_IA64_CR44, "CR44" },
    { CV_IA64_CR45, "CR45" },
    { CV_IA64_CR46, "CR46" },
    { CV_IA64_CR47, "CR47" },
    { CV_IA64_CR48, "CR48" },
    { CV_IA64_CR49, "CR49" },
    { CV_IA64_CR50, "CR50" },
    { CV_IA64_CR51, "CR51" },
    { CV_IA64_CR52, "CR52" },
    { CV_IA64_CR53, "CR53" },
    { CV_IA64_CR54, "CR54" },
    { CV_IA64_CR55, "CR55" },
    { CV_IA64_CR56, "CR56" },
    { CV_IA64_CR57, "CR57" },
    { CV_IA64_CR58, "CR58" },
    { CV_IA64_CR59, "CR59" },
    { CV_IA64_CR60, "CR60" },
    { CV_IA64_CR61, "CR61" },
    { CV_IA64_CR62, "CR62" },
    { CV_IA64_CR63, "CR63" },
    { CV_IA64_SaLID, "SaLID" },
    { CV_IA64_SaIVR, "SaIVR" },
    { CV_IA64_SaTPR, "SaTPR" },
    { CV_IA64_SaEOI, "SaEOI" },
    { CV_IA64_SaIRR0, "SaIRR0" },
    { CV_IA64_SaIRR1, "SaIRR1" },
    { CV_IA64_SaIRR2, "SaIRR2" },
    { CV_IA64_SaIRR3, "SaIRR3" },
    { CV_IA64_SaITV, "SaITV" },
    { CV_IA64_SaPMV, "SaPMV" },
    { CV_IA64_SaCMCV, "SaCMCV" },
    { CV_IA64_CR75, "CR75" },
    { CV_IA64_CR76, "CR76" },
    { CV_IA64_CR77, "CR77" },
    { CV_IA64_CR78, "CR78" },
    { CV_IA64_CR79, "CR79" },
    { CV_IA64_SaLRR0, "SaLRR0" },
    { CV_IA64_SaLRR1, "SaLRR1" },
    { CV_IA64_CR82, "CR82" },
    { CV_IA64_CR83, "CR83" },
    { CV_IA64_CR84, "CR84" },
    { CV_IA64_CR85, "CR85" },
    { CV_IA64_CR86, "CR86" },
    { CV_IA64_CR87, "CR87" },
    { CV_IA64_CR88, "CR88" },
    { CV_IA64_CR89, "CR89" },
    { CV_IA64_CR90, "CR90" },
    { CV_IA64_CR91, "CR91" },
    { CV_IA64_CR92, "CR92" },
    { CV_IA64_CR93, "CR93" },
    { CV_IA64_CR94, "CR94" },
    { CV_IA64_CR95, "CR95" },
    { CV_IA64_SaIRR0, "SaIRR0" },
    { CV_IA64_CR97, "CR97" },
    { CV_IA64_SaIRR1, "SaIRR1" },
    { CV_IA64_CR99, "CR99" },
    { CV_IA64_SaIRR2, "SaIRR2" },
    { CV_IA64_CR101, "CR101" },
    { CV_IA64_SaIRR3, "SaIRR3" },
    { CV_IA64_CR103, "CR103" },
    { CV_IA64_CR104, "CR104" },
    { CV_IA64_CR105, "CR105" },
    { CV_IA64_CR106, "CR106" },
    { CV_IA64_CR107, "CR107" },
    { CV_IA64_CR108, "CR108" },
    { CV_IA64_CR109, "CR109" },
    { CV_IA64_CR110, "CR110" },
    { CV_IA64_CR111, "CR111" },
    { CV_IA64_CR112, "CR112" },
    { CV_IA64_CR113, "CR113" },
    { CV_IA64_SaITV, "SaITV" },
    { CV_IA64_CR115, "CR115" },
    { CV_IA64_SaPMV, "SaPMV" },
    { CV_IA64_SaLRR0, "SaLRR0" },
    { CV_IA64_SaLRR1, "SaLRR1" },
    { CV_IA64_SaCMCV, "SaCMCV" },
    { CV_IA64_CR120, "CR120" },
    { CV_IA64_CR121, "CR121" },
    { CV_IA64_CR122, "CR122" },
    { CV_IA64_CR123, "CR123" },
    { CV_IA64_CR124, "CR124" },
    { CV_IA64_CR125, "CR125" },
    { CV_IA64_CR126, "CR126" },
    { CV_IA64_CR127, "CR127" },
    { CV_IA64_Pkr0, "Pkr0" },
    { CV_IA64_Pkr1, "Pkr1" },
    { CV_IA64_Pkr2, "Pkr2" },
    { CV_IA64_Pkr3, "Pkr3" },
    { CV_IA64_Pkr4, "Pkr4" },
    { CV_IA64_Pkr5, "Pkr5" },
    { CV_IA64_Pkr6, "Pkr6" },
    { CV_IA64_Pkr7, "Pkr7" },
    { CV_IA64_Pkr8, "Pkr8" },
    { CV_IA64_Pkr9, "Pkr9" },
    { CV_IA64_Pkr10, "Pkr10" },
    { CV_IA64_Pkr11, "Pkr11" },
    { CV_IA64_Pkr12, "Pkr12" },
    { CV_IA64_Pkr13, "Pkr13" },
    { CV_IA64_Pkr14, "Pkr14" },
    { CV_IA64_Pkr15, "Pkr15" },
    { CV_IA64_Rr0, "Rr0" },
    { CV_IA64_Rr1, "Rr1" },
    { CV_IA64_Rr2, "Rr2" },
    { CV_IA64_Rr3, "Rr3" },
    { CV_IA64_Rr4, "Rr4" },
    { CV_IA64_Rr5, "Rr5" },
    { CV_IA64_Rr6, "Rr6" },
    { CV_IA64_Rr7, "Rr7" },
    { CV_IA64_PFD0, "PFD0" },
    { CV_IA64_PFD1, "PFD1" },
    { CV_IA64_PFD2, "PFD2" },
    { CV_IA64_PFD3, "PFD3" },
    { CV_IA64_PFD4, "PFD4" },
    { CV_IA64_PFD5, "PFD5" },
    { CV_IA64_PFD6, "PFD6" },
    { CV_IA64_PFD7, "PFD7" },
    { CV_IA64_PFC0, "PFC0" },
    { CV_IA64_PFC1, "PFC1" },
    { CV_IA64_PFC2, "PFC2" },
    { CV_IA64_PFC3, "PFC3" },
    { CV_IA64_PFC4, "PFC4" },
    { CV_IA64_PFC5, "PFC5" },
    { CV_IA64_PFC6, "PFC6" },
    { CV_IA64_PFC7, "PFC7" },
    { CV_IA64_TrI0, "TrI0" },
    { CV_IA64_TrI1, "TrI1" },
    { CV_IA64_TrI2, "TrI2" },
    { CV_IA64_TrI3, "TrI3" },
    { CV_IA64_TrI4, "TrI4" },
    { CV_IA64_TrI5, "TrI5" },
    { CV_IA64_TrI6, "TrI6" },
    { CV_IA64_TrI7, "TrI7" },
    { CV_IA64_TrD0, "TrD0" },
    { CV_IA64_TrD1, "TrD1" },
    { CV_IA64_TrD2, "TrD2" },
    { CV_IA64_TrD3, "TrD3" },
    { CV_IA64_TrD4, "TrD4" },
    { CV_IA64_TrD5, "TrD5" },
    { CV_IA64_TrD6, "TrD6" },
    { CV_IA64_TrD7, "TrD7" },
    { CV_IA64_DbI0, "DbI0" },
    { CV_IA64_DbI1, "DbI1" },
    { CV_IA64_DbI2, "DbI2" },
    { CV_IA64_DbI3, "DbI3" },
    { CV_IA64_DbI4, "DbI4" },
    { CV_IA64_DbI5, "DbI5" },
    { CV_IA64_DbI6, "DbI6" },
    { CV_IA64_DbI7, "DbI7" },
    { CV_IA64_DbD0, "DbD0" },
    { CV_IA64_DbD1, "DbD1" },
    { CV_IA64_DbD2, "DbD2" },
    { CV_IA64_DbD3, "DbD3" },
    { CV_IA64_DbD4, "DbD4" },
    { CV_IA64_DbD5, "DbD5" },
    { CV_IA64_DbD6, "DbD6" },
    { CV_IA64_DbD7, "DbD7" }
};

const char * const rgUdtKind[] =
{
    "struct",
    "class",
    "union",
    "interface",
};

const char * const rgAccess[] =
{
    "",                     // No access specifier
    "private",
    "protected",
    "public"
};


#endif // QWINPDB_DEF

