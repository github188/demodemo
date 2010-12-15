#if !defined(AFX_SETCOLORDLG_H__5C2B2912_ED97_46E5_A73A_36305878849D__INCLUDED_)
#define AFX_SETCOLORDLG_H__5C2B2912_ED97_46E5_A73A_36305878849D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetColorDlg.h : header file
//
#include "NiceSlider.h"


/////////////////////////////////////////////////////////////////////////////
// CSetColorDlg dialog

class CSetColorDlg : public CDialog
{
// Construction
public:
	CSetColorDlg(CWnd* pParent = NULL);   // standard constructor

	void SetColorSliders(void);

// Dialog Data
	//{{AFX_DATA(CSetColorDlg)
	enum { IDD = IDD_DIALOG_COLORSET };
	CNiceSliderCtrl	m_SliderSaturation;
	CNiceSliderCtrl	m_SliderHue;
	CNiceSliderCtrl	m_SliderContrast;
	CNiceSliderCtrl	m_SliderBrightness;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetColorDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSetColorDlg)
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETCOLORDLG_H__5C2B2912_ED97_46E5_A73A_36305878849D__INCLUDED_)
