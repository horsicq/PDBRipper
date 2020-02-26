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

#include "qwinpdb.h"

bool sortLessThan(const QWinPDB::SYMBOL_RECORD &v1, const QWinPDB::SYMBOL_RECORD &v2)
{
//    return v1.sName<v2.sName; // TODO id sort
    return v1.dwID<v2.dwID;
}


QWinPDB::QWinPDB(QObject *parent) : QObject(parent)
{
    pDiaDataSource=nullptr;
    pGlobal=nullptr;
    pDiaSession=nullptr;
}


bool QWinPDB::loadFromFile(QString sFileName)
{
    cleanup();

    // TODO msdia option

    HRESULT hr=NoRegCoCreate(L"msdia140.dll", _uuidof(DiaSourceAlt),
                              _uuidof(IDiaDataSource),
                              (void **)(&pDiaDataSource));

    if(FAILED(hr))
    {
        qFatal("Cannot load msdia library!");
    }


    QByteArray baBuffer; // TODO function
    baBuffer.resize((sFileName.length()+1)*2);
    baBuffer.fill(0);
    wchar_t *pwszFileName=(wchar_t *)baBuffer.data();
    sFileName.toWCharArray(pwszFileName);
    hr=pDiaDataSource->loadDataFromPdb(pwszFileName);

    if(FAILED(hr))
    {
        return false;
    }

    hr=pDiaDataSource->openSession(&pDiaSession);

    if(FAILED(hr))
    {
        return false;
    }

    // TODO Exe info
    hr=pDiaSession->get_globalScope(&pGlobal);

    DWORD dwAge;
    hr=pGlobal->get_age(&dwAge);

    BSTR bstrName;

    if(pGlobal->get_name(&bstrName) != S_OK)
    {
        return false;
    }

    QString sString=QString::fromWCharArray(bstrName);

    if(hr!=S_OK)
    {
        return false;
    }

    DWORD dwTemp=0;

    dwMachineType=CV_CFL_80386;
    if(pGlobal->get_machineType(&dwTemp)==S_OK)
    {
        switch(dwTemp)
        {
            case IMAGE_FILE_MACHINE_I386:   dwMachineType=CV_CFL_80386;     break;
            case IMAGE_FILE_MACHINE_IA64:   dwMachineType=CV_CFL_IA64;      break;
            case IMAGE_FILE_MACHINE_AMD64:  dwMachineType=CV_CFL_AMD64;     break;
        }
    }

    return true;
}

QString QWinPDB::generateGUID()
{
    return QUuid::createUuid().toString();
}

QWinPDB::VALUE QWinPDB::getValue(IDiaSymbol *pSymbol)
{
    VALUE vResult={};
    VARIANT value;
    value.vt=VT_EMPTY;

    if(pSymbol->get_value(&value)==S_OK)
    {
        vResult.bIsValid=true;
        switch(value.vt)
        {
            case VT_UI1:        vResult.vValue=value.bVal;               break;
            case VT_UI2:        vResult.vValue=value.uiVal;              break;
            case VT_UI4:        vResult.vValue=(quint32)value.ulVal;     break;
            case VT_UINT:       vResult.vValue=value.uintVal;               break;
            case VT_INT:        vResult.vValue=value.intVal;             break;
            case VT_I1:         vResult.vValue=value.cVal;               break;
            case VT_I2:         vResult.vValue=value.iVal;               break;
            case VT_I4:         vResult.vValue=value.lVal;               break;
            default:            qDebug("Unknown VARIANT");
        }
    }
//    else
//    {
//        qDebug("QWinPDB::getValue error"); // TODO remove
//    }

    return vResult;
}

qint64 QWinPDB::variantToQint64(VARIANT value)
{
    qint64 result=0;

    switch(value.vt)
    {
        case VT_UI1:        result=value.bVal;              break;
        case VT_UI2:        result=value.uiVal;             break;
        case VT_UI4:        result=value.ulVal;             break;
        case VT_UINT:       result= value.uintVal;          break;
        case VT_INT:        result=value.intVal;            break;
        case VT_I1:         result=value.cVal;              break;
        case VT_I2:         result=value.iVal;              break;
        case VT_I4:         result=value.lVal;              break;
        default:            qDebug("Unknown VARIANT");
    }

    return result;
}


QString QWinPDB::indent(int nLevel)
{
    QString sResult;

    for(int i=0; i<nLevel; i++)
    {
        sResult+="    ";
    }

    return sResult;
}

QWinPDB::RECORD_UDT QWinPDB::_getRecordUDT(IDiaSymbol *pSymbol)
{
    RECORD_UDT result={};

    BSTR bstring=nullptr;

    pSymbol->get_classParentId(&result._classParentId);
    pSymbol->get_constructor(&result._constructor);
    pSymbol->get_constType(&result._constType);
    pSymbol->get_hasAssignmentOperator(&result._hasAssignmentOperator);
    pSymbol->get_hasCastOperator(&result._hasCastOperator);
    pSymbol->get_hasNestedTypes(&result._hasNestedTypes);
    pSymbol->get_length(&result._length);
    pSymbol->get_lexicalParentId(&result._lexicalParentId);
    if(pSymbol->get_name(&bstring)==S_OK) {result._name=QString::fromWCharArray(bstring);        SysFreeString(bstring);}
    pSymbol->get_nested(&result._nested);
    pSymbol->get_overloadedOperator(&result._overloadedOperator);
    pSymbol->get_packed(&result._packed);
    pSymbol->get_scoped(&result._scoped);
    pSymbol->get_symIndexId(&result._symIndexId);
    pSymbol->get_udtKind(&result._udtKind);
    pSymbol->get_unalignedType(&result._unalignedType);
    pSymbol->get_virtualTableShapeId(&result._virtualTableShapeId);
    pSymbol->get_volatileType(&result._volatileType);

    if(result._udtKind==0)      result.sType="struct";
    else if(result._udtKind==1) result.sType="class";
    else if(result._udtKind==2) result.sType="union";
    else if(result._udtKind==3) result.sType="interface";

    return result;
}

QWinPDB::RECORD_FUNCTION QWinPDB::_getRecordFunction(IDiaSymbol *pSymbol)
{
    RECORD_FUNCTION result={};

    BSTR bstring=nullptr;

    pSymbol->get_access(&result._access);
    pSymbol->get_addressOffset(&result._addressOffset);
    pSymbol->get_addressSection(&result._addressSection);
    pSymbol->get_classParentId(&result._classParentId);
    pSymbol->get_constType(&result._constType);
    pSymbol->get_customCallingConvention(&result._customCallingConvention);
    pSymbol->get_farReturn(&result._farReturn);
    pSymbol->get_hasAlloca(&result._hasAlloca);
    pSymbol->get_hasEH(&result._hasEH);
    pSymbol->get_hasEHa(&result._hasEHa);
    pSymbol->get_hasInlAsm(&result._hasInlAsm);
    pSymbol->get_hasLongJump(&result._hasLongJump);
    pSymbol->get_hasSecurityChecks(&result._hasSecurityChecks);
    pSymbol->get_hasSEH(&result._hasSEH);
    pSymbol->get_hasSetJump(&result._hasSetJump);
    pSymbol->get_interruptReturn(&result._interruptReturn);
    pSymbol->get_intro(&result._intro);
    pSymbol->get_inlSpec(&result._inlSpec);
    pSymbol->get_isNaked(&result._isNaked);
    pSymbol->get_isStatic(&result._isStatic);
    pSymbol->get_length(&result._length);
    pSymbol->get_lexicalParentId(&result._lexicalParentId);
    pSymbol->get_locationType(&result._locationType);
    if(pSymbol->get_name(&bstring)==S_OK) {result._name=QString::fromWCharArray(bstring);                   SysFreeString(bstring);}
    pSymbol->get_noInline(&result._noInline);
    pSymbol->get_notReached(&result._notReached);
    pSymbol->get_noReturn(&result._noReturn);
    pSymbol->get_noStackOrdering(&result._noStackOrdering);
    pSymbol->get_optimizedCodeDebugInfo(&result._optimizedCodeDebugInfo);
    pSymbol->get_pure(&result._pure);
    pSymbol->get_relativeVirtualAddress(&result._relativeVirtualAddress);
    pSymbol->get_symIndexId(&result._symIndexId);
    pSymbol->get_token(&result._token);
    pSymbol->get_typeId(&result._typeId);
    pSymbol->get_unalignedType(&result._unalignedType);
    if(pSymbol->get_undecoratedName(&bstring)==S_OK) {result._undecoratedName=QString::fromWCharArray(bstring);        SysFreeString(bstring);}
    //if(pSymbol->get_undecoratedNameEx(==S_OK) {record._undecoratedNameEx=QString::fromWCharArray(bstring);      SysFreeString(bstring);}
    pSymbol->get_virtual(&result._virtual);
    pSymbol->get_virtualAddress(&result._virtualAddress);
    pSymbol->get_virtualBaseOffset(&result._virtualBaseOffset);
    pSymbol->get_volatileType(&result._volatileType);

    result.rtype=getSymbolType(pSymbol);
    result.rtype.sName=result._name;
    result.rtype.nAccess=result._access;

    // TODO BitFields

    return result;
}

