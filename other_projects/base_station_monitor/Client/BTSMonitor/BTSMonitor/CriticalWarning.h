#pragma once


// CCriticalWarning dialog

class CCriticalWarning : public CPropertyPage
{
	DECLARE_DYNAMIC(CCriticalWarning)

public:
	CCriticalWarning();
	virtual ~CCriticalWarning();

// Dialog Data
	enum { IDD = IDD_CRITICAL_WARNING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CMFCListCtrl  m_lstCriticalWarning;
};
