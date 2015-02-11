// WAPIWrapperCpp.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "..\header\WAPIWrapperCpp.h"
#include "WindCppAPIImp.h"
#include "WindDataParser.h"
#include "ATLComTime.h"

//////////////////////////////////////////////////////////////////////////
WindData::WindData() : errorCode(0)
{
	InitWindData();
}

WindData::~WindData()
{
	ClearWindData();
}

VOID WindData::InitWindData()
{
	WindCppAPIImp::InitWindData(*this);
}

VOID WindData::ClearWindData()
{
	WindCppAPIImp::ClearWindData(*this);
}

INT WindData::GetTimesLength() const
{
	return WindDataParser::GetCountOfSafeArray(times);
}

INT WindData::GetCodesLength() const
{
	return WindDataParser::GetCountOfSafeArray(codes);
}

INT WindData::GetFieldsLength() const
{
	return WindDataParser::GetCountOfSafeArray(fields);
}

LPCWSTR WindData::GetCodeByIndex(int index) const
{
	return WindDataParser::GetInternalStrByIndex(codes, index);
}

LPCWSTR WindData::GetFieldsByIndex(int index) const
{
	return WindDataParser::GetInternalStrByIndex(fields, index);
}

DATE WindData::GetTimeByIndex(int index) const
{
	return (DATE) WindDataParser::GetDoubeItemByIndex(times, index) - 693960;
}

BOOL WindData::GetTimeByIndex(int index, WCHAR timeBuffer[], int& length) const
{
	DATE time = GetTimeByIndex(index);
	WCHAR *buffer = DateToString(time);

	length = wcslen(buffer) + 1;
	wcscpy_s(timeBuffer, length, buffer);

	delete[] buffer;
	buffer = NULL;

	return TRUE;
}

BOOL WindData::GetDataItem(int timeIndex, int codesIndex, int fieldsIndex, VARIANT& outItem) const
{
	VariantInit(&outItem);
	outItem = WindDataParser::GetVarOfDataFromWindData(timeIndex, codesIndex, fieldsIndex, *this);
	return TRUE;
}

INT WindData::GetRecordCount() const
{
	int fieldsLength = GetFieldsLength();
	int dataLength = WindDataParser::GetCountOfSafeArray(data);

	if (fieldsLength > 0)
	{
		return dataLength/fieldsLength;
	}

	return 0;
}

BOOL WindData::GetTradeItem(int recordIndex, int fieldsIndex, VARIANT& outItem) const
{
	VariantInit(&outItem);
	int fieldsLength = GetFieldsLength();
	int pos = recordIndex * fieldsLength + fieldsIndex;
	outItem = WindDataParser::GetVarOfDataFromWindData(0, 0, pos, *this);
	return TRUE;
}

LPCWSTR WindData::GetErrorMsg() const
{
	if (errorCode != 0)
	{
		int i = WindDataParser::GetStrItemIndexOfSafeArray(fields, L"OUTMESSAGE");

		if (i != -1)
		{
			VARIANT var = WindDataParser::GetVarFromArray(data, i);
			return (LPCWSTR)var.bstrVal;
		}
		else
		{
			i = WindDataParser::GetStrItemIndexOfSafeArray(fields, L"ErrorMsg");
			if (i != -1)
			{
				VARIANT var = WindDataParser::GetVarFromArray(data, i);
				return (LPCWSTR)var.bstrVal;
			}
		}

		return L"Unknown Error";
	}

	return L"OK";
}

//获取登录ID
LONG WindData::GetLogonID() const
{
	if (errorCode == 0)
	{
		int i = WindDataParser::GetStrItemIndexOfSafeArray(fields, L"LogonID");
		if (i != -1)
		{
			VARIANT var = WindDataParser::GetVarFromArray(data, i);
			return var.lVal;
		}
	}

	return -1;
}

//获取Order请求Id
LONG WindData::GetOrderRequestID() const
{
	if (errorCode == 0)
	{
		int i = WindDataParser::GetStrItemIndexOfSafeArray(fields, L"RequestID");
		if (i != -1)
		{
			VARIANT var = WindDataParser::GetVarFromArray(data, i);
			return var.lVal;
		}
	}

	return -1;
}

LPCWSTR WindData::GetOrderNumber(int recordIndex) const
{
	if (errorCode == 0)
	{
		int i = WindDataParser::GetStrItemIndexOfSafeArray(fields, L"OrderNumber");
		if (i != -1)
		{
			VARIANT var;
			GetTradeItem(recordIndex, i, var);
			return var.bstrVal;
		}
	}

	return NULL;
}

