#ifndef _WINDCPPAPIIMP_H
#define _WINDCPPAPIIMP_H

#include <memory>

class WindDataComSink;
class WindData;
enum ErrMsgLang;

// 回调函数定义
typedef LONG (WINAPI *WsqAsynCallBack)(const WindData &windData);

//接口类
class WindCppAPIImp {

public:

	static WindCppAPIImp& getIntance();

	//基本函数
	//////////////////////////////////////////////////////////////////////////
	//Wind接口启动函数
	LONG start(LPCWSTR options = L"", LPCWSTR options2 = L"", LONG timeout = 5000);

	//Wind接口终止函数
	LONG stop();

	//判断连接状态
	LONG isconnected();

	//若 requestId 为0 取消所有请求；否则只取消给定ID号的请求
	VOID cancelRequest(ULONGLONG requestId);

	//取消所有请求
	VOID cancelAllRequest();

	LONG readdata(ULONGLONG requestid, WindData& outWindData, LONG& requestState);

	//获取错误码相应的错误信息
	BOOL getErrorMsg(LONG errCode, ErrMsgLang lang, WCHAR msg[], int& nBufferLength);
	BOOL getErrorMsg(LONG errCode, ErrMsgLang lang, VARIANT &msg);
	//////////////////////////////////////////////////////////////////////////

	//数据函数
	//////////////////////////////////////////////////////////////////////////
	//多值函数wsd，获得日期序列
	LONG wsd(LPCWSTR windCodes, LPCWSTR fields, LPCWSTR startTime, LPCWSTR endTime, LPCWSTR options, WindData& outWindData);

	//多值函数wss，获得历史快照
	LONG wss(LPCWSTR windCodes, LPCWSTR fields, LPCWSTR options, WindData& outWindData);

	//多值函数wsi，获得分钟序列
	LONG wsi(LPCWSTR windCodes, LPCWSTR fields, LPCWSTR startTime, LPCWSTR endTime, LPCWSTR options, WindData& outWindData);

	//多值函数wst，获得日内跳价
	LONG wst(LPCWSTR windCodes, LPCWSTR fields, LPCWSTR startTime, LPCWSTR endTime, LPCWSTR options, WindData& outWindData);

	//多值函数wsq，获得实时行情
	//非订阅模式，取一次性快照数据
	LONG wsqSync(LPCWSTR windCodes, LPCWSTR fields, LPCWSTR options, WindData& outWindData, bool isTD = false);

	//订阅模式，订阅实时数据，数据通过回调函数返回
	LONG wsqAsyn(LPCWSTR windCodes, LPCWSTR fields, LPCWSTR options, WsqAsynCallBack callBack, BOOL updateAllFields, ULONGLONG &reqId, bool isTD = false);

	//多值函数wset，获得指定数据集
	LONG wset(LPCWSTR reportName, LPCWSTR options, WindData& outWindData);

	//多值函数edb，获得经济数据
	LONG edb(WindData& outWindData, LPCWSTR windCodes, LPCWSTR startTime, LPCWSTR endTime, LPCWSTR options);

	//组合报表函数
	LONG wpf(LPCWSTR portfolioName, LPCWSTR viewName, LPCWSTR options, WindData& outWindData);

	//组合上传函数
	LONG wupf(LPCWSTR portfolioName, LPCWSTR tradeDate, LPCWSTR windCodes, LPCWSTR quantity, LPCWSTR costPrice, LPCWSTR options, WindData& outWindData);

	//证劵筛选函数
	LONG weqs(LPCWSTR planName, LPCWSTR options, WindData& outWindData);

	//日历日、工作日、交易日的日期序列函数
	LONG tdays(LPCWSTR startTime, LPCWSTR endTime, LPCWSTR options, WindData& outWindData);

	//日历日、工作日、交易日的日期偏移计算
	LONG tdaysoffset(LPCWSTR startTime, LONG offset, LPCWSTR options, DATE& outDate);

	//日历日、工作日、交易日的日期天数计算
	LONG tdayscount(LPCWSTR startTime, LPCWSTR endTime, LPCWSTR options, LONG& outCount);
	//////////////////////////////////////////////////////////////////////////

	//交易函数
	//////////////////////////////////////////////////////////////////////////
	//交易账号登陆
	LONG tlogon(LPCWSTR brokerID, LPCWSTR departmentID, LPCWSTR accountID, LPCWSTR password, LPCWSTR accountType, LPCWSTR options, WindData& outWindData);

	//交易账号登出，如options为登录ID，格式如："LogonID=1"
	LONG tlogout(LPCTSTR options);

	//下单
	LONG torder(LPCWSTR windCodes, LPCWSTR tradeSide, LPCWSTR orderPrice, LPCWSTR orderVolume, LPCWSTR options, WindData& outWindData);

	//平仓
	LONG tcovered(LPCWSTR windCodes, LPCWSTR tradeSide, LPCWSTR orderVolume, LPCWSTR options, WindData& outWindData);

	//撤单
	LONG tcancel(LPCWSTR orderNumber, LPCWSTR options);

	//交易情况查询
	LONG tquery(LPCWSTR qryCode, LPCWSTR options, WindData& outWindData);
	//////////////////////////////////////////////////////////////////////////

	//回测函数
	//////////////////////////////////////////////////////////////////////////
	//回测开始
	LONG bktstart(LPCWSTR strategyName, LPCWSTR startDate, LPCWSTR endDate, LPCWSTR options, WindData& outWindData);

	//回测查询
	LONG bktquery(LPCWSTR qrycode, LPCWSTR qrytime, LPCWSTR options, WindData& outWindData);

	//回测下单
	LONG bktorder(LPCWSTR tradeTime, LPCWSTR securityCode, LPCWSTR tradeSide, LPCWSTR tradeVol, LPCWSTR options, WindData& outWindData);

	//回测结束
	LONG bktend(LPCWSTR options, WindData& outWindData);

	//查看回测状态
	LONG bktstatus(LPCWSTR options, WindData& outWindData);

	//回测概要
	LONG bktsummary(LPCWSTR bktID, LPCWSTR view, LPCWSTR options, WindData& outWindData);

	//回测删除
	LONG bktdelete(LPCWSTR bktID, LPCWSTR options, WindData& outWindData);

	//返回策略列表
	LONG bktstrategy(LPCWSTR options, WindData& outWindData);

	//////////////////////////////////////////////////////////////////////////
	~WindCppAPIImp();
private:
	WindCppAPIImp();
	

	BOOL initialize();
	VOID release();
public:
	static VOID InitWindData(WindData& windData);
	static VOID ClearWindData(WindData& windData);
	static VOID EmptyWindData(WindData& windData);

private:

	WindDataCOMLib::_DWindDataCOMPtr	m_pDWindData;
	WindDataComSink*					m_pSink;
	DWORD								m_dwAdvise;
};

#endif