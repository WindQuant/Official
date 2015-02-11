#include "stdafx.h"
#include "WindCppAPIImp.h"
#include "..\header\WAPIWrapperCpp.h"
#include "windDataComSink.h"
#include "WindDataParser.h"

//////////////////////////////////////////////////////////////////////////

WindCppAPIImp::WindCppAPIImp():m_pDWindData(NULL), m_pSink(NULL), m_dwAdvise(0)
{
	if(!SUCCEEDED(CoInitialize(NULL)))
	{
		return;
	}

	initialize();
}

WindCppAPIImp::~WindCppAPIImp()
{
	release();

	CoUninitialize();
}

BOOL WindCppAPIImp::initialize()
{
	if (m_pDWindData != NULL)
	{
		return TRUE;
	}

	HRESULT hr = E_FAIL;
	hr = m_pDWindData.CreateInstance(__uuidof(WindDataCOMLib::WindDataCOM), NULL, CLSCTX_ALL);

	if(!SUCCEEDED(hr))
	{
		return FALSE;
	}

	m_pDWindData->enableAsyn(1);

	IConnectionPointContainer *pCPC;
	hr = m_pDWindData.QueryInterface(IID_IConnectionPointContainer, (void **)&pCPC);
	if(!SUCCEEDED(hr))
	{
		return FALSE;
	}

	IConnectionPoint *pCP;
	hr = pCPC->FindConnectionPoint(__uuidof(WindDataCOMLib::_DWindDataCOMEvents), &pCP);
	if (!SUCCEEDED(hr))
	{
		return FALSE;
	}
	pCPC->Release();

	IUnknown *m_pSinkUnk;
	m_pSink = new WindDataComSink();

	hr = m_pSink->QueryInterface(IID_IUnknown, (void **)&m_pSinkUnk);
	if(!SUCCEEDED(hr))
	{
		pCP->Release();
		return FALSE;
	}

	hr = pCP->Advise(m_pSinkUnk, &m_dwAdvise);
	if(!SUCCEEDED(hr))
	{
		return FALSE;
	}
	pCP->Release();

	return TRUE;
}

VOID WindCppAPIImp::release()
{
	if (m_pDWindData != NULL)
	{
		cancelAllRequest();

		HRESULT hr = E_FAIL;
		IConnectionPointContainer *pCPC;
		hr = m_pDWindData.QueryInterface(IID_IConnectionPointContainer, (void **)&pCPC);
		if(SUCCEEDED(hr))
		{
			IConnectionPoint *pCP;
			hr = pCPC->FindConnectionPoint(__uuidof(WindDataCOMLib::_DWindDataCOMEvents), &pCP);
			if (SUCCEEDED(hr))
			{
				hr = pCP->Unadvise(m_dwAdvise);
				pCP->Release();
			}
			pCPC->Release();
		}

		m_pDWindData.Release();
		m_pDWindData = NULL;
		
		if (m_pSink != NULL)
		{
			delete m_pSink;
			m_pSink = NULL;
		}

		m_dwAdvise = 0;
	}	
}


VOID WindCppAPIImp::InitWindData(WindData& windData)
{
	VariantInit(&windData.data);
	VariantInit(&windData.codes);
	VariantInit(&windData.fields);
	VariantInit(&windData.times);
	windData.errorCode = 0;
}

VOID WindCppAPIImp::ClearWindData(WindData& windData)
{
	VariantClear(&windData.data);
	VariantClear(&windData.codes);
	VariantClear(&windData.fields);
	VariantClear(&windData.times);
	windData.errorCode = 0;
}

VOID WindCppAPIImp::EmptyWindData(WindData& windData)
{
	V_VT(&windData.data) = VT_EMPTY;
	V_VT(&windData.codes) = VT_EMPTY;
	V_VT(&windData.fields) = VT_EMPTY;
	V_VT(&windData.times) = VT_EMPTY;
	windData.errorCode = 0;
}

WindCppAPIImp& WindCppAPIImp::getIntance()
{
	static WindCppAPIImp s_instance;
	return s_instance;
}


LONG WindCppAPIImp::start(LPCWSTR options /* = L"" */, LPCWSTR options2 /* = L"" */, LONG timeout /* = 5000 */)
{
	if (m_pDWindData == NULL)
	{
		return noRegErr;
	}

	return m_pDWindData->start_cpp(options, options2, timeout);
}

LONG WindCppAPIImp::stop()
{
	if (m_pDWindData == NULL)
	{
		return noRegErr;
	}

	cancelAllRequest();

	return m_pDWindData->stop();
}