QWinPDB::RECORD_DATA QWinPDB::_getRecordData(IDiaSymbol *pSymbol)
{
    RECORD_DATA result={};

    BSTR bstring=nullptr;

    pSymbol->get_access(&result._access);
    pSymbol->get_addressOffset(&result._addressOffset);
    pSymbol->get_addressSection(&result._addressSection);
    pSymbol->get_addressTaken(&result._addressTaken);
    pSymbol->get_bitPosition(&result._bitPosition);
    pSymbol->get_classParentId(&result._classParentId);
    pSymbol->get_compilerGenerated(&result._compilerGenerated);
    pSymbol->get_constType(&result._constType);
    pSymbol->get_dataKind(&result._dataKind);
    pSymbol->get_isAggregated(&result._isAggregated);
    pSymbol->get_isSplitted(&result._isSplitted);
    pSymbol->get_length(&result._length);
    pSymbol->get_lexicalParentId(&result._lexicalParentId);
    pSymbol->get_locationType(&result._locationType);
    if(pSymbol->get_name(&bstring)==S_OK) {result._name=QString::fromWCharArray(bstring);   SysFreeString(bstring);}
    pSymbol->get_offset(&result._offset);
    pSymbol->get_registerId(&result._registerId);
    pSymbol->get_relativeVirtualAddress(&result._relativeVirtualAddress);
    pSymbol->get_slot(&result._slot);
    pSymbol->get_symIndexId(&result._symIndexId);
    pSymbol->get_token(&result._token);
    pSymbol->get_typeId(&result._typeId);
    pSymbol->get_unalignedType(&result._unalignedType);
    pSymbol->get_virtualAddress(&result._virtualAddress);
    pSymbol->get_volatileType(&result._volatileType);

    result.value=getValue(pSymbol);

    // TODO extra options

    result.rtype=getSymbolType(pSymbol);
    result.rtype.sName=result._name;
    result.rtype.nOffset=result._offset;
    result.rtype.nAccess=result._access;
    result.rtype.nBitOffset=result._bitPosition;
    result.rtype.nBitSize=result._length;

    return result;
}

QWinPDB::RECORD_BASETYPE QWinPDB::_getRecordBaseType(IDiaSymbol *pSymbol)
{
    RECORD_BASETYPE result={};

    pSymbol->get_baseType(&result._baseType);
    pSymbol->get_constType(&result._constType);
    pSymbol->get_length(&result._length);
    pSymbol->get_lexicalParentId(&result._lexicalParentId);
    pSymbol->get_symIndexId(&result._symIndexId);
    pSymbol->get_unalignedType(&result._unalignedType);
    pSymbol->get_volatileType(&result._volatileType);

    return result;
}

QWinPDB::RECORD_FUNCTIONARGTYPE QWinPDB::_getRecordFunctionArgType(IDiaSymbol *pSymbol)
{
    RECORD_FUNCTIONARGTYPE result={};

    pSymbol->get_classParentId(&result._classParentId);
    pSymbol->get_lexicalParentId(&result._lexicalParentId);
    pSymbol->get_symIndexId(&result._symIndexId);
    pSymbol->get_typeId(&result._typeId);

    return result;
}

QWinPDB::RECORD_VTABLE QWinPDB::_getRecordVTable(IDiaSymbol *pSymbol)
{
    RECORD_VTABLE result={};

    pSymbol->get_classParentId(&result._classParentId);
    pSymbol->get_constType(&result._constType);
    pSymbol->get_lexicalParentId(&result._lexicalParentId);
    pSymbol->get_symIndexId(&result._symIndexId);
    pSymbol->get_typeId(&result._typeId);
    pSymbol->get_unalignedType(&result._unalignedType);
    pSymbol->get_volatileType(&result._volatileType);

    return result;
}

QWinPDB::RECORD_FUNCTIONTYPE QWinPDB::_getRecordFunctionType(IDiaSymbol *pSymbol)
{
    RECORD_FUNCTIONTYPE result={};

    pSymbol->get_callingConvention(&result._callingConvention);
    pSymbol->get_classParentId(&result._classParentId);
    pSymbol->get_constType(&result._constType);
    pSymbol->get_count(&result._count);
    pSymbol->get_lexicalParentId(&result._lexicalParentId);
    pSymbol->get_symIndexId(&result._symIndexId);
    pSymbol->get_thisAdjust(&result._thisAdjust);
    pSymbol->get_typeId(&result._typeId);
    pSymbol->get_unalignedType(&result._unalignedType);
    pSymbol->get_volatileType(&result._volatileType);

    return result;
}

QWinPDB::RECORD_BASECLASS QWinPDB::_getRecordBaseClass(IDiaSymbol *pSymbol)
{
    RECORD_BASECLASS result={};

    BSTR bstring=nullptr;

    pSymbol->get_access(&result._access);
    pSymbol->get_classParentId(&result._classParentId);
    pSymbol->get_constructor(&result._constructor);
    pSymbol->get_constType(&result._constType);
    pSymbol->get_hasAssignmentOperator(&result._hasAssignmentOperator);
    pSymbol->get_hasCastOperator(&result._hasCastOperator);
    pSymbol->get_hasNestedTypes(&result._hasNestedTypes);
    pSymbol->get_indirectVirtualBaseClass(&result._indirectVirtualBaseClass);
    pSymbol->get_length(&result._length);
    pSymbol->get_lexicalParentId(&result._lexicalParentId);
    if(pSymbol->get_name(&bstring)==S_OK) {result._name=QString::fromWCharArray(bstring);        SysFreeString(bstring);}
    pSymbol->get_nested(&result._nested);
    pSymbol->get_offset(&result._offset);
    pSymbol->get_overloadedOperator(&result._overloadedOperator);
    pSymbol->get_packed(&result._packed);
    pSymbol->get_scoped(&result._scoped);
    pSymbol->get_symIndexId(&result._symIndexId);
    pSymbol->get_typeId(&result._typeId);
    pSymbol->get_udtKind(&result._udtKind);
    pSymbol->get_unalignedType(&result._unalignedType);
    pSymbol->get_virtualBaseClass(&result._virtualBaseClass);
    pSymbol->get_virtualBaseDispIndex(&result._virtualBaseDispIndex);
    pSymbol->get_virtualBasePointerOffset(&result._virtualBasePointerOffset);
    pSymbol->get_virtualTableShapeId(&result._virtualTableShapeId);
    pSymbol->get_volatileType(&result._volatileType);

    return result;
}

QWinPDB::RECORD_POINTERTYPE QWinPDB::_getRecordPointerType(IDiaSymbol *pSymbol)
{
    RECORD_POINTERTYPE result={};

    pSymbol->get_constType(&result._constType);
    pSymbol->get_length(&result._length);
    pSymbol->get_lexicalParentId(&result._lexicalParentId);
    pSymbol->get_reference(&result._reference);
    pSymbol->get_symIndexId(&result._symIndexId);
    pSymbol->get_typeId(&result._typeId);
    pSymbol->get_unalignedType(&result._unalignedType);
    pSymbol->get_volatileType(&result._volatileType);

    return result;
}

QWinPDB::RECORD_ENUM QWinPDB::_getRecordEnum(IDiaSymbol *pSymbol)
{
    RECORD_ENUM result={};

    BSTR bstring=nullptr;

    pSymbol->get_baseType(&result._baseType);
    pSymbol->get_classParentId(&result._classParentId); // Does not work on root items. test
    pSymbol->get_constructor(&result._constructor);
    pSymbol->get_constType(&result._constType);
    pSymbol->get_hasAssignmentOperator(&result._hasAssignmentOperator);
    pSymbol->get_hasCastOperator(&result._hasCastOperator);
    pSymbol->get_hasNestedTypes(&result._hasNestedTypes);
    pSymbol->get_length(&result._length);
    pSymbol->get_lexicalParentId(&result._lexicalParentId);
    if(pSymbol->get_name(&bstring)==S_OK) {result._name=QString::fromWCharArray(bstring);        SysFreeString(bstring);}
    pSymbol->get_nested(&result._nested);
    pSymbol->get_overloadedOperator(&result._overloadedOperator);
    pSymbol->get_packed(&result._packed);
    pSymbol->get_scoped(&result._scoped);
    pSymbol->get_symIndexId(&result._symIndexId);
    pSymbol->get_typeId(&result._typeId);
    pSymbol->get_udtKind(&result._udtKind);
    pSymbol->get_unalignedType(&result._unalignedType);
    pSymbol->get_volatileType(&result._volatileType);

    return result;
}