//获取日期列表
const DATE* WindData::GetTDaysInfo(LONG& lDateCount) const
{
	lDateCount = 0;
	if (errorCode == 0)
	{
		if (data.vt != (VT_ARRAY | VT_R8) && data.vt != (VT_ARRAY | VT_DATE))
			return NULL;

		if (data.parray != NULL)
		{
			lDateCount = WindDataParser::GetCountOfSafeArray(data);
			DATE* pDateData = (DATE*)data.parray->pvData;

			return pDateData;
		}
	}

	return NULL;
}

//日期转化函数(需要delete)
WCHAR* WindData::DateToString(DATE date, LPCWSTR strFormat)
{
	wstring str = COleDateTime(date).Format(strFormat);
	int length = str.length() + 1;
	WCHAR* timeBuffer = new WCHAR[length];
	swprintf(timeBuffer, length, L"%s", str.c_str());

	return timeBuffer;
}

VOID WindData::FreeString(WCHAR*& pStr)
{
	if (NULL != pStr)
	{
		delete[] pStr;
		pStr = NULL;
	}
}

//////////////////////////////////////////////////////////////////////////

LONG CWAPIWrapperCpp::start(LPCWSTR options /* = NULL */, LPCWSTR options2 /* = NULL */, LONG timeout /* = 5000 */)
{
	return WindCppAPIImp::getIntance().start(options, options2, timeout);
}

LONG CWAPIWrapperCpp::stop()
{
	return WindCppAPIImp::getIntance().stop();
}

LONG CWAPIWrapperCpp::isconnected()
{
	return WindCppAPIImp::getIntance().isconnected();
}

VOID CWAPIWrapperCpp::cancelRequest(ULONGLONG requestId)
{
	return WindCppAPIImp::getIntance().cancelRequest(requestId);
}

VOID CWAPIWrapperCpp::cancelAllRequest()
{
	return WindCppAPIImp::getIntance().cancelAllRequest();
}

BOOL CWAPIWrapperCpp::getErrorMsg(LONG errCode, ErrMsgLang lang, WCHAR msg[], int& msgLength)
{
	return WindCppAPIImp::getIntance().getErrorMsg(errCode, lang, msg, msgLength);
}

//////////////////////////////////////////////////////////////////////////

LONG CWAPIWrapperCpp::wsd(WindData& outWindData, LPCWSTR windCodes, LPCWSTR fields, LPCWSTR startTime /* = NULL */, LPCWSTR endTime /* = NULL */, LPCWSTR options /* = NULL */)
{
	return WindCppAPIImp::getIntance().wsd(windCodes, fields, startTime, endTime, options, outWindData);
}

LONG CWAPIWrapperCpp::wss(WindData& outWindData, LPCWSTR windCodes, LPCWSTR fields, LPCWSTR options /* = NULL */)
{
	return WindCppAPIImp::getIntance().wss(windCodes, fields, options, outWindData);
}

LONG CWAPIWrapperCpp::wsi(WindData& outWindData, LPCWSTR windCodes, LPCWSTR fields, LPCWSTR startTime, LPCWSTR endTime, LPCWSTR options /* = NULL */)
{
	return WindCppAPIImp::getIntance().wsi(windCodes, fields, startTime, endTime, options, outWindData);
}

LONG CWAPIWrapperCpp::wst(WindData& outWindData, LPCWSTR windCodes, LPCWSTR fields, LPCWSTR startTime, LPCWSTR endTime, LPCWSTR options /* = NULL */)
{
	return WindCppAPIImp::getIntance().wst(windCodes, fields, startTime, endTime, options, outWindData);
}

LONG CWAPIWrapperCpp::wsq(WindData& outWindData, LPCWSTR windCodes, LPCWSTR fields, LPCWSTR options /* = NULL */)
{
	return WindCppAPIImp::getIntance().wsqSync(windCodes, fields, options, outWindData);
}

LONG CWAPIWrapperCpp::wsq(ULONGLONG &reqId, LPCWSTR windCodes, LPCWSTR fields, WsqCallBack callBack, LPCWSTR options /* = NULL */, BOOL updateAllFields /* = FALSE */)
{
	return WindCppAPIImp::getIntance().wsqAsyn(windCodes, fields, options, callBack, updateAllFields, reqId);
}

