
// WindQuantCppSampleDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "WindQuantCppSample.h"
#include "WindQuantCppSampleDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CWindQuantCppSampleDlg 对话框




CWindQuantCppSampleDlg::CWindQuantCppSampleDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CWindQuantCppSampleDlg::IDD, pParent)
	, m_func(0)
	, m_reqId(0)
	, m_windCodes(_T(""))
	, m_indicators(_T(""))
	, m_startTime(_T(""))
	, m_endTime(_T(""))
	, m_options(_T(""))
	, m_quantity(_T(""))
	, m_costPrice(_T(""))
	, m_offset(0)
	, m_reportName(_T(""))
	, m_portfolioName(_T(""))
	, m_viewName(_T(""))
	, m_planName(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CWindQuantCppSampleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_listCtrl);
	DDX_Radio(pDX, IDC_RADIO_WSD, m_func);
	DDX_Text(pDX, IDC_EDIT1, m_windCodes);
	DDX_Text(pDX, IDC_EDIT2, m_indicators);
	DDX_Text(pDX, IDC_EDIT3, m_startTime);
	DDX_Text(pDX, IDC_EDIT4, m_endTime);
	DDX_Text(pDX, IDC_EDIT6, m_options);
	DDX_Text(pDX, IDC_EDIT5, m_quantity);
	DDX_Text(pDX, IDC_EDIT11, m_costPrice);
	DDX_Text(pDX, IDC_EDIT12, m_offset);
	DDX_Text(pDX, IDC_EDIT7, m_reportName);
	DDX_Text(pDX, IDC_EDIT8, m_portfolioName);
	DDX_Text(pDX, IDC_EDIT9, m_viewName);
	DDX_Text(pDX, IDC_EDIT10, m_planName);
	DDX_Control(pDX, IDC_EDIT1, m_cWindCodes);
	DDX_Control(pDX, IDC_EDIT10, m_cPlanName);
	DDX_Control(pDX, IDC_EDIT11, m_cCostPrice);
	DDX_Control(pDX, IDC_EDIT12, m_cOffset);
	DDX_Control(pDX, IDC_EDIT2, m_cIndicators);
	DDX_Control(pDX, IDC_EDIT3, m_cStartTime);
	DDX_Control(pDX, IDC_EDIT4, m_cEndTime);
	DDX_Control(pDX, IDC_EDIT5, m_cQuantity);
	DDX_Control(pDX, IDC_EDIT6, m_cOptions);
	DDX_Control(pDX, IDC_EDIT7, m_cReportName);
	DDX_Control(pDX, IDC_EDIT8, m_cPortfolioName);
	DDX_Control(pDX, IDC_EDIT9, m_cViewName);
	DDX_Control(pDX, IDC_CHECK_Sub, m_sub);
}

BEGIN_MESSAGE_MAP(CWindQuantCppSampleDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CWindQuantCppSampleDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CWindQuantCppSampleDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CWindQuantCppSampleDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_RADIO_WSD, &CWindQuantCppSampleDlg::OnBnClickedRadioWsd)
	ON_BN_CLICKED(IDC_RADIO_WSS, &CWindQuantCppSampleDlg::OnBnClickedRadioWss)
	ON_BN_CLICKED(IDC_RADIO_WSI, &CWindQuantCppSampleDlg::OnBnClickedRadioWsi)
	ON_BN_CLICKED(IDC_RADIO_WST, &CWindQuantCppSampleDlg::OnBnClickedRadioWst)
	ON_BN_CLICKED(IDC_RADIO_WSQ, &CWindQuantCppSampleDlg::OnBnClickedRadioWsq)
	ON_BN_CLICKED(IDC_RADIO_WSET, &CWindQuantCppSampleDlg::OnBnClickedRadioWset)
	ON_BN_CLICKED(IDC_RADIO_WPF, &CWindQuantCppSampleDlg::OnBnClickedRadioWpf)
	ON_BN_CLICKED(IDC_RADIO_TDAYS, &CWindQuantCppSampleDlg::OnBnClickedRadioTdays)
	ON_BN_CLICKED(IDC_RADIO_TDAYSCOUNT, &CWindQuantCppSampleDlg::OnBnClickedRadioTdayscount)
	ON_BN_CLICKED(IDC_RADIO_TDAYSOFFSET, &CWindQuantCppSampleDlg::OnBnClickedRadioTdaysoffset)
	ON_BN_CLICKED(IDC_CHECK_Sub, &CWindQuantCppSampleDlg::OnBnClickedCheckSub)
