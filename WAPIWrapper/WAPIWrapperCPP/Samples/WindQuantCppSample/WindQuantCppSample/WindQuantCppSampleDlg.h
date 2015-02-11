
// WindQuantCppSampleDlg.h : 头文件
//
#include "WAPIWrapperCpp.h"
#include "afxwin.h"


#pragma once

LONG WINAPI CallBack(const WindData &windData);

// CWindQuantCppSampleDlg 对话框
class CWindQuantCppSampleDlg : public CDialogEx
{
// 构造
public:
	CWindQuantCppSampleDlg(CWnd* pParent = NULL);	// 标准构造函数

	void updateList(const WindData& wd);

// 对话框数据
	enum { IDD = IDD_WINDQUANTCPPSAMPLE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	void initList();
	void likeWssList(const WindData& wd);
	void likeWsdList1(const WindData& wd);
	void likeWsdList2(const WindData& wd);
	void likeTDays(const WindData& wd);

	CString toString(const LPVARIANT data);

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_listCtrl;
	ULONGLONG m_reqId;

	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedRadioWsd();
	int m_func;
	afx_msg void OnBnClickedRadioWss();
	afx_msg void OnBnClickedRadioWsi();
	afx_msg void OnBnClickedRadioWst();
	afx_msg void OnBnClickedRadioWsq();
	afx_msg void OnBnClickedRadioWset();
	afx_msg void OnBnClickedRadioWpf();
	afx_msg void OnBnClickedRadioTdays();
	afx_msg void OnBnClickedRadioTdayscount();
	afx_msg void OnBnClickedRadioTdaysoffset();
	CString m_windCodes;
	CString m_indicators;
	CString m_startTime;
	CString m_endTime;
	CString m_options;
	CString m_quantity;
	CString m_costPrice;
	int m_offset;
	CString m_reportName;
	CString m_portfolioName;
	CString m_viewName;
	CString m_planName;
	CEdit m_cWindCodes;
	CEdit m_cPlanName;
	CEdit m_cCostPrice;
	CEdit m_cOffset;
	CEdit m_cIndicators;
	CEdit m_cStartTime;
	CEdit m_cEndTime;
	CEdit m_cQuantity;
	CEdit m_cOptions;
	CEdit m_cReportName;
	CEdit m_cPortfolioName;
	CEdit m_cViewName;
	afx_msg void OnBnClickedCheckSub();
	CButton m_sub;
};
