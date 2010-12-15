#if !defined(AFX_AVICONVERTDLG_H__9BC2470B_FF52_440A_A1DD_CCB776B01D29__INCLUDED_)
#define AFX_AVICONVERTDLG_H__9BC2470B_FF52_440A_A1DD_CCB776B01D29__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AVIConvertDlg.h : header file
//
#include "HoverButton.h"


/////////////////////////////////////////////////////////////////////////////
// CAVIConvertDlg dialog

class CAVIConvertDlg : public CDialog
{
// Construction
public:
	CAVIConvertDlg(CWnd* pParent = NULL);   // standard constructor

public:
	CString m_csSourceFile;
	CString m_csTargetFile;

	BOOL m_bAVIConvEnable;
	void AVIConv_Thread();

	int m_iChangeCount;

// Dialog Data
	//{{AFX_DATA(CAVIConvertDlg)
	enum { IDD = IDD_DIALOG_AVICONV };
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAVIConvertDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAVIConvertDlg)
	afx_msg void OnButtonSrcfile();
	afx_msg void OnButtonTargfile();
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonConvert();
	virtual void OnCancel();
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AVICONVERTDLG_H__9BC2470B_FF52_440A_A1DD_CCB776B01D29__INCLUDED_)
