// WindTradeCppSample.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include "WAPIWrapperCpp.h"

#include "atlstr.h"
#include "atlcomtime.h"
#include <iostream>
using namespace std;

CString toString(const LPVARIANT data);
void printErrMsg(long errorCode);
bool testLogonWFT();
void testLogoutWFT();
bool testLogonAccount();
void testLogoutAccount();

void testOrder();
void testCancelOrder();
void testQueryPosition();

long logonID = 0;
long reqID1 = 0;
long reqID2 = 0;

int _tmain(int argc, _TCHAR* argv[])
{
	wcout.imbue(locale("chs"));

	if (!testLogonWFT())
	{
		return 0;
	}
	if (!testLogonAccount())
	{
		return 0;
	}

	testOrder();
	testCancelOrder();

	testQueryPosition();

	testLogoutAccount();
	testLogoutWFT();

	system("pause");
	return 0;
}

CString toString(const LPVARIANT data)
{
	CString msg;
	switch(data->vt & VT_BSTR_BLOB)
	{
	case VT_I4:
		msg.Format(_T("%d"), data->intVal);
		break;
	case VT_I8:
		msg.Format(_T("%I64d"), data->llVal);
		break;
	case VT_R8:
		msg.Format(_T("%f"), data->dblVal);
		break;
	case VT_EMPTY:
		msg = _T("NaN");
		break;
	case VT_BSTR:
		msg.Format(_T("%s"), data->bstrVal);
		break;
	case VT_DATE:
		msg = COleDateTime(data->date).Format();
		break;
	default:
		break;
	}
	return msg;
}

void printErrMsg(long errorCode)
{
	WCHAR buffer[64];
	int length = 64;
	CWAPIWrapperCpp::getErrorMsg(errorCode, eCHN, buffer, length);
	wcout << buffer << endl;
}

bool testLogonWFT()
{
	long errorCode = CWAPIWrapperCpp::start();
	if (errorCode == 0)
	{
		cout << "终端认证成功" << endl;
		return true;
	}
	else
	{
		printErrMsg(errorCode);
		return false;
	}

	cout << endl;
}

void testLogoutWFT()
{
	long errorCode = CWAPIWrapperCpp::stop();
	if (errorCode == 0)
	{
		cout << "终端认证登出成功" << endl;
	}
	else
	{
		printErrMsg(errorCode);
	}

	cout << endl;
}

bool testLogonAccount()
{
	cout << "输入模拟交易账户:";
	WCHAR strUserName[32] = {0};
	wcin>> strUserName;

	WindData wd;
	long errorCode = CWAPIWrapperCpp::tlogon(wd, L"0000", L"0", strUserName, L"000000", L"SHSZ");
	if (errorCode == 0)
	{
		logonID = wd.GetLogonID();
		cout << "登录交易账户成功，登录ID： " << logonID << endl;
		return true;
	}
	else
	{
		wcout << wd.GetErrorMsg() << endl;

		return false;
	}
}

void testLogoutAccount()
{
	long errorCode = CWAPIWrapperCpp::tlogout(logonID);
	if (errorCode == 0)
	{
		cout << "登出交易账户成功" << endl;
	}
	else
	{
		printErrMsg(errorCode);
	}

	cout << endl;
}

void testOrder()
{
	cout << "测试下单：" << endl;
	WCHAR* errMsg = NULL;
	WindData wd;
	long errorCode = CWAPIWrapperCpp::torder(wd, L"600000.SH", L"Buy", L"10.3", L"200", L"");//单账户登录可以不指定LogonId
	if (errorCode == 0)//如果没有错误，errMsg仍然为NULL
	{
		reqID1 = wd.GetOrderRequestID();
		cout << "下单 600000.SH 成功， 请求号为：" << reqID1 << endl;
	}
	else
	{
		wcout << wd.GetErrorMsg() << endl;
	}

	errorCode = CWAPIWrapperCpp::torder(reqID2, L"600004.SH", L"Buy", 10.0, 300, L"", &errMsg);
	if (errorCode == 0)//如果没有错误，errMsg仍然为NULL
	{
		cout << "下单 600004.SH 成功，请求号为：" << reqID2 << endl;
	}
	else
	{
		wcout << errMsg << endl;
		//需要手动释放
		delete[] errMsg;
		errMsg = NULL;
	}

	cout << endl;
}

void testCancelOrder()
{
	cout << "测试撤单：" << endl;

	WindData wd;
	
	long errorCode = CWAPIWrapperCpp::tquery(wd, L"Order", L"OrderType=Withdrawable");
	if (errorCode == 0)
	{
		long lRecordCount = wd.GetRecordCount();
		if (lRecordCount > 0)
		{
			CString strOrderNumber = wd.GetOrderNumber(0);
			errorCode = CWAPIWrapperCpp::tcancel(strOrderNumber);
			if (errorCode == 0)
			{
				cout << "撤单成功" << endl;
			}
		}
	}
	else
	{
		wcout << wd.GetErrorMsg() << endl;
	}

	cout << endl;
}

void testQueryPosition()
{
	cout << "测试查询持仓：" << endl;
	WindData wd;

	long errorCode = CWAPIWrapperCpp::tquery(wd, L"Position");
	if (errorCode == 0)
	{
		long lRecordCount = wd.GetRecordCount();
		for (int i = 0; i < lRecordCount; i++)
		{
			int fl = wd.GetFieldsLength();
			for (int j = 0; j < fl; j++)
			{
				VARIANT var;
				wd.GetTradeItem(i, j, var);
				CString str = toString(&var);
				wcout << str.GetBuffer(0) << endl;
				
			}
		}
	}
	else
	{
		wcout << wd.GetErrorMsg() << endl;
	}

	cout << endl;
}