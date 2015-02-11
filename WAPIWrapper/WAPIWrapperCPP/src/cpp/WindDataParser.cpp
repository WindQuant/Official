#include "StdAfx.h"
#include "WindDataParser.h"
#include "..\header\WAPIWrapperCpp.h"

WindDataParser::WindDataParser(void)
{
}

WindDataParser::~WindDataParser(void)
{
}

bool WindDataParser::IsEmpty(const VARIANT& var)
{
	if (var.vt == VT_EMPTY || var.vt == VT_NULL)
		return true;
	return false;
}

bool WindDataParser::IsArray(const VARIANT& var)
{
	if((var.vt & VT_ARRAY) && (var.parray != NULL))
	{
		return true;
	}
	return false;
}

long WindDataParser::GetCountOfSafeArray(const VARIANT& safeArray)
{
	if(!IsArray(safeArray))
	{
		return 0;
	}

	long totalCount = 1;
	for(int i=0; i< safeArray.parray->cDims; i++)
	{
		totalCount *= safeArray.parray->rgsabound[i].cElements;
	}

	return totalCount;
}

int WindDataParser::GetStrItemIndexOfSafeArray(const VARIANT& safeArray, const wstring& itemNameStr)
{
	if(!IsArray(safeArray))
	{
		return -1;
	}

	int length = GetCountOfSafeArray(safeArray);

	HRESULT hr ;
	BSTR *pbItems;

	hr = SafeArrayAccessData(safeArray.parray, (void HUGEP**)&pbItems);
	if (FAILED(hr))
	{
		return -1;
	}
	int nRetIndex = -1;
	for(int index = 0; index < length; index++)
	{
		if (0 == itemNameStr.compare(pbItems[index]))
		{
			nRetIndex = index;
			break;
		}
	}
	SafeArrayUnaccessData(safeArray.parray);

	return nRetIndex;
}

wstring WindDataParser::GetStrItemByIndex(const VARIANT& safeArray, int index)
{
	if(!IsArray(safeArray))
	{
		return L"";
	}

	if (VT_BSTR != (safeArray.vt & VT_BSTR_BLOB))
	{
		return L"";
	}

	HRESULT hr ;
	BSTR *pbItems;

	if (index >= GetCountOfSafeArray(safeArray))
	{
		return L"";
	}

	hr = SafeArrayAccessData(safeArray.parray, (void HUGEP**)&pbItems);
	if (FAILED(hr))
	{
		return L"";
	}

	wstring itemStr = pbItems[index];
	
	SafeArrayUnaccessData(safeArray.parray);

	return itemStr;
}

LPCWSTR WindDataParser::GetInternalStrByIndex(const VARIANT& safeArray, int index)
{
	if(!IsArray(safeArray))
	{
		return L"";
	}

	if (VT_BSTR != (safeArray.vt & VT_BSTR_BLOB))
	{
		return L"";
	}

	HRESULT hr ;
	BSTR *pbItems;

	if (index >= GetCountOfSafeArray(safeArray))
	{
		return L"";
	}

	hr = SafeArrayAccessData(safeArray.parray, (void HUGEP**)&pbItems);
	if (FAILED(hr))
	{
		return L"";
	}

	LPCWSTR itemStr = pbItems[index];

	SafeArrayUnaccessData(safeArray.parray);

	return itemStr;
}

DOUBLE WindDataParser::GetDoubeItemByIndex(const VARIANT& safeArray, int index)
{
	if(!IsArray(safeArray))
	{
		return 0.0;
	}

	if (VT_R8 != (safeArray.vt & VT_BSTR_BLOB) && VT_DATE != (safeArray.vt & VT_BSTR_BLOB))
	{
		return 0.0;
	}
	long lPos = (long)index;
	DOUBLE dbl = 0;

	SafeArrayGetElement(safeArray.parray, &lPos, &dbl);
	
	return dbl;
}

int WindDataParser::GetDoubleItemIndexOfSafeArray(const VARIANT& safeArray, DOUBLE dbl)
{
	if(!IsArray(safeArray))
	{
		return -1;
	}

	int length = GetCountOfSafeArray(safeArray);

	HRESULT hr;
	DOUBLE *pbItems;

	hr = SafeArrayAccessData(safeArray.parray, (void HUGEP**)&pbItems);
	if (FAILED(hr))
	{
		return -1;
	}
	int nRetIndex = -1;
	for(int index = 0; index < length; index++)
	{
		if (dbl == pbItems[index])
		{
			nRetIndex = index;
			break;
		}
	}
	SafeArrayUnaccessData(safeArray.parray);

	return nRetIndex;
}

void WindDataParser::SetDataToWindData(int time, int codes, int fields, const VARIANT& data, WindData& wd)
{
	int timenum = GetCountOfSafeArray(wd.times);
	int codenum = GetCountOfSafeArray(wd.codes);
	int indnum = GetCountOfSafeArray(wd.fields);

	if (time < timenum && codes < codenum && fields < indnum)
	{
		int pos = time * codenum * indnum + codes * indnum + fields;

		PutItemToVariant(wd.data, data, pos);
	}
}