LONG CWAPIWrapperCpp::wsqtd(WindData& outWindData, LPCWSTR windCodes, LPCWSTR fields, LPCWSTR options /* = NULL */)
{
	return WindCppAPIImp::getIntance().wsqSync(windCodes, fields, options, outWindData, true);
}

LONG CWAPIWrapperCpp::wsqtd(ULONGLONG &reqId, LPCWSTR windCodes, LPCWSTR fields, WsqCallBack callBack, LPCWSTR options /* = NULL */, BOOL updateAllFields /* = FALSE */)
{
	return WindCppAPIImp::getIntance().wsqAsyn(windCodes, fields, options, callBack, updateAllFields, reqId, true);
}

LONG CWAPIWrapperCpp::wset(WindData& outWindData, LPCWSTR reportName, LPCWSTR options /* = NULL */)
{
	return WindCppAPIImp::getIntance().wset(reportName, options, outWindData);
}

LONG CWAPIWrapperCpp::edb(WindData& outWindData, LPCWSTR windCodes, LPCWSTR startTime /* = NULL */, LPCWSTR endTime /* = NULL */, LPCWSTR options /* = NULL */)
{
	return WindCppAPIImp::getIntance().edb(outWindData, windCodes, startTime, endTime, options);
}

LONG CWAPIWrapperCpp::wpf(WindData& outWindData, LPCWSTR portfolioName, LPCWSTR viewName, LPCWSTR options /* = NULL */)
{
	return WindCppAPIImp::getIntance().wpf(portfolioName, viewName, options, outWindData);
}

LONG CWAPIWrapperCpp::wupf(WindData& outWindData, LPCWSTR portfolioName, LPCWSTR tradeDate, LPCWSTR windCodes, LPCWSTR quantity, LPCWSTR costPrice, LPCWSTR options /* = NULL */)
{
	return WindCppAPIImp::getIntance().wupf(portfolioName, tradeDate, windCodes, quantity, costPrice, options, outWindData);
}

LONG CWAPIWrapperCpp::weqs(WindData& outWindData, LPCWSTR planName, LPCWSTR options /* = NULL */)
{
	return WindCppAPIImp::getIntance().weqs(planName, options, outWindData);
}

LONG CWAPIWrapperCpp::tdays(WindData& outWindData, LPCWSTR startTime, LPCWSTR endTime, LPCWSTR options /* = NULL */)
{
	return WindCppAPIImp::getIntance().tdays(startTime, endTime, options, outWindData);
}

LONG CWAPIWrapperCpp::tdaysoffset(DATE& outDate, LPCWSTR startTime, LONG offset, LPCWSTR options /* = NULL */)
{
	return WindCppAPIImp::getIntance().tdaysoffset(startTime, offset, options, outDate);
}

LONG CWAPIWrapperCpp::tdayscount(LONG& outCount, LPCWSTR startTime, LPCWSTR endTime, LPCWSTR options /* = NULL */)
{
	return WindCppAPIImp::getIntance().tdayscount(startTime, endTime, options, outCount);
}

//////////////////////////////////////////////////////////////////////////

LONG CWAPIWrapperCpp::tlogon(WindData& outWindData, LPCWSTR brokerID, LPCWSTR departmentID, LPCWSTR accountID, LPCWSTR password, LPCWSTR accountType, LPCWSTR options /* = NULL */)
{
	return WindCppAPIImp::getIntance().tlogon(brokerID, departmentID, accountID, password, accountType, options, outWindData);
}

LONG CWAPIWrapperCpp::tlogout(INT longId/* =0 */)
{
	WCHAR options[32] = {0};
	swprintf(options, 32, L"LogonID=%d", longId);
	return WindCppAPIImp::getIntance().tlogout(options);
}

LONG CWAPIWrapperCpp::torder(WindData& outWindData, LPCWSTR windCodes, LPCWSTR tradeSide, LPCWSTR orderPrice, LPCWSTR orderVolume, LPCWSTR options /* = NULL */)
{
	return WindCppAPIImp::getIntance().torder(windCodes, tradeSide, orderPrice, orderVolume, options, outWindData);
}

