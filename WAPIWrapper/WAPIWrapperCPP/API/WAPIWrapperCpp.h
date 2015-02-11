#ifndef _WAPIWRAPPERCPP_H
#define _WAPIWRAPPERCPP_H

#include <windows.h>
#include <comutil.h>

#ifdef WAPIWRAPPERCPP_EXPORTS
#define WAPIWRAPPERCPP_EXP __declspec(dllexport)
#else
#define WAPIWRAPPERCPP_EXP __declspec(dllimport)
#endif

//控件未注册错误
const LONG noRegErr = -1;

enum ErrMsgLang //错误信息语言
{
	eENG = 0,	///< 英语
	eCHN,		///< 中文
};

class WAPIWRAPPERCPP_EXP WindData
{
public:
	VARIANT	data;	///< 数据
	VARIANT codes;	///< Code列表
	VARIANT fields;	///< 指标列表
	VARIANT times;	///< 时间列表
	LONG errorCode;	///< 错误码
public:

	WindData();
	~WindData();

	VOID InitWindData();
	VOID ClearWindData();

	//获取错误信息
	LPCWSTR GetErrorMsg() const;

	//日期转化函数(需要delete)
	static WCHAR* DateToString(DATE date, LPCWSTR strFormat = L"%Y-%m-%d");
	static VOID FreeString(WCHAR*& pStr);

	//////////////////////////////////////////////////////////////////////////
	//数据使用

	INT GetCodesLength() const;
	INT GetFieldsLength() const;
	INT GetTimesLength() const;

	LPCWSTR GetCodeByIndex(int index) const;
	LPCWSTR GetFieldsByIndex(int index) const;
	BOOL GetTimeByIndex(int index, WCHAR timeBuffer[], int& length) const;
	DATE GetTimeByIndex(int index) const;

	//获取数据查询接口OneData
	BOOL GetDataItem(int timeIndex, int codesIndex, int fieldsIndex, VARIANT& outItem) const;

	//获取日期列表
	const DATE* GetTDaysInfo(LONG& lDateCount) const;

	//////////////////////////////////////////////////////////////////////////
	//交易使用

	INT GetRecordCount() const;

	//获取交易接口OneData
	BOOL GetTradeItem(int recordIndex, int fieldsIndex, VARIANT& outItem) const;

	//获取登录ID
	LONG GetLogonID() const;

	//获取Order请求Id
	LONG GetOrderRequestID() const;

	//获取OrderNumber
	LPCWSTR GetOrderNumber(int recordIndex = 0) const;
};

// 回调函数定义
typedef LONG (WINAPI *WsqCallBack)(const WindData &windData);

// 此类是从 WAPIWrapperCpp.dll 导出的
class WAPIWRAPPERCPP_EXP CWAPIWrapperCpp {
public:
	
	//基本函数
	//////////////////////////////////////////////////////////////////////////
	//Wind接口启动函数
	static LONG start(LPCWSTR options = NULL, LPCWSTR options2 = NULL, LONG timeout = 5000);

	//Wind接口终止函数
	static LONG stop();

	//判断连接状态
	static LONG isconnected();

	//若 requestId 为0 取消所有请求；否则只取消给定ID号的请求
	static VOID cancelRequest(ULONGLONG requestId);

	//取消所有请求
	static VOID cancelAllRequest();

	//获取错误码相应的错误信息
	static BOOL getErrorMsg(LONG errCode, ErrMsgLang lang, WCHAR msg[], int& /*inout*/msgLength);

	//////////////////////////////////////////////////////////////////////////

	//数据函数
	//////////////////////////////////////////////////////////////////////////
	//多值函数wsd，获得日期序列
	static LONG wsd(WindData& outWindData, LPCWSTR windCodes, LPCWSTR fields, LPCWSTR startTime = NULL, LPCWSTR endTime = NULL, LPCWSTR options = NULL);

	//多值函数wss，获得历史快照
	static LONG wss(WindData& outWindData, LPCWSTR windCodes, LPCWSTR fields, LPCWSTR options = NULL);

	//多值函数wsi，获得分钟序列
	static LONG wsi(WindData& outWindData, LPCWSTR windCodes, LPCWSTR fields, LPCWSTR startTime, LPCWSTR endTime, LPCWSTR options = NULL);

	//多值函数wst，获得日内跳价
	static LONG wst(WindData& outWindData, LPCWSTR windCodes, LPCWSTR fields, LPCWSTR startTime, LPCWSTR endTime, LPCWSTR options = NULL);

	//多值函数wsq，获得实时行情
	//非订阅模式，取一次性快照数据
	static LONG wsq(WindData& outWindData, LPCWSTR windCodes, LPCWSTR fields, LPCWSTR options = NULL);

