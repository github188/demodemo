#pragma once
#include "afxcmn.h"
#include "afxwin.h"

#include "const.h"
// CRuntimeWarning dialog
class CWarnPopVideo;
class CRuntimeWarning : public CPropertyPage
{
	DECLARE_DYNAMIC(CRuntimeWarning)

public:
	CRuntimeWarning();
	virtual ~CRuntimeWarning();

// Dialog Data
	enum { IDD = IDD_RUNTIME_WARNING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CMFCListCtrl  m_lstRuntimeWarning;

public:
	void AddListView(ALARM_COMING_TYPE type); //

private:
	CImageList m_imagelist;
	CWarnPopVideo* m_pPopVideoDlg[cnMAX_POP_WINDOW];
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnNMRClickLstRuntimeWarning(NMHDR *pNMHDR, LRESULT *pResult);
//	afx_msg void OnWarningAck();
//	afx_msg void OnDummyCompile();
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnWarningAck();
	afx_msg void OnWarningScroolingOff();
	afx_msg void OnWarningScroolingOn();
private:
	int m_nCurItem;
	long m_alarmIndex;
public:
//	afx_msg void OnHdnItemdblclickLstRuntimeWarning(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkLstRuntimeWarning(NMHDR *pNMHDR, LRESULT *pResult);
private:
	int m_nPopViewCount;
public:
	void DecPopVedioCount(void);
	void IncPopVedioCount(void);
	bool AckedWarning(CString sUUID);
};
