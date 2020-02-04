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
#ifndef QWINPDB_H
#define QWINPDB_H

#pragma comment(lib,"Advapi32.lib")
#pragma comment(lib,"OleAut32.lib")

#define ALIGN_DOWN(x, align)    ((x) & ~((align) - 1))
#define ALIGN_UP(x, align) (((x) & ((align) - 1)) ? ALIGN_DOWN((x), (align)) + (align) : (x))


#include <QObject>

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QUuid>
#include <QMap>
#include <QDebug>

#include "qwinpdb_def.h"

class QWinPDB : public QObject
{
#define QWINPDB_UNKNOWN "<Unknown>"
    Q_OBJECT
public:

    struct VALUE
    {
        bool bIsValid;
        QVariant vValue;
    };

    struct SYMTAG
    {
        QString sName;
    };

    struct RECORD_COMPILAND
    {
        BOOL _editAndContinueEnabled;
        DWORD _lexicalParentId;
        QString _libraryName;
        QString _name;
        QString _sourceFileName;
        DWORD _symIndexId;
    };

    enum RD
    {
        RD_UNKNOWN=0,
        RD_BASETYPE,
        RD_UDT,
        RD_ENUM
    };

    struct RTYPE
    {
        RD type;
        int nBaseType;
        QString sType;
        QString sName;
        int nSize;
        int nOffset;
        int nAccess;
        bool bIsConst;
        bool bIsVolatile;
        bool bIsUnaligned;
        bool bIsPointer;
        bool bIsReference;
        int nPointerDeep;
        bool bIsArray;
        QList<int> listArrayCount;
    };

    enum ELEMENT_TYPE
    {
        ELEMENT_TYPE_UNKNOWN=0,
        ELEMENT_TYPE_STRUCT,
        ELEMENT_TYPE_CLASS,
        ELEMENT_TYPE_UNION,
        ELEMENT_TYPE_DATA
    };

    struct ELEMENT
    {
        ELEMENT_TYPE elementType;
        RTYPE rtype;
        QList<ELEMENT> listChildren;
    };

    struct RECORD_DATA
    {
        DWORD _access; // TODO enum
        DWORD _addressOffset;
        DWORD _addressSection;
        BOOL _addressTaken;
        DWORD _bitPosition;
        DWORD _classParentId;
        BOOL _compilerGenerated;
        BOOL _constType;
        DWORD _dataKind; // enum
        BOOL _isAggregated;
        BOOL _isSplitted;
        ULONGLONG _length;
        DWORD _lexicalParentId;
        DWORD _locationType;
        QString _name;
        LONG _offset;
        DWORD _registerId;
        DWORD _relativeVirtualAddress;
        DWORD _slot;
        DWORD _symIndexId;
        DWORD _token;
        DWORD _typeId;
        BOOL _unalignedType;
        // VARIANT _value; // TODO QVariant
        VALUE value;
        ULONGLONG _virtualAddress;
        BOOL _volatileType;

        RTYPE rtype;
    };

    struct RECORD_FUNCTION
    {
        DWORD _access; // TODO enum
        DWORD _addressOffset;
        DWORD _addressSection;
        DWORD _classParentId;
        BOOL _constType;
        BOOL _customCallingConvention;
        BOOL _farReturn;
        BOOL _hasAlloca;
        BOOL _hasEH;
        BOOL _hasEHa;
        BOOL _hasInlAsm;
        BOOL _hasLongJump;
        BOOL _hasSecurityChecks;
        BOOL _hasSEH;
        BOOL _hasSetJump;
        BOOL _interruptReturn;
        BOOL _intro;
        BOOL _inlSpec;
        BOOL _isNaked;
        BOOL _isStatic;
        ULONGLONG _length;
        DWORD _lexicalParentId;
        DWORD _locationType;
        QString _name;
        BOOL _noInline;
        BOOL _notReached;
        BOOL _noReturn;
        BOOL _noStackOrdering;
        BOOL _optimizedCodeDebugInfo;
        BOOL _pure;
        DWORD _relativeVirtualAddress;
        DWORD _symIndexId;
        DWORD _token;
        DWORD _typeId;
        BOOL _unalignedType;
        QString _undecoratedName;
//        QString _undecoratedNameEx;
        BOOL _virtual;
        ULONGLONG _virtualAddress;
        DWORD _virtualBaseOffset;
        BOOL _volatileType;
    };

    struct RECORD_UDT
    {
        DWORD _classParentId;
        BOOL _constructor;
        BOOL _constType;
        BOOL _hasAssignmentOperator;
        BOOL _hasCastOperator;
        BOOL _hasNestedTypes;
        ULONGLONG _length;
        DWORD _lexicalParentId;
        QString _name;
        BOOL _nested;
        BOOL _overloadedOperator;
        BOOL _packed;
        BOOL _scoped;
        DWORD _symIndexId;
        DWORD _udtKind;
        BOOL _unalignedType;
        DWORD _virtualTableShapeId;
        BOOL _volatileType;
    };

