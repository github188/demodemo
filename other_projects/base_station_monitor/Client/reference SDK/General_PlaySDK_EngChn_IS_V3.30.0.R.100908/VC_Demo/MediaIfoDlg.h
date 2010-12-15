#if !defined(AFX_MEDIAIFODLG_H__12006EC5_D0BC_458D_9EAA_AE7509EC72CD__INCLUDED_)
#define AFX_MEDIAIFODLG_H__12006EC5_D0BC_458D_9EAA_AE7509EC72CD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MediaIfoDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMediaIfoDlg dialog

class CMediaIfoDlg : public CDialog
{
// Construction
public:
	CMediaIfoDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMediaIfoDlg)
	enum { IDD = IDD_DIALOG_MEDIAINFO };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMediaIfoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMediaIfoDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MEDIAIFODLG_H__12006EC5_D0BC_458D_9EAA_AE7509EC72CD__INCLUDED_)