QWinPDB::RECORD_PUBLICSYMBOL QWinPDB::_getRecordPublicSymbol(IDiaSymbol *pSymbol)
{
    RECORD_PUBLICSYMBOL result={};

    BSTR bstring=nullptr;

    pSymbol->get_addressOffset(&result._addressOffset);
    pSymbol->get_addressSection(&result._addressSection);
    pSymbol->get_code(&result._code);
    pSymbol->get_function(&result._function);
    pSymbol->get_length(&result._length);
    pSymbol->get_lexicalParentId(&result._lexicalParentId);
    pSymbol->get_locationType(&result._locationType);
    pSymbol->get_managed(&result._managed);
    pSymbol->get_msil(&result._msil);
    if(pSymbol->get_name(&bstring)==S_OK) {result._name=QString::fromWCharArray(bstring);        SysFreeString(bstring);}
    pSymbol->get_symIndexId(&result._symIndexId);
    pSymbol->get_relativeVirtualAddress(&result._relativeVirtualAddress);
    if(pSymbol->get_undecoratedName(&bstring)==S_OK) {result._undecoratedName=QString::fromWCharArray(bstring);        SysFreeString(bstring);}

    return result;
}

QWinPDB::RECORD_TYPEDEF QWinPDB::_getRecordTypeDef(IDiaSymbol *pSymbol)
{
    RECORD_TYPEDEF result={};

    BSTR bstring=nullptr;

    pSymbol->get_baseType(&result._baseType);
    pSymbol->get_classParentId(&result._classParentId);
    pSymbol->get_constructor(&result._constructor);
    pSymbol->get_constType(&result._constType);
    pSymbol->get_hasAssignmentOperator(&result._hasAssignmentOperator);
    pSymbol->get_hasCastOperator(&result._hasCastOperator);
    pSymbol->get_hasNestedTypes(&result._hasNestedTypes);
    pSymbol->get_length(&result._length);
    pSymbol->get_lexicalParentId(&result._lexicalParentId);
    if(pSymbol->get_name(&bstring)==S_OK) {result._name=QString::fromWCharArray(bstring);        SysFreeString(bstring);}
    pSymbol->get_nested(&result._nested);
    pSymbol->get_overloadedOperator(&result._overloadedOperator);
    pSymbol->get_packed(&result._packed);
    pSymbol->get_reference(&result._reference);
    pSymbol->get_scoped(&result._scoped);
    pSymbol->get_symIndexId(&result._symIndexId);
    pSymbol->get_typeId(&result._typeId);
    pSymbol->get_udtKind(&result._udtKind);
    pSymbol->get_unalignedType(&result._unalignedType);
    pSymbol->get_virtualTableShapeId(&result._virtualTableShapeId);
    pSymbol->get_volatileType(&result._volatileType);

    return result;
}

QWinPDB::RECORD_COMPILAND QWinPDB::_getRecordCompiland(IDiaSymbol *pSymbol)
{
    RECORD_COMPILAND result={};

    BSTR bstring=nullptr;

    pSymbol->get_editAndContinueEnabled(&result._editAndContinueEnabled);
    pSymbol->get_lexicalParentId(&result._lexicalParentId);
    pSymbol->get_symIndexId(&result._symIndexId);
    if(pSymbol->get_sourceFileName(&bstring)==S_OK) {result._sourceFileName=QString::fromWCharArray(bstring);   SysFreeString(bstring);}
    if(pSymbol->get_name(&bstring)==S_OK)           {result._name=QString::fromWCharArray(bstring);             SysFreeString(bstring);}
    if(pSymbol->get_libraryName(&bstring)==S_OK)    {result._libraryName=QString::fromWCharArray(bstring);      SysFreeString(bstring);}

    return result;
}

QWinPDB::RECORD_ARRAYTYPE QWinPDB::_getRecordArrayType(IDiaSymbol *pSymbol)
{
    RECORD_ARRAYTYPE result={};

    pSymbol->get_arrayIndexTypeId(&result._arrayIndexTypeId);
    pSymbol->get_constType(&result._constType);
    pSymbol->get_count(&result._count);
    pSymbol->get_length(&result._length);
    pSymbol->get_lexicalParentId(&result._lexicalParentId);
    pSymbol->get_rank(&result._rank);
    pSymbol->get_symIndexId(&result._symIndexId);
    pSymbol->get_typeId(&result._typeId);
    pSymbol->get_unalignedType(&result._unalignedType);
    pSymbol->get_volatileType(&result._volatileType);

    return result;
}

QWinPDB::RECORD_FUNCDEBUGSTART QWinPDB::_getRecordFuncDebugStart(IDiaSymbol *pSymbol)
{
    RECORD_FUNCDEBUGSTART result={};

    pSymbol->get_addressOffset(&result._addressOffset);
    pSymbol->get_addressSection(&result._addressSection);
    pSymbol->get_customCallingConvention(&result._customCallingConvention);
    pSymbol->get_farReturn(&result._farReturn);
    pSymbol->get_interruptReturn(&result._interruptReturn);
    pSymbol->get_isStatic(&result._isStatic);
    pSymbol->get_lexicalParentId(&result._lexicalParentId);
    pSymbol->get_locationType(&result._locationType);
    pSymbol->get_noInline(&result._noInline);
    pSymbol->get_noReturn(&result._noReturn);
    pSymbol->get_notReached(&result._notReached);
    pSymbol->get_offset(&result._offset);
    pSymbol->get_optimizedCodeDebugInfo(&result._optimizedCodeDebugInfo);
    pSymbol->get_relativeVirtualAddress(&result._relativeVirtualAddress);
    pSymbol->get_symIndexId(&result._symIndexId);
    pSymbol->get_symTag(&result._symTag);
    pSymbol->get_virtualAddress(&result._virtualAddress);

    return result;
}

QWinPDB::RECORD_FUNCDEBUGEND QWinPDB::_getRecordFuncDebugEnd(IDiaSymbol *pSymbol)
{
    RECORD_FUNCDEBUGEND result={};

    pSymbol->get_addressOffset(&result._addressOffset);
    pSymbol->get_addressSection(&result._addressSection);
    pSymbol->get_customCallingConvention(&result._customCallingConvention);
    pSymbol->get_farReturn(&result._farReturn);
    pSymbol->get_interruptReturn(&result._interruptReturn);
    pSymbol->get_isStatic(&result._isStatic);
    pSymbol->get_lexicalParentId(&result._lexicalParentId);
    pSymbol->get_locationType(&result._locationType);
    pSymbol->get_noInline(&result._noInline);
    pSymbol->get_noReturn(&result._noReturn);
    pSymbol->get_notReached(&result._notReached);
    pSymbol->get_offset(&result._offset);
    pSymbol->get_optimizedCodeDebugInfo(&result._optimizedCodeDebugInfo);
    pSymbol->get_relativeVirtualAddress(&result._relativeVirtualAddress);
    pSymbol->get_symIndexId(&result._symIndexId);
    pSymbol->get_symTag(&result._symTag);
    pSymbol->get_virtualAddress(&result._virtualAddress);

    return result;
}

QWinPDB::RECORD_LABEL QWinPDB::_getRecordLabel(IDiaSymbol *pSymbol)
{
    RECORD_LABEL result={};

    BSTR bstring=nullptr;

    pSymbol->get_addressOffset(&result._addressOffset);
    pSymbol->get_addressSection(&result._addressSection);
    pSymbol->get_customCallingConvention(&result._customCallingConvention);
    pSymbol->get_farReturn(&result._farReturn);
    pSymbol->get_interruptReturn(&result._interruptReturn);
    pSymbol->get_lexicalParentId(&result._lexicalParentId);
    pSymbol->get_locationType(&result._locationType);
    if(pSymbol->get_name(&bstring)==S_OK)           {result._name=QString::fromWCharArray(bstring);             SysFreeString(bstring);}
    pSymbol->get_noInline(&result._noInline);
    pSymbol->get_noReturn(&result._noReturn);
    pSymbol->get_notReached(&result._notReached);
    pSymbol->get_offset(&result._offset);
    pSymbol->get_optimizedCodeDebugInfo(&result._optimizedCodeDebugInfo);
    pSymbol->get_relativeVirtualAddress(&result._relativeVirtualAddress);
    pSymbol->get_symIndexId(&result._symIndexId);
    pSymbol->get_symTag(&result._symTag);
    pSymbol->get_virtualAddress(&result._virtualAddress);

    return result;
}