    struct RECORD_TYPEDEF
    {
        DWORD _baseType;
        DWORD _classParentId;
        BOOL _constructor;
        BOOL _constType;
        BOOL _hasAssignmentOperator;
        BOOL _hasCastOperator;
        BOOL _hasNestedTypes;
        ULONGLONG _length;
        DWORD _lexicalParentId;
        QString _name;
        BOOL _nested;
        BOOL _overloadedOperator;
        BOOL _packed;
        BOOL _reference;
        BOOL _scoped;
        DWORD _symIndexId;
        DWORD _typeId;
        DWORD _udtKind;
        BOOL _unalignedType;
        DWORD _virtualTableShapeId;
        BOOL _volatileType;
    };

    struct RECORD_PUBLICSYMBOL
    {
        DWORD _addressOffset;
        DWORD _addressSection;
        BOOL _code;
        BOOL _function;
        ULONGLONG _length;
        DWORD _lexicalParentId;
        DWORD _locationType;
        BOOL _managed;
        BOOL _msil;
        QString _name;
        DWORD _symIndexId;
        DWORD _relativeVirtualAddress;
        QString _undecoratedName;
    };

    struct RECORD_ENUM
    {
        DWORD _baseType;
        DWORD _classParentId;
        BOOL _constructor;
        BOOL _constType;
        BOOL _hasAssignmentOperator;
        BOOL _hasCastOperator;
        BOOL _hasNestedTypes;
        ULONGLONG _length;
        DWORD _lexicalParentId;
        QString _name;
        BOOL _nested;
        BOOL _overloadedOperator;
        BOOL _packed;
        BOOL _scoped;
        DWORD _symIndexId;
        DWORD _typeId;
        DWORD _udtKind;
        BOOL _unalignedType;
        BOOL _volatileType;
    };

    struct RECORD_POINTERTYPE
    {
        BOOL _constType;
        ULONGLONG _length;
        DWORD _lexicalParentId;
        BOOL _reference;
        DWORD _symIndexId;
        DWORD _typeId;
        BOOL _unalignedType;
        BOOL _volatileType;
    };

    struct RECORD_BASECLASS
    {
        DWORD _access; // TODO enum
        DWORD _classParentId;
        BOOL _constructor;
        BOOL _constType;
        BOOL _hasAssignmentOperator;
        BOOL _hasCastOperator;
        BOOL _hasNestedTypes;
        BOOL _indirectVirtualBaseClass;
        ULONGLONG _length;
        DWORD _lexicalParentId;
        QString _name;
        BOOL _nested;
        LONG _offset;
        BOOL _overloadedOperator;
        BOOL _packed;
        BOOL _scoped;
        DWORD _symIndexId;
        DWORD _typeId;
        DWORD _udtKind;
        BOOL _unalignedType;
        BOOL _virtualBaseClass;
        DWORD _virtualBaseDispIndex;
        LONG _virtualBasePointerOffset;
        DWORD _virtualTableShapeId;
        BOOL _volatileType;
    };

    struct RECORD_FUNCTIONTYPE
    {
        DWORD _callingConvention; // TODO enum
        DWORD _classParentId;
        BOOL _constType;
        DWORD _count;
        DWORD _lexicalParentId;
        DWORD _symIndexId;
        LONG _thisAdjust;
        DWORD _typeId;
        BOOL _unalignedType;
        BOOL _volatileType;
    };

    struct RECORD_ARRAYTYPE
    {
        DWORD _arrayIndexTypeId; // TODO enum
        BOOL _constType;
        DWORD _count;
        ULONGLONG _length;
        DWORD _lexicalParentId;
        DWORD _rank;
        DWORD _symIndexId;
        DWORD _typeId;
        BOOL _unalignedType;
        BOOL _volatileType;
    };

    struct RECORD_VTABLE
    {
        DWORD _classParentId;
        BOOL _constType;
        DWORD _lexicalParentId;
        DWORD _symIndexId;
        DWORD _typeId;
        BOOL _unalignedType;
        BOOL _volatileType;
    };

    struct RECORD_FUNCTIONARGTYPE
    {
        DWORD _classParentId;
        DWORD _lexicalParentId;
        DWORD _symIndexId;
        DWORD _typeId;
    };

    struct RECORD_BASETYPE
    {
        DWORD _baseType;
        BOOL _constType;
        ULONGLONG _length;
        DWORD _lexicalParentId;
        DWORD _symIndexId;
        BOOL _unalignedType;
        BOOL _volatileType;
    };

