#pragma once


// CWarningMgr dialog

class CWarningMgr : public CDialog
{
	DECLARE_DYNAMIC(CWarningMgr)

public:
	CWarningMgr(CWnd* pParent = NULL);   // standard constructor
	virtual ~CWarningMgr();

// Dialog Data
	enum { IDD = IDD_WARNING_MGR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};
