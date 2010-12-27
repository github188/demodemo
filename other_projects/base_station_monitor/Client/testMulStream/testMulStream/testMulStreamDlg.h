
// testMulStreamDlg.h : header file
//

#pragma once
#include "PlayWnd.h"
#include "include/iPlay.h"
#include "GokuClient.cpp"

UINT video_read_thread(LPVOID param);
int start_new_thread(AFX_THREADPROC fun, LPVOID param);
int play_video(int sessionId, char *pBuffer, int len);
//static void *video_read_thread(void *param);
//static int start_new_thread(void *fun, void *param);

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
	CPlayWnd *pwBig;
	CRect *prectBig;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
};
