#pragma once
#include "afxwin.h"
#include "afxdtctl.h"
#include "afxcmn.h"


// CTaskMgr dialog

class CTaskMgr : public CDialog
{
	DECLARE_DYNAMIC(CTaskMgr)

public:
	CTaskMgr(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTaskMgr();

// Dialog Data
	enum { IDD = IDD_TASK_MGR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnTaskAdd();
	virtual BOOL OnInitDialog();
private:
	CComboBox m_cboEndHour;
	CComboBox m_cboEndMinute;
	CComboBox m_cboEndSecond;
	CComboBox m_cboStartHour;
	CComboBox m_cboStartMinute;
	CComboBox m_cboStartSecond;
	CComboBox m_cboTargetWindow;
	CDateTimeCtrl m_dtStart;
	CDateTimeCtrl m_dtEnd;
	CEdit m_edtTaskName;
	CListCtrl m_lstTask;
public:
	void InitTaskLstView(void);
};