	//订阅模式，订阅实时数据，数据通过回调函数返回
	static LONG wsq(ULONGLONG &reqId, LPCWSTR windCodes, LPCWSTR fields, WsqCallBack callBack, LPCWSTR options = NULL, BOOL updateAllFields = FALSE);

	//多值函数wset，获得指定数据集
	static LONG wset(WindData& outWindData, LPCWSTR reportName, LPCWSTR options = NULL);

	//多值函数edb，获得经济数据
	static LONG edb(WindData& outWindData, LPCWSTR windCodes, LPCWSTR startTime = NULL, LPCWSTR endTime = NULL, LPCWSTR options = NULL);

	//组合报表函数
	static LONG wpf(WindData& outWindData, LPCWSTR portfolioName, LPCWSTR viewName, LPCWSTR options = NULL);

	//组合上传函数
	static LONG wupf(WindData& outWindData, LPCWSTR portfolioName, LPCWSTR tradeDate, LPCWSTR windCodes, LPCWSTR quantity, LPCWSTR costPrice, LPCWSTR options = NULL);

	//证劵筛选函数
	static LONG weqs(WindData& outWindData, LPCWSTR planName, LPCWSTR options = NULL);

	//日历日、工作日、交易日的日期序列函数
	static LONG tdays(WindData& outWindData, LPCWSTR startTime, LPCWSTR endTime, LPCWSTR options = NULL);

	//日历日、工作日、交易日的日期偏移计算
	static LONG tdaysoffset(DATE& outDate, LPCWSTR startTime, LONG offset, LPCWSTR options = NULL);

	//日历日、工作日、交易日的日期天数计算
	static LONG tdayscount(LONG& outCount, LPCWSTR startTime, LPCWSTR endTime, LPCWSTR options = NULL);
	//////////////////////////////////////////////////////////////////////////

	//交易函数
	//////////////////////////////////////////////////////////////////////////
	//交易账号登陆
	static LONG tlogon(WindData& outWindData, LPCWSTR brokerID, LPCWSTR departmentID, LPCWSTR accountID, LPCWSTR password, LPCWSTR accountType, LPCWSTR options = NULL);

	//交易账号登出, 0:登出全部已登录的账号
	static LONG tlogout(INT longId=0);

	//下单
	static LONG torder(WindData& outWindData, LPCWSTR windCodes, LPCWSTR tradeSide, LPCWSTR orderPrice, LPCWSTR orderVolume, LPCWSTR options = NULL);
	static LONG torder(LONG& reqId, LPCWSTR windCodes, LPCWSTR tradeSide, DOUBLE orderPrice, LONG orderVolume, LPCWSTR options = NULL, WCHAR** pErrosMsg = NULL);

	//备兑
	static LONG tcovered(WindData& outWindData, LPCWSTR windCodes, LPCWSTR tradeSide, LPCWSTR orderVolume, LPCWSTR options = NULL);
	static LONG tcovered(LONG& reqId, LPCWSTR windCodes, LPCWSTR tradeSide, LONG orderVolume, LPCWSTR options = NULL, WCHAR** pErrosMsg = NULL);

	//交易情况查询
	static LONG tquery(WindData& outWindData, LPCWSTR qryCode, LPCWSTR options = NULL);

	//撤单
	static LONG tcancel(LPCWSTR orderNumber, LPCWSTR options = NULL);
	//////////////////////////////////////////////////////////////////////////

	//回测函数
	//////////////////////////////////////////////////////////////////////////
	//回测开始
	static LONG bktstart(WindData& outWindData, LPCWSTR strategyName, LPCWSTR startDate, LPCWSTR endDate, LPCWSTR options = NULL);

	//回测查询
	static LONG bktquery(WindData& outWindData, LPCWSTR qrycode, LPCWSTR qrytime, LPCWSTR options = NULL);

	//回测下单
	static LONG bktorder(WindData& outWindData, LPCWSTR tradeTime, LPCWSTR securityCode, LPCWSTR tradeSide, LPCWSTR tradeVol, LPCWSTR options = NULL);

	//回测结束
	static LONG bktend(WindData& outWindData, LPCWSTR options);

	//查看回测状态
	static LONG bktstatus(WindData& outWindData, LPCWSTR options);

	//回测概要
	static LONG bktsummary(WindData& outWindData, LPCWSTR bktID, LPCWSTR view, LPCWSTR options);

	//回测删除
	static LONG bktdelete(WindData& outWindData, LPCWSTR bktID, LPCWSTR options);

	//返回策略列表
	static LONG bktstrategy(WindData& outWindData, LPCWSTR options);

	//////////////////////////////////////////////////////////////////////////
};

#endif
