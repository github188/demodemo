#if !defined(AFX_SETPICPATH_H__A44CB985_6055_4BF5_8AD0_2E2DBAEA5922__INCLUDED_)
#define AFX_SETPICPATH_H__A44CB985_6055_4BF5_8AD0_2E2DBAEA5922__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetPicPath.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSetPicPath dialog

class CSetPicPath : public CDialog
{
// Construction
public:
	CSetPicPath(CWnd* pParent = NULL);   // standard constructor

public:
	CString m_csPicPath;

// Dialog Data
	//{{AFX_DATA(CSetPicPath)
	enum { IDD = IDD_DIALOG_SETPATH };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetPicPath)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSetPicPath)
	afx_msg void OnPathset();
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeEditPicpath();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETPICPATH_H__A44CB985_6055_4BF5_8AD0_2E2DBAEA5922__INCLUDED_)
