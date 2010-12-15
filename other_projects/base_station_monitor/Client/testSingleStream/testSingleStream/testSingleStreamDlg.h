
// testSingleStreamDlg.h : header file
//

#pragma once

#define WM_STOPMSG WM_USER+100

#include "include/iPlay.h"

// CtestSingleStreamDlg dialog
class CtestSingleStreamDlg : public CDialog
{
// Construction
public:
	CtestSingleStreamDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_TESTSINGLESTREAM_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;
	CWinThread *mythread;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedOk();
};
