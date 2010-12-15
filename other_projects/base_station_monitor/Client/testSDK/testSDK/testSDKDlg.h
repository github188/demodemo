
// testSDKDlg.h : header file
//

#pragma once

#include "PlayWnd.h"

// CtestSDKDlg dialog
class CtestSDKDlg : public CDialog
{
// Construction
public:
	CtestSDKDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_TESTSDK_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;
	CList<CWnd *,CWnd *> playwndList;
	int frameWidth;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	void playinWnd(int nport, CPlayWnd *playwnd, CString file);
};
