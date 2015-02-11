#include "stdafx.h"

#include "windDataComSink.h"
#include "WindCppAPIImp.h"
#include "WindDataParser.h"
#include "ATLComTime.h"

WindDataComSink::WindDataComSink() : dwRefCount(0)
{
}

WindDataComSink::~WindDataComSink()
{
	dwRefCount = 0;
	//销毁map中在堆栈中的WsqReq对象
	clearMap();
}

void WindDataComSink::insertToMap(ULONGLONG reqId, WsqReq* pWsqReq)
{
	m_wsqMap.insert(std::make_pair(reqId, pWsqReq));
}

void WindDataComSink::deleteFromMap(ULONGLONG reqId)
{
	map<ULONGLONG, WsqReq*>::iterator itor = m_wsqMap.find(reqId);
	if (itor != m_wsqMap.end())
	{
		if (NULL != itor->second)
		{
			delete itor->second;
			itor->second = NULL;
		}
		m_wsqMap.erase(itor);
	}
}

void WindDataComSink::clearMap()
{
	map<ULONGLONG, WsqReq*>::iterator itor;
	for (itor = m_wsqMap.begin(); itor != m_wsqMap.end(); itor++)
	{
		if (NULL != itor->second)
		{
			delete itor->second;
			itor->second = NULL;
		}
	}
	m_wsqMap.clear();
}

void WindDataComSink::updateWsqAllFields(WindData &newWd, WindData &oldWd)
{
	oldWd.errorCode = newWd.errorCode;

	//oldWd是空的，则直接覆盖
	if (WindDataParser::IsEmpty(oldWd.times) || WindDataParser::IsEmpty(oldWd.fields) || WindDataParser::IsEmpty(oldWd.codes))
	{
		WindCppAPIImp::ClearWindData(oldWd);
		oldWd = newWd;
		WindCppAPIImp::EmptyWindData(newWd);
		return;
	}

	int newFieldsLen = WindDataParser::GetCountOfSafeArray(newWd.fields);
	int newCodesLen = WindDataParser::GetCountOfSafeArray(newWd.codes);
	for (int newIndexCodes = 0; newIndexCodes < newCodesLen; newIndexCodes++)
	{
		wstring codesName = WindDataParser::GetStrItemByIndex(newWd.codes, newIndexCodes);
		int oldIndexCode = WindDataParser::GetStrItemIndexOfSafeArray(oldWd.codes, codesName.c_str());
		if (oldIndexCode != -1)
		{
			for (int newIndexFields = 0; newIndexFields < newFieldsLen; newIndexFields++)
			{
				wstring fieldsName = WindDataParser::GetStrItemByIndex(newWd.fields, newIndexFields);
				int oldIndexField = WindDataParser::GetStrItemIndexOfSafeArray(oldWd.fields, fieldsName);
				if (oldIndexField != -1)
				{
					VARIANT data = WindDataParser::GetVarOfDataFromWindData(0, newIndexCodes, newIndexFields, newWd);
					WindDataParser::SetDataToWindData(0, oldIndexCode, oldIndexField, data, oldWd);
				}
			}
		}
	}
}

STDMETHODIMP WindDataComSink::Fire_StateChanged(long state, unsigned __int64 requestid, long errorCode)
{
	map<ULONGLONG, WsqReq*>::iterator itor = m_wsqMap.find((ULONGLONG)requestid);
	if (itor != m_wsqMap.end())
	{
		WsqReq *pWsqReq = itor->second;

		if (pWsqReq->callback == NULL)
		{
			return S_OK;
		}

		WindData tempData;
		if (state == 1 && errorCode == 0)
		{
			LONG requestState = 0;
			LONG errCode = WindCppAPIImp::getIntance().readdata(requestid, tempData, requestState);

			if (pWsqReq->updateAllFields)
			{
				updateWsqAllFields(tempData, pWsqReq->windData);
				pWsqReq->callback(pWsqReq->windData);
			}
			else
			{
				pWsqReq->callback(tempData);
			}
		}
		else if (errorCode != 0)
		{
			_variant_t vt = _bstr_t(L"OUTMESSAGE");
			tempData.fields = vt.Detach();
			
			VARIANT codeVar;
			codeVar.vt = VT_BSTR;
			codeVar.bstrVal = L"ErrorReport";
			WindDataParser::PutItemToVariant(tempData.codes, codeVar, 0);

			VARIANT timeVar;
			timeVar.vt = VT_DATE;
			COleDateTime time = COleDateTime::GetCurrentTime();
			timeVar.date = time.m_dt;
			WindDataParser::PutItemToVariant(tempData.times, timeVar, 0);

 			WindCppAPIImp::getIntance().getErrorMsg(errorCode, eCHN, tempData.data);
 			tempData.errorCode = errorCode;

 			pWsqReq->callback(tempData);

		}
	}

	return S_OK;;
}

