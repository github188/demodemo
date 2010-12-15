#if !defined(AFX_WATERCHECKDLG_H__993AA182_2D2C_43CF_96B3_9883F5ADE5B7__INCLUDED_)
#define AFX_WATERCHECKDLG_H__993AA182_2D2C_43CF_96B3_9883F5ADE5B7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WaterCheckDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CWaterCheckDlg dialog

class CWaterCheckDlg : public CDialog
{
// Construction
public:
	CWaterCheckDlg(CWnd* pParent = NULL);   // standard constructor
	~CWaterCheckDlg();


public:
	CString m_csFilePath;

	BOOL   m_bCheckEnable;
	HANDLE m_hCheckThread;

	long   m_lCheckErrorNum;

	void Watermark_Thread();
	BOOL GetTimeStamp(long lTimeStamp, CString& csTime);

// Dialog Data
	//{{AFX_DATA(CWaterCheckDlg)
	enum { IDD = IDD_DIALOG_WATERMARK };
	CListCtrl	m_WaterInfoList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWaterCheckDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CWaterCheckDlg)
	afx_msg void OnCheckfile();
	afx_msg void OnCheck();
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WATERCHECKDLG_H__993AA182_2D2C_43CF_96B3_9883F5ADE5B7__INCLUDED_)
