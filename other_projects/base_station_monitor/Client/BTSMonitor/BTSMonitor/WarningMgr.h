#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "BTSManager.h"
#include "AlarmManager.h"
#include "atltypes.h"
// CWarningMgr dialog
class CDlgImage;
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
	afx_msg void OnBnClickedBtnWarnMgrDeviceFind();
	afx_msg void OnBnClickedFindTargetWarning();
private:
	CString m_sFilter;
	CString m_sFindStr;
	CTreeCtrl m_treeWarnMgr;
	CListCtrl m_lstFindWarnResult;
	CComboBox m_cboWarnType;
	CComboBox m_cboWarnAckType;
	CDateTimeCtrl m_dateBegin;
	CDateTimeCtrl m_dateEnd;
	CComboBox m_cboStartHour;
	CComboBox m_cboEndHour;
	CComboBox m_cboStartMin;
	CComboBox m_cboEndMin;
	CComboBox m_cboStartSec;
	CComboBox m_cboEndSec;
	CStatic m_wndWarnVedio;
	CImageList m_imagelist;
	CImageList m_treeImages;
	BTSManager m_btsMgr;
	AlarmManager m_alarmMgr;

	HTREEITEM m_hItemCurFind;
	HTREEITEM m_hRootItem;
public:
	HTREEITEM FindTarget(HTREEITEM  hItem, CString sName, bool bFindNext=FALSE);
	BOOL FindNewTarget(CString sFindStr);
	BOOL FindNextTarget(void);


public:
	void InitVedioDeviceTree(void);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnNMClickTreeWarnMgrDevice(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnHdnItemclickLstTargetWarning(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickLstTargetWarning(NMHDR *pNMHDR, LRESULT *pResult);
private:
	int m_nCurItem;
	CDlgImage *pDlgImage;
public:
	afx_msg void OnNMDblclkLstTargetWarning(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRClickLstTargetWarning(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
private:
	CPoint m_menuPt;
	bool m_bPopMenu;
public:
	afx_msg void OnWarningmgrExport();
	afx_msg void OnUpdateWarningmgrExport(CCmdUI *pCmdUI);
	afx_msg void OnWarningmgrSaveas();
	afx_msg void OnUpdateWarningmgrSaveas(CCmdUI *pCmdUI);
private:
	CRect m_rcVedio;
public:
	void ShowButton(bool bShow);
};