HRESULT STDMETHODCALLTYPE WindDataComSink::QueryInterface(REFIID iid, void **ppvObject)
{
	if (iid == __uuidof(WindDataCOMLib::_DWindDataCOMEvents))
	{
		dwRefCount++;
		*ppvObject = (void *)this;
		return S_OK;
	}

	if (iid == IID_IUnknown)
	{
		dwRefCount++;            
		*ppvObject = (void *)this;
		return S_OK;
	}

	return E_NOINTERFACE;
}
ULONG STDMETHODCALLTYPE WindDataComSink::AddRef()
{
	dwRefCount++;
	return dwRefCount;
}

ULONG STDMETHODCALLTYPE WindDataComSink::Release()
{
	ULONG l;

	l  = dwRefCount--;

	if ( 0 == dwRefCount)
	{
		delete this;
	}

	return l;
}

HRESULT STDMETHODCALLTYPE WindDataComSink::GetTypeInfoCount( 
	/* [out] */ __RPC__out UINT *pctinfo)
{
	return S_OK;
}

HRESULT STDMETHODCALLTYPE WindDataComSink::GetTypeInfo( 
	/* [in] */ UINT iTInfo,
	/* [in] */ LCID lcid,
	/* [out] */ __RPC__deref_out_opt ITypeInfo **ppTInfo)
{
	return S_OK;
}

HRESULT STDMETHODCALLTYPE WindDataComSink::GetIDsOfNames( 
	/* [in] */ __RPC__in REFIID riid,
	/* [size_is][in] */ __RPC__in_ecount_full(cNames) LPOLESTR *rgszNames,
	/* [range][in] */ UINT cNames,
	/* [in] */ LCID lcid,
	/* [size_is][out] */ __RPC__out_ecount_full(cNames) DISPID *rgDispId)
{
	return S_OK;
}

/* [local] */ HRESULT STDMETHODCALLTYPE WindDataComSink::Invoke( 
	/* [in] */ DISPID dispIdMember,
	/* [in] */ REFIID riid,
	/* [in] */ LCID lcid,
	/* [in] */ WORD wFlags,
	/* [out][in] */ DISPPARAMS *pDispParams,
	/* [out] */ VARIANT *pVarResult,
	/* [out] */ EXCEPINFO *pExcepInfo,
	/* [out] */ UINT *puArgErr)
{
	switch(dispIdMember)	// 根据不同的dispIdMember,完成不同的回调函数
	{
	case 2:
		{
			// 1st param : [in] long lValue.
			VARIANT varlValue;
			long state = 0;
			unsigned __int64 requestid = 0;
			long errorCode = 0;

			//FireEvent参数倒序
			VariantInit(&varlValue);
			varlValue = (pDispParams->rgvarg)[2];
			state = V_I4(&varlValue);

			VariantClear(&varlValue);
			varlValue = (pDispParams->rgvarg)[1];
			requestid = V_I8(&varlValue);

			VariantClear(&varlValue);
			varlValue = (pDispParams->rgvarg)[0];
			errorCode = V_I4(&varlValue);

			Fire_StateChanged(state, requestid, errorCode);
		}
		break;
	default:
		break;
	}

	return S_OK;
}