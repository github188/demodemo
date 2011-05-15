#pragma once
#include "afxwin.h"

// CLoginDlg dialog


class CLoginDlg : public CDialog
{
	DECLARE_DYNAMIC(CLoginDlg)

public:
	CLoginDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CLoginDlg();

// Dialog Data
	enum { IDD = IDD_DLG_LOGIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg LRESULT LoginOK(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT LoginFail(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT GetBtsListOK(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT GetBtsListFail(WPARAM wParam, LPARAM lParam);
	virtual BOOL OnInitDialog();
private:
	CString m_strPort;
	CStdioFile	m_fSaveInfo;
public:
	CComboBox m_cboUser;
	CComboBox m_cboServer;
	CComboBox m_cboPort;
	CWinThread*	loginThread;
};