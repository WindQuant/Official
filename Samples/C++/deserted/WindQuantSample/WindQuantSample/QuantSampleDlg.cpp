
// QuantSampleDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "WindQuantAPI.h"
#include "QuantSample.h"
#include "QuantSampleDlg.h"

#pragma warning(disable:4996)

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CQuantSampleDlg 对话框


CQuantSampleDlg* pDlg = NULL;

CQuantSampleDlg::CQuantSampleDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CQuantSampleDlg::IDD, pParent)
    , m_Windcodes(_T("000001.SZ"))
    , m_Indicators(_T("low,amt"))
    , m_EndTime(_T("2013-04-26 10:00:00"))
    , m_Params(_T(""))
    , m_Func(2)
    , m_Output(_T(""))
    , m_Username(_T(""))
    , m_Password(_T(""))
    , m_BeginTime(_T("2013-04-26 09:00:00"))
    , m_subs(FALSE)
    , mCancelID(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CQuantSampleDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT3, m_Windcodes);
    DDX_Text(pDX, IDC_EDIT2, m_Indicators);
    DDX_Text(pDX, IDC_EDIT4, m_EndTime);
    DDX_Text(pDX, IDC_EDIT5, m_Params);
    DDX_Radio(pDX, IDC_RADIO1, m_Func);
    DDV_MinMaxInt(pDX, m_Func, 0, 20);
    DDX_Text(pDX, IDC_EDIT1, m_Output);
    DDX_Text(pDX, IDC_EDIT7, m_Username);
    DDX_Text(pDX, IDC_EDIT6, m_Password);
    DDX_Text(pDX, IDC_EDIT8, m_BeginTime);
    DDX_Control(pDX, IDC_EDIT1, m_OutputEdit);
    DDX_Check(pDX, IDC_CHECK1, m_subs);
    DDX_Text(pDX, IDC_EDIT9, mCancelID);
}

BEGIN_MESSAGE_MAP(CQuantSampleDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
    ON_MESSAGE(WM_USER+1, &CQuantSampleDlg::OnResponse)
    ON_BN_CLICKED(IDC_BTN_START, &CQuantSampleDlg::OnBnClickedBtnStart)
    ON_BN_CLICKED(IDC_BTN_LOGIN, &CQuantSampleDlg::OnBnClickedLogin)
    ON_BN_CLICKED(IDC_BTN_LOGOUT, &CQuantSampleDlg::OnBnClickedLogout)
    ON_BN_CLICKED(IDC_BTN_EXEC, &CQuantSampleDlg::OnBnClickedBtnExec)
    ON_BN_CLICKED(IDC_RADIO_WSD, &CQuantSampleDlg::OnBnClickedRadioWSD)
    ON_BN_CLICKED(IDC_RADIO_WSS, &CQuantSampleDlg::OnBnClickedRadioWSS)
    ON_BN_CLICKED(IDC_RADIO_WSI, &CQuantSampleDlg::OnBnClickedRadioWSI)
    ON_BN_CLICKED(IDC_RADIO_WST, &CQuantSampleDlg::OnBnClickedRadioWST)
    ON_BN_CLICKED(IDC_RADIO_WSQ, &CQuantSampleDlg::OnBnClickedRadioWSQ)
    ON_BN_CLICKED(IDC_RADIO_TDAYSCNT, &CQuantSampleDlg::OnBnClickedRadioTDAYSCNT)
    ON_BN_CLICKED(IDC_RADIO_TDAYSOFF, &CQuantSampleDlg::OnBnClickedRadioTDAYSOFF)
    ON_BN_CLICKED(IDC_RADIO_TDAYS, &CQuantSampleDlg::OnBnClickedRadioTDAYS)
    ON_WM_CLOSE()
    ON_BN_CLICKED(IDC_BTN_CANCEL, &CQuantSampleDlg::OnBnClickedBtnCancel)
    ON_BN_CLICKED(IDC_RADIO_WSET, &CQuantSampleDlg::OnBnClickedRadioWSET)
    ON_BN_CLICKED(IDC_RADIO_WEQS, &CQuantSampleDlg::OnBnClickedRadioWEQS)
END_MESSAGE_MAP()


// CQuantSampleDlg 消息处理程序

BOOL CQuantSampleDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
    pDlg = this;
    UpdateData(FALSE);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CQuantSampleDlg::OnPaint()
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
		CDialog::OnPaint();

        //INT rMin, rMax;
        //m_OutputEdit.GetScrollRange(SB_VERT, &rMin, &rMax);
        //int cur = m_OutputEdit.GetScrollPos(SB_VERT);
        //if (cur != rMax)
        //    m_OutputEdit.SetScrollPos(SB_VERT, rMax);
        //m_OutputEdit.LineScroll(m_OutputEdit.GetLineCount());
	}
}