END_MESSAGE_MAP()


// CWindQuantCppSampleDlg 消息处理程序

BOOL CWindQuantCppSampleDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	m_listCtrl.SetExtendedStyle(m_listCtrl.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_DOUBLEBUFFER);
	OnBnClickedRadioWsd();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CWindQuantCppSampleDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CWindQuantCppSampleDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CWindQuantCppSampleDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

CString CWindQuantCppSampleDlg::toString(const LPVARIANT data)
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

void CWindQuantCppSampleDlg::initList()
{
	int nColumnCount = m_listCtrl.GetHeaderCtrl()->GetItemCount(); 
	for (int i=0; i < nColumnCount; i++)
	{ 
		m_listCtrl.DeleteColumn(0);
	}
	m_listCtrl.DeleteAllItems();
}

void CWindQuantCppSampleDlg::updateList(const WindData& wd)
{
	initList();

	int timeLength = wd.GetTimesLength();
	int codeLength = wd.GetCodesLength();
	int fieldsLength = wd.GetFieldsLength();

	if (timeLength > 1)
	{
		if (fieldsLength > 1)
		{
			likeWsdList1(wd);
		}
		else //fieldsLength <= 1
		{
			if (codeLength >= 1)
			{
				likeWsdList2(wd);
			}
		}
	}
	else if (codeLength > 1)//timeLength <= 1
	{
		if (fieldsLength > 1)
		{
			likeWssList(wd);
		}
		else
		{
			likeWsdList2(wd);
		}
	} 
	else //timeLength <= 1 && codeLength <=1
	{
		likeWssList(wd);
	}
}

void CWindQuantCppSampleDlg::likeWssList(const WindData& wd)
{
	CRect listRect;
	m_listCtrl.GetClientRect(&listRect);

	int codesLen = wd.GetCodesLength();
	int fieldsLen = wd.GetFieldsLength();

	int colNum = fieldsLen + 1;
	m_listCtrl.InsertColumn(0, _T("WindCodes"), LVCFMT_CENTER, listRect.Width()/colNum);
	for (int i = 1; i < colNum; i++)
	{
		m_listCtrl.InsertColumn(i, wd.GetFieldsByIndex(i-1), LVCFMT_CENTER, listRect.Width()/colNum);
	}

	for (int i = 0; i < codesLen; i++)
	{
		m_listCtrl.InsertItem(i, wd.GetCodeByIndex(i));
		for (int j = 0; j <fieldsLen; j++)
		{
			VARIANT var;
			wd.GetDataItem(0, i, j, var);
			//不能使用VariantClear掉var变量，因为var字符串是指向data中的字符串
			//VariantClear(&var);
			m_listCtrl.SetItemText(i, j+1, toString(&var));
		}
	}
}

//一只股票，多个指标
void CWindQuantCppSampleDlg::likeWsdList1(const WindData& wd)
{
	CRect listRect;
	m_listCtrl.GetClientRect(&listRect);

	int timeLen = wd.GetTimesLength();
	int fieldsLen = wd.GetFieldsLength();

	int colNum = fieldsLen + 1;
	m_listCtrl.InsertColumn(0, _T("DateTime"), LVCFMT_CENTER, listRect.Width()/colNum);
	for (int i = 1; i < colNum; i++)
	{
		m_listCtrl.InsertColumn(i, wd.GetFieldsByIndex(i-1), LVCFMT_CENTER, listRect.Width()/colNum);
	}

	CString dateFormat = L"%Y-%m-%d";
	if (m_func == 2 || m_func == 3)
	{
		dateFormat = L"%Y-%m-%d %H:%M:%S";
	}

	for (int i = 0; i < timeLen; i++)
	{
		DATE date = wd.GetTimeByIndex(i);
		 
		WCHAR* time = WindData::DateToString(date, dateFormat);
		m_listCtrl.InsertItem(i, time);
		WindData::FreeString(time);

		for (int j = 0; j < fieldsLen; j++)
		{
			VARIANT var;
			wd.GetDataItem(i, 0, j, var);
			m_listCtrl.SetItemText(i, j+1, toString(&var));
		}
	}
}

