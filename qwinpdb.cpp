// copyright (c) 2020-2021 hors<horsicq@gmail.com>
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

// TODO mb add typedefs

bool sortLessThan(const QWinPDB::SYMBOL_RECORD &v1, const QWinPDB::SYMBOL_RECORD &v2)
{
    return v1.dwID<v2.dwID;
}

bool sortElemInfoID(const QWinPDB::ELEM_INFO &v1, const QWinPDB::ELEM_INFO &v2)
{
    return v1.baseInfo.nID<v2.baseInfo.nID;
}

bool sortElemInfoName(const QWinPDB::ELEM_INFO &v1, const QWinPDB::ELEM_INFO &v2)
{
    QCollator collator;

    bool bResult=collator.compare(v1.baseInfo.sName,v2.baseInfo.sName)<0;

    return bResult;
}

bool sortElemInfoDeps(const QWinPDB::ELEM_INFO &v1, const QWinPDB::ELEM_INFO &v2)
{
    bool bResult=true;

    int nCount=v1.listChildrenBaseInfos.count();

    for(int i=0;i<nCount;i++)
    {
        if(v1.listChildrenBaseInfos.at(i).nTypeID)
        {
            if(v2.baseInfo.sTypeName==v1.listChildrenBaseInfos.at(i).sTypeName)
            {
                bResult=false;

                break;
            }
        }
    }

    return bResult;
}

QWinPDB::QWinPDB(QObject *parent) : QObject(parent)
{
    pDiaDataSource=nullptr;
    pGlobal=nullptr;
    pDiaSession=nullptr;
    setProcessEnable(true);
}

QWinPDB::HANDLE_OPTIONS QWinPDB::getDefaultHandleOptions()
{
    HANDLE_OPTIONS result={};

    result.bFixTypes=false;
    result.bAddAlignment=false;
    result.bShowComments=false;
//    result.fixOffsets=FO_STRUCTSANDUNIONS;
    result.fixOffsets=FO_NO;
    result.sortType=ST_ID;
    result.exportType=ET_CPLUSPLUS;

    return result;
}

bool QWinPDB::loadFromFile(QString sFileName)
{
    cleanup();

    // TODO msdia option

    HRESULT hr=NoRegCoCreate(L"msdia140.dll", _uuidof(DiaSourceAlt),
                              _uuidof(IDiaDataSource),
                              (void **)(&pDiaDataSource));

//    HRESULT hr=NoRegCoCreate(L"msdia120.dll", _uuidof(DiaSourceAlt),
//                              _uuidof(IDiaDataSource),
//                              (void **)(&pDiaDataSource));

    if(FAILED(hr))
    {
        emit errorMessage(tr("Cannot load msdia library"));

        return false;
    }

    QByteArray baBuffer; // TODO function
    baBuffer.resize((sFileName.length()+1)*2);
    baBuffer.fill(0);
    wchar_t *pwszFileName=(wchar_t *)baBuffer.data();
    sFileName.toWCharArray(pwszFileName);
    hr=pDiaDataSource->loadDataFromPdb(pwszFileName);

    if(FAILED(hr))
    {
        emit errorMessage(tr("Cannot load data from PDB"));

        return false;
    }

    hr=pDiaDataSource->openSession(&pDiaSession);

    if(FAILED(hr))
    {
        emit errorMessage(tr("Cannot open session"));

        return false;
    }

    // TODO Exe info
    hr=pDiaSession->get_globalScope(&pGlobal);

    BSTR bstrName;

    if(pGlobal->get_name(&bstrName)!=S_OK)
    {
        emit errorMessage(tr("Cannot get PDB name"));

        return false;
    }

    DWORD dwAge;
    hr=pGlobal->get_age(&dwAge);

    if(hr!=S_OK)
    {
        emit errorMessage(tr("Cannot get PDB age"));

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
            default:            emit infoMessage(QString("Unknown VARIANT"));
        }
    }

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
        default:            emit infoMessage(QString("Unknown VARIANT"));
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

QWinPDB::RECORD_FUNCTION QWinPDB::_getRecordFunction(IDiaSymbol *pSymbol,QWinPDB::HANDLE_OPTIONS *pHandleOptions)
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

    result.rtype=getSymbolType(pSymbol,pHandleOptions);
    result.rtype.sName=result._name;
    result.rtype.nAccess=result._access;

    // TODO BitFields

    return result;
}

QWinPDB::RECORD_DATA QWinPDB::_getRecordData(IDiaSymbol *pSymbol,QWinPDB::HANDLE_OPTIONS *pHandleOptions)
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

    result.rtype=getSymbolType(pSymbol,pHandleOptions);
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