LRESULT CQuantSampleDlg::OnResponse( WPARAM, LPARAM )
{
    UpdateData(FALSE);
    //INT rMin, rMax;
    //m_OutputEdit.GetScrollRange(SB_VERT, &rMin, &rMax);
    //int cur = m_OutputEdit.GetScrollPos(SB_VERT);
    //if (cur != rMax)
    //    m_OutputEdit.SetScrollPos(SB_VERT, rMax);
    m_OutputEdit.LineScroll(m_OutputEdit.GetLineCount());
    return 0;
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CQuantSampleDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

CString toString(const LPVARIANT data)
{
    CString msg;
    switch(data->vt & VT_BSTR_BLOB)
    {
    case VT_R8:
        msg.Format(_T("   %f"), data->dblVal);
        break;
    case VT_EMPTY:
        msg = _T("  NaN");
        break;
    case VT_BSTR:
        msg.Format(_T("   %s"), data->bstrVal);
        break;
    case VT_DATE:
        msg = _T("   ") + COleDateTime(data->date).Format();
        break;
    default:
        //msg = "  ";
        //msg += data.bstrVal;
        //msg += "\r\n";
        //msg.Format(_T("   %s"), data->bstrVal);
        break;
    }
    return msg;
}

CString toString(PVOID pData, int pos, VARTYPE vt)
{
    CString msg;
    switch (vt & VT_BSTR_BLOB)
    {
    case VT_VARIANT:
        {
            LPVARIANT data = (LPVARIANT)pData + pos;
            msg = toString(data);
        }
        break;
    case VT_BSTR:
        {
            LPBSTR data = (LPBSTR)pData + pos;
            //msg.Format(_T("   %s"), data);
            msg = _T("  ");
            msg += *data;
            break;
        }
    case VT_R8:
        {
            DOUBLE * data = (DOUBLE *)pData + pos;
            msg.Format(_T("   %f"), *data);
            break;
        }
    case VT_DATE:
        {
            DATE * data = (DATE *)pData + pos;
            msg = _T("   ") + COleDateTime(*data).Format();
            break;
        }
    case VT_I4:
        {
            INT* data = (INT*)pData + pos;
            msg.Format(_T("   %d"), *data);
            break;
        }
    case VT_I8:
        {
            LONGLONG* data = (LONGLONG*)pData + pos;
            msg.Format(_T("   %I64d"), *data);
            break;
        }
    default:        
        break;
    }
    return msg;
}

int CQuantSampleDlg::QuantCallback(WQEvent* pEvent, LPVOID pParam)
{
    if (!pEvent || !pDlg)
        return 0;

    CString msg;
    msg.Format(_T("Received event: type=%d, err=%d, reqid=%d, evtid=%d.\r\n"), pEvent->EventType, pEvent->ErrCode, pEvent->RequestID, pEvent->EventID);
    //pDlg->m_Output += msg;

    if (pEvent->pQuantData != NULL)
    {
        int codenum = pEvent->pQuantData->ArrWindCode.arrLen;
        int indnum = pEvent->pQuantData->ArrWindFields.arrLen;
        int timenum = pEvent->pQuantData->ArrDateTime.arrLen;
        msg += _T("Windcode = \r\n");
        //msg += pEvent->pQuantData->ArrWindCode.codeArray[0];
        for (int i = 0; i < codenum; i++)
        {
            msg += "  ";
            msg += pEvent->pQuantData->ArrWindCode.codeArray[i];
            msg += "\r\n";
        }
        //msg += "\r\n";

        msg += _T("Indicators = \r\n");
        //msg += pEvent->pQuantData->ArrWindFields.fieldsArray[0];
        for (int i = 0; i < indnum; i++)
        {
            msg += "  ";
            msg += pEvent->pQuantData->ArrWindFields.fieldsArray[i];
            msg += "\r\n";
        }
        //msg += "\r\n";

        msg += _T("Times = \r\n");
        //msg += COleDateTime(pEvent->pQuantData->ArrDateTime.timeArray[0]).Format();
        for (int i = 0; i < timenum; i++)
        {
            msg += "  ";
            msg += COleDateTime(pEvent->pQuantData->ArrDateTime.timeArray[i]).Format();
            msg += "\r\n";
        }

        msg += _T("Data = \r\n");

        VARIANT& data = pEvent->pQuantData->MatrixData;
        if ((data.vt & ::VT_ARRAY) && (data.parray!=NULL))
        {
            if (timenum == 1)
            {
                for (int i = 0; i < timenum; i++)
                {
                    for (int j = 0; j < codenum; j++)
                    {
                        for (int k = 0; k < indnum; k++)
                        {
                            int pos = i * codenum * indnum + j * indnum + k;
                            
                            msg += toString(data.parray->pvData, pos, data.vt);
                        }
                        msg += _T("\r\n");
                    }
                    msg += _T("\r\n");
                }
            }
            else if (codenum == 1)
            {
                for (int i = 0; i < timenum; i++)
                {
                    for (int j = 0; j < codenum; j++)
                        for (int k = 0; k < indnum; k++)
                        {
                            int pos = i * codenum * indnum + j * indnum + k;
                            
                            msg += toString(data.parray->pvData, pos, data.vt);
                        }
                    msg += _T("\r\n");
                }
            }
            else //if (indnum == 1)
            {
                for (int i = 0; i < timenum; i++)
                {
                    for (int j = 0; j < codenum; j++)
                    {
                        for (int k = 0; k < indnum; k++)
                        {
                            int pos = i * codenum * indnum + j * indnum + k;
                            
                            msg += toString(data.parray->pvData, pos, data.vt);
                        }
                    }
                    msg += _T("\r\n");
                }
            }
        }
        else
        {
            msg += toString(&data) + _T("\r\n");
        }
    }
    pDlg->m_Output += msg;

    ::PostMessage(pDlg->GetSafeHwnd(), WM_USER+1, 0, 0);
    //pDlg->UpdateData(FALSE);

    return 0;
}

void CQuantSampleDlg::OnBnClickedBtnStart()
{
    // TODO: Add your control notification handler code here
    SetEventHandler(QuantCallback);
    m_Output += _T("EventHandler set.\r\n");
    UpdateData(FALSE);
}

void CQuantSampleDlg::OnBnClickedLogin()
{
    // TODO: Add your control notification handler code here
    UpdateData();
    WQAUTH_INFO info;
    info.bSilentLogin = 1;
    _tcscpy(info.strUserName, m_Username);
    _tcscpy(info.strPassword, m_Password);
    WDataAuthorize(&info);
}

void CQuantSampleDlg::OnBnClickedLogout()
{
    // TODO: Add your control notification handler code here
    WDataAuthQuit();
}

void CQuantSampleDlg::OnBnClickedBtnExec()
{
    UpdateData();
    WQID reqID = 0;
    switch(m_Func)
    {
    case 0://wsd
        reqID = WSD(m_Windcodes, m_Indicators, m_BeginTime, m_EndTime, m_Params, NULL, NULL);
        break;
    case 1://wss
        reqID = WSS(m_Windcodes, m_Indicators, m_Params, NULL, NULL);
        break;
    case 2://wst
        reqID = WST(m_Windcodes, m_Indicators, m_BeginTime, m_EndTime, m_Params, NULL, NULL);
        break;
    case 3://wsi
        reqID = WSI(m_Windcodes, m_Indicators, m_BeginTime, m_EndTime, m_Params, NULL, NULL);
        break;
    case 4://wsq
        reqID = WSQ(m_Windcodes, m_Indicators, m_Params, m_subs ? QuantCallback : NULL, NULL);
        break;
    case 5://wset
        reqID = WSET(m_Windcodes, m_Params, NULL, NULL);
        break;
    case 6://werr
        {
            CString msg;
            msg.Format(_T("The description of errCode %I64d is (%s).\r\n"), mCancelID, WErr(mCancelID, eCHN));

            m_Output += msg;
            UpdateData(FALSE);
        }
        return;
    case 7://tdays
        reqID = TDays(m_BeginTime, m_EndTime, m_Params, NULL, NULL);
        break;
    case 8://tdaysoffset
        reqID = TDaysOffset(m_BeginTime, _T(""), m_Params, NULL, NULL);
        break;
    case 9://tdayscount
        reqID = TDaysCount(m_BeginTime, m_EndTime, m_Params, NULL, NULL);
        break;
    case 10://weqs
        reqID = WEQS(m_Windcodes, m_Params, NULL, NULL);
        break;
    case 11://wpf
        break;
    default:
        break;
    }

    // TODO: Add your control notification handler code here
    CString msg;
    if (reqID <= 0)
        msg.Format(_T("Request sent failed. errCode=%d.\r\n"), reqID);
    else
        msg.Format(_T("Request sent successfully. reqID=%d.\r\n"), reqID);
    m_Output += msg;
    UpdateData(FALSE);
}

//void CQuantSampleDlg::OnEnUpdateEdit1()
//{
//    // TODO:  If this is a RICHEDIT control, the control will not
//    // send this notification unless you override the CDialog::OnInitDialog()
//    // function to send the EM_SETEVENTMASK message to the control
//    // with the ENM_UPDATE flag ORed into the lParam mask.
//
//    // TODO:  Add your control notification handler code here
//}

void CQuantSampleDlg::OnBnClickedRadioWSD()
{
    // TODO: Add your control notification handler code here
    m_Windcodes = _T("000001.SZ");
    m_Indicators = _T("low,amt");
    m_BeginTime = _T("2013-04-15");
    m_EndTime = _T("2013-04-26");
    m_Params = _T("");
    m_Func = 0;
    UpdateData(FALSE);
}

void CQuantSampleDlg::OnBnClickedRadioWSS()
{
    // TODO: Add your control notification handler code here
    m_Windcodes = _T("000001.SZ,000002.SZ");
    m_Indicators = _T("comp_name,comp_name_eng,regcapital");
    m_BeginTime = _T("");
    m_EndTime = _T("");
    m_Params = _T("");
    m_Func = 1;
    UpdateData(FALSE);
}

void CQuantSampleDlg::OnBnClickedRadioWSI()
{
    // TODO: Add your control notification handler code here
    m_Windcodes = _T("000001.SZ");
    m_Indicators = _T("low,amt");
    m_BeginTime = _T("2013-04-26 09:40:00");
    m_EndTime = _T("2013-04-26 10:00:00");
    m_Params = _T("");
    m_Func = 3;
    UpdateData(FALSE);
}

void CQuantSampleDlg::OnBnClickedRadioWST()
{
    // TODO: Add your control notification handler code here
    m_Windcodes = _T("000001.SZ");
    m_Indicators = _T("low,amt");
    m_BeginTime = _T("2013-04-26 09:40:00");
    m_EndTime = _T("2013-04-26 10:00:00");
    m_Params = _T("");
    m_Func = 2;
    UpdateData(FALSE);
}

void CQuantSampleDlg::OnBnClickedRadioWSQ()
{
    // TODO: Add your control notification handler code here
    m_Windcodes = _T("USDCAD.FX");
    m_Indicators = _T("rt_last,rt_amt");
    m_BeginTime = _T("");
    m_EndTime = _T("");
    m_Params = _T("");
    m_Func = 4;
    m_subs = TRUE;
    UpdateData(FALSE);
}

void CQuantSampleDlg::OnBnClickedRadioTDAYSCNT()
{
    m_Windcodes = _T("");
    m_Indicators = _T("");
    m_BeginTime = _T("2002-09-10");
    m_EndTime = _T("2002-10-10");
    m_Params = _T("days=Alldays");
    m_Func = 9;
    UpdateData(FALSE);
}

void CQuantSampleDlg::OnBnClickedRadioTDAYSOFF()
{
    m_Windcodes = _T("");
    m_Indicators = _T("");
    m_BeginTime = _T("2013-05-08");
    m_EndTime = _T("");
    m_Params = _T("Offset=-4;days=Alldays");
    m_Func = 8;
    UpdateData(FALSE);
}

void CQuantSampleDlg::OnBnClickedRadioTDAYS()
{
    m_Windcodes = _T("");
    m_Indicators = _T("");
    m_BeginTime = _T("2013-04-08");
    m_EndTime = _T("2013-05-08");
    m_Params = _T("TradingCalendar=COMEX;days=Alldays");
    m_Func = 7;
    UpdateData(FALSE);
}

void CQuantSampleDlg::OnClose()
{
    // TODO: Add your message handler code here and/or call default
    pDlg = NULL;
    WDataAuthQuit();
    CDialog::OnClose();
}

void CQuantSampleDlg::OnBnClickedBtnCancel()
{
    // TODO: Add your control notification handler code here
    //UpdateData();
    CancelRequest(mCancelID);
}

void CQuantSampleDlg::OnBnClickedRadioWSET()
{
    // TODO: Add your control notification handler code here
    
    m_Windcodes = _T("SectorConstituent");
    m_Indicators = _T("");
    m_BeginTime = _T("");
    m_EndTime = _T("");
    m_Params = _T("date=20130628;sector=全部A股;field=wind_code");
    m_Func = 5;
    UpdateData(FALSE);
}

void CQuantSampleDlg::OnBnClickedRadioWEQS()
{
    m_Windcodes = _T("MACD 低位金叉");
    m_Indicators = _T("");
    m_BeginTime = _T("");
    m_EndTime = _T("");
    m_Params = _T("");
    m_Func = 10;
    UpdateData(FALSE);
    // TODO: Add your control notification handler code here
}