//多支股票，一个指标
void CWindQuantCppSampleDlg::likeWsdList2(const WindData& wd)
{
	CRect listRect;
	m_listCtrl.GetClientRect(&listRect);

	int timeLen = wd.GetTimesLength();
	int codesLen = wd.GetCodesLength();

	int colNum = codesLen + 1;
	m_listCtrl.InsertColumn(0, _T("DateTime"), LVCFMT_CENTER, listRect.Width()/colNum);
	for (int i = 1; i < colNum; i++)
	{
		m_listCtrl.InsertColumn(i, wd.GetCodeByIndex(i-1), LVCFMT_CENTER, listRect.Width()/colNum);
	}

	for (int i = 0; i < timeLen; i++)
	{
		DATE date = wd.GetTimeByIndex(i);
		WCHAR* time = WindData::DateToString(date);
		m_listCtrl.InsertItem(i, time);
		WindData::FreeString(time);

		for (int j = 0; j < codesLen; j++)
		{
			VARIANT var;
			wd.GetDataItem(i, j, 0, var);
			m_listCtrl.SetItemText(i, j+1, toString(&var));
		}
	}
}

void CWindQuantCppSampleDlg::likeTDays(const WindData& wd)
{
	CRect listRect;
	m_listCtrl.GetClientRect(&listRect);

	m_listCtrl.InsertColumn(0, _T("DateTime"), LVCFMT_CENTER, listRect.Width());

	long dateLength = 0;
	const DATE* dateList = wd.GetTDaysInfo(dateLength);

	for (int i = 0; i < dateLength; i++)
	{
		WCHAR* time = WindData::DateToString(dateList[i]);
		m_listCtrl.InsertItem(i, time);
		WindData::FreeString(time);
	}
}

void CWindQuantCppSampleDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	long errCode = CWAPIWrapperCpp::start();
	if (errCode == 0)
	{
		MessageBox(L"登录数据接口成功！", L"提示", MB_OK);
	}
	else
	{
		WCHAR buffer[128];
		int bufferSize = 128;
		CWAPIWrapperCpp::getErrorMsg(errCode, eCHN, buffer, bufferSize);
		MessageBox(buffer, L"登录失败", MB_OK);
	}
}


void CWindQuantCppSampleDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	long errCode = CWAPIWrapperCpp::stop();
	if (errCode == 0)
	{
		MessageBox(L"登出数据接口成功！", L"提示", MB_OK);
	}
	else
	{
		WCHAR buffer[128];
		int bufferSize = 128;
		CWAPIWrapperCpp::getErrorMsg(errCode, eCHN, buffer, bufferSize);
		MessageBox(buffer, L"提示", MB_OK);
	}
}


void CWindQuantCppSampleDlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码

	UpdateData(TRUE);

	long errCode = 0;
	WindData wdata;
	switch(m_func)
	{
	case 0://wsd
		{
			errCode = CWAPIWrapperCpp::wsd(wdata, m_windCodes, m_indicators, m_startTime, m_endTime);
			if (errCode == 0)
			{
				if (wdata.GetFieldsLength() > 1)
				{
					updateList(wdata);
				}
				else
				{
					updateList(wdata);
				}
			}
			else
			{
				// 			WCHAR buffer[128] = {0};
				// 			int bufferSize = 128;
				// 			CWAPIWrapperCpp::getErrorMsg(errCode, eCHN, buffer, bufferSize);
				// 			MessageBox(buffer, L"提示", MB_OK);
				MessageBox(wdata.GetErrorMsg(), L"提示", MB_OK);
			}
		}
		break;
	case 1://wss
		{
			errCode = CWAPIWrapperCpp::wss(wdata, m_windCodes, m_indicators, m_options);
			if (errCode == 0)
			{
				updateList(wdata);
			}
			else
			{
				MessageBox(wdata.GetErrorMsg(), L"提示", MB_OK);
			}
		}
		break;
	case 2://wsi
		{
			errCode = CWAPIWrapperCpp::wsi(wdata, m_windCodes, m_indicators, m_startTime, m_endTime, m_options);
			if (errCode == 0)
			{
				updateList(wdata);
			}
			else
			{
				MessageBox(wdata.GetErrorMsg(), L"提示", MB_OK);
			}
		}
		break;
	case 3://wst
		{
			errCode = CWAPIWrapperCpp::wst(wdata, m_windCodes, m_indicators, m_startTime, m_endTime, m_options);
			if (errCode == 0)
			{
				updateList(wdata);
			}
			else
			{
				MessageBox(wdata.GetErrorMsg(), L"提示", MB_OK);
			}
		}
		break;
	case 4://wsq
		{
			int state = m_sub.GetCheck();
			if (state == 0)
			{
				errCode = CWAPIWrapperCpp::wsq(wdata, m_windCodes, m_indicators, m_options);
				if (errCode == 0)
				{
					updateList(wdata);
				}
			}
			else
			{
				errCode = CWAPIWrapperCpp::wsq(m_reqId, m_windCodes, m_indicators, CallBack, m_options, TRUE);
			}

			if (errCode != 0)
			{
				WCHAR buffer[128] = {0};
				int bufferSize = 128;
				CWAPIWrapperCpp::getErrorMsg(errCode, eCHN, buffer, bufferSize);
				MessageBox(buffer, L"提示", MB_OK);
			}
		}
		break;
	case 5://wset
		{
			errCode = CWAPIWrapperCpp::wset(wdata, m_reportName, m_options);
			if (errCode == 0)
			{
				updateList(wdata);
			}
			else
			{
				MessageBox(wdata.GetErrorMsg(), L"提示", MB_OK);
			}
		}
		break;
	case 6://wpf
		{
			errCode = CWAPIWrapperCpp::wpf(wdata, m_portfolioName, m_viewName, m_options);
			if (errCode == 0)
			{
				updateList(wdata);
			}
			else
			{
				MessageBox(wdata.GetErrorMsg(), L"提示", MB_OK);
			}
		}
		break;
	case 7://tdays
		{
			errCode = CWAPIWrapperCpp::tdays(wdata, m_startTime, m_endTime, m_options);
			if (errCode == 0)
			{
				initList();
				likeTDays(wdata);
			}
			else
			{
				MessageBox(wdata.GetErrorMsg(), L"提示", MB_OK);
			}
		}
		break;
	case 8://tdayscount
		{
  			LONG count = 0;
			errCode = CWAPIWrapperCpp::tdayscount(count, m_startTime, m_endTime, m_options);
			if (errCode == 0)
			{
				CString str;
				str.Format(_T("%d"), count);
				MessageBox(str);
			}
		}
		break;
	case 9://tdaysoffset
		{
			DATE date = 0;
			errCode = CWAPIWrapperCpp::tdaysoffset(date, m_startTime, m_offset, m_options);
			if (errCode == 0)
			{
				WCHAR *buffer = WindData::DateToString(date);
				CString cstrDate = buffer;
				WindData::FreeString(buffer);
				MessageBox(cstrDate);
			}
		}
		break;
	default:
		break;
	}

	UpdateData(FALSE);
}


void CWindQuantCppSampleDlg::OnBnClickedRadioWsd()
{
	// TODO: 在此添加控件通知处理程序代码
	m_func = 0;

	m_windCodes = _T("600000.SH");
	m_indicators = _T("high,low,open,close");
	m_startTime = _T("2014-01-01");
	m_endTime = _T("2014-10-01");
	m_options = _T("");
	m_planName = _T("");
	m_portfolioName = _T("");
	m_reportName = _T("");
	m_viewName = _T("");
	m_quantity = _T("");
	m_costPrice = _T("");
	m_offset = 0;

	m_cWindCodes.SetReadOnly(FALSE);
	m_cIndicators.SetReadOnly(FALSE);
	m_cStartTime.SetReadOnly(FALSE);
	m_cEndTime.SetReadOnly(FALSE);
	m_cOptions.SetReadOnly(FALSE);
	m_cPlanName.SetReadOnly(TRUE);
	m_cPortfolioName.SetReadOnly(TRUE);
	m_cReportName.SetReadOnly(TRUE);
	m_cViewName.SetReadOnly(TRUE);
	m_cQuantity.SetReadOnly(TRUE);
	m_cCostPrice.SetReadOnly(TRUE);
	m_cOffset.SetReadOnly(TRUE);

	UpdateData(FALSE);
}




