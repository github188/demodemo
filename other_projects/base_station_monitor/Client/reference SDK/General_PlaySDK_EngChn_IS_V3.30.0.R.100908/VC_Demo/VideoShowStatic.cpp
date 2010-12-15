// VideoShowStatic.cpp : implementation file
//

#include "stdafx.h"
#include "player264demo.h"
#include "VideoShowStatic.h"

#include "Player264demoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CVideoShowStatic

CVideoShowStatic::CVideoShowStatic()
{
}

CVideoShowStatic::~CVideoShowStatic()
{
}


BEGIN_MESSAGE_MAP(CVideoShowStatic, CStatic)
	//{{AFX_MSG_MAP(CVideoShowStatic)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVideoShowStatic message handlers

void CVideoShowStatic::OnPaint() 
{
//	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	
	// Do not call CStatic::OnPaint() for painting messages
	CStatic::OnPaint();
	CPlayer264demoDlg *p = (CPlayer264demoDlg*)AfxGetMainWnd();
	p->Refurish();
}