QWinPDB::RECORD_CALLSITE QWinPDB::_getRecordCallSite(IDiaSymbol *pSymbol, HANDLE_OPTIONS *pHandleOptions)
{
    RECORD_CALLSITE result={};

    pSymbol->get_addressOffset(&result._addressOffset);
    pSymbol->get_addressSection(&result._addressSection);
    pSymbol->get_lexicalParentId(&result._lexicalParentId);
    pSymbol->get_relativeVirtualAddress(&result._relativeVirtualAddress);
    pSymbol->get_symIndexId(&result._symIndexId);
    pSymbol->get_symTag(&result._symTag);

    result.rtype=getSymbolType(pSymbol,pHandleOptions);

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

void QWinPDB::_checkSymbol(IDiaSymbol *pSymbol)
{
    DWORD dwTest=0;
    BSTR bstrTest=nullptr;
    IDiaSymbol *diaSymbol=nullptr;
    IDiaLineNumber *diaLineNumber=nullptr;
    ULONGLONG llTest=0;
    LONG lTest=0;
    BOOL bTest=0;
    VARIANT vTest;
    GUID gTest;

    if(pSymbol->get_symIndexId(&dwTest)==S_OK) qDebug("get_symIndexId");
    if(pSymbol->get_symTag(&dwTest)==S_OK) qDebug("get_symTag");
    if(pSymbol->get_name(&bstrTest)==S_OK) qDebug("get_name");
    if(pSymbol->get_lexicalParent(&diaSymbol)==S_OK) qDebug("get_lexicalParent");
    if(pSymbol->get_classParent(&diaSymbol)==S_OK) qDebug("get_classParent");
    if(pSymbol->get_type(&diaSymbol)==S_OK) qDebug("get_type");
    if(pSymbol->get_dataKind(&dwTest)==S_OK) qDebug("get_dataKind");
    if(pSymbol->get_locationType(&dwTest)==S_OK) qDebug("get_locationType");
    if(pSymbol->get_addressSection(&dwTest)==S_OK) qDebug("get_addressSection");
    if(pSymbol->get_addressOffset(&dwTest)==S_OK) qDebug("get_addressOffset");
    if(pSymbol->get_relativeVirtualAddress(&dwTest)==S_OK) qDebug("get_relativeVirtualAddress");
    if(pSymbol->get_virtualAddress(&llTest)==S_OK) qDebug("get_virtualAddress");
    if(pSymbol->get_registerId(&dwTest)==S_OK) qDebug("get_registerId");
    if(pSymbol->get_offset(&lTest)==S_OK) qDebug("get_offset");
    if(pSymbol->get_length(&llTest)==S_OK) qDebug("get_length");
    if(pSymbol->get_slot(&dwTest)==S_OK) qDebug("get_slot");
    if(pSymbol->get_volatileType(&bTest)==S_OK) qDebug("get_volatileType");
    if(pSymbol->get_constType(&bTest)==S_OK) qDebug("get_constType");
    if(pSymbol->get_unalignedType(&bTest)==S_OK) qDebug("get_unalignedType");
    if(pSymbol->get_access(&dwTest)==S_OK) qDebug("get_access");
    if(pSymbol->get_libraryName(&bstrTest)==S_OK) qDebug("get_libraryName");
    if(pSymbol->get_platform(&dwTest)==S_OK) qDebug("get_platform");
    if(pSymbol->get_language(&dwTest)==S_OK) qDebug("get_language");
    if(pSymbol->get_editAndContinueEnabled(&bTest)==S_OK) qDebug("get_editAndContinueEnabled");
    if(pSymbol->get_frontEndMajor(&dwTest)==S_OK) qDebug("get_frontEndMajor");
    if(pSymbol->get_frontEndMinor(&dwTest)==S_OK) qDebug("get_frontEndMinor");
    if(pSymbol->get_frontEndBuild(&dwTest)==S_OK) qDebug("get_frontEndBuild");
    if(pSymbol->get_backEndMajor(&dwTest)==S_OK) qDebug("get_backEndMajor");
    if(pSymbol->get_backEndMinor(&dwTest)==S_OK) qDebug("get_backEndMinor");
    if(pSymbol->get_backEndBuild(&dwTest)==S_OK) qDebug("get_backEndBuild");
    if(pSymbol->get_sourceFileName(&bstrTest)==S_OK) qDebug("get_sourceFileName");
    if(pSymbol->get_unused(&bstrTest)==S_OK) qDebug("get_unused");
    if(pSymbol->get_thunkOrdinal(&dwTest)==S_OK) qDebug("get_thunkOrdinal");
    if(pSymbol->get_thisAdjust(&lTest)==S_OK) qDebug("get_thisAdjust");
    if(pSymbol->get_virtualBaseOffset(&dwTest)==S_OK) qDebug("get_virtualBaseOffset");
    if(pSymbol->get_virtual(&bTest)==S_OK) qDebug("get_virtual");
    if(pSymbol->get_intro(&bTest)==S_OK) qDebug("get_intro");
    if(pSymbol->get_pure(&bTest)==S_OK) qDebug("get_pure");
    if(pSymbol->get_callingConvention(&dwTest)==S_OK) qDebug("get_callingConvention");
    if(pSymbol->get_value(&vTest)==S_OK) qDebug("get_value");
    if(pSymbol->get_baseType(&dwTest)==S_OK) qDebug("get_baseType");
    if(pSymbol->get_token(&dwTest)==S_OK) qDebug("get_token");
    if(pSymbol->get_timeStamp(&dwTest)==S_OK) qDebug("get_timeStamp");
    if(pSymbol->get_guid(&gTest)==S_OK) qDebug("get_guid");
    if(pSymbol->get_symbolsFileName(&bstrTest)==S_OK) qDebug("get_symbolsFileName");
    if(pSymbol->get_reference(&bTest)==S_OK) qDebug("get_reference");
    if(pSymbol->get_count(&dwTest)==S_OK) qDebug("get_count");
    if(pSymbol->get_bitPosition(&dwTest)==S_OK) qDebug("get_bitPosition");
    if(pSymbol->get_arrayIndexType(&diaSymbol)==S_OK) qDebug("get_arrayIndexType");
    if(pSymbol->get_packed(&bTest)==S_OK) qDebug("get_packed");
    if(pSymbol->get_constructor(&bTest)==S_OK) qDebug("get_constructor");
    if(pSymbol->get_overloadedOperator(&bTest)==S_OK) qDebug("get_overloadedOperator");
    if(pSymbol->get_nested(&bTest)==S_OK) qDebug("get_nested");
    if(pSymbol->get_hasNestedTypes(&bTest)==S_OK) qDebug("get_hasNestedTypes");
    if(pSymbol->get_hasAssignmentOperator(&bTest)==S_OK) qDebug("get_hasAssignmentOperator");
    if(pSymbol->get_hasCastOperator(&bTest)==S_OK) qDebug("get_hasCastOperator");
    if(pSymbol->get_scoped(&bTest)==S_OK) qDebug("get_scoped");
    if(pSymbol->get_virtualBaseClass(&bTest)==S_OK) qDebug("get_virtualBaseClass");
    if(pSymbol->get_indirectVirtualBaseClass(&bTest)==S_OK) qDebug("get_indirectVirtualBaseClass");
    if(pSymbol->get_virtualBasePointerOffset(&lTest)==S_OK) qDebug("get_virtualBasePointerOffset");
    if(pSymbol->get_virtualTableShape(&diaSymbol)==S_OK) qDebug("get_virtualTableShape");
    if(pSymbol->get_lexicalParentId(&dwTest)==S_OK) qDebug("get_lexicalParentId");
    if(pSymbol->get_classParentId(&dwTest)==S_OK) qDebug("get_classParentId");
    if(pSymbol->get_typeId(&dwTest)==S_OK) qDebug("get_typeId");
    if(pSymbol->get_arrayIndexTypeId(&dwTest)==S_OK) qDebug("get_arrayIndexTypeId");
    if(pSymbol->get_virtualTableShapeId(&dwTest)==S_OK) qDebug("get_virtualTableShapeId");
    if(pSymbol->get_code(&bTest)==S_OK) qDebug("get_code");
    if(pSymbol->get_function(&bTest)==S_OK) qDebug("get_function");
    if(pSymbol->get_managed(&bTest)==S_OK) qDebug("get_managed");
    if(pSymbol->get_msil(&bTest)==S_OK) qDebug("get_msil");
    if(pSymbol->get_virtualBaseDispIndex(&dwTest)==S_OK) qDebug("get_virtualBaseDispIndex");
    if(pSymbol->get_undecoratedName(&bstrTest)==S_OK) qDebug("get_undecoratedName");
    if(pSymbol->get_age(&dwTest)==S_OK) qDebug("get_age");
    if(pSymbol->get_signature(&dwTest)==S_OK) qDebug("get_signature");
    if(pSymbol->get_compilerGenerated(&bTest)==S_OK) qDebug("get_compilerGenerated");
    if(pSymbol->get_addressTaken(&bTest)==S_OK) qDebug("get_addressTaken");
    if(pSymbol->get_rank(&dwTest)==S_OK) qDebug("get_rank");
    if(pSymbol->get_lowerBound(&diaSymbol)==S_OK) qDebug("get_lowerBound");
    if(pSymbol->get_upperBound(&diaSymbol)==S_OK) qDebug("get_upperBound");
    if(pSymbol->get_lowerBoundId(&dwTest)==S_OK) qDebug("get_lowerBoundId");
    if(pSymbol->get_upperBoundId(&dwTest)==S_OK) qDebug("get_upperBoundId");
    if(pSymbol->get_targetSection(&dwTest)==S_OK) qDebug("get_targetSection");
    if(pSymbol->get_targetOffset(&dwTest)==S_OK) qDebug("get_targetOffset");
    if(pSymbol->get_targetRelativeVirtualAddress(&dwTest)==S_OK) qDebug("get_targetRelativeVirtualAddress");
    if(pSymbol->get_targetVirtualAddress(&llTest)==S_OK) qDebug("get_targetVirtualAddress");
    if(pSymbol->get_machineType(&dwTest)==S_OK) qDebug("get_machineType");
    if(pSymbol->get_oemId(&dwTest)==S_OK) qDebug("get_oemId");
    if(pSymbol->get_oemSymbolId(&dwTest)==S_OK) qDebug("get_oemSymbolId");
    if(pSymbol->get_objectPointerType(&diaSymbol)==S_OK) qDebug("get_objectPointerType");
    if(pSymbol->get_udtKind(&dwTest)==S_OK) qDebug("get_udtKind");
    if(pSymbol->get_noReturn(&bTest)==S_OK) qDebug("get_noReturn");
    if(pSymbol->get_customCallingConvention(&bTest)==S_OK) qDebug("get_customCallingConvention");
    if(pSymbol->get_noInline(&bTest)==S_OK) qDebug("get_noInline");
    if(pSymbol->get_optimizedCodeDebugInfo(&bTest)==S_OK) qDebug("get_optimizedCodeDebugInfo");
    if(pSymbol->get_notReached(&bTest)==S_OK) qDebug("get_notReached");
    if(pSymbol->get_interruptReturn(&bTest)==S_OK) qDebug("get_interruptReturn");
    if(pSymbol->get_farReturn(&bTest)==S_OK) qDebug("get_farReturn");
    if(pSymbol->get_isStatic(&bTest)==S_OK) qDebug("get_isStatic");
    if(pSymbol->get_hasDebugInfo(&bTest)==S_OK) qDebug("get_hasDebugInfo");
    if(pSymbol->get_isLTCG(&bTest)==S_OK) qDebug("get_isLTCG");
    if(pSymbol->get_isDataAligned(&bTest)==S_OK) qDebug("get_isDataAligned");
    if(pSymbol->get_hasSecurityChecks(&bTest)==S_OK) qDebug("get_hasSecurityChecks");
    if(pSymbol->get_compilerName(&bstrTest)==S_OK) qDebug("get_compilerName");
    if(pSymbol->get_hasAlloca(&bTest)==S_OK) qDebug("get_hasAlloca");
    if(pSymbol->get_hasSetJump(&bTest)==S_OK) qDebug("get_hasSetJump");
    if(pSymbol->get_hasLongJump(&bTest)==S_OK) qDebug("get_hasLongJump");
    if(pSymbol->get_hasInlAsm(&bTest)==S_OK) qDebug("get_hasInlAsm");
    if(pSymbol->get_hasEH(&bTest)==S_OK) qDebug("get_hasEH");
    if(pSymbol->get_hasSEH(&bTest)==S_OK) qDebug("get_hasSEH");
    if(pSymbol->get_hasEHa(&bTest)==S_OK) qDebug("get_hasEHa");
    if(pSymbol->get_isNaked(&bTest)==S_OK) qDebug("get_isNaked");
    if(pSymbol->get_isAggregated(&bTest)==S_OK) qDebug("get_isAggregated");
    if(pSymbol->get_isSplitted(&bTest)==S_OK) qDebug("get_isSplitted");
    if(pSymbol->get_container(&diaSymbol)==S_OK) qDebug("get_container");
    if(pSymbol->get_inlSpec(&bTest)==S_OK) qDebug("get_inlSpec");
    if(pSymbol->get_noStackOrdering(&bTest)==S_OK) qDebug("get_noStackOrdering");
    if(pSymbol->get_virtualBaseTableType(&diaSymbol)==S_OK) qDebug("get_virtualBaseTableType");
    if(pSymbol->get_hasManagedCode(&bTest)==S_OK) qDebug("get_hasManagedCode");
    if(pSymbol->get_isHotpatchable(&bTest)==S_OK) qDebug("get_isHotpatchable");
    if(pSymbol->get_isCVTCIL(&bTest)==S_OK) qDebug("get_isCVTCIL");
    if(pSymbol->get_isMSILNetmodule(&bTest)==S_OK) qDebug("get_isMSILNetmodule");
    if(pSymbol->get_isCTypes(&bTest)==S_OK) qDebug("get_isCTypes");
    if(pSymbol->get_isStripped(&bTest)==S_OK) qDebug("get_isStripped");
    if(pSymbol->get_frontEndQFE(&dwTest)==S_OK) qDebug("get_frontEndQFE");
    if(pSymbol->get_backEndQFE(&dwTest)==S_OK) qDebug("get_backEndQFE");
    if(pSymbol->get_wasInlined(&bTest)==S_OK) qDebug("get_wasInlined");
    if(pSymbol->get_strictGSCheck(&bTest)==S_OK) qDebug("get_strictGSCheck");
    if(pSymbol->get_isCxxReturnUdt(&bTest)==S_OK) qDebug("get_isCxxReturnUdt");
    if(pSymbol->get_isConstructorVirtualBase(&bTest)==S_OK) qDebug("get_isConstructorVirtualBase");
    if(pSymbol->get_RValueReference(&bTest)==S_OK) qDebug("get_RValueReference");
    if(pSymbol->get_unmodifiedType(&diaSymbol)==S_OK) qDebug("get_unmodifiedType");
    if(pSymbol->get_framePointerPresent(&bTest)==S_OK) qDebug("get_framePointerPresent");
    if(pSymbol->get_isSafeBuffers(&bTest)==S_OK) qDebug("get_isSafeBuffers");
    if(pSymbol->get_intrinsic(&bTest)==S_OK) qDebug("get_intrinsic");
    if(pSymbol->get_sealed(&bTest)==S_OK) qDebug("get_sealed");
    if(pSymbol->get_hfaFloat(&bTest)==S_OK) qDebug("get_hfaFloat");
    if(pSymbol->get_hfaDouble(&bTest)==S_OK) qDebug("get_hfaDouble");
    if(pSymbol->get_liveRangeStartAddressSection(&dwTest)==S_OK) qDebug("get_liveRangeStartAddressSection");
    if(pSymbol->get_liveRangeStartAddressOffset(&dwTest)==S_OK) qDebug("get_liveRangeStartAddressOffset");
    if(pSymbol->get_liveRangeStartRelativeVirtualAddress(&dwTest)==S_OK) qDebug("get_liveRangeStartRelativeVirtualAddress");
    if(pSymbol->get_countLiveRanges(&dwTest)==S_OK) qDebug("get_countLiveRanges");
    if(pSymbol->get_liveRangeLength(&llTest)==S_OK) qDebug("get_liveRangeLength");
    if(pSymbol->get_offsetInUdt(&dwTest)==S_OK) qDebug("get_offsetInUdt");
    if(pSymbol->get_paramBasePointerRegisterId(&dwTest)==S_OK) qDebug("get_paramBasePointerRegisterId");
    if(pSymbol->get_localBasePointerRegisterId(&dwTest)==S_OK) qDebug("get_localBasePointerRegisterId");
    if(pSymbol->get_isLocationControlFlowDependent(&bTest)==S_OK) qDebug("get_isLocationControlFlowDependent");
    if(pSymbol->get_stride(&dwTest)==S_OK) qDebug("get_stride");
    if(pSymbol->get_numberOfRows(&dwTest)==S_OK) qDebug("get_numberOfRows");
    if(pSymbol->get_numberOfColumns(&dwTest)==S_OK) qDebug("get_numberOfColumns");
    if(pSymbol->get_isMatrixRowMajor(&bTest)==S_OK) qDebug("get_isMatrixRowMajor");
    if(pSymbol->get_isReturnValue(&bTest)==S_OK) qDebug("get_isReturnValue");
    if(pSymbol->get_isOptimizedAway(&bTest)==S_OK) qDebug("get_isOptimizedAway");
    if(pSymbol->get_builtInKind(&dwTest)==S_OK) qDebug("get_builtInKind");
    if(pSymbol->get_registerType(&dwTest)==S_OK) qDebug("get_registerType");
    if(pSymbol->get_baseDataSlot(&dwTest)==S_OK) qDebug("get_baseDataSlot");
    if(pSymbol->get_baseDataOffset(&dwTest)==S_OK) qDebug("get_baseDataOffset");
    if(pSymbol->get_textureSlot(&dwTest)==S_OK) qDebug("get_textureSlot");
    if(pSymbol->get_samplerSlot(&dwTest)==S_OK) qDebug("get_samplerSlot");
    if(pSymbol->get_uavSlot(&dwTest)==S_OK) qDebug("get_uavSlot");
    if(pSymbol->get_sizeInUdt(&dwTest)==S_OK) qDebug("get_sizeInUdt");
    if(pSymbol->get_memorySpaceKind(&dwTest)==S_OK) qDebug("get_memorySpaceKind");
    if(pSymbol->get_unmodifiedTypeId(&dwTest)==S_OK) qDebug("get_unmodifiedTypeId");
    if(pSymbol->get_subTypeId(&dwTest)==S_OK) qDebug("get_subTypeId");
    if(pSymbol->get_subType(&diaSymbol)==S_OK) qDebug("get_subType");
    if(pSymbol->get_numberOfModifiers(&dwTest)==S_OK) qDebug("get_numberOfModifiers");
    if(pSymbol->get_numberOfRegisterIndices(&dwTest)==S_OK) qDebug("get_numberOfRegisterIndices");
    if(pSymbol->get_isHLSLData(&bTest)==S_OK) qDebug("get_isHLSLData");
    if(pSymbol->get_isPointerToDataMember(&bTest)==S_OK) qDebug("get_isPointerToDataMember");
    if(pSymbol->get_isPointerToMemberFunction(&bTest)==S_OK) qDebug("get_isPointerToMemberFunction");
    if(pSymbol->get_isSingleInheritance(&bTest)==S_OK) qDebug("get_isSingleInheritance");
    if(pSymbol->get_isMultipleInheritance(&bTest)==S_OK) qDebug("get_isMultipleInheritance");
    if(pSymbol->get_isVirtualInheritance(&bTest)==S_OK) qDebug("get_isVirtualInheritance");
    if(pSymbol->get_restrictedType(&bTest)==S_OK) qDebug("get_restrictedType");
    if(pSymbol->get_isPointerBasedOnSymbolValue(&bTest)==S_OK) qDebug("get_isPointerBasedOnSymbolValue");
    if(pSymbol->get_baseSymbol(&diaSymbol)==S_OK) qDebug("get_baseSymbol");
    if(pSymbol->get_baseSymbolId(&dwTest)==S_OK) qDebug("get_baseSymbolId");
    if(pSymbol->get_objectFileName(&bstrTest)==S_OK) qDebug("get_objectFileName");
    if(pSymbol->get_isAcceleratorGroupSharedLocal(&bTest)==S_OK) qDebug("get_isAcceleratorGroupSharedLocal");
    if(pSymbol->get_isAcceleratorPointerTagLiveRange(&bTest)==S_OK) qDebug("get_isAcceleratorPointerTagLiveRange");
    if(pSymbol->get_isAcceleratorStubFunction(&bTest)==S_OK) qDebug("get_isAcceleratorStubFunction");
    if(pSymbol->get_numberOfAcceleratorPointerTags(&dwTest)==S_OK) qDebug("get_numberOfAcceleratorPointerTags");
    if(pSymbol->get_isSdl(&bTest)==S_OK) qDebug("get_isSdl");
    if(pSymbol->get_isWinRTPointer(&bTest)==S_OK) qDebug("get_isWinRTPointer");
    if(pSymbol->get_isRefUdt(&bTest)==S_OK) qDebug("get_isRefUdt");
    if(pSymbol->get_isValueUdt(&bTest)==S_OK) qDebug("get_isValueUdt");
    if(pSymbol->get_isInterfaceUdt(&bTest)==S_OK) qDebug("get_isInterfaceUdt");
    if(pSymbol->getSrcLineOnTypeDefn(&diaLineNumber)==S_OK) qDebug("getSrcLineOnTypeDefn");
    if(pSymbol->get_isPGO(&bTest)==S_OK) qDebug("get_isPGO");
    if(pSymbol->get_hasValidPGOCounts(&bTest)==S_OK) qDebug("get_hasValidPGOCounts");
    if(pSymbol->get_isOptimizedForSpeed(&bTest)==S_OK) qDebug("get_isOptimizedForSpeed");
    if(pSymbol->get_PGOEntryCount(&dwTest)==S_OK) qDebug("get_PGOEntryCount");
    if(pSymbol->get_PGOEdgeCount(&dwTest)==S_OK) qDebug("get_PGOEdgeCount");
    if(pSymbol->get_PGODynamicInstructionCount(&llTest)==S_OK) qDebug("get_PGODynamicInstructionCount");
    if(pSymbol->get_staticSize(&dwTest)==S_OK) qDebug("get_staticSize");
    if(pSymbol->get_finalLiveStaticSize(&dwTest)==S_OK) qDebug("get_finalLiveStaticSize");
    if(pSymbol->get_phaseName(&bstrTest)==S_OK) qDebug("get_phaseName");
    if(pSymbol->get_hasControlFlowCheck(&bTest)==S_OK) qDebug("get_hasControlFlowCheck");
    if(pSymbol->get_constantExport(&bTest)==S_OK) qDebug("get_constantExport");
    if(pSymbol->get_dataExport(&bTest)==S_OK) qDebug("get_dataExport");
    if(pSymbol->get_privateExport(&bTest)==S_OK) qDebug("get_privateExport");
    if(pSymbol->get_noNameExport(&bTest)==S_OK) qDebug("get_noNameExport");
    if(pSymbol->get_exportHasExplicitlyAssignedOrdinal(&bTest)==S_OK) qDebug("get_exportHasExplicitlyAssignedOrdinal");
    if(pSymbol->get_exportIsForwarder(&bTest)==S_OK) qDebug("get_exportIsForwarder");
    if(pSymbol->get_ordinal(&dwTest)==S_OK) qDebug("get_ordinal");
    if(pSymbol->get_frameSize(&dwTest)==S_OK) qDebug("get_frameSize");
    if(pSymbol->get_exceptionHandlerAddressSection(&dwTest)==S_OK) qDebug("get_exceptionHandlerAddressSection");
    if(pSymbol->get_exceptionHandlerAddressOffset(&dwTest)==S_OK) qDebug("get_exceptionHandlerAddressOffset");
    if(pSymbol->get_exceptionHandlerRelativeVirtualAddress(&dwTest)==S_OK) qDebug("get_exceptionHandlerRelativeVirtualAddress");
    if(pSymbol->get_exceptionHandlerVirtualAddress(&llTest)==S_OK) qDebug("get_exceptionHandlerVirtualAddress");
    if(pSymbol->get_characteristics(&dwTest)==S_OK) qDebug("get_characteristics");
    if(pSymbol->get_coffGroup(&diaSymbol)==S_OK) qDebug("get_coffGroup");
    if(pSymbol->get_bindID(&dwTest)==S_OK) qDebug("get_bindID");
    if(pSymbol->get_bindSpace(&dwTest)==S_OK) qDebug("get_bindSpace");
    if(pSymbol->get_bindSlot(&dwTest)==S_OK) qDebug("get_bindSlot");
}

QWinPDB::RTYPE QWinPDB::getSymbolType(IDiaSymbol *pSymbol,QWinPDB::HANDLE_OPTIONS *pHandleOptions)
{
    RTYPE result={};

    IDiaSymbol *pType=0;
    pSymbol->get_type(&pType);

    if(pType)
    {
        result=_getType(pType,pHandleOptions);
    }
    else
    {
//        qDebug("No type"); // TODO Check
    }

//    pType->Release();

    return result;
}

QWinPDB::RTYPE QWinPDB::_getType(IDiaSymbol *pType,QWinPDB::HANDLE_OPTIONS *pHandleOptions)
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

            if(pHandleOptions->bFixTypes)
            {
                if(((result.nBaseType==7)||(result.nBaseType==14))&&(result.nSize!=4)) // "unsigned int"
                {
                    switch(result.nSize)
                    {
                        case 1:     result.sTypeName="unsigned char";       break;
                        case 2:     result.sTypeName="unsigned short";      break;
                        case 4:     result.sTypeName="unsigned int";        break;
                        case 8:     result.sTypeName="unsigned long long";  break;
                    }
                }

                if(((result.nBaseType==6)||(result.nBaseType==13))&&(result.nSize!=4)) // "int"
                {
                    switch(result.nSize)
                    {
                        case 1:     result.sTypeName="char";                break;
                        case 2:     result.sTypeName="short";               break;
                        case 4:     result.sTypeName="int";                 break;
                        case 8:     result.sTypeName="long long";           break;
                    }
                }
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

            result=_getType(_pType,pHandleOptions);
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

            result=_getType(_pType,pHandleOptions);
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
            RTYPE res_ret=getSymbolType(pType,pHandleOptions);
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
                            RTYPE rtype=getSymbolType(pSymbol,pHandleOptions);

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
            result=getSymbolType(pType,pHandleOptions); // TODO
        }
        else
        {
             emit infoMessage(QString("Unknown TYPE"));
        }
    }
    else
    {
        emit infoMessage(QString("pType error"));
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
        emit infoMessage(QString("No type")); // TODO Check
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

QString QWinPDB::rtypeToString(QWinPDB::RTYPE rtype, bool bIsClass)
{
    QString sResult;

    if(bIsClass)
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
//    qDebug("QWinPDB::~QWinPDB()");
    cleanup();
}

QWinPDB::PDB_INFO QWinPDB::getAllTags(QWinPDB::HANDLE_OPTIONS *pHandleOptions)
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
                            RECORD_DATA record=_getRecordData(pSymbol,pHandleOptions);

                            result.mapData.insert(record._symIndexId,record);
                        }
                        else if(dwSymTag==SymTagFunction)
                        {
                            RECORD_FUNCTION record=_getRecordFunction(pSymbol,pHandleOptions);

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
                            emit infoMessage(QString(rgTags[dwSymTag]));
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

QWinPDB::STATS QWinPDB::getStats()
{
    setProcessEnable(true);

    emit infoMessage(QString("Get stats"));

    STATS result={};

    QMap<QString,qint64> mapUDT;

    IDiaEnumSymbols *pEnumSymbols;
    LONG nCount;
//    if(pGlobal->findChildren(SymTagUDT, nullptr, nsNone, &pEnumSymbols)==S_OK)
    if(pGlobal->findChildren(SymTagNull,nullptr,nsNone,&pEnumSymbols)==S_OK)
    {
        if(pEnumSymbols->get_Count(&nCount)==S_OK)
        {
            if(nCount)
            {
                emit setProgressMinimum(0);
                emit setProgressMaximum(nCount);

                IDiaSymbol *pSymbol;
                ULONG celt=0;
                ULONG iMod=1;

                int nCurrentIndex=0;
                int nCurrentProcent=0;
                int nProcent=nCount/100;

                while(SUCCEEDED(pEnumSymbols->Next(1,&pSymbol,&celt))&&(celt==1)&&(!__bIsProcessStop)) // TODO Stop
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

                    if(nCurrentIndex>nCurrentProcent*nProcent)
                    {
                        nCurrentProcent++;
                        emit setProgressValue(nCurrentIndex);
                    }

                    nCurrentIndex++;
                }
            }
        }

        pEnumSymbols->Release();
    }

    qSort(result.listSymbols.begin(),result.listSymbols.end(),sortLessThan);

    setProcessEnable(true);

    emit completed();

    return result;
}

