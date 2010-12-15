#if !defined(AFX_QUERYALARMSTATE_H__970881FD_31EC_4D16_98B3_39099292E70C__INCLUDED_)
#define AFX_QUERYALARMSTATE_H__970881FD_31EC_4D16_98B3_39099292E70C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// QueryAlarmState.h : header file
//
#include "dhnetsdk.h"

/////////////////////////////////////////////////////////////////////////////
// CQueryAlarmState dialog

class CQueryAlarmState : public CDialog
{
// Construction
public:
	CQueryAlarmState(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CQueryAlarmState)
	enum { IDD = IDD_DIALOG_QUERYALARMSTATE };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CQueryAlarmState)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CQueryAlarmState)
	afx_msg void OnDoubleclickedButtonQuery();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	NET_CLIENT_STATE m_stuComAlarm;
	void SetAlarmInfo();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_QUERYALARMSTATE_H__970881FD_31EC_4D16_98B3_39099292E70C__INCLUDED_)
