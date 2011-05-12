#pragma once


// CDlgFind dialog

class CDlgFind : public CDialog
{
	DECLARE_DYNAMIC(CDlgFind)

public:
	CDlgFind(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgFind();

// Dialog Data
	enum { IDD = IDD_DLG_FIND };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	CString GetFindStr(void);
	CString m_sFindStr;
};
