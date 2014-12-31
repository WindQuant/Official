/* Copyright 2013. Wind Info上海万得信息技术股份有限公司
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:  The above
 * copyright notice and this permission notice shall be included in all copies
 * or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#ifndef _WINDQUANTAPI_H
#define _WINDQUANTAPI_H

#include <windows.h>
#include <tchar.h>
//#include <ATLComTime.h>

#define SMALLSTRING_SIZE 64
#define DATA_DIMENSION_SIZE 64

// 错误码的类型定义
typedef int WQErr;

// 请求ID的类型定义
typedef __int64 WQID;

// 错误码的具体定义，更详细的说明，可以调用WErr函数获取

// 操作成功
#define WQERR_OK                      0              

#define WQERR_BASE                    -40520000

// 一般性错误
#define WQERR_GENERAL_CLASS           WQERR_BASE

// 未知错误 
#define WQERR_UNKNOWN                 WQERR_GENERAL_CLASS - 1          

// 内部错误
#define WQERR_INTERNAL_ERROR          WQERR_GENERAL_CLASS - 2          

// 操作系统原因
#define WQERR_SYSTEM_REASON           WQERR_GENERAL_CLASS - 3           

// 登陆失败
#define WQERR_LOGON_FAILED            WQERR_GENERAL_CLASS - 4           

// 无登陆权限
#define WQERR_LOGON_NOAUTH            WQERR_GENERAL_CLASS - 5           

// 用户取消
#define WQERR_USER_CANCEL             WQERR_GENERAL_CLASS - 6           

// 没有可用数据
#define WQERR_NO_DATA_AVAILABLE       WQERR_GENERAL_CLASS - 7           

// 请求超时
#define WQERR_TIMEOUT                 WQERR_GENERAL_CLASS - 8           

// Wbox错误
#define WQERR_LOST_WBOX               WQERR_GENERAL_CLASS - 9           

// 未找到相关内容
#define WQERR_ITEM_NOT_FOUND          WQERR_GENERAL_CLASS - 10          

// 未找到相关服务
#define WQERR_SERVICE_NOT_FOUND       WQERR_GENERAL_CLASS - 11          

// 未找到相关ID
#define WQERR_ID_NOT_FOUND            WQERR_GENERAL_CLASS - 12 

// 已在本机使用其他账号登录万得其他产品，故无法使用指定账号登录
#define WQERR_LOGON_CONFLICT          WQERR_GENERAL_CLASS - 13

// 未登录使用WIM工具，故无法登录
#define WQERR_LOGON_NO_WIM            WQERR_GENERAL_CLASS - 14

// 连续登录失败次数过多
#define WQERR_TOO_MANY_LOGON_FAILURE  WQERR_GENERAL_CLASS - 15

// 网络数据存取错误
#define WQERR_IOERROR_CLASS           WQERR_BASE - 1000
// IO操作错误
#define WQERR_IO_ERROR                WQERR_IOERROR_CLASS - 1

// 后台服务器不可用
#define WQERR_SERVICE_NOT_AVAL        WQERR_IOERROR_CLASS - 2

// 网络连接失败
#define WQERR_CONNECT_FAILED          WQERR_IOERROR_CLASS - 3

// 请求发送失败
#define WQERR_SEND_FAILED             WQERR_IOERROR_CLASS - 4

// 数据接收失败
#define WQERR_RECEIVE_FAILED          WQERR_IOERROR_CLASS - 5

// 网络错误
#define WQERR_NETWORK_ERROR           WQERR_IOERROR_CLASS - 6

// 服务器拒绝请求
#define WQERR_SERVER_REFUSED          WQERR_IOERROR_CLASS - 7

// 错误的应答
#define WQERR_SVR_BAD_RESPONSE        WQERR_IOERROR_CLASS - 8

// 数据解码失败
#define WQERR_DECODE_FAILED           WQERR_IOERROR_CLASS - 9

// 网络超时
#define WQERR_INTERNET_TIMEOUT        WQERR_IOERROR_CLASS - 10

// 频繁访问
#define WQERR_ACCESS_FREQUENTLY       WQERR_IOERROR_CLASS - 11

//服务器内部错误
#define WQERR_SERVER_INTERNAL_ERROR	  WQERR_IOERROR_CLASS - 12


// 请求输入错误
#define WQERR_INVALID_CLASS           WQERR_BASE - 2000
// 无合法会话
#define WQERR_ILLEGAL_SESSION         WQERR_INVALID_CLASS - 1

// 非法数据服务
#define WQERR_ILLEGAL_SERVICE         WQERR_INVALID_CLASS - 2

// 非法请求
#define WQERR_ILLEGAL_REQUEST         WQERR_INVALID_CLASS - 3

// 万得代码语法错误
#define WQERR_WINDCODE_SYNTAX_ERR     WQERR_INVALID_CLASS - 4

// 不支持的万得代码
#define WQERR_ILLEGAL_WINDCODE        WQERR_INVALID_CLASS - 5

// 指标语法错误
#define WQERR_INDICATOR_SYNTAX_ERR    WQERR_INVALID_CLASS - 6

// 不支持的指标
#define WQERR_ILLEGAL_INDICATOR       WQERR_INVALID_CLASS - 7

// 指标参数语法错误
#define WQERR_OPTION_SYNTAX_ERR       WQERR_INVALID_CLASS - 8

// 不支持的指标参数
#define WQERR_ILLEGAL_OPTION          WQERR_INVALID_CLASS - 9

// 日期与时间语法错误
#define WQERR_DATE_TIME_SYNTAX_ERR    WQERR_INVALID_CLASS - 10

// 不支持的日期与时间
#define WQERR_INVALID_DATE_TIME       WQERR_INVALID_CLASS - 11

// 不支持的请求参数
#define WQERR_ILLEGAL_ARG             WQERR_INVALID_CLASS - 12

// 数组下标越界
#define WQERR_INDEX_OUT_OF_RANGE      WQERR_INVALID_CLASS - 13

// 重复的WQID
#define WQERR_DUPLICATE_WQID          WQERR_INVALID_CLASS - 14

// 请求无相应权限
#define WQERR_UNSUPPORTED_NOAUTH      WQERR_INVALID_CLASS - 15

// 不支持的数据类型
#define WQERR_UNSUPPORTED_DATA_TYPE   WQERR_INVALID_CLASS - 16

// 数据提取量超限
#define WQERR_DATA_QUOTA_EXCEED       WQERR_INVALID_CLASS - 17

// 不支持的请求参数
#define WQERR_ILLEGAL_ARG_COMBINATION WQERR_INVALID_CLASS - 18

// 日期时间数组定义
typedef struct 
{
    int arrLen;
    DATE* timeArray;
} WQDateTimeArray;

typedef char WQWindCode[SMALLSTRING_SIZE];
typedef char WQWindField[SMALLSTRING_SIZE];
// Windcode数组定义
typedef struct 
{
    int arrLen;
    WQWindCode* codeArray;
} WQWindCodeArray;

// 数据指标数组定义
typedef struct 
{
    int arrLen;
    WQWindField* fieldsArray;
} WQWindFieldsArray;

// 数据结构定义
// 所有通过万得函数接口取到的数据都是以下这个三维数组结构
// 三维分别是：
// 第一维时间，第二维品种，第三维指标。
typedef struct tagQuantData
{
    WQDateTimeArray ArrDateTime;        // 时间列表（表头）
    WQWindCodeArray ArrWindCode;        // 品种列表（表头）
    WQWindFieldsArray ArrWindFields;    // 指标列表（表头）
    VARIANT MatrixData;                 // 数据
} WQData;

// 
// 返回Events结构类型定义
typedef enum 
{
    eWQLogin = 1,                       // 登录相关信息
    eWQResponse,                        // 数据返回
    eWQPartialResponse,                 // 部分数据返回（订阅时适用）
    eWQErrorReport,                     // 出错信息
    eWQOthers                           // 其他信息
} WQEventType;

// 取错误信息时的语言定义
typedef enum 
{
    eENG = 0,
    eCHN,
} WQLangType;

// 返回Events数据结构定义
typedef struct tagQuantEvent
{
    int Version;							// 版本号，以备今后扩充
    WQEventType EventType;                  // Event类型
    WQErr ErrCode;							// 错误码
    WQID RequestID;							// 对应的request
    WQID EventID;						    // 流水号
    WQData* pQuantData;      			    // 包含的数据
} WQEvent;

// 回调函数定义
typedef int (WINAPI *IEventHandler) (WQEvent* pEvent, LPVOID lpUserParam);

// 登录信息定义
typedef struct
{
	int bSilentLogin; ///@param  是否显示登陆对话框，0：显示；1：不显示
	WCHAR strUserName[SMALLSTRING_SIZE]; ///登录用户名,当bSilentLogin=true时有效
	WCHAR strPassword[SMALLSTRING_SIZE];///登录密码,当bSilentLogin=true时有效
} WQAUTH_INFO, *PWQAUTH_INFO;

#ifdef __cplusplus
extern "C" 
{
#endif

    // 设置主回调函数，必须一开始调用
    WQErr       WINAPI SetEventHandler(IEventHandler eventHandler);
    // Wind认证登录，必须成功登录方可调用接口函数
    WQErr       WINAPI WDataAuthorize(WQAUTH_INFO *pAuthInfo);
    // wind认证登出
    WQErr       WINAPI WDataAuthQuit();

    // Wind函数使用说明：
    // 以下所有的取数据的函数都是异步函数。
    // 一般性的共通语法要求如下：
    // reqEventHandler参数：如果设置了这个参数，则调用此回调函数返回数据；否则使用预先指定的主回调函数返回数据。
    // lpReqParam参数：用户参数，回调时原样返回。
    // windcode参数：函数能够接受的windcode。如果有多个，用半角逗号隔开。例如"000001.SZ,000002.SZ"。
    // indicators参数：函数能够接受的指标名称。如果有多个，用半角逗号隔开。例如"high,open,low,close"。
    // beginTime与endTime参数：函数能够接受的时间和日期字符串。可接受的字符串必须形如"yyyymmdd"，"yyyy-mm-dd"，"yyyymmdd HHMMSS"或者"yyyy-mm-dd HH:MM:SS"
    // params参数：函数能够接受的参数。形如"param1=value1;param2=value2"，多个参数用半角分号隔开。
    //
    // WSD函数，取时间序列数据，支持单品种多指标多时间
    WQID        WINAPI WSD (LPCWSTR windcode, LPCWSTR indicators, LPCWSTR beginTime, LPCWSTR endTime, LPCWSTR params, IEventHandler reqEventHandler, LPVOID lpReqParam);
    // WSS函数，取快照数据，支持多品种多指标单时间
    WQID        WINAPI WSS (LPCWSTR windcodes, LPCWSTR indicators, LPCWSTR params, IEventHandler reqEventHandler, LPVOID lpReqParam);
    // WST函数，取日内跳价数据，现为单品种的当日数据
    WQID        WINAPI WST (LPCWSTR windcode, LPCWSTR indicators, LPCWSTR beginTime, LPCWSTR endTime, LPCWSTR params, IEventHandler reqEventHandler, LPVOID lpReqParam);
    // WSI函数，取分钟序列数据，现支持单品种最近一年的数据（单次限制为三个月）
    WQID        WINAPI WSI (LPCWSTR windcode, LPCWSTR indicators, LPCWSTR beginTime, LPCWSTR endTime, LPCWSTR params, IEventHandler reqEventHandler, LPVOID lpReqParam);
    // WSQ函数，取实时行情数据，支持多品种与多指标，可订阅，可取快照
    WQID        WINAPI WSQ (LPCWSTR windcodes, LPCWSTR indicators, LPCWSTR params, IEventHandler reqEventHandler, LPVOID lpReqParam);
    // WSET函数，取相关数据集数据，如指数成分等
    WQID        WINAPI WSET (LPCWSTR reportName, LPCWSTR params, IEventHandler reqEventHandler, LPVOID lpReqParam);
    // WPF函数，取组合管理数据
    WQID		WINAPI WPF (LPCWSTR portfolioName, LPCWSTR viewName, LPCWSTR params, IEventHandler reqEventHandler, LPVOID lpReqParam);
    // WUPF函数,组合上传
	WQID		WINAPI WUPF(LPCWSTR PortfolioName, LPCWSTR TradeDate, LPCWSTR WindCode, LPCWSTR Quantity, LPCWSTR CostPrice, LPCWSTR params, IEventHandler reqEventHandler, LPVOID lpReqParam);
	// WEQS函数，完成证券筛选功能。需要在万得终端里预先定义筛选方案
    WQID		WINAPI WEQS (LPCWSTR filterName, LPCWSTR params, IEventHandler reqEventHandler, LPVOID lpReqParam);
    // TDays函数，生成指定的日期序列
    WQID        WINAPI TDays (LPCWSTR beginTime, LPCWSTR endTime, LPCWSTR params, IEventHandler reqEventHandler, LPVOID lpReqParam);
    // TDaysOffset函数，给定一个日期及一个偏移量，计算另一个日期
    WQID        WINAPI TDaysOffset (LPCWSTR beginTime, LPCWSTR offset, LPCWSTR params, IEventHandler reqEventHandler, LPVOID lpReqParam);
    // TDaysCount函数，计算两个给定日期的间隔
    WQID        WINAPI TDaysCount (LPCWSTR beginTime, LPCWSTR endTime, LPCWSTR params, IEventHandler reqEventHandler, LPVOID lpReqParam); 

	// EDB函数, 取经济数据
	WQID WINAPI EDB(LPCTSTR windcode, LPCTSTR beginTime, LPCTSTR endTime, LPCTSTR params, IEventHandler reqEventHandler, LPVOID lpReqParam);

	//回测函数
	WQID  BktStart(LPCWSTR StrategyName, LPCWSTR StartDate, LPCWSTR EndDate, LPCWSTR params, ::IEventHandler reqEventHandler, LPVOID lpReqParam);
	WQID  BktQuery(LPCWSTR qrycode, LPCWSTR qrytime, LPCWSTR params, ::IEventHandler reqEventHandler, LPVOID lpReqParam);
	WQID  BktOrder(LPCWSTR TradeTime, LPCWSTR SecurityCode, LPCWSTR TradeSide, LPCWSTR TradeVol, LPCWSTR params, ::IEventHandler reqEventHandler, LPVOID lpReqParam);
	WQID  BktStatus(LPCWSTR params, ::IEventHandler reqEventHandler, LPVOID lpReqParam);
	WQID  BktEnd(LPCWSTR params, ::IEventHandler reqEventHandler, LPVOID lpReqParam);
	WQID  BktSummary(LPCWSTR BktID, LPCWSTR View, LPCWSTR params, ::IEventHandler reqEventHandler, LPVOID lpReqParam);
	WQID  BktDelete(LPCWSTR BktID, LPCWSTR params, ::IEventHandler reqEventHandler, LPVOID lpReqParam);
	WQID  BktStrategy(LPCWSTR params, ::IEventHandler reqEventHandler, LPVOID lpReqParam);
	WQID  BktFocus(LPCWSTR StrategyID,LPCWSTR params, ::IEventHandler reqEventHandler, LPVOID lpReqParam);
	WQID  BktShare(LPCWSTR StrategyID,LPCWSTR params, ::IEventHandler reqEventHandler, LPVOID lpReqParam);

	// 取消订阅请求
    WQErr       WINAPI CancelRequest(WQID id);
    // 取消所有请求
    WQErr       WINAPI CancelAllRequest();
    // 取错误码的说明信息
    const WCHAR* WINAPI WErr(WQErr errCode, WQLangType lang);

    // Internal Use
    int         WINAPI GetInternalStatus();

#ifdef __cplusplus
}
#endif

#endif // _WINDQUANTAPI_H