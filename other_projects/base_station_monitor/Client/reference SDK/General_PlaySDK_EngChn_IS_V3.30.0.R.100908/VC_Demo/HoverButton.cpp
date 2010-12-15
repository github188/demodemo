// ColorButton.cpp : implementation file
//
// Copyright (c) 2009.
//

#include "stdafx.h"
#include "HoverButton.h"

#include <afxtempl.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(CHoverButton, CBitmapButton)
	//{{AFX_MSG_MAP(CHoverButton)
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_MOUSELEAVE,OnMouseLeave)
	ON_MESSAGE(WM_MOUSEHOVER,OnMouseHover)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////////////////////////////
CHoverButton::CHoverButton()
{
    m_bHover = FALSE;       
    m_bTracking = FALSE;
	m_bButtonEnable = TRUE;
}

CHoverButton::~CHoverButton()
{

}

BOOL CHoverButton::LoadBitmap(UINT bitmapid)
{
    //载入图片
    mybitmap.Attach(::LoadImage(::AfxGetInstanceHandle(),MAKEINTRESOURCE(bitmapid), IMAGE_BITMAP,0,0,LR_LOADMAP3DCOLORS));
    BITMAP    bitmapbits;
    //获取位图信息并存入bitmapbits结构中
    mybitmap.GetBitmap(&bitmapbits);
    
    //取位图相应的高度和1/4宽度。
    m_ButtonSize.cy=bitmapbits.bmHeight;
    m_ButtonSize.cx=bitmapbits.bmWidth/4;
    
    SetWindowPos(NULL, 0,0, m_ButtonSize.cx,m_ButtonSize.cy,SWP_NOMOVE |SWP_NOOWNERZORDER);
    return TRUE;
}

void CHoverButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
    //获取保存在DRAWITEMSTRUCT结构中且在绘制按钮时必须使用的设备上下文
    CDC* mydc=CDC::FromHandle(lpDrawItemStruct->hDC);
	
    //创建兼容的设备上下文
    CDC* pMemDC = new CDC;
    pMemDC->CreateCompatibleDC(mydc);
	
    //保存旧对象
    CBitmap * pOldBitmap;
    pOldBitmap = pMemDC->SelectObject(&mybitmap);
    
    CPoint point(0,0);    
    
    //判断按钮是否处于选择状态，如果是则绘制选择状态的按钮位图，在我们提供的位图中，选中状态的按钮图片是第二个
    if(lpDrawItemStruct->itemState & ODS_SELECTED)
    {
        mydc->BitBlt(0,0,m_ButtonSize.cx,m_ButtonSize.cy,pMemDC,m_ButtonSize.cx*2,0,SRCCOPY);
    }
    else
    {   //判断鼠标是否离开还是在按钮上面，以便绘制相应的位图
        if(m_bHover)
        {
            mydc->BitBlt(0,0,m_ButtonSize.cx,m_ButtonSize.cy,pMemDC,m_ButtonSize.cx,0,SRCCOPY);
        }else
        {
            mydc->BitBlt(0,0,m_ButtonSize.cx,m_ButtonSize.cy,pMemDC,0,0,SRCCOPY);
        }    
    }

	if (!m_bButtonEnable)
		mydc->BitBlt(0,0,m_ButtonSize.cx,m_ButtonSize.cy,pMemDC,m_ButtonSize.cx*3,0,SRCCOPY);
	
    // clean up
    pMemDC->SelectObject(pOldBitmap);
    delete pMemDC;
}

void CHoverButton::OnMouseMove(UINT nFlags, CPoint point)
{
    if (!m_bTracking)
    {
        TRACKMOUSEEVENT tme;
        tme.cbSize = sizeof(tme);
        tme.hwndTrack = m_hWnd;
        tme.dwFlags = TME_LEAVE|TME_HOVER;
        tme.dwHoverTime = 1;
        m_bTracking = _TrackMouseEvent(&tme);
    }
    CBitmapButton::OnMouseMove(nFlags, point);
}

LRESULT CHoverButton::OnMouseLeave(WPARAM wparam, LPARAM lparam)
{
    m_bTracking = FALSE;
    m_bHover=FALSE;
    //重画按钮
    Invalidate(TRUE);
    return 0;
}

LRESULT CHoverButton::OnMouseHover(WPARAM wparam, LPARAM lparam) 
{	
    m_bHover=TRUE;
    Invalidate(TRUE);
    return 0;
}

BOOL CHoverButton::EnableWindow(BOOL bEnable)
{
	m_bButtonEnable = bEnable;
	Invalidate(TRUE);
    return TRUE;
}