QWinPDB::RECORD_BLOCK QWinPDB::_getRecordBlock(IDiaSymbol *pSymbol)
{
    RECORD_BLOCK result={};

    BSTR bstring=nullptr;

    pSymbol->get_addressOffset(&result._addressOffset);
    pSymbol->get_addressSection(&result._addressSection);
    pSymbol->get_length(&result._length);
    pSymbol->get_lexicalParentId(&result._lexicalParentId);
    pSymbol->get_locationType(&result._locationType);
    if(pSymbol->get_name(&bstring)==S_OK)           {result._name=QString::fromWCharArray(bstring);             SysFreeString(bstring);}
    pSymbol->get_relativeVirtualAddress(&result._relativeVirtualAddress);
    pSymbol->get_symIndexId(&result._symIndexId);
    pSymbol->get_symTag(&result._symTag);
    pSymbol->get_virtualAddress(&result._virtualAddress);

    return result;
}

QWinPDB::RTYPE QWinPDB::getSymbolType(IDiaSymbol *pSymbol)
{
    RTYPE result={};

    IDiaSymbol *pType=0;
    pSymbol->get_type(&pType);

    if(pType)
    {
        result=_getType(pType);
    }
    else
    {
        qDebug("No type"); // TODO Check
    }

//    pType->Release();

    return result;
}

