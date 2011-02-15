// PopPlayView.cpp : implementation file
//

#include "stdafx.h"
#include "BTSMonitor.h"
#include "PopPlayView.h"
#include "WarnPopVideo.h"
// CPopPlayView

IMPLEMENT_DYNCREATE(CPopPlayView, CView)

CPopPlayView::CPopPlayView()
{

}

CPopPlayView::~CPopPlayView()
{
}

BEGIN_MESSAGE_MAP(CPopPlayView, CView)
	ON_WM_SIZE()
	ON_WM_MOUSEACTIVATE()
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()


// CPopPlayView drawing

void CPopPlayView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}


// CPopPlayView diagnostics

#ifdef _DEBUG
void CPopPlayView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CPopPlayView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CPopPlayView message handlers

void CPopPlayView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
}

void CPopPlayView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: Add your specialized code here and/or call the base class
}

int CPopPlayView::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	// TODO: Add your message handler code here and/or call default
	return CWnd::OnMouseActivate(pDesktopWnd, nHitTest, message);
	
	//if this view is created based on CDialog ,shouldn't call following
	//如果有问题，请参看“对话框中的CView.aspx.htm ”
	//return CView::OnMouseActivate(pDesktopWnd, nHitTest, message);
}

void CPopPlayView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CWarnPopVideo *pPopVideo = (CWarnPopVideo*)GetParent();
	if (pPopVideo)
	{
		pPopVideo->FullScreenPopVideo();
	}

	CView::OnLButtonDblClk(nFlags, point);
}