void WindDataParser::PutItemToVariant(VARIANT& var, const VARIANT& data, long pos)
{
	if (IsEmpty(var))
	{
		var.vt = VT_ARRAY;
		pos = 0;

		switch (data.vt)
		{
		case VT_R8:
			SafeArrayPutElement(var.parray, (long*)&pos, (void*)&data.dblVal);
			break;
		case VT_BSTR:
			{
				BSTR pTmpBstr = SysAllocString(data.bstrVal);
				SafeArrayPutElement(var.parray, (long*)&pos, (void*)&pTmpBstr);
			}
			break;
		case VT_I4:
			SafeArrayPutElement(var.parray, (long*)&pos, (void*)&data.lVal);
			break;
		case VT_I8:
			SafeArrayPutElement(var.parray, (long*)&pos, (void*)&data.llVal);
			break;
		case VT_DATE:
			SafeArrayPutElement(var.parray, (long*)&pos, (void*)&data.date);
			break;
		case VT_ARRAY:
			SafeArrayPutElement(var.parray, (long*)&pos, (void*)&data.parray);
			break;
		default:
			break;
		}
	}
	else
	{
		if ((var.vt & VT_ARRAY) && (var.parray != NULL))
		{
			switch (data.vt)
			{
			case VT_R8:
				SafeArrayPutElement(var.parray, (long*)&pos, (void*)&data.dblVal);
				break;
			case VT_BSTR:
				{
					BSTR pTmpBstr = SysAllocString(data.bstrVal);
					SafeArrayPutElement(var.parray, (long*)&pos, (void*)&pTmpBstr);
				}
				break;
			case VT_I4:
				SafeArrayPutElement(var.parray, (long*)&pos, (void*)&data.lVal);
				break;
			case VT_I8:
				SafeArrayPutElement(var.parray, (long*)&pos, (void*)&data.llVal);
				break;
			case VT_DATE:
				SafeArrayPutElement(var.parray, (long*)&pos, (void*)&data.date);
				break;
			case VT_ARRAY:
				{
					// 目前没有Array中套Array的情况
					/*long lBound = 0;
					SafeArrayGetLBound(data.parray, 0, &lBound);
					LONG length = GetCountOfSafeArray(data);
					SAFEARRAY* pNewSafeAry = SafeArrayCreateVector(data.vt, lBound, length);
					SafeArrayCopy(data.parray, &pNewSafeAry);
					SafeArrayPutElement(var.parray, (long*)&pos, (void*)&pNewSafeAry);*/
				}
				break;
			default:
				break;
			}
		}
	}
}

VARIANT WindDataParser::GetVarOfDataFromWindData(int time, int codes, int fields, const WindData& wd)
{	
	int timenum = GetCountOfSafeArray(wd.times);
	int codenum = GetCountOfSafeArray(wd.codes);
	int indnum = GetCountOfSafeArray(wd.fields);

	if (timenum == 0 && codenum == 0 && indnum > 0)
	{
		return GetVarFromArray(wd.data, fields);
	}

	if (time < timenum && codes < codenum && fields < indnum)
	{
		int pos = time * codenum * indnum + codes * indnum + fields;

		return GetVarFromArray(wd.data, pos);
	}

	VARIANT var;
	VariantInit(&var);

	return var;
}

VARIANT WindDataParser::GetVarFromArray(const VARIANT& safeArray, int pos)
{
	VARIANT var;
	VariantInit(&var);

	if ((safeArray.vt & VT_ARRAY) && (safeArray.parray != NULL))
	{
		PVOID pvData = safeArray.parray->pvData;
		switch (safeArray.vt & VT_BSTR_BLOB)
		{
		case VT_VARIANT:
			var = *((LPVARIANT)pvData + pos);
			break;
		case VT_BSTR:
			{
				var.vt = VT_BSTR;
				BSTR* pCurBSTR = (BSTR*)pvData + pos;
				//直接指向safeArray中的bstr，而不是SysAllocString重新分配一个内存来存储字符串
				var.bstrVal = *pCurBSTR;//SysAllocString(*pCurBSTR);
			}
			break;
		case VT_R8:
			var.vt = VT_R8;
			var.dblVal = *((DOUBLE*)pvData + pos);
			break;
		case VT_I4:
			var.vt = VT_I4;
			var.lVal = *((INT*)pvData + pos);
			break;
		case VT_I8:
			var.vt = VT_I8;
			var.llVal = *((LONGLONG*)pvData + pos);
			break;
		case VT_DATE:
			var.vt = VT_DATE;
			var.date = *((DATE*)pvData + pos);
			break;
		default:
			break;
		}
	}

	return var;
}