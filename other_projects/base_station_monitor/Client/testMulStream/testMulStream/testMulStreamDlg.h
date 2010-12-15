
// testMulStreamDlg.h : header file
//

#pragma once
#include "PlayWnd.h"
#include "include/iPlay.h"

UINT recvThread(LPVOID param);

// CtestMulStreamDlg dialog
class CtestMulStreamDlg : public CDialog
{
// Construction
public:
	CtestMulStreamDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_TESTMULSTREAM_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;
	CList<CWnd *,CWnd *> playwndList;
	int frameWidth;
	CWinThread *mythread;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
};
