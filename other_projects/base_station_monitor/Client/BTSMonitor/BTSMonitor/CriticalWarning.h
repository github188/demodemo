#pragma once

#include "ColoredListCtrl.h"

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
	//CMFCListCtrl  m_lstCriticalWarning;
	CColoredListCtrl m_lstCriticalWarning;

public:
	void AddListView(ALARM_COMING_TYPE type); 
	bool AckedWarning(CString sUUID);

private:
	CImageList m_imagelist;
	int m_nCurItem;
	long m_alarmIndex;
	bool m_bOnScroll;
public:
	afx_msg void OnNMRClickListCriticalWarning(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnWarningAck();
	afx_msg void OnWarningScroolingOff();
	afx_msg void OnWarningScroolingOn();
	bool DeleteItemByUUID(CString sUUID);
};