void CWindQuantCppSampleDlg::OnBnClickedRadioWss()
{
	// TODO: 在此添加控件通知处理程序代码
	m_func = 1;

	m_windCodes = _T("600000.SH,000001.SZ,000002.SZ");
	m_indicators = _T("high,low,open,close");
	m_startTime = _T("");
	m_endTime = _T("");
	m_options = _T("tradeDate=20141111");
	m_planName = _T("");
	m_portfolioName = _T("");
	m_reportName = _T("");
	m_viewName = _T("");
	m_quantity = _T("");
	m_costPrice = _T("");
	m_offset = 0;

	m_cWindCodes.SetReadOnly(FALSE);
	m_cIndicators.SetReadOnly(FALSE);
	m_cStartTime.SetReadOnly(TRUE);
	m_cEndTime.SetReadOnly(TRUE);
	m_cOptions.SetReadOnly(FALSE);
	m_cPlanName.SetReadOnly(TRUE);
	m_cPortfolioName.SetReadOnly(TRUE);
	m_cReportName.SetReadOnly(TRUE);
	m_cViewName.SetReadOnly(TRUE);
	m_cQuantity.SetReadOnly(TRUE);
	m_cCostPrice.SetReadOnly(TRUE);
	m_cOffset.SetReadOnly(TRUE);

	UpdateData(FALSE);
}


void CWindQuantCppSampleDlg::OnBnClickedRadioWsi()
{
	// TODO: 在此添加控件通知处理程序代码
	m_func = 2;

	m_windCodes = _T("600000.SH");
	m_indicators = _T("high,low,volume,amt");
	m_startTime = _T("2014-11-12 09:00:00");
	m_endTime = _T("2014-11-12 10:30:00");
	m_options = _T("");
	m_planName = _T("");
	m_portfolioName = _T("");
	m_reportName = _T("");
	m_viewName = _T("");
	m_quantity = _T("");
	m_costPrice = _T("");
	m_offset = 0;

	m_cWindCodes.SetReadOnly(FALSE);
	m_cIndicators.SetReadOnly(FALSE);
	m_cStartTime.SetReadOnly(FALSE);
	m_cEndTime.SetReadOnly(FALSE);
	m_cOptions.SetReadOnly(FALSE);
	m_cPlanName.SetReadOnly(TRUE);
	m_cPortfolioName.SetReadOnly(TRUE);
	m_cReportName.SetReadOnly(TRUE);
	m_cViewName.SetReadOnly(TRUE);
	m_cQuantity.SetReadOnly(TRUE);
	m_cCostPrice.SetReadOnly(TRUE);
	m_cOffset.SetReadOnly(TRUE);

	UpdateData(FALSE);
}

void CWindQuantCppSampleDlg::OnBnClickedRadioWst()
{
	// TODO: 在此添加控件通知处理程序代码
	m_func = 3;

	m_windCodes = _T("600000.SH");
	m_indicators = _T("last,ask,bid,volume,amt");
	m_startTime = _T("2014-11-12 09:00:00");
	m_endTime = _T("2014-11-12 10:30:00");
	m_options = _T("");
	m_planName = _T("");
	m_portfolioName = _T("");
	m_reportName = _T("");
	m_viewName = _T("");
	m_quantity = _T("");
	m_costPrice = _T("");
	m_offset = 0;

	m_cWindCodes.SetReadOnly(FALSE);
	m_cIndicators.SetReadOnly(FALSE);
	m_cStartTime.SetReadOnly(FALSE);
	m_cEndTime.SetReadOnly(FALSE);
	m_cOptions.SetReadOnly(FALSE);
	m_cPlanName.SetReadOnly(TRUE);
	m_cPortfolioName.SetReadOnly(TRUE);
	m_cReportName.SetReadOnly(TRUE);
	m_cViewName.SetReadOnly(TRUE);
	m_cQuantity.SetReadOnly(TRUE);
	m_cCostPrice.SetReadOnly(TRUE);
	m_cOffset.SetReadOnly(TRUE);

	UpdateData(FALSE);
}


