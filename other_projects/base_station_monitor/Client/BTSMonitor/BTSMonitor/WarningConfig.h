#pragma once
#include "afxwin.h"


// CWarningConfig dialog

class CWarningConfig : public CDialog
{
	DECLARE_DYNAMIC(CWarningConfig)

public:
	CWarningConfig(CWnd* pParent = NULL);   // standard constructor
	virtual ~CWarningConfig();

// Dialog Data
	enum { IDD = IDD_WARNING_CONFIG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	CButton m_chkWarnEnable; //No Warning
	CString m_strWarnSound;
	CComboBox m_cboPopWinCnt; //Warning Pop Win Count
	afx_msg void OnBnClickedWaveBrowse();
};
