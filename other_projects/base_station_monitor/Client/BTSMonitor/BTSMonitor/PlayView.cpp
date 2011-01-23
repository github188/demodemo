// PlayView.cpp : implementation file
//

#include "stdafx.h"
#include "BTSMonitor.h"
#include "PlayView.h"
#include "const.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
// CPlayView

IMPLEMENT_DYNCREATE(CPlayView, CView)

CPlayView::CPlayView()
: m_nViewID(0)
, m_nActiveViewID(0)
, m_bIsVisible(false)
{

}

CPlayView::~CPlayView()
{
}

BEGIN_MESSAGE_MAP(CPlayView, CView)
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()


// CPlayView drawing

void CPlayView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}


// CPlayView diagnostics

#ifdef _DEBUG
void CPlayView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CPlayView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CPlayView message handlers

//void CPlayView::OnPrint(CDC* pDC, CPrintInfo* pInfo)
//{
//	// TODO: Add your specialized code here and/or call the base class
//	DrawSelectedWindow( );
//	
//	//CView::OnPrint(pDC, pInfo);
//}

void CPlayView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: Add your specialized code here and/or call the base class
}

void CPlayView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	//AfxGetApp()->GetMainWnd()->SendMessage(WM_NOTIFY_MESSAGE,(WPARAM)MSG_SELECT_VIDEO,m_nViewID);
	CWnd *pWnd = GetParent();
	if (pWnd && pWnd->m_hWnd)
		::SendMessage(pWnd->m_hWnd,WM_PLAYVIEW_SELECTED,(WPARAM)MSG_SELECT_VIDEO,m_nViewID);

	CView::OnLButtonDown(nFlags, point);
}

void CPlayView::SetVedioIndex(int nViewID)
{
	m_nViewID = nViewID;
}

int CPlayView::GetVedioIndex(void)
{
	return m_nViewID;
}

void CPlayView::DrawSelectedWindow(void)
{
	if (!m_bIsVisible)
		return;

	CPen   *pNew = NULL;
	CPen   *pOldPen;
	CRect rect;
	GetClientRect(&rect);

	POINT points[] =
	{
	   { rect.left,		rect.top},
	   { rect.right,	rect.top},
	   { rect.right,	rect.bottom},
	   { rect.left,		rect.bottom},

	   { rect.left,		rect.top}
	};


	CClientDC dc(this);
	CDC* pDC=&dc;
	COLORREF crRed = RGB(255,0,0);
	COLORREF crBlk = RGB(0,0,0);

	int lineWidth = 3;

	if (m_nActiveViewID == m_nViewID)
	{
		pNew = new CPen(PS_SOLID, lineWidth, crRed);
		pOldPen = pDC->SelectObject(pNew);
		pDC->Polyline(points, 5); // (sizeof(points) / sizeof(POINT))); 		
		pDC->SelectObject(pOldPen);
	}
	else
	{
		pNew = new CPen(PS_SOLID, lineWidth, crBlk);
		pOldPen = pDC->SelectObject(pNew);
		pDC->Polyline(points, 5); // (sizeof(points) / sizeof(POINT))); 		
		pDC->SelectObject(pOldPen);
	}

	delete pNew;
}

void CPlayView::SetActiveViewID(int  nActiveViewID)
{
	 m_nActiveViewID = nActiveViewID;
}

void CPlayView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CView::OnPaint() for painting messages
	
	DrawSelectedWindow();
}

void CPlayView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CWnd *pWnd = GetParent();
	if (pWnd && pWnd->m_hWnd)
		::SendMessage(pWnd->m_hWnd,WM_PLAYVIEW_SELECTED,(WPARAM)MSG_FULL_WINDOW,m_nViewID);
	

	CView::OnLButtonDblClk(nFlags, point);
}

void CPlayView::HidePlayView(void)
{
	m_bIsVisible = FALSE;
}

void CPlayView::ShowPlayView(void)
{
	m_bIsVisible = TRUE;
}