    struct RECORD_FUNCDEBUGSTART
    {
        DWORD _addressOffset;
        DWORD _addressSection;
        BOOL _customCallingConvention;
        BOOL _farReturn;
        BOOL _interruptReturn;
        BOOL _isStatic;
        DWORD _lexicalParentId;
        DWORD _locationType;
        BOOL _noInline;
        BOOL _noReturn;
        BOOL _notReached;
        LONG _offset;
        BOOL _optimizedCodeDebugInfo;
        DWORD _relativeVirtualAddress;
        DWORD _symIndexId;
        DWORD _symTag;
        ULONGLONG _virtualAddress;
    };

    struct RECORD_FUNCDEBUGEND
    {
        DWORD _addressOffset;
        DWORD _addressSection;
        BOOL _customCallingConvention;
        BOOL _farReturn;
        BOOL _interruptReturn;
        BOOL _isStatic;
        DWORD _lexicalParentId;
        DWORD _locationType;
        BOOL _noInline;
        BOOL _noReturn;
        BOOL _notReached;
        LONG _offset;
        BOOL _optimizedCodeDebugInfo;
        DWORD _relativeVirtualAddress;
        DWORD _symIndexId;
        DWORD _symTag;
        ULONGLONG _virtualAddress;
    };

    struct RECORD_CALLSITE
    {
        DWORD _addressOffset;
        DWORD _addressSection;
        BOOL _customCallingConvention;
        BOOL _farReturn;
        BOOL _interruptReturn;
        BOOL _isStatic;
        DWORD _lexicalParentId;
        DWORD _locationType;
        BOOL _noInline;
        BOOL _noReturn;
        BOOL _notReached;
        LONG _offset;
        BOOL _optimizedCodeDebugInfo;
        DWORD _relativeVirtualAddress;
        DWORD _symIndexId;
        DWORD _symTag;
        ULONGLONG _virtualAddress;
    };

    struct RECORD_LABEL
    {
        DWORD _addressOffset;
        DWORD _addressSection;
        BOOL _customCallingConvention;
        BOOL _farReturn;
        BOOL _interruptReturn;
        DWORD _lexicalParentId;
        DWORD _locationType;
        QString _name;
        BOOL _noInline;
        BOOL _noReturn;
        BOOL _notReached;
        LONG _offset;
        BOOL _optimizedCodeDebugInfo;
        DWORD _relativeVirtualAddress;
        DWORD _symIndexId;
        DWORD _symTag;
        ULONGLONG _virtualAddress;
    };

    struct RECORD_BLOCK
    {
        DWORD _addressOffset;
        DWORD _addressSection;
        ULONGLONG _length;
        DWORD _lexicalParentId;
        DWORD _locationType;
        QString _name;
        DWORD _relativeVirtualAddress;
        DWORD _symIndexId;
        DWORD _symTag;
        ULONGLONG _virtualAddress;
    };

    struct PDB_INFO
    {
        QMap<quint32,RECORD_COMPILAND> mapCompiland;
        QMap<quint32,RECORD_DATA> mapData;
        QMap<quint32,RECORD_FUNCTION> mapFunction;
        QMap<quint32,RECORD_UDT> mapUDT_struct;
        QMap<quint32,RECORD_UDT> mapUDT_class;
        QMap<quint32,RECORD_UDT> mapUDT_union;
        QMap<quint32,RECORD_UDT> mapUDT_interface;
        QMap<quint32,RECORD_TYPEDEF> mapTypeDef;
        QMap<quint32,RECORD_PUBLICSYMBOL> mapPublicSymbol;
        QMap<quint32,RECORD_ENUM> mapEnum;
        QMap<quint32,RECORD_POINTERTYPE> mapPointerType;
        QMap<quint32,RECORD_BASECLASS> mapBaseClass;
        QMap<quint32,RECORD_FUNCTIONTYPE> mapFunctionType;
        QMap<quint32,RECORD_ARRAYTYPE> mapArrayType;
        QMap<quint32,RECORD_VTABLE> mapVTable;
        QMap<quint32,RECORD_FUNCTIONARGTYPE> mapFunctionArgType;
        QMap<quint32,RECORD_BASETYPE> mapBaseType;
    };

    struct PDB_RECORD
    {
        DWORD dwID;
        QString sName;
    };

    struct STATS
    {
        QList<PDB_RECORD> listStructs;
        QList<PDB_RECORD> listClasses;
        QList<PDB_RECORD> listUnions;
        QList<PDB_RECORD> listInterfaces;
        QList<PDB_RECORD> listEnums;
    };

    struct HANDLE_OPTIONS
    {
        bool bOffsets;
        bool bSizes;
    };

    explicit QWinPDB(QObject *parent = 0);

    bool loadFromFile(QString sFileName);

    ~QWinPDB();

    PDB_INFO getAllTags();
    QList<PDB_RECORD> getUDTList(DWORD dwKind);
    QList<PDB_RECORD> getClasses(); // TODO remove

