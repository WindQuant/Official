
// QuantSampleDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
struct tagQuantEvent;
typedef struct tagQuantEvent WQEvent;

// CQuantSampleDlg 对话框
class CQuantSampleDlg : public CDialog
{
// 构造
public:
	CQuantSampleDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_QUANTSAMPLE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

    static int WINAPI QuantCallback(WQEvent* pEvent, LPVOID pParam);

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

protected:
    int m_Func;
    CEdit m_OutputEdit;

public:
    CString m_Windcodes;
    CString m_Indicators;
    CString m_EndTime;
    CString m_Params;
    CString m_Output;
    CString m_Username;
    CString m_Password;
    CString m_BeginTime;
    LONGLONG mCancelID;
    BOOL m_subs;

public:
    afx_msg void OnBnClickedBtnStart();
    afx_msg void OnBnClickedLogin();
    afx_msg void OnBnClickedLogout();
    afx_msg void OnBnClickedBtnExec();
    afx_msg LRESULT OnResponse( WPARAM, LPARAM );
    afx_msg void OnBnClickedRadioWSD();
    afx_msg void OnBnClickedRadioWSS();
    afx_msg void OnBnClickedRadioWSI();
    afx_msg void OnBnClickedRadioWST();
    afx_msg void OnBnClickedRadioWSQ();
    afx_msg void OnBnClickedRadioTDAYSCNT();
    afx_msg void OnBnClickedRadioTDAYSOFF();
    afx_msg void OnBnClickedRadioTDAYS();
    afx_msg void OnBnClickedBtnCancel();
    afx_msg void OnBnClickedRadioWSET();
    afx_msg void OnBnClickedRadioWEQS();
    afx_msg void OnClose();

};
