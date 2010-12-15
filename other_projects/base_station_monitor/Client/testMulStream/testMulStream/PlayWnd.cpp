// PlayWnd.cpp : implementation file
//

#include "stdafx.h"
#include "PlayWnd.h"

// CPlayWnd

IMPLEMENT_DYNAMIC(CPlayWnd, CWnd)

CPlayWnd::CPlayWnd()
{

}

CPlayWnd::~CPlayWnd()
{
}


BEGIN_MESSAGE_MAP(CPlayWnd, CWnd)
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()



// CPlayWnd message handlers



void CPlayWnd::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
}

BOOL CPlayWnd::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	CRect rt;
	GetClientRect(&rt);
	CBrush br;
	br.CreateSolidBrush(VIDEO_BACK_COLOR);
	pDC->FillRect(&rt,&br);

	return TRUE;

	return CWnd::OnEraseBkgnd(pDC);
}