QWinPDB::RTYPE QWinPDB::_getType(IDiaSymbol *pType)
{
    RTYPE result={};

    if(pType)
    {
        DWORD dwSymTag=0;

        pType->get_symTag(&dwSymTag);

        if(dwSymTag==SymTagBaseType)
        {
            RECORD_BASETYPE baseType=_getRecordBaseType(pType);
            result.nSize=baseType._length;
            result.bIsConst=baseType._constType;
            result.bIsUnaligned=baseType._unalignedType;
            result.bIsVolatile=baseType._volatileType;

            result.type=RD_BASETYPE;
            result.nBaseType=baseType._baseType; // TODO const

            // TODO Check!
            switch(result.nBaseType)
            {
                case 0:     result.sTypeName="<btNoType>";          break;
                case 1:     result.sTypeName="void";                break;
                case 2:     result.sTypeName="char";                break;
                case 3:     result.sTypeName="wchar_t";             break;
                case 4:     result.sTypeName="signed char";         break;
                case 5:     result.sTypeName="unsigned char";       break;
                case 6:     result.sTypeName="int";                 break;
                case 7:     result.sTypeName="unsigned int";        break;
                case 8:     result.sTypeName="float";               break;
                case 9:     result.sTypeName="BCD";                 break;
                case 10:    result.sTypeName="bool";                break;
                case 11:    result.sTypeName="short";               break;
                case 12:    result.sTypeName="unsigned short";      break;
                case 13:    result.sTypeName="long";                break;
                case 14:    result.sTypeName="unsigned long";       break;
                case 15:    result.sTypeName="__int8";              break;
                case 16:    result.sTypeName="__int16";             break;
                case 17:    result.sTypeName="__int32";             break;
                case 18:    result.sTypeName="__int64";             break;
                case 19:    result.sTypeName="__int128";            break;
                case 20:    result.sTypeName="unsigned __int8";     break;
                case 21:    result.sTypeName="unsigned __int16";    break;
                case 22:    result.sTypeName="unsigned __int32";    break;
                case 23:    result.sTypeName="unsigned __int64";    break;
                case 24:    result.sTypeName="unsigned __int128";   break;
                case 25:    result.sTypeName="CURRENCY";            break;
                case 26:    result.sTypeName="DATE";                break;
                case 27:    result.sTypeName="VARIANT";             break;
                case 28:    result.sTypeName="COMPLEX";             break;
                case 29:    result.sTypeName="BIT";                 break;
                case 30:    result.sTypeName="BSTR";                break;
                case 31:    result.sTypeName="HRESULT";             break;
                case 32:    result.sTypeName="char16_t";            break;
                case 33:    result.sTypeName="char32_t";            break;
            }
        }
        else if(dwSymTag==SymTagUDT)
        {
            RECORD_UDT udt=_getRecordUDT(pType);
            result.nSize=udt._length;
            result.bIsConst=udt._constType;
            result.bIsUnaligned=udt._unalignedType;
            result.bIsVolatile=udt._volatileType;
            result.type=RD_UDT;
            result.sTypeName=QString("%1 %2").arg(udt.sType).arg(udt._name); // TODO const
        }
        else if(dwSymTag==SymTagPointerType)
        {
            RECORD_POINTERTYPE pointerType=_getRecordPointerType(pType);
            // TODO reference
            IDiaSymbol *_pType=0;
            pType->get_type(&_pType);

            result=_getType(_pType);
            if(pointerType._reference)
            {
                result.bIsReference=true;
            }
            else
            {
                result.bIsPointer=true;
            }

            result.nPointerDeep++;
            result.nSize=pointerType._length;
            result.bIsConst=pointerType._constType;
            result.bIsUnaligned=pointerType._unalignedType;
            result.bIsVolatile=pointerType._volatileType;

            _pType->Release();
        }
        else if(dwSymTag==SymTagArrayType)
        {
            RECORD_ARRAYTYPE arrayType=_getRecordArrayType(pType);

            IDiaSymbol *_pType=0;
            pType->get_type(&_pType);

            result=_getType(_pType);
            result.bIsArray=true;
            result.listArrayCount.append(arrayType._count);
            result.nSize*=arrayType._count;
            result.bIsConst=arrayType._constType;
            result.bIsUnaligned=arrayType._unalignedType;
            result.bIsVolatile=arrayType._volatileType;

            _pType->Release();
        }
        else if(dwSymTag==SymTagEnum)
        {
            RECORD_ENUM enumType=_getRecordEnum(pType);

            result.nSize=enumType._length;
            result.bIsConst=enumType._constType;
            result.bIsUnaligned=enumType._unalignedType;
            result.bIsVolatile=enumType._volatileType;

            result.type=RD_ENUM;
            result.sTypeName=QString("enum %1").arg(enumType._name); // TODO const
        }
        else if(dwSymTag==SymTagFunctionType)
        {
            RTYPE res_ret=getSymbolType(pType);
            result.sFunctionRet=rtypeToString(res_ret,false);

            RECORD_FUNCTIONTYPE ft=_getRecordFunctionType(pType);
            result.bIsConst=ft._constType;
            result.bIsUnaligned=ft._unalignedType;
            result.bIsVolatile=ft._volatileType;
            result.type=RD_FUNCTION;

            IDiaEnumSymbols *pEnumSymbols;
            if(pType->findChildren(SymTagNull, nullptr, nsNone, &pEnumSymbols)==S_OK)
            {
                LONG nCount;
                if(pEnumSymbols->get_Count(&nCount)==S_OK)
                {
                    if(nCount)
                    {
                        IDiaSymbol *pSymbol;
                        ULONG celt = 0;

                        while(SUCCEEDED(pEnumSymbols->Next(1,&pSymbol,&celt))&&(celt==1))
                        {
                            RTYPE rtype=getSymbolType(pSymbol);

                            result.listFunctionArgs.append(rtypeToString(rtype,false));

                            pSymbol->Release();
                        }
                    }
                }

                pEnumSymbols->Release();
            }
        }
        else if(dwSymTag==SymTagFunctionArgType)
        {
            result=getSymbolType(pType); // TODO
        }
        else
        {
             qDebug("Unknown TYPE !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
        }
    }
    else
    {
        qDebug("Error!!!");
    }


    return result;
}

QString QWinPDB::getSymbolTypeString(IDiaSymbol *pSymbol)
{
    QString sResult;

    IDiaSymbol *pType=0;
    pSymbol->get_type(&pType);

    if(pType)
    {
        sResult=_getTypeString(pType);
    }
    else
    {
        qDebug("No type"); // TODO Check
    }

    pType->Release();

    return sResult;
}

QString QWinPDB::_getTypeString(IDiaSymbol *pType)
{
    QString sResult;

    // TODO

    return sResult;
}

DWORD QWinPDB::_getSymTag(IDiaSymbol *pSymbol)
{
    DWORD dwResult=0;

    pSymbol->get_symTag(&dwResult);

    return dwResult;
}

bool QWinPDB::getSymbolByID(DWORD dwID, IDiaSymbol **ppSymbol)
{
    bool bResult=false;

    IDiaEnumSymbols *pEnumSymbols;
    LONG nCount;
    if(pGlobal->findChildren(SymTagNull, NULL, nsNone, &pEnumSymbols)==S_OK)
    {
        if(pEnumSymbols->get_Count(&nCount)==S_OK)
        {
            if(nCount)
            {
                ULONG celt = 0;
                ULONG iMod = 1;
                while(SUCCEEDED(pEnumSymbols->Next(1, ppSymbol, &celt)) && (celt == 1))
                {
                    DWORD _dwID=0;

                    if(SUCCEEDED((*ppSymbol)->get_symIndexId(&_dwID)))
                    {
                        if(_dwID==dwID)
                        {
                            bResult=true;

                            break;
                        }
                    }

                    (*ppSymbol)->Release();
                }
            }
        }

        pEnumSymbols->Release();
    }

    return bResult;
}

QString QWinPDB::_handle(IDiaSymbol *pParent, QWinPDB::HANDLE_OPTIONS *pHandleOptions, SUBOPT subopt)
{
    QString sResult;

    // TODO classes with 2 publics
    bool bClass=false;
    bool bFunction=false;
    bool bTypedef=false;
    bool bData=false;
    bool bStruct=false;
    bool bUnion=false;
    bool bInterface=false;
    bool bEnum=false;
    int nOffset=0;
    int nSize=0;

    QString sName;

    DWORD dwSymTag=_getSymTag(pParent);
    if(dwSymTag==SymTagUDT)
    {
        RECORD_UDT record=_getRecordUDT(pParent);

        if(record._udtKind==0) // Struct
        {
            sName=record._name;
            bStruct=true;
        }
        else if(record._udtKind==1) // Class
        {
            sName=record._name;
            bClass=true;
        }
        else if(record._udtKind==2) // Union
        {
            sName=record._name;
            bUnion=true;
        }
        else if(record._udtKind==3) // Interface
        {
            sName=record._name;
            bInterface=true;
        }
    }
    else if(dwSymTag==SymTagFunction)
    {
        bFunction=true;

        RECORD_FUNCTION record=_getRecordFunction(pParent);

        if(record._undecoratedName!="")// TODO options
        {
            sName=record._undecoratedName;
        }
        else
        {
            sName=record._name;
        }
    }
    else if(dwSymTag==SymTagTypedef)
    {
        bTypedef=true;
        sResult+=indent(subopt.nIndent)+QString("%1\r\n").arg(rgTags[dwSymTag]);
    }
    else if(dwSymTag==SymTagData)
    {
        RECORD_DATA record=_getRecordData(pParent);

        sName=record._name;
        nOffset=record._offset;
        nSize=record.rtype.nSize;

        bData=true;
    }
    else if(dwSymTag==SymTagEnum)
    {
        RECORD_ENUM record=_getRecordEnum(pParent);

        sName=record._name;
        bEnum=true;
    }
    else if(dwSymTag==SymTagBaseClass)
    {
        RECORD_BASECLASS record=_getRecordBaseClass(pParent);

        sName=record._name;
        sResult+=indent(subopt.nIndent)+QString("Base class: %1\r\n").arg(record._name);
    }
    else
    {
        qFatal("Unknown type!!!");
        sResult+=indent(subopt.nIndent)+QString("%1\r\n").arg(rgTags[dwSymTag]);
    }

    // TODO if flags return

    bool bProcess=true;

    if(subopt.bNoFunctions&&bFunction)
    {
        bProcess=false;
    }

    if(bProcess)
    {
        if(bStruct)
        {
            sResult+=indent(subopt.nIndent)+QString("struct %1\r\n").arg(sName);
            sResult+=indent(subopt.nIndent)+QString("{\r\n");
        }
        if(bInterface)
        {
            sResult+=indent(subopt.nIndent)+QString("interface %1\r\n").arg(sName);
            sResult+=indent(subopt.nIndent)+QString("{\r\n");
        }
        if(bUnion)
        {
            sResult+=indent(subopt.nIndent)+QString("union %1\r\n").arg(sName);
            sResult+=indent(subopt.nIndent)+QString("{\r\n");
        }
        if(bEnum)
        {
            sResult+=indent(subopt.nIndent)+QString("enum %1\r\n").arg(sName);
            sResult+=indent(subopt.nIndent)+QString("{\r\n");
        }

        if(bFunction)
        {
            sResult+=indent(subopt.nIndent)+QString("%1;\r\n").arg(sName);
        }

        if(bData)
        {
            QString sComment;

//            if(pHandleOptions->bOffsets||pHandleOptions->bSizes)
//            {
//                sComment+=" //";
//                if(pHandleOptions->bOffsets)
//                {
//                    sComment+=QString(" Offset=%1").arg(nOffset);
//                }
//                if(pHandleOptions->bSizes)
//                {
//                    sComment+=QString(" Size=%1").arg(nSize);
//                }
//            }

            sResult+=indent(subopt.nIndent)+QString("%1;%2\r\n").arg(sName).arg(sComment);
        }

        if(bStruct||bUnion||bClass)
        {
            ELEMENT element=getElement(pParent);
            sResult+=elementToString(&element,pHandleOptions,subopt);
        }
        else if(!bTypedef) // TODO without enums
        {
            IDiaEnumSymbols *pEnumSymbols;
            if(pParent->findChildren(SymTagNull, nullptr, nsNone, &pEnumSymbols)==S_OK)
            {
                LONG nCount;
                if(pEnumSymbols->get_Count(&nCount)==S_OK)
                {
                    if(nCount)
                    {
                        IDiaSymbol *pSymbol;
                        ULONG celt = 0;

                        while(SUCCEEDED(pEnumSymbols->Next(1, &pSymbol, &celt)) && (celt == 1))
                        {
                            SUBOPT _subopt={};
                            _subopt.nIndent=subopt.nIndent+1;

                            if(bStruct)
                            {
                                _subopt.bNoFunctions=true;
                            }

                            sResult+=_handle(pSymbol,pHandleOptions,_subopt);

                            pSymbol->Release();
                        }
                    }
                }

                pEnumSymbols->Release();
            }
        }
        if(bStruct)
        {
            sResult+=indent(subopt.nIndent)+QString("};\r\n");
        }
        if(bUnion)
        {
            sResult+=indent(subopt.nIndent)+QString("};\r\n");
        }
        if(bInterface)
        {
            sResult+=indent(subopt.nIndent)+QString("};\r\n");
        }
        if(bEnum)
        {
            sResult+=indent(subopt.nIndent)+QString("};\r\n");
        }
    }

    return sResult;
}

QWinPDB::ELEMENT QWinPDB::getElement(IDiaSymbol *pParent)
{
    ELEMENT result={};

    DWORD dwSymTag=_getSymTag(pParent);
    if(dwSymTag==SymTagUDT)
    {
        RECORD_UDT record=_getRecordUDT(pParent);

        result.rtype.sName=record._name;
        result.rtype.nSize=record._length;

        if(record._udtKind==0)
        {
            result.elementType=ELEMENT_TYPE_STRUCT;
        }
        if(record._udtKind==1)
        {
            result.elementType=ELEMENT_TYPE_CLASS;
        }
        else if(record._udtKind==2)
        {
            result.elementType=ELEMENT_TYPE_UNION;
        }

        if((result.elementType==ELEMENT_TYPE_STRUCT)||(result.elementType==ELEMENT_TYPE_CLASS)||(result.elementType==ELEMENT_TYPE_UNION))
        {
            IDiaEnumSymbols *pEnumSymbols;
            LONG nCount;
            if(pParent->findChildren(SymTagNull, NULL, nsNone, &pEnumSymbols)==S_OK)
            {
                if(pEnumSymbols->get_Count(&nCount)==S_OK)
                {
                    if(nCount)
                    {
                        IDiaSymbol *pSymbol;
                        ULONG celt = 0;

                        int nCurrentOffset=0;
                        QList<QList<ELEMENT>> lstElements;
                        QList<ELEMENT> listCurrent;

                        while(SUCCEEDED(pEnumSymbols->Next(1, &pSymbol, &celt)) && (celt == 1))
                        {
                            DWORD dwSymTag=0;
                            if(SUCCEEDED(pSymbol->get_symTag(&dwSymTag)))
                            {
                                if(dwSymTag==SymTagData)
                                {
                                    ELEMENT record={};
                                    RECORD_DATA rd=_getRecordData(pSymbol);
                                    record.elementType=ELEMENT_TYPE_DATA;
                                    record.rtype=rd.rtype;

                                    if(nCurrentOffset==record.rtype.nOffset)
                                    {

                                    }
                                    else if(ALIGN_UP(nCurrentOffset,4)==record.rtype.nOffset)
                                    {
                                        // TODO alignments
                                    }
                                    else
                                    {
                                        lstElements.append(listCurrent);
                                        listCurrent.clear();
                                    }

                                    nCurrentOffset=record.rtype.nOffset+record.rtype.nSize;

                                    listCurrent.append(record);
                                }
                                else
                                {
                                    qDebug("Unknown member");
                                    qDebug(rgTags[dwSymTag]);
                                }
                            }
                            pSymbol->Release();
                        }

                        if(listCurrent.size())
                        {
                            lstElements.append(listCurrent);
                        }

                        if(lstElements.size()==1)
                        {
                            result.listChildren.append(lstElements.at(0));
                        }
                        else
                        {
                            int nCount=lstElements.size();
                            for(int i=0;i<nCount;i++)
                            {
                                result.listChildren.append(lstElements.at(i));
                            }
//                            int nCount=lstElements.size();
//                            for(int i=0;i<nCount;i++)
//                            {
//                                if(i) // Without the first element
//                                {
//                                    if(lstElements.at(i).count())
//                                    {
//                                        int nCurrentOffset=lstElements.at(i).at(0).nOffset;
//                                        // Find
//                                        for(int j=i-1;j<=nCount-j;j--)
//                                        {
//                                            int nPrevOffset=lstElements.at(j).at(0).nOffset;
//                                            if(nCurrentOffset>nPrevOffset) // mb TODO more checks
//                                            {
//                                                // 2 parts
//                                                QList<ELEMENT> listElems1;
//                                                QList<ELEMENT> listElems2;

//                                                for(int k=0;k<lstElements.at(j).count();k++)
//                                                {
//                                                    if(lstElements.at(j).at(k).nOffset<nCurrentOffset)
//                                                    {
//                                                        listElems1.append(lstElements.at(j).at(k));
//                                                    }
//                                                    else
//                                                    {
//                                                        listElems2.append(lstElements.at(j).at(k));
//                                                    }
//                                                }

//                                                lstElements.removeAt(j);

//                                                lstElements.insert(j,listElems1);

//                                                if(listElems2.count())
//                                                {
//                                                    lstElements.insert(j+1,listElems2);

//                                                    i++;
//                                                    nCount++;
//                                                }

//                                                break;
//                                            }
//                                        }
//                                    }
//                                }
//                            }

                        }

                        int z=0;
                        z++;
                    }
                }
                pEnumSymbols->Release();
            }
        }
    }

    return result;
}

QString QWinPDB::elementToString(QWinPDB::ELEMENT *pElement,HANDLE_OPTIONS *pHandleOptions,SUBOPT subopt)
{
    QString sResult;

//    if((pElement->elementType==ELEMENT_TYPE_STRUCT)||(pElement->elementType==ELEMENT_TYPE_CLASS)||(pElement->elementType==ELEMENT_TYPE_UNION))
//    {
//        QString sComment;
//        if(pHandleOptions->bSizes)
//        {
//            sComment=QString(" // Size=%1").arg(pElement->rtype.nSize);
//        }

//        if(pElement->elementType==ELEMENT_TYPE_STRUCT)
//        {
//            sResult+=indent(subopt.nIndent)+QString("struct %1%2\r\n").arg(pElement->rtype.sName).arg(sComment);
//        }
//        else if(pElement->elementType==ELEMENT_TYPE_CLASS)
//        {
//            sResult+=indent(subopt.nIndent)+QString("class %1%2\r\n").arg(pElement->rtype.sName).arg(sComment);
//        }
//        else if(pElement->elementType==ELEMENT_TYPE_UNION)
//        {
//            sResult+=indent(subopt.nIndent)+QString("union %1%2\r\n").arg(pElement->rtype.sName).arg(sComment);
//        }
//    }
//    else if(pElement->elementType==ELEMENT_TYPE_DATA)
//    {
//        QString sComment;
//        if(pHandleOptions->bSizes||pHandleOptions->bOffsets)
//        {
//            sComment+=QString(" //");

//            if(pHandleOptions->bSizes)
//            {
//                sComment+=QString(" Size=%1").arg(pElement->rtype.nSize);
//            }
//            if(pHandleOptions->bOffsets)
//            {
//                sComment+=QString(" Offset=%1").arg(pElement->rtype.nOffset);
//            }
//        }

//        QString sName=rtypeToString(pElement->rtype,pHandleOptions,b);

//        sResult+=indent(subopt.nIndent)+QString("%1;%2\r\n").arg(sName).arg(sComment);
//    }


//    if((pElement->elementType==ELEMENT_TYPE_STRUCT)||(pElement->elementType==ELEMENT_TYPE_CLASS)||(pElement->elementType==ELEMENT_TYPE_UNION))
//    {
//        sResult+=indent(subopt.nIndent)+QString("{\r\n");
//    }

//    for(int i=0;i<pElement->listChildren.count();i++)
//    {
//        SUBOPT _subopt;
//        _subopt.nIndent=subopt.nIndent+1;

//        QWinPDB::ELEMENT elChild=pElement->listChildren.at(i);
//        sResult+=elementToString(&elChild,pHandleOptions,_subopt);
//    }

//    if((pElement->elementType==ELEMENT_TYPE_STRUCT)||(pElement->elementType==ELEMENT_TYPE_CLASS)||(pElement->elementType==ELEMENT_TYPE_UNION))
//    {
//        sResult+=indent(subopt.nIndent)+QString("};\r\n");
//    }


    return sResult;
}

QString QWinPDB::rtypeToString(QWinPDB::RTYPE rtype, bool bIsStruct)
{
    QString sResult;

    if(!bIsStruct)
    {
        QString sAccess=getAccessString(rtype.nAccess);

        if(sAccess!="")
        {
            sResult+=QString("%1 ").arg(sAccess);
        }
    }

    if(rtype.bIsConst)
    {
        sResult+="const ";
    }

    if(rtype.type==RD_UDT)
    {
        sResult+=rtype.sTypeName;
    }
    else if(rtype.type==RD_ENUM)
    {
        sResult+=rtype.sTypeName;
    }
    else if(rtype.type==RD_BASETYPE)
    {
        sResult+=rtype.sTypeName;
    }

    bool bFuncPointer=(rtype.type==RD_FUNCTION)&&(rtype.nSize);

    if((rtype.type==RD_FUNCTION)&&(!bFuncPointer))
    {
        sResult+=rtype.sFunctionRet;
    }

    if(bFuncPointer)
    {
        sResult+=QString("%1 (").arg(rtype.sFunctionRet);
    }

    sResult+=" "; // TODO !!!

    if(rtype.bIsReference)
    {
        sResult+="&";
    }

    if(rtype.bIsPointer)
    {
        for(int i=0;i<rtype.nPointerDeep;i++)
        {
            sResult+="*";
        }
    }

    if(bFuncPointer)
    {
        sResult+=QString("%1)").arg(rtype.sName);
    }
    else
    {
        sResult+=rtype.sName;
    }

    if(rtype.bIsArray)
    {
        for(int i=rtype.listArrayCount.count()-1;i>=0;i--)
        {
            sResult+=QString("[%1]").arg(rtype.listArrayCount.at(i));
        }
    }

    else if(rtype.type==RD_FUNCTION)
    {
        sResult+="(";

        int nCount=rtype.listFunctionArgs.count();

        for(int i=0;i<nCount;i++)
        {
            sResult+=rtype.listFunctionArgs.at(i);

            if(i!=(nCount-1))
            {
                sResult+=",";
            }
        }

        sResult+=")";
    }

    if(rtype.nBitSize)
    {
        sResult+=QString(":%1").arg(rtype.nBitSize);
    }

    return sResult;
}

QString QWinPDB::getAccessString(int nAccess)
{
    QString sResult;

    switch(nAccess)
    {
        case 0: sResult=""; break;
        case 1: sResult="private"; break;
        case 2: sResult="protected"; break;
        case 3: sResult="public"; break;
    }

    return sResult;
}

QString QWinPDB::_getTab(int nLevel)
{
    QString sResult;

    for(int i=0;i<nLevel;i++)
    {
        sResult+="    ";
    }

    return sResult;
}

QWinPDB::~QWinPDB()
{
    qDebug("QWinPDB::~QWinPDB()");
    cleanup();
}

QWinPDB::PDB_INFO QWinPDB::getAllTags()
{
    PDB_INFO result={};

    IDiaEnumSymbols *pEnumSymbols;
    LONG nCount;
    if(pGlobal->findChildren(SymTagNull, NULL, nsNone, &pEnumSymbols)==S_OK)
    {
        if(pEnumSymbols->get_Count(&nCount)==S_OK)
        {
            if(nCount)
            {
                IDiaSymbol *pSymbol;
                ULONG celt = 0;

                QMap<QString,int> mapTypes;

                while(SUCCEEDED(pEnumSymbols->Next(1, &pSymbol, &celt)) && (celt == 1))
                {
                    DWORD dwSymTag=0;
                    if(SUCCEEDED(pSymbol->get_symTag(&dwSymTag)))
                    {
                        BSTR bstring;
                        if(dwSymTag==SymTagCompiland)
                        {
                            RECORD_COMPILAND record=_getRecordCompiland(pSymbol);

                            result.mapCompiland.insert(record._symIndexId,record);
                        }
                        else if(dwSymTag==SymTagData)
                        {
                            RECORD_DATA record=_getRecordData(pSymbol);

                            result.mapData.insert(record._symIndexId,record);
                        }
                        else if(dwSymTag==SymTagFunction)
                        {
                            RECORD_FUNCTION record=_getRecordFunction(pSymbol);

                            result.mapFunction.insert(record._symIndexId,record);
                        }
                        else if(dwSymTag==SymTagUDT)
                        {
                            RECORD_UDT record=_getRecordUDT(pSymbol);

                            switch(record._udtKind)
                            {
                                case 0: result.mapUDT_struct.insert(record._symIndexId,record);     break;
                                case 1: result.mapUDT_class.insert(record._symIndexId,record);      break;
                                case 2: result.mapUDT_union.insert(record._symIndexId,record);      break;
                                case 3: result.mapUDT_interface.insert(record._symIndexId,record);  break;
                            }
                        }
                        else if(dwSymTag==SymTagTypedef)
                        {
                            RECORD_TYPEDEF record=_getRecordTypeDef(pSymbol);

                            result.mapTypeDef.insert(record._symIndexId,record);
                        }
                        else if(dwSymTag==SymTagPublicSymbol)
                        {
                            RECORD_PUBLICSYMBOL record=_getRecordPublicSymbol(pSymbol);

                            result.mapPublicSymbol.insert(record._symIndexId,record);
                        }
                        else if(dwSymTag==SymTagEnum)
                        {
                            RECORD_ENUM record=_getRecordEnum(pSymbol);

                            result.mapEnum.insert(record._symIndexId,record);
                        }
                        else if(dwSymTag==SymTagPointerType)
                        {
                            RECORD_POINTERTYPE record=_getRecordPointerType(pSymbol);

                            result.mapPointerType.insert(record._symIndexId,record);
                        }
                        else if(dwSymTag==SymTagBaseClass)
                        {
                            RECORD_BASECLASS record=_getRecordBaseClass(pSymbol);

                            result.mapBaseClass.insert(record._symIndexId,record);
                        }
                        else if(dwSymTag==SymTagFunctionType)
                        {
                            RECORD_FUNCTIONTYPE record=_getRecordFunctionType(pSymbol);

                            result.mapFunctionType.insert(record._symIndexId,record);
                        }
                        else if(dwSymTag==SymTagArrayType)
                        {
                            RECORD_ARRAYTYPE record=_getRecordArrayType(pSymbol);

                            result.mapArrayType.insert(record._symIndexId,record);
                        }
                        else if(dwSymTag==SymTagVTable)
                        {
                            RECORD_VTABLE record=_getRecordVTable(pSymbol);

                            result.mapVTable.insert(record._symIndexId,record);
                        }
                        else if(dwSymTag==SymTagFunctionArgType)
                        {
                            RECORD_FUNCTIONARGTYPE record=_getRecordFunctionArgType(pSymbol);

                            result.mapFunctionArgType.insert(record._symIndexId,record);
                        }
                        else if(dwSymTag==SymTagBaseType)
                        {
                            RECORD_BASETYPE record=_getRecordBaseType(pSymbol);

                            result.mapBaseType.insert(record._symIndexId,record);
                        }
                        else
                        {
                            qDebug(rgTags[dwSymTag]);
                            int z=0;
                            z++;
                        }
//                        qDebug(rgTags[dwSymTag]);
                        mapTypes.insert(rgTags[dwSymTag],mapTypes.value(rgTags[dwSymTag],0)+1);
                    }
                    pSymbol->Release();
                }

                QList<QString> list=mapTypes.keys();

                for(int i=0;i<list.count();i++)
                {
                    QString sDebugString=QString("%1(%2)").arg(list.at(i)).arg(mapTypes.value(list.at(i)));
                    qDebug(sDebugString.toLatin1().data());
                }
            }
        }

        pEnumSymbols->Release();
    }

    return result;
}

QList<QWinPDB::SYMBOL_RECORD> QWinPDB::getUDTList(DWORD dwKind)
{
    QList<SYMBOL_RECORD> listResult;

    IDiaEnumSymbols *pEnumSymbols;
    LONG nCount;
    if(pGlobal->findChildren(SymTagUDT, nullptr, nsNone, &pEnumSymbols)==S_OK)
    {
        if(pEnumSymbols->get_Count(&nCount)==S_OK)
        {
            if(nCount)
            {
                IDiaSymbol *pSymbol;
                ULONG celt = 0;
                ULONG iMod = 1;

                QMap<QString,int> mapTypes;

                while(SUCCEEDED(pEnumSymbols->Next(1, &pSymbol, &celt)) && (celt == 1))
                {
                    DWORD _dwKind=0;
                    pSymbol->get_udtKind(&_dwKind);

                    if(dwKind==_dwKind)
                    {
                        BSTR bstring;
                        SYMBOL_RECORD record={};

                        pSymbol->get_symIndexId(&record.dwID);
                        if(pSymbol->get_name(&bstring)==S_OK) {record.sName=QString::fromWCharArray(bstring);        SysFreeString(bstring);}

                        listResult.append(record);
                    }

                    pSymbol->Release();
                }
            }
        }

        pEnumSymbols->Release();
    }

    return listResult;
}
QList<QWinPDB::SYMBOL_RECORD> QWinPDB::getClasses()
{
    return getUDTList(1);
}

QWinPDB::STATS QWinPDB::getStats()
{
    STATS result={};

    QMap<QString,qint64> mapUDT;

    IDiaEnumSymbols *pEnumSymbols;
    LONG nCount;
//    if(pGlobal->findChildren(SymTagUDT, nullptr, nsNone, &pEnumSymbols)==S_OK)
    if(pGlobal->findChildren(SymTagNull, nullptr, nsNone, &pEnumSymbols)==S_OK)
    {
        if(pEnumSymbols->get_Count(&nCount)==S_OK)
        {
            if(nCount)
            {
                IDiaSymbol *pSymbol;
                ULONG celt = 0;
                ULONG iMod = 1;

                QMap<QString,int> mapTypes;

                while(SUCCEEDED(pEnumSymbols->Next(1, &pSymbol, &celt)) && (celt == 1))
                {
                    SYMBOL_RECORD record={};
                    BSTR bstring;
                    DWORD dwSymTag=0;
                    pSymbol->get_symTag(&dwSymTag);

                    if(dwSymTag==SymTagUDT)
                    {
                        DWORD dwParentClass=0;

                        pSymbol->get_classParentId(&dwParentClass);

                        if(dwParentClass==0)
                        {
                            ULONGLONG nLen=0;
                            pSymbol->get_length(&nLen);
                            if(pSymbol->get_name(&bstring)==S_OK)
                            {
                                record.sName=QString::fromWCharArray(bstring);
                                SysFreeString(bstring);
                            }

                            if(mapUDT.value(record.sName,-1)!=nLen)
                            {
                                DWORD _dwKind=0;
                                pSymbol->get_udtKind(&_dwKind);
                                pSymbol->get_symIndexId(&record.dwID);

                                if(_dwKind==0)
                                {
                                    record.type=SYMBOL_TYPE_STRUCT;
                                }
                                else if(_dwKind==1)
                                {
                                    record.type=SYMBOL_TYPE_CLASS;
                                }
                                else if(_dwKind==2)
                                {
                                    record.type=SYMBOL_TYPE_UNION;
                                }
                                else if(_dwKind==3)
                                {
                                    record.type=SYMBOL_TYPE_INTERFACE;
                                }

                                result.listSymbols.append(record);
                            }
                        }

                    }
                    else if(dwSymTag==SymTagEnum)
                    {
                        DWORD dwParentClass=0;

                        pSymbol->get_classParentId(&dwParentClass);

                        if(dwParentClass==0)
                        {
                            if(pSymbol->get_name(&bstring)==S_OK)
                            {
                                record.sName=QString::fromWCharArray(bstring);
                                SysFreeString(bstring);
                            }
                            pSymbol->get_symIndexId(&record.dwID);
                            record.type=SYMBOL_TYPE_ENUM;

                            result.listSymbols.append(record);
                        }
                    }

                    pSymbol->Release();
                }
            }
        }

        pEnumSymbols->Release();
    }

    qSort(result.listSymbols.begin(),result.listSymbols.end(),sortLessThan);

    return result;
}

QString QWinPDB::handle(quint32 nID, QWinPDB::HANDLE_OPTIONS *pHandleOptions)
{
    QString sResult;

    IDiaSymbol *pParent=nullptr;

    if(getSymbolByID(nID,&pParent))
    {
        SUBOPT subopt={};
        subopt.nIndent=0;
        sResult+=_handle(pParent,pHandleOptions,subopt);

        pParent->Release();
    }

    return sResult;
}

QWinPDB::ELEM QWinPDB::getElem(quint32 nID)
{
    ELEM result={};

    IDiaSymbol *pParent=nullptr;

    if(getSymbolByID(nID,&pParent))
    {
        result=_getElem(pParent);

        pParent->Release();
    }

    return result;
}

QWinPDB::ELEM QWinPDB::_getElem(IDiaSymbol *pParent)
{
    ELEM result={};

    bool bChildren=true;

    DWORD dwSymTag=_getSymTag(pParent);
    if(dwSymTag==SymTagUDT)
    {
        result.elemType=ELEM_TYPE_UDT;
        result._udt=_getRecordUDT(pParent);
        result.dwSize=result._udt._length;
    }
    else if(dwSymTag==SymTagFunction)
    {
        result.elemType=ELEM_TYPE_FUNCTION;
        result._function=_getRecordFunction(pParent);
        result.dwSize=result._function._length; // TODO Check!
    }
    else if(dwSymTag==SymTagTypedef)
    {
        result.elemType=ELEM_TYPE_TYPEDEF;
        result._typedef=_getRecordTypeDef(pParent);
        bChildren=false;
    }
    else if(dwSymTag==SymTagData)
    {
        result.elemType=ELEM_TYPE_DATA;
        result._data=_getRecordData(pParent);
        result.dwSize=result._data.rtype.nSize;
        result.dwOffset=result._data.rtype.nOffset;

        if(result._data.rtype.bIsPointer||result._data.rtype.bIsReference)
        {
            bChildren=false;
        }
    }
    else if(dwSymTag==SymTagEnum)
    {
        result.elemType=ELEM_TYPE_ENUM;
        result._enum=_getRecordEnum(pParent);
    }
    else if(dwSymTag==SymTagBaseClass)
    {
        result.elemType=ELEM_TYPE_BASECLASS;
        result._baseclass=_getRecordBaseClass(pParent);
        bChildren=false;
    }
    else if(dwSymTag==SymTagVTable)
    {
        result.elemType=ELEM_TYPE_VTABLE;
        result._vtable=_getRecordVTable(pParent);
    }
    else if(dwSymTag==SymTagFuncDebugStart)
    {
        result.elemType=ELEM_TYPE_FUNCDEBUGSTART;
        result._funcdebugstart=_getRecordFuncDebugStart(pParent);
    }
    else if(dwSymTag==SymTagFuncDebugEnd)
    {
        result.elemType=ELEM_TYPE_FUNCDEBUGEND;
        result._funcdebugend=_getRecordFuncDebugEnd(pParent);
    }
    else if(dwSymTag==SymTagCallSite)
    {
        result.elemType=ELEM_TYPE_CALLSITE;
        qDebug("SymTagCallSite");
    }
    else if(dwSymTag==SymTagLabel)
    {
        result.elemType=ELEM_TYPE_LABEL;
        result._label=_getRecordLabel(pParent);
    }
    else if(dwSymTag==SymTagBlock)
    {
        result.elemType=ELEM_TYPE_BLOCK;
        result._block=_getRecordBlock(pParent);
    }
    else
    {
        qFatal(rgTags[dwSymTag]);
    }

    if(bChildren)
    {
        IDiaEnumSymbols *pEnumSymbols;
        if(pParent->findChildren(SymTagNull, nullptr, nsNone, &pEnumSymbols)==S_OK)
        {
            LONG nCount;
            if(pEnumSymbols->get_Count(&nCount)==S_OK)
            {
                if(nCount)
                {
                    IDiaSymbol *pSymbol;
                    ULONG celt = 0;

                    while(SUCCEEDED(pEnumSymbols->Next(1, &pSymbol, &celt)) && (celt == 1))
                    {
                        ELEM elemChild=_getElem(pSymbol);
                        result.listChildren.append(elemChild);

//                        QString sTest;
//                        BSTR bstring=nullptr;
//                        if(pSymbol->get_name(&bstring)==S_OK) {sTest=QString::fromWCharArray(bstring);   SysFreeString(bstring);}

//                        qDebug(sTest.toLatin1().data());

                        pSymbol->Release();
                    }
                }
            }

            pEnumSymbols->Release();
        }
    }

    return result;
}

QString QWinPDB::elemToString(const ELEM *pElem, HANDLE_OPTIONS *pHandleOptions, int nLevel, bool bIsStruct)
{
    QString sResult;

    // TODO TYPEDEFS !!!

    if(pElem->elemType==ELEM_TYPE_ENUM)
    {
        sResult+=_getTab(nLevel)+QString("enum %1\r\n").arg(pElem->_enum._name);
        sResult+=_getTab(nLevel)+"{\r\n";

        int nCount=pElem->listChildren.count();

        for(int i=0;i<nCount;i++)
        {
            sResult+=_getTab(nLevel+1)+QString("%1=%2").arg(pElem->listChildren.at(i)._data._name).arg(pElem->listChildren.at(i)._data.value.vValue.toString());

            if(i!=(nCount-1))
            {
                sResult+=",";
            }
            sResult+="\r\n";
        }

        sResult+=_getTab(nLevel)+"};\r\n";
    }
    else if((pElem->elemType==ELEM_TYPE_UDT)||(pElem->elemType==ELEM_TYPE_FAKEUNION))
    {
        sResult+=_getTab(nLevel)+QString("%1 %2").arg(pElem->_udt.sType).arg(pElem->_udt._name);

        QList<QString> listBaseClasses;

        int nChildrenCount=pElem->listChildren.count();

        // Get basic clasess
        for(int i=0;i<nChildrenCount;i++)
        {
            if(pElem->listChildren.at(i).elemType==ELEM_TYPE_BASECLASS)
            {
                listBaseClasses.append(QString("%1 %2").arg(getAccessString(pElem->listChildren.at(i)._baseclass._access)).arg(pElem->listChildren.at(i)._baseclass._name));
            }
        }

        int nBaseClassCount=listBaseClasses.count();

        for(int i=0;i<nBaseClassCount;i++)
        {
            if(i==0)
            {
                sResult+=QString(" : ");
            }

            sResult+=listBaseClasses.at(i);

            if(i!=nBaseClassCount-1)
            {
                sResult+=QString(", ");
            }
        }

        if(pHandleOptions->bShowComments)
        {
            sResult+=QString("// Size=0x%1").arg(pElem->dwSize,0,16);
        }

        sResult+="\r\n";
        sResult+=_getTab(nLevel)+"{\r\n";

        // TODO typedefs
        for(int i=0;i<nChildrenCount;i++)
        {
            if(pElem->listChildren.at(i).elemType!=ELEM_TYPE_BASECLASS)
            {
                sResult+=_getTab(nLevel)+elemToString(&(pElem->listChildren.at(i)),pHandleOptions,nLevel+1,(pElem->_udt._udtKind==0));

                if(pHandleOptions->bShowComments)
                {
                    if(pElem->listChildren.at(i).dwSize)
                    {
                        sResult+=QString("// Offset=0x%1 Size=0x%2").arg(pElem->listChildren.at(i).dwOffset,0,16).arg(pElem->listChildren.at(i).dwSize,0,16);
                    }

                    if(pElem->listChildren.at(i)._data.rtype.nBitSize)
                    {
                        sResult+=QString(" BitOffset=0x%1 BitSize=0x%2").arg(pElem->listChildren.at(i)._data.rtype.nBitOffset).arg(pElem->listChildren.at(i)._data.rtype.nBitSize);
                    }
                }

                if( (pElem->listChildren.at(i).elemType!=ELEM_TYPE_UDT)&&
                    (pElem->listChildren.at(i).elemType!=ELEM_TYPE_ENUM))
                {
                    sResult+=";\r\n";
                }
            }
        }

        sResult+=_getTab(nLevel)+"};\r\n";
    }
    else if(pElem->elemType==ELEM_TYPE_DATA)
    {
        sResult+=_getTab(nLevel)+rtypeToString(pElem->_data.rtype,bIsStruct);

        if(pElem->_data.value.bIsValid) // TODO Check
        {
            sResult+=QString("=%1").arg(pElem->_data.value.vValue.toString());
        }
    }
    else if(pElem->elemType==ELEM_TYPE_FUNCTION)
    {
        sResult+=_getTab(nLevel)+rtypeToString(pElem->_function.rtype,bIsStruct);
        // TODO function start,end
    }
    else if(pElem->elemType==ELEM_TYPE_TYPEDEF)
    {
        // TODO Check typedef
        qDebug("TYPEDEF");
    }
    else
    {
        qDebug("Unknown ELEM_TYPE");
    }

    // TODO if struct has basic class -> interface

    return sResult;
}

void QWinPDB::cleanup()
{
    if(pGlobal)
    {
        pGlobal->Release();
        pGlobal=nullptr;
    }

    if(pDiaSession)
    {
        pDiaSession->Release();
        pDiaSession=nullptr;
    }

    if(pDiaDataSource)
    {
        pDiaDataSource->Release();
        pDiaDataSource=nullptr;
    }
}
