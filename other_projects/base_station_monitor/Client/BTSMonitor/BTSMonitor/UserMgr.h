#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CUserMgr dialog

class CUserMgr : public CDialog
{
	DECLARE_DYNAMIC(CUserMgr)

public:
	CUserMgr(CWnd* pParent = NULL);   // standard constructor
	virtual ~CUserMgr();

// Dialog Data
	enum { IDD = IDD_USER_MGR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
	CString m_strUser;
	CString m_strOldPw;
	CString m_strNewPw;
	CString m_strNewPw2;
public:
	afx_msg void OnBnClickedBtnMod();
	virtual BOOL OnInitDialog();
};