    STATS getStats();
    QString handle(quint32 nID,HANDLE_OPTIONS *pHandleOptions); // TODO remove

    enum ELEM_TYPE
    {
        ELEM_TYPE_UNKNOWN=0,
        ELEM_TYPE_UDT,
        ELEM_TYPE_FUNCTION,
        ELEM_TYPE_TYPEDEF,
        ELEM_TYPE_DATA,
        ELEM_TYPE_ENUM,
        ELEM_TYPE_BASECLASS,
        ELEM_TYPE_VTABLE,
        ELEM_TYPE_FUNCDEBUGSTART,
        ELEM_TYPE_FUNCDEBUGEND,
        ELEM_TYPE_CALLSITE,
        ELEM_TYPE_LABEL,
        ELEM_TYPE_BLOCK,
    };

    struct ELEM
    {
        ELEM_TYPE elemType;
        RECORD_UDT _udt;
        RECORD_FUNCTION _function;
        RECORD_DATA _data;
        RECORD_TYPEDEF _typedef;
        RECORD_ENUM _enum;
        RECORD_BASECLASS _baseclass;
        RECORD_BLOCK _block;
        RECORD_LABEL _label;
        RECORD_VTABLE _vtable;
        RECORD_FUNCDEBUGSTART _funcdebugstart;
        RECORD_FUNCDEBUGEND _funcdebugend;

        QList<ELEM> listChildren;
    };


    ELEM getElem(quint32 nID);
    ELEM _getElem(IDiaSymbol *pParent);

    struct SUBOPT
    {
        int nIndent;
        bool bNoFunctions;
    };

    static QString elemToString(const ELEM *pElem, HANDLE_OPTIONS *pHandleOptions, int nLevel, bool bIsStruct);

private:
    void cleanup();

    QString generateGUID();

    VALUE getValue(IDiaSymbol *pSymbol);
    qint64 variantToQint64(VARIANT value);

    QString indent(int nLevel);

    RECORD_UDT _getRecordUDT(IDiaSymbol *pSymbol);
    RECORD_FUNCTION _getRecordFunction(IDiaSymbol *pSymbol);
    RECORD_DATA _getRecordData(IDiaSymbol *pSymbol);
    RECORD_BASETYPE _getRecordBaseType(IDiaSymbol *pSymbol);
    RECORD_FUNCTIONARGTYPE _getRecordFunctionArgType(IDiaSymbol *pSymbol);
    RECORD_VTABLE _getRecordVTable(IDiaSymbol *pSymbol);
    RECORD_FUNCTIONTYPE _getRecordFunctionType(IDiaSymbol *pSymbol);
    RECORD_BASECLASS _getRecordBaseClass(IDiaSymbol *pSymbol);
    RECORD_POINTERTYPE _getRecordPointerType(IDiaSymbol *pSymbol);
    RECORD_ENUM _getRecordEnum(IDiaSymbol *pSymbol);
    RECORD_PUBLICSYMBOL _getRecordPublicSymbol(IDiaSymbol *pSymbol);
    RECORD_TYPEDEF _getRecordTypeDef(IDiaSymbol *pSymbol);
    RECORD_COMPILAND _getRecordCompiland(IDiaSymbol *pSymbol);
    RECORD_ARRAYTYPE _getRecordArrayType(IDiaSymbol *pSymbol);
    RECORD_FUNCDEBUGSTART _getRecordFuncDebugStart(IDiaSymbol *pSymbol);
    RECORD_FUNCDEBUGEND _getRecordFuncDebugEnd(IDiaSymbol *pSymbol);
    RECORD_LABEL _getRecordLabel(IDiaSymbol *pSymbol);
    RECORD_BLOCK _getRecordBlock(IDiaSymbol *pSymbol);

    RTYPE getSymbolType(IDiaSymbol *pSymbol);
    RTYPE _getType(IDiaSymbol *pType);

    DWORD _getSymTag(IDiaSymbol *pSymbol);
    bool getSymbolByID(DWORD dwID,IDiaSymbol **ppSymbol);
    QString _handle(IDiaSymbol *pParent,HANDLE_OPTIONS *pHandleOptions,SUBOPT subopt); // TODO return struct
    ELEMENT getElement(IDiaSymbol *pParent);
    QString elementToString(ELEMENT *pElement,HANDLE_OPTIONS *pHandleOptions,SUBOPT subopt);
    static QString rtypeToString(RTYPE rtype,HANDLE_OPTIONS *pHandleOptions, bool bIsStruct);
    static QString getAccessString(int nAccess);

    static QString _getTab(int nLevel);

private:
    IDiaDataSource *pDiaDataSource;
    IDiaSession *pDiaSession;
    IDiaSymbol *pGlobal;
    DWORD dwMachineType;
};

#endif // QWINPDB_H