void CWindQuantCppSampleDlg::OnBnClickedRadioWsq()
{
	// TODO: 在此添加控件通知处理程序代码
	m_func = 4;
	
	m_windCodes = _T("600000.SH,000001.SZ,000002.SZ");
	m_indicators = _T("rt_high,rt_low,rt_last");
	m_startTime = _T("");
	m_endTime = _T("");
	m_options = _T("");
	m_planName = _T("");
	m_portfolioName = _T("");
	m_reportName = _T("");
	m_viewName = _T("");
	m_quantity = _T("");
	m_costPrice = _T("");
	m_offset = 0;

	m_cWindCodes.SetReadOnly(FALSE);
	m_cIndicators.SetReadOnly(FALSE);
	m_cStartTime.SetReadOnly(TRUE);
	m_cEndTime.SetReadOnly(TRUE);
	m_cOptions.SetReadOnly(FALSE);
	m_cPlanName.SetReadOnly(TRUE);
	m_cPortfolioName.SetReadOnly(TRUE);
	m_cReportName.SetReadOnly(TRUE);
	m_cViewName.SetReadOnly(TRUE);
	m_cQuantity.SetReadOnly(TRUE);
	m_cCostPrice.SetReadOnly(TRUE);
	m_cOffset.SetReadOnly(TRUE);

	UpdateData(FALSE);
}


void CWindQuantCppSampleDlg::OnBnClickedRadioWset()
{
	// TODO: 在此添加控件通知处理程序代码
	m_func = 5;

	m_windCodes = _T("");
	m_indicators = _T("");
	m_startTime = _T("");
	m_endTime = _T("");
	m_options = _T("date=20141111;sector=全部A股");
	m_planName = _T("");
	m_portfolioName = _T("");
	m_reportName = _T("SectorConstituent");
	m_viewName = _T("");
	m_quantity = _T("");
	m_costPrice = _T("");
	m_offset = 0;

	m_cWindCodes.SetReadOnly(TRUE);
	m_cIndicators.SetReadOnly(TRUE);
	m_cStartTime.SetReadOnly(TRUE);
	m_cEndTime.SetReadOnly(TRUE);
	m_cOptions.SetReadOnly(FALSE);
	m_cPlanName.SetReadOnly(TRUE);
	m_cPortfolioName.SetReadOnly(TRUE);
	m_cReportName.SetReadOnly(FALSE);
	m_cViewName.SetReadOnly(TRUE);
	m_cQuantity.SetReadOnly(TRUE);
	m_cCostPrice.SetReadOnly(TRUE);
	m_cOffset.SetReadOnly(TRUE);

	UpdateData(FALSE);
}


void CWindQuantCppSampleDlg::OnBnClickedRadioWpf()
{
	// TODO: 在此添加控件通知处理程序代码
	m_func = 6;

	m_windCodes = _T("");
	m_indicators = _T("");
	m_startTime = _T("");
	m_endTime = _T("");
	m_options = _T("startdate=20141011;enddate=20141111;reportcurrency=CNY;owner=W0805016");
	m_planName = _T("");
	m_portfolioName = _T("全球投资组合管理演示");
	m_reportName = _T("");
	m_viewName = _T("PMS.PortfolioDaily");
	m_quantity = _T("");
	m_costPrice = _T("");
	m_offset = 0;

	m_cWindCodes.SetReadOnly(TRUE);
	m_cIndicators.SetReadOnly(TRUE);
	m_cStartTime.SetReadOnly(TRUE);
	m_cEndTime.SetReadOnly(TRUE);
	m_cOptions.SetReadOnly(FALSE);
	m_cPlanName.SetReadOnly(TRUE);
	m_cPortfolioName.SetReadOnly(FALSE);
	m_cReportName.SetReadOnly(TRUE);
	m_cViewName.SetReadOnly(FALSE);
	m_cQuantity.SetReadOnly(TRUE);
	m_cCostPrice.SetReadOnly(TRUE);
	m_cOffset.SetReadOnly(TRUE);

	UpdateData(FALSE);
}