void QWinPDB::stop()
{
    setProcessEnable(false);
}

void QWinPDB::setProcessEnable(bool bState)
{
    __bIsProcessStop=!bState;
}

QWinPDB::ELEM QWinPDB::getElem(quint32 nID,QWinPDB::HANDLE_OPTIONS *pHandleOptions)
{
    ELEM result={};

    IDiaSymbol *pParent=nullptr;

    if(getSymbolByID(nID,&pParent))
    {
        result=_getElem(pParent,pHandleOptions);

        pParent->Release();
    }

    return result;
}

QWinPDB::ELEM QWinPDB::_getElem(IDiaSymbol *pParent, HANDLE_OPTIONS *pHandleOptions)
{
    ELEM result={};

    result.baseInfo=getBaseInfo(pParent);

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
        result._function=_getRecordFunction(pParent,pHandleOptions);
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
        result._data=_getRecordData(pParent,pHandleOptions);
        result.dwSize=result._data.rtype.nSize;
        result.dwOffset=result._data.rtype.nOffset;
        result.dwBitOffset=result._data.rtype.nBitOffset;
        result.dwBitSize=result._data.rtype.nBitSize;

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
//        _checkSymbol(pParent);

        result.elemType=ELEM_TYPE_CALLSITE;
        result._callsite=_getRecordCallSite(pParent,pHandleOptions);
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
        emit infoMessage(rgTags[dwSymTag]);
    }

    if(bChildren)
    {
        IDiaEnumSymbols *pEnumSymbols;
        if(pParent->findChildren(SymTagNull,nullptr,nsNone,&pEnumSymbols)==S_OK)
        {
            LONG nCount;
            if(pEnumSymbols->get_Count(&nCount)==S_OK)
            {
                if(nCount)
                {
                    IDiaSymbol *pSymbol;
                    ULONG celt = 0;

                    qint64 nCurrentOffset=0;
                    int nAlignCount=0;

                    while(SUCCEEDED(pEnumSymbols->Next(1,&pSymbol,&celt))&&(celt==1))
                    {
                        ELEM elemChild=_getElem(pSymbol,pHandleOptions);

                        bool bAdd=true;

                        if(elemChild.elemType==ELEM_TYPE_VTABLE)
                        {
                            bAdd=false;
                        }

                        if(elemChild.elemType==ELEM_TYPE_TYPEDEF)
                        {
                            bAdd=false;
                        }

                        if(bAdd)
                        {
                            if(pHandleOptions->bAddAlignment)
                            {
                                bool bAddAlignment=false;

                                if((elemChild.dwOffset)&&(elemChild.dwOffset>nCurrentOffset))
                                {
                                    bAddAlignment=true;
                                }

                                if(result.elemType==ELEM_TYPE_FUNCTION)
                                {
                                    bAddAlignment=false;
                                }

                                if(result.elemType==ELEM_TYPE_BLOCK)
                                {
                                    bAddAlignment=false;
                                }

                                if(bAddAlignment)
                                {
                                    ELEM alignElem={};

                                    alignElem.elemType=ELEM_TYPE_FAKEDATA;
                                    alignElem.dwOffset=nCurrentOffset;
                                    alignElem.dwSize=elemChild.dwOffset-nCurrentOffset;
                                    alignElem._data.rtype.bIsArray=true;
                                    alignElem._data.rtype.sName=QString("__align%1").arg(nAlignCount);
                                    alignElem._data.rtype.listArrayCount.append(alignElem.dwSize);
                                    alignElem._data.rtype.type=RD_BASETYPE;
                                    alignElem._data.rtype.sTypeName="unsigned char";
                                    alignElem._data.rtype.nBaseType=5; // unsigned char
                                    alignElem._data.rtype.nAccess=1; // private

                                    result.listChildren.append(alignElem);

                                    nAlignCount++;
                                }
                            }

                            // TODO Alignment
                            result.listChildren.append(elemChild);

                            if(elemChild.dwSize)
                            {
                                nCurrentOffset=elemChild.dwOffset+elemChild.dwSize;
                            }
                        }
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

void QWinPDB::fixOffsets(QWinPDB::ELEM *pElem) // TODO Options
{
    QWinPDB::ELEM elem=*pElem;

    elem.listChildren.clear();

    QList<ELEM> listChildren=pElem->listChildren;

    listChildren=_fixBitFields(&listChildren);

    int nCount=listChildren.count();

    _appendElem(&elem,&listChildren,0,nCount);

    *pElem=elem;
}

void QWinPDB::_appendElem(QWinPDB::ELEM *pElem, QList<QWinPDB::ELEM> *pListChildren, int nStartPosition, int nEndPosition)
{
    for(int i=nStartPosition;i<nEndPosition;i++)
    {
        if((pListChildren->at(i).dwSize)&&(pElem->elemType!=ELEM_TYPE_ENUM))
        {
            quint32 dwOffset=pListChildren->at(i).dwOffset;
            quint32 dwBitOffset=pListChildren->at(i).dwBitOffset;
            quint32 dwSize=pListChildren->at(i).dwSize;

            quint32 _dwSize=dwSize;
            quint32 _nCount=1;
            quint32 _nPosition=i;

            QList<quint32> listSizes;
            QList<quint32> listCounts;
            QList<quint32> listPositions;

            for(int j=i+1;j<nEndPosition;j++)
            {
                if( (pListChildren->at(j).dwOffset==dwOffset)&&
                    (pListChildren->at(j).dwBitOffset==dwBitOffset)&&
                    (pListChildren->at(j).dwSize))
                {
                    listSizes.append(_dwSize);
                    listCounts.append(_nCount);
                    listPositions.append(_nPosition);

                    _dwSize=0;
                    _nCount=0;
                    _nPosition=j;
                }

                _dwSize=qMax((DWORD)_dwSize,pListChildren->at(j).dwOffset+pListChildren->at(j).dwSize);
                _nCount++;
            }

            int nCount=listSizes.count();

            if(nCount)
            {
                // The last position
                quint32 _dwMaxSize=0;

                for(int j=0;j<nCount;j++)
                {
                    _dwMaxSize=qMax(_dwMaxSize,listSizes.at(j));
                }

                _dwSize=0;
                _nCount=0;

                for(int j=_nPosition;j<nEndPosition;j++)
                {
                    if((_dwSize>=_dwMaxSize))
                    {
                        break;
                    }

                    _dwSize=qMax((DWORD)_dwSize,pListChildren->at(j).dwOffset+pListChildren->at(j).dwSize);
                    _nCount++;
                }

                listSizes.append(_dwSize);
                listCounts.append(_nCount);
                listPositions.append(_nPosition);

                nCount++;

                bool bNewUnion=true;

                if((pElem->_udt.sType=="union")&&(pElem->dwSize==_dwSize))
                {
                    bNewUnion=false;
                }

                QWinPDB::ELEM *pElemUnion=0;

                if(bNewUnion)
                {
                    pElemUnion=new QWinPDB::ELEM();
                    *pElemUnion={};
                    pElemUnion->elemType=ELEM_TYPE_FAKEUNION;
                    pElemUnion->dwSize=_dwMaxSize;
                    pElemUnion->_udt.sType="union";
                }
                else
                {
                    pElemUnion=pElem;
                }

                // TODO Check
                for(int j=0;j<nCount;j++)
                {
//                    if(listCounts.at(j)>1)
//                    {
//                        QWinPDB::ELEM *pElemStruct=new QWinPDB::ELEM();
//                        *pElemStruct={};
//                        pElemStruct->elemType=ELEM_TYPE_FAKESTRUCT;
//                        pElemStruct->dwSize=listSizes.at(j);
//                        pElemStruct->_udt.sType="struct";

//                        _appendElem(pElemStruct,pListChildren,listPositions.at(j),listPositions.at(j)+listCounts.at(j));

//                        pElemUnion->listChildren.append(*pElemStruct);

//                        delete pElemStruct;
//                    }
//                    else
//                    {
//                        _appendElem(pElemUnion,pListChildren,listPositions.at(j),listPositions.at(j)+listCounts.at(j));
//                    }

                    _appendElem(pElemUnion,pListChildren,listPositions.at(j),listPositions.at(j)+listCounts.at(j));

                    i+=listCounts.at(j);
                }

                i--;

                if(bNewUnion)
                {
                    pElem->listChildren.append(*pElemUnion);
                    delete pElemUnion;
                }
            }
            else
            {
                pElem->listChildren.append(pListChildren->at(i));
            }
        }
        else
        {
            pElem->listChildren.append(pListChildren->at(i));
        }
    }
}

QList<QWinPDB::ELEM> QWinPDB::_fixBitFields(QList<QWinPDB::ELEM> *pListChildren)
{
    QList<QWinPDB::ELEM> listResult;

    int nCount=pListChildren->count();

    for(int i=0;i<nCount;i++)
    {
        if(pListChildren->at(i).dwBitSize)
        {
            quint32 nSize=pListChildren->at(i).dwSize;
            quint32 nOffset=pListChildren->at(i).dwOffset;

            QList<QWinPDB::ELEM> listBitFields;

            for(;i<nCount;i++)
            {
                if(pListChildren->at(i).dwBitSize==0)
                {
                    i--;
                    break;
                }

                nSize=(pListChildren->at(i).dwOffset+pListChildren->at(i).dwSize)-nOffset;

                listBitFields.append(pListChildren->at(i));
            }

            if(nSize==3) // TODO !!!
            {
                if((pListChildren->at(i+1).dwSize==1)&&(pListChildren->at(i+1).dwOffset==(nOffset+nSize)))
                {
                    i++;
                    listBitFields.append(pListChildren->at(i));
                    nSize=4;
                }
            }

            QWinPDB::ELEM elemStruct={};
            elemStruct.elemType=ELEM_TYPE_FAKESTRUCT;
            elemStruct.dwSize=nSize;
            elemStruct.dwOffset=nOffset;
            elemStruct._udt.sType="struct";

            _appendElem(&elemStruct,&listBitFields,0,listBitFields.count());

            listResult.append(elemStruct);
        }
        else
        {
            listResult.append(pListChildren->at(i));
        }
    }

    return listResult;
}

QWinPDB::ELEM_INFO QWinPDB::getElemInfo(const ELEM *pElem, HANDLE_OPTIONS *pHandleOptions, int nLevel, bool bIsClass)
{
    ELEM_INFO result;

    result.baseInfo=pElem->baseInfo;

    // TODO TYPEDEFS !!!

    if(pElem->elemType==ELEM_TYPE_ENUM)
    {
        result.sText+=_getTab(nLevel)+QString("enum %1\r\n").arg(pElem->_enum._name);
        result.sText+=_getTab(nLevel)+"{\r\n";

        int nCount=pElem->listChildren.count();

        for(int i=0;i<nCount;i++)
        {
            result.sText+=_getTab(nLevel+1)+QString("%1=%2").arg(pElem->listChildren.at(i)._data._name).arg(pElem->listChildren.at(i)._data.value.vValue.toString());

            if(i!=(nCount-1))
            {
                result.sText+=",";
            }
            result.sText+="\r\n";
        }

        result.sText+=_getTab(nLevel)+"}";
    }
    else if((pElem->elemType==ELEM_TYPE_UDT)||(pElem->elemType==ELEM_TYPE_FAKEUNION)||(pElem->elemType==ELEM_TYPE_FAKESTRUCT))
    {
        result.sText+=_getTab(nLevel)+QString("%1 %2").arg(pElem->_udt.sType).arg(pElem->_udt._name);

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
                result.sText+=QString(" : ");
            }

            result.sText+=listBaseClasses.at(i);

            if(i!=nBaseClassCount-1)
            {
                result.sText+=QString(", ");
            }
        }

        if(pHandleOptions->bShowComments)
        {
            result.sText+=QString("// Size=0x%1").arg(pElem->dwSize,0,16);
        }

        result.sText+="\r\n";
        result.sText+=_getTab(nLevel)+"{\r\n";

        // TODO typedefs
        for(int i=0;i<nChildrenCount;i++)
        {
            if(pElem->listChildren.at(i).elemType!=ELEM_TYPE_BASECLASS)
            {
                result.sText+=getElemInfo(&(pElem->listChildren.at(i)),pHandleOptions,nLevel+1,(pElem->_udt._udtKind==1)).sText;

                result.sText+=";";

                if(pHandleOptions->bShowComments)
                {
                    bool bShowComments=false;

                    if(pElem->listChildren.at(i).listChildren.count()==0)
                    {
                        bShowComments=true;
                    }

                    if(pElem->listChildren.at(i).elemType==ELEM_TYPE_FUNCTION)
                    {
                        bShowComments=true;
                    }

                    // TODO Functions start end

                    if(bShowComments)
                    {
                        if(pElem->listChildren.at(i).dwSize)
                        {
                            result.sText+=QString("// Offset=0x%1 Size=0x%2").arg(pElem->listChildren.at(i).dwOffset,0,16).arg(pElem->listChildren.at(i).dwSize,0,16);
                        }

                        if(pElem->listChildren.at(i).dwBitSize)
                        {
                            result.sText+=QString(" BitOffset=0x%1 BitSize=0x%2").arg(pElem->listChildren.at(i).dwBitOffset,0,16).arg(pElem->listChildren.at(i).dwBitSize,0,16);
                        }
                    }
                }

                result.sText+="\r\n";
            }

            result.listChildrenBaseInfos.append(pElem->listChildren.at(i).baseInfo);
        }

        result.sText+=_getTab(nLevel)+"}";
    }
    else if((pElem->elemType==ELEM_TYPE_DATA)||(pElem->elemType==ELEM_TYPE_FAKEDATA))
    {
        result.sText+=_getTab(nLevel)+rtypeToString(pElem->_data.rtype,bIsClass);

        if(pElem->_data.value.bIsValid) // TODO Check
        {
            result.sText+=QString("=%1").arg(pElem->_data.value.vValue.toString());
        }
    }
    else if(pElem->elemType==ELEM_TYPE_FUNCTION)
    {
        result.sText+=_getTab(nLevel)+rtypeToString(pElem->_function.rtype,bIsClass);
        // TODO function start,end
    }
    else if(pElem->elemType==ELEM_TYPE_TYPEDEF)
    {
        // TODO Check typedef
        emit infoMessage(QString("TYPEDEF"));
    }
    else if(pElem->elemType==ELEM_TYPE_VTABLE)
    {
        // TODO Check typedef
        emit infoMessage(QString("VTABLE"));
    }
    else
    {
        emit infoMessage(QString("Unknown ELEM_TYPE"));
    }

    if(nLevel==0)
    {
        result.sText+=";";
    }

    // TODO if struct has basic class -> interface

    return result;
}

QWinPDB::ELEM_INFO QWinPDB::handleElement(quint32 nID, QWinPDB::HANDLE_OPTIONS *pHandleOptions)
{
    QWinPDB::ELEM elem=getElem(nID,pHandleOptions);

    if( (pHandleOptions->fixOffsets==FO_ALL)||
        ((pHandleOptions->fixOffsets==FO_STRUCTSANDUNIONS)&&
        ((elem._udt._udtKind==0)||(elem._udt._udtKind==2))))
    {
        fixOffsets(&elem);
    }

    return QWinPDB::getElemInfo(&elem,pHandleOptions,0,false);
}

QString QWinPDB::exportString(QWinPDB::STATS *pStats, QWinPDB::HANDLE_OPTIONS *pHandleOptions)
{
    QString sResult;

    setProcessEnable(true);

    QList<ELEM_INFO> listElemInfos;

    if(!__bIsProcessStop)
    {
        int nCount=pStats->listSymbols.count();

        emit setProgressMinimum(0);
        emit setProgressMaximum(nCount);

        int nCurrentIndex=0;
        int nCurrentProcent=0;
        int nProcent=nCount/1000;

        for(int i=0;(i<nCount)&&(!__bIsProcessStop);i++)
        {
            ELEM_INFO elemInfo=handleElement(pStats->listSymbols.at(i).dwID,pHandleOptions);
            listElemInfos.append(elemInfo);

            if(nCurrentIndex>nCurrentProcent*nProcent)
            {
                nCurrentProcent++;
                emit setProgressValue(nCurrentIndex);
            }

            nCurrentIndex++;

            emit infoMessage(QString("[%1/%2] %3: %4").arg(i+1).arg(nCount).arg(tr("Get element")).arg(elemInfo.baseInfo.sName));
        }
    }

    if(!__bIsProcessStop)
    {
        emit infoMessage(tr("Sort elements"));

        if(pHandleOptions->sortType==ST_ID)
        {
            qSort(listElemInfos.begin(),listElemInfos.end(),sortElemInfoID);
        }
        else if(pHandleOptions->sortType==ST_NAME)
        {
            qSort(listElemInfos.begin(),listElemInfos.end(),sortElemInfoName);
        }
        else if(pHandleOptions->sortType==ST_DEP)
        {
            qSort(listElemInfos.begin(),listElemInfos.end(),sortElemInfoDeps);
        }
    }

    if(pHandleOptions->exportType==ET_CPLUSPLUS)
    {
        QString sExportName=pHandleOptions->sResultFileName.toUpper();

        sExportName=sExportName.replace(".","_");

        if(sExportName=="")
        {
            sExportName="_EXPORT_H";
        }
        else
        {
            sExportName="_"+sExportName;
        }

        sResult+=QString("#ifndef %1\r\n").arg(sExportName);
        sResult+=QString("#define %1\r\n").arg(sExportName);
        sResult+="\r\n";
        sResult+=QString("// File generated by %1 v%2 (http://www.ntinfo.biz)\r\n").arg(X_APPLICATIONNAME).arg(X_APPLICATIONVERSION);
        sResult+=QString("// Bugreports : horsicq@gmail.com\r\n");
        sResult+="\r\n";

        int nCount=listElemInfos.count();

        emit setProgressMinimum(0);
        emit setProgressMaximum(nCount);

        int nCurrentIndex=0;
        int nCurrentProcent=0;
        int nProcent=nCount/1000;

        if(nCount)
        {
            for(int i=0;(i<nCount)&&(!__bIsProcessStop);i++)
            {
                sResult+=listElemInfos.at(i).sText;
                sResult+="\r\n";
                sResult+="\r\n";

                if(nCurrentIndex>nCurrentProcent*nProcent)
                {
                    nCurrentProcent++;
                    emit setProgressValue(nCurrentIndex);
                }

                nCurrentIndex++;
            }
        }

        sResult+=QString("#endif\r\n");
    }
    else if(pHandleOptions->exportType==ET_XNTSV)
    {
        int nCount=listElemInfos.count();

        emit setProgressMinimum(0);
        emit setProgressMaximum(nCount);

        int nCurrentIndex=0;
        int nCurrentProcent=0;
        int nProcent=nCount/1000;

        QJsonObject recordObject;
        recordObject.insert("name",QJsonValue::fromVariant(pHandleOptions->sResultFileName));

        if(nCount)
        {
            for(int i=0;(i<nCount)&&(!__bIsProcessStop);i++)
            {
                // TODO
                //sResult+=listElemInfos.at(i).sText;

                if(nCurrentIndex>nCurrentProcent*nProcent)
                {
                    nCurrentProcent++;
                    emit setProgressValue(nCurrentIndex);
                }

                nCurrentIndex++;
            }
        }

        QJsonDocument doc(recordObject);
        sResult=doc.toJson(QJsonDocument::Indented);
    }

    setProcessEnable(true);

    emit completed();

    return sResult;
}

QWinPDB::ELEM_BASEINFO QWinPDB::getBaseInfo(IDiaSymbol *pParent)
{
    ELEM_BASEINFO result={};

    BSTR bstring=nullptr;

    pParent->get_symIndexId(&result.nID);
    if(pParent->get_name(&bstring)==S_OK) {result.sName=QString::fromWCharArray(bstring);        SysFreeString(bstring);}

    IDiaSymbol *pType=0;

    pParent->get_type(&pType);
    if(pType)
    {
        pType->get_symIndexId(&result.nTypeID);
        if(pType->get_name(&bstring)==S_OK) {result.sTypeName=QString::fromWCharArray(bstring);        SysFreeString(bstring);}
    }

    return result;
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