LONG WindCppAPIImp::isconnected()
{
	if (m_pDWindData == NULL)
	{
		return noRegErr;
	}

	LONG ret = -1;
	m_pDWindData->getConnectionState(&ret);

	return ret;
}

VOID WindCppAPIImp::cancelRequest(ULONGLONG requestId)
{
	if (requestId == 0)
	{
		cancelAllRequest();
		return;
	}

	if (m_pDWindData != NULL)
	{
		m_pDWindData->cancelRequest(requestId);
		m_pSink->deleteFromMap(requestId);
	}
}

VOID WindCppAPIImp::cancelAllRequest()
{
	if (m_pDWindData != NULL)
	{
		m_pDWindData->cancelRequest(0);
		m_pSink->clearMap();
	}
}

LONG WindCppAPIImp::readdata(ULONGLONG requestid, WindData& outWindData , LONG& requestState)
{
	InitWindData(outWindData);
	if (m_pDWindData == NULL)
	{
		return noRegErr;
	}

	_variant_t varRes = m_pDWindData->readdata(requestid, &outWindData.codes, &outWindData.fields, &outWindData.times, &requestState, &outWindData.errorCode);
	outWindData.data = varRes.Detach();

	return outWindData.errorCode;
}

BOOL WindCppAPIImp::getErrorMsg(LONG errCode, ErrMsgLang lang, WCHAR msg[], int& nBufferLength)
{
	if (m_pDWindData == NULL)
	{
		return FALSE;
	}
	
	_variant_t varRes = m_pDWindData->getErrorMsg(errCode, lang);
	
	int msgLength = (int)wcslen(varRes.bstrVal) + 1;
	nBufferLength = (nBufferLength > msgLength) ? msgLength : nBufferLength;
	swprintf(msg, nBufferLength, L"%s", varRes.bstrVal);
	
	return TRUE;
}

