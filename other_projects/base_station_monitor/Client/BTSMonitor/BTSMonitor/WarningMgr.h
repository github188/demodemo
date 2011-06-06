#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "BTSManager.h"
#include "AlarmManager.h"
#include "atltypes.h"
#include "ColoredListCtrl.h"

// CWarningMgr dialog
typedef struct tagCUR_ALARM_PARA
{
	CString sCategory;
	CString sUUID;
	CString sCh;
	CString strStartDate;
	CString strStartTime;
	CString sAckType;
	CString sLevel;
	CString sLimit;
	CString sOffset;
	int		nTotalPg;
	int		nCurPg;
}CUR_ALARM_PARA;
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
	CString	  m_sFilter;
	CString	  m_sFindStr;
	CTreeCtrl m_treeWarnMgr;
	//CListCtrl m_lstFindWarnResult;
	CColoredListCtrl m_lstFindWarnResult;
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
	CStatic m_alarmVideoCtrl;

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
	CUR_ALARM_PARA m_CurAlarmPara;
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
	int	  m_nAlarmVideoSaveCnt;		//Saving Video.
	bool  m_bSaving[cnALARM_VIDEO_VV];
	bool  m_bIsSaving;
	int  m_nPlayingStatus; //0 No playing, 1 playing, 2 pause, 3 gotoBegin, 4 gotoEnd, 5 goForward,6 goBackword, 
public:
	void ShowButton(bool bShow);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnBnClickedBtnPlay();
	afx_msg void OnBnClickedBtnPause();
	afx_msg void OnBnClickedBtnStop();
	afx_msg void OnBnClickedBtnFastBackward();
	afx_msg void OnBnClickedBtnFastForward();
	afx_msg void OnBnClickedBtnGotoBegin();
	afx_msg void OnBnClickedBtnGotoEnd();
	afx_msg void OnBnClickedFirst();
	afx_msg void OnBnClickedPrevious();
	afx_msg void OnBnClickedNext();
	afx_msg void OnBnClickedLast();
	afx_msg void OnBnClickedGoto();
	CComboBox m_cboPageIndex;
	CString m_strPageInfo;
	void ShowQueryAlarmInfo(CString strQueryAlarm);
};
