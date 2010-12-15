// ClientDemo6Dlg.h : header file
//

#if !defined(AFX_CLIENTDEMO6DLG_H__BA8171BF_9F63_4F5C_B596_62044F809667__INCLUDED_)
#define AFX_CLIENTDEMO6DLG_H__BA8171BF_9F63_4F5C_B596_62044F809667__INCLUDED_

#include "dhnetsdk.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CClientDemo6Dlg dialog

class CClientDemo6Dlg : public CDialog
{
// Construction
public:
	void DeviceDisConnect(LONG lLoginID, char *sDVRIP, LONG nDVRPort);

	friend void CALLBACK DisConnectFunc(LONG lLoginID, char *pchDVRIP, LONG nDVRPort, DWORD dwUser);

	CClientDemo6Dlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CClientDemo6Dlg)
	enum { IDD = IDD_CLIENTDEMO6_DIALOG };
	CListCtrl	m_ctlQueryResult;
	CComboBox	m_ctlQueryType;
	CIPAddressCtrl	m_ctlDvrIP;
	UINT	m_nPort;
	CString	m_strUserName;
	CString	m_strPwd;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CClientDemo6Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CClientDemo6Dlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBtnLogin();
	afx_msg void OnBtnLogout();
	afx_msg void OnDestroy();
	afx_msg void OnButtonQuery();
	afx_msg void OnButtonQueryxml();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CString ConvertCfgType(int nCfgType);
	void DisplayQueryXMLResult(char *recBuf, int nLength);
	BOOL JudgeType(int nQueryType,int nInfoType);
	void InsertListViewItem(CString strLogType,CString strLogTime,CString strLogContext);
	void DisplayQueryResult(char * recBuf,int nLength);
	void InitListView();
	CString GetDvrIP();
	long m_LoginID;
	void InitNetSDK();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CLIENTDEMO6DLG_H__BA8171BF_9F63_4F5C_B596_62044F809667__INCLUDED_)
