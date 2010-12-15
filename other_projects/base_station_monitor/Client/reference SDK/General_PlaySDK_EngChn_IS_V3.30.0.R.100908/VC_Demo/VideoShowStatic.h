#if !defined(AFX_VIDEOSHOWSTATIC_H__E611AE79_4318_4608_B962_2ECCC5F49D86__INCLUDED_)
#define AFX_VIDEOSHOWSTATIC_H__E611AE79_4318_4608_B962_2ECCC5F49D86__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// VideoShowStatic.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CVideoShowStatic window

class CVideoShowStatic : public CStatic
{
// Construction
public:
	CVideoShowStatic();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVideoShowStatic)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CVideoShowStatic();

	// Generated message map functions
protected:
	//{{AFX_MSG(CVideoShowStatic)
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIDEOSHOWSTATIC_H__E611AE79_4318_4608_B962_2ECCC5F49D86__INCLUDED_)
