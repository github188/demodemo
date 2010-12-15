// ClientDemo8.h : main header file for the CLIENTDEMO8 application
//

#if !defined(AFX_CLIENTDEMO8_H__F0C1156F_264C_4A5D_907C_4669C0F27E7A__INCLUDED_)
#define AFX_CLIENTDEMO8_H__F0C1156F_264C_4A5D_907C_4669C0F27E7A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CClientDemo8App:
// See ClientDemo8.cpp for the implementation of this class
//

class CClientDemo8App : public CWinApp
{
public:
	CClientDemo8App();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CClientDemo8App)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CClientDemo8App)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CString ConvertString(CString strText);
void g_SetWndStaticText(CWnd * pWnd);
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CLIENTDEMO8_H__F0C1156F_264C_4A5D_907C_4669C0F27E7A__INCLUDED_)