void CWindQuantCppSampleDlg::OnBnClickedRadioTdays()
{
	// TODO: 在此添加控件通知处理程序代码
	m_func = 7;

	m_windCodes = _T("");
	m_indicators = _T("");
	m_startTime = _T("2014-01-01");
	m_endTime = _T("2014-10-01");
	m_options = _T("");
	m_planName = _T("");
	m_portfolioName = _T("");
	m_reportName = _T("");
	m_viewName = _T("");
	m_quantity = _T("");
	m_costPrice = _T("");
	m_offset = 0;

	m_cWindCodes.SetReadOnly(TRUE);
	m_cIndicators.SetReadOnly(TRUE);
	m_cStartTime.SetReadOnly(FALSE);
	m_cEndTime.SetReadOnly(FALSE);
	m_cOptions.SetReadOnly(FALSE);
	m_cPlanName.SetReadOnly(TRUE);
	m_cPortfolioName.SetReadOnly(TRUE);
	m_cReportName.SetReadOnly(TRUE);
	m_cViewName.SetReadOnly(TRUE);
	m_cQuantity.SetReadOnly(TRUE);
	m_cCostPrice.SetReadOnly(TRUE);
	m_cOffset.SetReadOnly(TRUE);

	UpdateData(FALSE);
}


void CWindQuantCppSampleDlg::OnBnClickedRadioTdayscount()
{
	// TODO: 在此添加控件通知处理程序代码
	m_func = 8;

	m_windCodes = _T("");
	m_indicators = _T("");
	m_startTime = _T("2014-01-01");
	m_endTime = _T("2014-10-01");
	m_options = _T("");
	m_planName = _T("");
	m_portfolioName = _T("");
	m_reportName = _T("");
	m_viewName = _T("");
	m_quantity = _T("");
	m_costPrice = _T("");
	m_offset = 0;

	m_cWindCodes.SetReadOnly(TRUE);
	m_cIndicators.SetReadOnly(TRUE);
	m_cStartTime.SetReadOnly(FALSE);
	m_cEndTime.SetReadOnly(FALSE);
	m_cOptions.SetReadOnly(FALSE);
	m_cPlanName.SetReadOnly(TRUE);
	m_cPortfolioName.SetReadOnly(TRUE);
	m_cReportName.SetReadOnly(TRUE);
	m_cViewName.SetReadOnly(TRUE);
	m_cQuantity.SetReadOnly(TRUE);
	m_cCostPrice.SetReadOnly(TRUE);
	m_cOffset.SetReadOnly(TRUE);

	UpdateData(FALSE);
}


void CWindQuantCppSampleDlg::OnBnClickedRadioTdaysoffset()
{
	// TODO: 在此添加控件通知处理程序代码
	m_func = 9;

	m_windCodes = _T("");
	m_indicators = _T("");
	m_startTime = _T("2014-01-01");
	m_endTime = _T("");
	m_options = _T("");
	m_planName = _T("");
	m_portfolioName = _T("");
	m_reportName = _T("");
	m_viewName = _T("");
	m_quantity = _T("");
	m_costPrice = _T("");
	m_offset = 5;

	m_cWindCodes.SetReadOnly(TRUE);
	m_cIndicators.SetReadOnly(TRUE);
	m_cStartTime.SetReadOnly(FALSE);
	m_cEndTime.SetReadOnly(TRUE);
	m_cOptions.SetReadOnly(FALSE);
	m_cPlanName.SetReadOnly(TRUE);
	m_cPortfolioName.SetReadOnly(TRUE);
	m_cReportName.SetReadOnly(TRUE);
	m_cViewName.SetReadOnly(TRUE);
	m_cQuantity.SetReadOnly(TRUE);
	m_cCostPrice.SetReadOnly(TRUE);
	m_cOffset.SetReadOnly(FALSE);

	UpdateData(FALSE);
}


void CWindQuantCppSampleDlg::OnBnClickedCheckSub()
{
	// TODO: 在此添加控件通知处理程序代码
	//取消订阅
	int state = m_sub.GetCheck();
	if (state == 0 && m_reqId != 0)
	{
		CWAPIWrapperCpp::cancelRequest(m_reqId);
		m_reqId = 0;
	}
}

LONG WINAPI CallBack(const WindData &windData)
{
	CWindQuantCppSampleDlg* pDlg = (CWindQuantCppSampleDlg*)theApp.GetMainWnd();
	if (pDlg != NULL)
	{
		pDlg->updateList(windData);
	}

	return 0;
}