BOOL WindCppAPIImp::getErrorMsg(LONG errCode, ErrMsgLang lang, VARIANT &msg)
{
	VariantInit(&msg);

	if (m_pDWindData == NULL)
	{
		return FALSE;
	}

	msg = m_pDWindData->getErrorMsg(errCode, lang).Detach();

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////

LONG WindCppAPIImp::wsd(LPCWSTR windCodes, LPCWSTR fields, LPCWSTR startTime, LPCWSTR endTime, LPCWSTR options, WindData& outWindData)
{
	ClearWindData(outWindData);
	InitWindData(outWindData);

	if (m_pDWindData == NULL)
	{
		return noRegErr;
	}

	outWindData.data = m_pDWindData->wsd_syn(windCodes, fields, startTime, endTime, options, 
		&outWindData.codes, &outWindData.fields, &outWindData.times, &outWindData.errorCode).Detach();

	return outWindData.errorCode;
}

LONG WindCppAPIImp::wss(LPCWSTR windCodes, LPCWSTR fields, LPCWSTR options, WindData& outWindData)
{
	ClearWindData(outWindData);
	InitWindData(outWindData);

	if (m_pDWindData == NULL)
	{
		return noRegErr;
	}

	outWindData.data = m_pDWindData->wss_syn(windCodes, fields, options, 
		&outWindData.codes, &outWindData.fields, &outWindData.times, &outWindData.errorCode).Detach();

	return outWindData.errorCode;
}

LONG WindCppAPIImp::wsi(LPCWSTR windCodes, LPCWSTR fields, LPCWSTR startTime, LPCWSTR endTime, LPCWSTR options, WindData& outWindData)
{
	ClearWindData(outWindData);
	InitWindData(outWindData);

	if (m_pDWindData == NULL)
	{
		return noRegErr;
	}

	outWindData.data = m_pDWindData->wsi_syn(windCodes, fields, startTime, endTime, options, 
		&outWindData.codes, &outWindData.fields, &outWindData.times, &outWindData.errorCode).Detach();

	return outWindData.errorCode;
}

LONG WindCppAPIImp::wst(LPCWSTR windCodes, LPCWSTR fields, LPCWSTR startTime, LPCWSTR endTime, LPCWSTR options, WindData& outWindData)
{
	ClearWindData(outWindData);
	InitWindData(outWindData);

	if (m_pDWindData == NULL)
	{
		return noRegErr;
	}

	outWindData.data = m_pDWindData->wst_syn(windCodes, fields, startTime, endTime, options, 
		&outWindData.codes, &outWindData.fields, &outWindData.times, &outWindData.errorCode).Detach();

	return outWindData.errorCode;
}

LONG WindCppAPIImp::wsqSync(LPCWSTR windCodes, LPCWSTR fields, LPCWSTR options, WindData& outWindData, bool isTD /* = false */)
{
	ClearWindData(outWindData);
	InitWindData(outWindData);

	if (m_pDWindData == NULL)
	{
		return noRegErr;
	}

	if (!isTD)
	{
		outWindData.data = m_pDWindData->wsq_syn(windCodes, fields, options, 
			&outWindData.codes, &outWindData.fields, &outWindData.times, &outWindData.errorCode).Detach();
	}
	else
	{
		outWindData.data = m_pDWindData->wsqtd_syn(windCodes, fields, options, 
			&outWindData.codes, &outWindData.fields, &outWindData.times, &outWindData.errorCode).Detach();
	}

	return outWindData.errorCode;
}

LONG WindCppAPIImp::wsqAsyn(LPCWSTR windCodes, LPCWSTR fields, LPCWSTR options, WsqAsynCallBack callBack, BOOL updateAllFields, ULONGLONG &reqId, bool isTD /* = false */)
{
	if (m_pDWindData == NULL)
	{
		return noRegErr;
	}

	WsqReq *pWsqReq = new WsqReq();
	pWsqReq->callback = callBack;
	pWsqReq->updateAllFields = updateAllFields;
	if (pWsqReq->updateAllFields)
	{
		wsqSync(windCodes, fields, options, pWsqReq->windData, isTD);
	}

	wstring tmpOptions = options;
	tmpOptions += L";REALTIME=Y";
	LONG errCode = 0;
	if (!isTD)
	{
		reqId = m_pDWindData->wsq(windCodes, fields, tmpOptions.c_str(), &errCode);
	}
	else
	{
		reqId = m_pDWindData->wsqtd(windCodes, fields, tmpOptions.c_str(), &errCode);
	}

	if (errCode == 0)
	{
		m_pSink->insertToMap(reqId, pWsqReq);
	}
	else
	{
		delete pWsqReq;
		pWsqReq = NULL;
	}

	return errCode;
}

LONG WindCppAPIImp::wset(LPCWSTR reportName, LPCWSTR options, WindData& outWindData)
{
	ClearWindData(outWindData);
	InitWindData(outWindData);

	if (m_pDWindData == NULL)
	{
		return noRegErr;
	}

	outWindData.data = m_pDWindData->wset_syn(reportName, options, 
		&outWindData.codes, &outWindData.fields, &outWindData.times, &outWindData.errorCode).Detach();

	return outWindData.errorCode;
}

LONG WindCppAPIImp::edb(WindData& outWindData, LPCWSTR windCodes, LPCWSTR startTime, LPCWSTR endTime, LPCWSTR options)
{
	ClearWindData(outWindData);
	InitWindData(outWindData);

	if (m_pDWindData == NULL)
	{
		return noRegErr;
	}

	outWindData.data = m_pDWindData->edb_syn(windCodes, startTime, endTime, options, 
		&outWindData.codes, &outWindData.fields, &outWindData.times, &outWindData.errorCode).Detach();

	return outWindData.errorCode;
}

LONG WindCppAPIImp::wpf(LPCWSTR portfolioName, LPCWSTR viewName, LPCWSTR options, WindData& outWindData)
{
	ClearWindData(outWindData);
	InitWindData(outWindData);

	if (m_pDWindData == NULL)
	{
		return noRegErr;
	}

	outWindData.data = m_pDWindData->wpf_syn(portfolioName, viewName, options,
		&outWindData.codes, &outWindData.fields, &outWindData.times, &outWindData.errorCode).Detach();

	return outWindData.errorCode;
}

LONG WindCppAPIImp::wupf(LPCWSTR portfolioName, LPCWSTR tradeDate, LPCWSTR windCodes, LPCWSTR quantity, LPCWSTR costPrice, LPCWSTR options, WindData& outWindData)
{
	ClearWindData(outWindData);
	InitWindData(outWindData);

	if (m_pDWindData == NULL)
	{
		return noRegErr;
	}

	outWindData.data = m_pDWindData->wupf_syn(portfolioName, tradeDate, windCodes, quantity, costPrice, options,
		&outWindData.codes, &outWindData.fields, &outWindData.times, &outWindData.errorCode).Detach();

	return outWindData.errorCode;
}

LONG WindCppAPIImp::weqs(LPCWSTR planName, LPCWSTR options, WindData& outWindData)
{
	ClearWindData(outWindData);
	InitWindData(outWindData);

	if (m_pDWindData == NULL)
	{
		return noRegErr;
	}

	outWindData.data = m_pDWindData->weqs_syn(planName, options,
		&outWindData.codes, &outWindData.fields, &outWindData.times, &outWindData.errorCode).Detach();

	return outWindData.errorCode;
}

LONG WindCppAPIImp::tdays(LPCWSTR startTime, LPCWSTR endTime, LPCWSTR options, WindData& outWindData)
{
	ClearWindData(outWindData);
	InitWindData(outWindData);

	if (m_pDWindData == NULL)
	{
		return noRegErr;
	}

	outWindData.data = m_pDWindData->tdays_syn(startTime, endTime, options,
		&outWindData.codes, &outWindData.fields, &outWindData.times, &outWindData.errorCode).Detach();

	return outWindData.errorCode;
}

LONG WindCppAPIImp::tdaysoffset(LPCWSTR startTime, LONG offset, LPCWSTR options, DATE& outDate)
{
	WindData outWindData;
	InitWindData(outWindData);

	if (m_pDWindData == NULL)
	{
		return noRegErr;
	}

	_variant_t varRes = m_pDWindData->tdaysoffset_syn(startTime, offset, options,
		&outWindData.codes, &outWindData.fields, &outWindData.times, &outWindData.errorCode);

	outDate = WindDataParser::GetVarFromArray(varRes, 0).date;

	return outWindData.errorCode;
}

LONG WindCppAPIImp::tdayscount(LPCWSTR startTime, LPCWSTR endTime, LPCWSTR options, LONG& outCount)
{
	WindData outWindData;
	InitWindData(outWindData);

	if (m_pDWindData == NULL)
	{
		return noRegErr;
	}

	_variant_t varRes = m_pDWindData->tdayscount_syn(startTime, endTime, options,
		&outWindData.codes, &outWindData.fields, &outWindData.times, &outWindData.errorCode);

	outCount = WindDataParser::GetVarFromArray(varRes, 0).lVal;
	
	return outWindData.errorCode;
}

//////////////////////////////////////////////////////////////////////////

LONG WindCppAPIImp::tlogon(LPCWSTR brokerID, LPCWSTR departmentID, LPCWSTR accountID, LPCWSTR password, LPCWSTR accountType, LPCWSTR options, WindData& outWindData)
{
	ClearWindData(outWindData);
	InitWindData(outWindData);

	if (m_pDWindData == NULL)
	{
		return noRegErr;
	}

	outWindData.data = m_pDWindData->tLogon(brokerID, departmentID, accountID, password, accountType, options,
		&outWindData.fields, &outWindData.errorCode).Detach();

	return outWindData.errorCode;
}


LONG WindCppAPIImp::tlogout(LPCTSTR options)
{
	if (m_pDWindData == NULL)
	{
		return noRegErr;
	}

	long errCode = 0;
	_variant_t fields;
	m_pDWindData->tLogout(options, &fields, &errCode);

	return errCode;
}

LONG WindCppAPIImp::torder(LPCWSTR windCodes, LPCWSTR tradeSide, LPCWSTR orderPrice, LPCWSTR orderVolume, LPCWSTR options, WindData& outWindData)
{
	ClearWindData(outWindData);
	InitWindData(outWindData);

	if (m_pDWindData == NULL)
	{
		return noRegErr;
	}

	outWindData.data = m_pDWindData->tSendOrder(windCodes, tradeSide, orderPrice, orderVolume, options,
		&outWindData.fields, &outWindData.errorCode).Detach();

	return outWindData.errorCode;
}

LONG WindCppAPIImp::tcovered(LPCWSTR windCodes, LPCWSTR tradeSide, LPCWSTR orderVolume, LPCWSTR options, WindData& outWindData)
{
	ClearWindData(outWindData);
	InitWindData(outWindData);

	if (m_pDWindData == NULL)
	{
		return noRegErr;
	}

	outWindData.data = m_pDWindData->tSendCovered(windCodes, tradeSide, orderVolume, options,
		&outWindData.fields, &outWindData.errorCode).Detach();

	return outWindData.errorCode;
}

LONG WindCppAPIImp::tcancel(LPCWSTR orderNumber, LPCWSTR options)
{
	if (m_pDWindData == NULL)
	{
		return noRegErr;
	}

	long errorCode = 0;
	_variant_t fields;

	m_pDWindData->tCancelOrder(orderNumber, options, &fields, &errorCode).Detach();

	return errorCode;
}

LONG WindCppAPIImp::tquery(LPCWSTR qryCode, LPCWSTR options, WindData& outWindData)
{
	ClearWindData(outWindData);
	InitWindData(outWindData);

	if (m_pDWindData == NULL)
	{
		return noRegErr;
	}

	outWindData.data = m_pDWindData->tQuery(qryCode, options,
		&outWindData.fields, &outWindData.errorCode).Detach();

	return outWindData.errorCode;
}

//////////////////////////////////////////////////////////////////////////

LONG WindCppAPIImp::bktstart(LPCWSTR strategyName, LPCWSTR startDate, LPCWSTR endDate, LPCWSTR options, WindData& outWindData)
{
	ClearWindData(outWindData);
	InitWindData(outWindData);

	if (m_pDWindData == NULL)
	{
		return noRegErr;
	}

	outWindData.data = m_pDWindData->bktstart(strategyName, startDate, endDate, options,
		&outWindData.fields, &outWindData.errorCode).Detach();

	return outWindData.errorCode;
}

LONG WindCppAPIImp::bktquery(LPCWSTR qrycode, LPCWSTR qrytime, LPCWSTR options, WindData& outWindData)
{
	ClearWindData(outWindData);
	InitWindData(outWindData);

	if (m_pDWindData == NULL)
	{
		return noRegErr;
	}

	outWindData.data = m_pDWindData->bktquery(qrycode, qrytime, options,
		&outWindData.fields, &outWindData.errorCode).Detach();

	return outWindData.errorCode;
}

LONG WindCppAPIImp::bktorder(LPCWSTR tradeTime, LPCWSTR securityCode, LPCWSTR tradeSide, LPCWSTR tradeVol, LPCWSTR options, WindData& outWindData)
{
	ClearWindData(outWindData);
	InitWindData(outWindData);

	if (m_pDWindData == NULL)
	{
		return noRegErr;
	}

	outWindData.data = m_pDWindData->bktorder(tradeTime, securityCode, tradeSide, tradeVol, options,
		&outWindData.fields, &outWindData.errorCode).Detach();

	return outWindData.errorCode;
}

LONG WindCppAPIImp::bktend(LPCWSTR options, WindData& outWindData)
{
	ClearWindData(outWindData);
	InitWindData(outWindData);

	if (m_pDWindData == NULL)
	{
		return noRegErr;
	}

	outWindData.data = m_pDWindData->bktend(options,
		&outWindData.fields, &outWindData.errorCode).Detach();

	return outWindData.errorCode;
}

LONG WindCppAPIImp::bktstatus(LPCWSTR options, WindData& outWindData)
{
	ClearWindData(outWindData);
	InitWindData(outWindData);

	if (m_pDWindData == NULL)
	{
		return noRegErr;
	}

	outWindData.data = m_pDWindData->bktstatus(options,
		&outWindData.fields, &outWindData.errorCode).Detach();

	return outWindData.errorCode;
}

LONG WindCppAPIImp::bktsummary(LPCWSTR bktID, LPCWSTR view, LPCWSTR options, WindData& outWindData)
{
	ClearWindData(outWindData);
	InitWindData(outWindData);

	if (m_pDWindData == NULL)
	{
		return noRegErr;
	}

	outWindData.data = m_pDWindData->bktsummary(bktID, view, options,
		&outWindData.fields, &outWindData.errorCode).Detach();

	return outWindData.errorCode;
}

LONG WindCppAPIImp::bktdelete(LPCWSTR bktID, LPCWSTR options, WindData& outWindData)
{
	ClearWindData(outWindData);
	InitWindData(outWindData);

	if (m_pDWindData == NULL)
	{
		return noRegErr;
	}

	outWindData.data = m_pDWindData->bktdelete(bktID, options,
		&outWindData.fields, &outWindData.errorCode).Detach();

	return outWindData.errorCode;
}

LONG WindCppAPIImp::bktstrategy(LPCWSTR options, WindData& outWindData)
{
	ClearWindData(outWindData);
	InitWindData(outWindData);

	if (m_pDWindData == NULL)
	{
		return noRegErr;
	}

	outWindData.data = m_pDWindData->bktstrategy(options,
		&outWindData.fields, &outWindData.errorCode).Detach();

	return outWindData.errorCode;
}