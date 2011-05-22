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
	bool ModifyTask();
	CComboBox m_cboDeviceID;
	CComboBox m_cboChannel;
	afx_msg void OnCbnSelchangeCboDeviceId();
	afx_msg void OnBnClickedBtnDeleteTask();
	afx_msg void OnBnClickedBtnModSaveTask();
	enum TASK_MODIFY_SAVE{ 
		T_NONE = 1,
		T_MODIFY, 
		T_SAVE
	}m_enumTASK_MODIFY;
	afx_msg void OnBnClickedBtnCancelModify();
private:
	CString m_sTaskID;
	int m_nItem;
	CString m_sStatus;
};