LONG CWAPIWrapperCpp::torder(LONG& reqId, LPCWSTR windCodes, LPCWSTR tradeSide, DOUBLE orderPrice, LONG orderVolume, LPCWSTR options /* = NULL */, WCHAR** pErrosMsg /* = NULL */)
{
	WindData outWindData;
	WCHAR volumeBuffer[32] = {0};
	WCHAR priceBuffer[32] = {0};
	reqId = 0;
	swprintf(priceBuffer, 32, L"%.6f", orderPrice);
	swprintf(volumeBuffer, 32, L"%d", orderVolume);
	LONG errCode = torder(outWindData, windCodes, tradeSide, priceBuffer, volumeBuffer, options);
	if (0 == errCode)
	{
		reqId = outWindData.GetOrderRequestID();
	}
	else if (NULL != pErrosMsg)
	{
		LPCWSTR pError = outWindData.GetErrorMsg();
		if (pError != NULL)
		{
			int length = wcslen(pError) + 1;
			*pErrosMsg = new WCHAR[length];
			wcscpy_s(*pErrosMsg, length, pError);
		}
	}

	return errCode;
}

LONG CWAPIWrapperCpp::tcovered(WindData& outWindData, LPCWSTR windCodes, LPCWSTR tradeSide, LPCWSTR orderVolume, LPCWSTR options /* = NULL */)
{
	return WindCppAPIImp::getIntance().tcovered(windCodes, tradeSide, orderVolume, options, outWindData);
}

LONG CWAPIWrapperCpp::tcovered(LONG& reqId, LPCWSTR windCodes, LPCWSTR tradeSide, LONG orderVolume, LPCWSTR options /* = NULL */, WCHAR** pErrosMsg /* = NULL */)
{
	WindData outWindData;
	WCHAR volumeBuffer[32] = {0};
	reqId = 0;

	swprintf(volumeBuffer, 32, L"%d", orderVolume);
	LONG errCode = tcovered(outWindData, windCodes, tradeSide, volumeBuffer, options);
	if (0 == errCode)
	{
		reqId = outWindData.GetOrderRequestID();
	}
	else if (NULL != pErrosMsg)
	{
		LPCWSTR pError = outWindData.GetErrorMsg();
		if (pError != NULL)
		{
			int length = wcslen(pError) + 1;
			*pErrosMsg = new WCHAR[length];
			wcscpy_s(*pErrosMsg, length, pError);
		}
	}

	return errCode;
}

LONG CWAPIWrapperCpp::tcancel(LPCWSTR orderNumber, LPCWSTR options/* =NULL */)
{
	return WindCppAPIImp::getIntance().tcancel(orderNumber, options);
}

LONG CWAPIWrapperCpp::tquery(WindData& outWindData, LPCWSTR qryCode, LPCWSTR options /* = NULL */)
{
	return WindCppAPIImp::getIntance().tquery(qryCode, options, outWindData);
}

//////////////////////////////////////////////////////////////////////////

LONG CWAPIWrapperCpp::bktstart(WindData& outWindData, LPCWSTR strategyName, LPCWSTR startDate, LPCWSTR endDate, LPCWSTR options /* = NULL */)
{
	return WindCppAPIImp::getIntance().bktstart(strategyName, startDate, endDate, options, outWindData);
}

LONG CWAPIWrapperCpp::bktquery(WindData& outWindData, LPCWSTR qrycode, LPCWSTR qrytime, LPCWSTR options /* = NULL */)
{
	return WindCppAPIImp::getIntance().bktquery(qrycode, qrytime, options, outWindData);
}

LONG CWAPIWrapperCpp::bktorder(WindData& outWindData, LPCWSTR tradeTime, LPCWSTR securityCode, LPCWSTR tradeSide, LPCWSTR tradeVol, LPCWSTR options /* = NULL */)
{
	return WindCppAPIImp::getIntance().bktorder(tradeTime, securityCode, tradeSide, tradeVol, options, outWindData);
}

LONG CWAPIWrapperCpp::bktend(WindData& outWindData, LPCWSTR options)
{
	return WindCppAPIImp::getIntance().bktend(options, outWindData);
}

LONG CWAPIWrapperCpp::bktstatus(WindData& outWindData, LPCWSTR options)
{
	return WindCppAPIImp::getIntance().bktstatus(options, outWindData);
}

LONG CWAPIWrapperCpp::bktsummary(WindData& outWindData, LPCWSTR bktID, LPCWSTR view, LPCWSTR options)
{
	return WindCppAPIImp::getIntance().bktsummary(bktID, view, options, outWindData);
}

LONG CWAPIWrapperCpp::bktdelete(WindData& outWindData, LPCWSTR bktID, LPCWSTR options)
{
	return WindCppAPIImp::getIntance().bktdelete(bktID, options, outWindData);
}

LONG CWAPIWrapperCpp::bktstrategy(WindData& outWindData, LPCWSTR options)
{
	return WindCppAPIImp::getIntance().bktstrategy(options, outWindData);
}
