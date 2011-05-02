// PlayView.cpp : implementation file
//

#include "stdafx.h"
#include "BTSMonitor.h"
#include "PlayView.h"
#include "const.h"
#include "MainFrm.h"
#include "MonitorImage.h"

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
, m_nImageType(0)
, m_nIDEvent(0)
, m_sRealImageDir(_T(""))
{
	m_bIsFullScreen  = FALSE;
	m_pSaveParent = NULL;
	m_sPicture.Empty();
	m_pMonitorImage = NULL;
	m_bHasImage = false;
}

CPlayView::~CPlayView()
{
	if (m_pMonitorImage)
	{
		delete m_pMonitorImage;
		m_pMonitorImage = NULL;
	}
}

BEGIN_MESSAGE_MAP(CPlayView, CView)
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_PLAYVIEW_CLOSE, &CPlayView::OnPlayviewClose)
	ON_COMMAND(ID_PLAYVIEW_FULLSCREEN, &CPlayView::OnPlayviewFullscreen)
	ON_WM_ERASEBKGND()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CPlayView drawing

void CPlayView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
	if (m_nImageType==2)
		ShowPicture(pDC,m_sPicture,1,1);
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
	util::InitApp();
	m_sRealImageDir = util::GetAppPath();
	m_sRealImageDir +="\\RealImage";

}

void CPlayView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
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
	
	if (!m_sPicture.IsEmpty())
	{
		OnDraw(&dc);
		return;
	}

	DrawSelectedWindow();
}

void CPlayView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	
	/*
	CWnd *pWnd = GetParent();
	if (pWnd && pWnd->m_hWnd)
		::SendMessage(pWnd->m_hWnd,WM_PLAYVIEW_SELECTED,(WPARAM)MSG_FULL_WINDOW,m_nViewID);
	*/

	FullScreen();

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

void CPlayView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	// TODO: Add your message handler code here
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_PLAYVIEW, point.x, point.y, this, TRUE);	
}

void CPlayView::OnPlayviewClose()
{
	// TODO: Add your command handler code here
	CWnd *pWnd = GetParent();
	if (pWnd && pWnd->m_hWnd)
		::SendMessage(pWnd->m_hWnd,WM_PLAYVIEW_SELECTED,(WPARAM)MSG_UNSELECT_CAMERA_DEVICE,NULL);

}

void CPlayView::OnPlayviewFullscreen()
{
	// TODO: Add your command handler code here
	FullScreen();
}

void CPlayView::FullScreen(void)
{
	CMainFrame   *pFrame=(CMainFrame*)AfxGetApp()->m_pMainWnd;  	
	if(!m_bIsFullScreen)
	{
		m_bIsFullScreen=TRUE;
		m_pSaveParent=this->GetParent();
		this->SetParent(GetDesktopWindow());
		CRect rect;
		GetDesktopWindow()->GetWindowRect(&rect);
		this->SetWindowPos(&wndTopMost,rect.left,rect.top,rect.right,rect.bottom,SWP_SHOWWINDOW);
	}
	else
	{
		m_bIsFullScreen=FALSE;
		this->SetParent( m_pSaveParent);
		((CMainFrame *)AfxGetMainWnd())->RecalcLayout();	
		
		//m_pSaveParent->Invalidate();
		::SendMessage(m_pSaveParent->m_hWnd,WM_PLAYVIEW_SELECTED,(WPARAM)MSG_RESTORE_VIEW,NULL);

		//AfxGetApp()->GetMainWnd()->SendMessage(WM_NOTIFY_MESSAGE,NULL,NULL);
	}

}

BOOL CPlayView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	CRect rt;
	GetClientRect(&rt);
	CBrush br;
	br.CreateSolidBrush(VIDEO_BACK_COLOR);
	pDC->FillRect(&rt,&br);

	return TRUE;

	//return CView::OnEraseBkgnd(pDC);
}

BOOL	CPlayView::ShowPicture(CDC*  pDC, CString strPicName, int nWidth , int nHeight)
{
	IStream *pStm;  
	CFileStatus fstatus;  
	CFile file;  
	LONG cb;  

	//打开文件并检测文件的有效性
	if (file.Open(strPicName,CFile::modeRead)&&file.GetStatus(strPicName,fstatus)&&((cb = fstatus.m_size) != -1))  
	{  
		HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, cb);  
		LPVOID pvData = NULL;  
		if (hGlobal != NULL)  
		{  
			pvData = GlobalLock(hGlobal);
			if (pvData != NULL)  
			{  
				//file.ReadHuge(pvData, cb);  //6.0中可能是用这个函数
				file.Read(pvData, cb);  //VC2005.NET中用这个函数
				GlobalUnlock(hGlobal);  
				CreateStreamOnHGlobal(hGlobal, TRUE, &pStm);  
			}
		}
	}
	else
	{
		return false;
	} //打开文件结束


	//显示JPEG和GIF格式的图片，GIF只能显示一帧，还不能显示动画，

	//要显示动画GIF请使用ACTIVE控件。

	IPicture *pPic;

	//load image from file stream

	if(SUCCEEDED(OleLoadPicture(pStm,fstatus.m_size,TRUE,IID_IPicture,(LPVOID*)&pPic)))

	{
		OLE_XSIZE_HIMETRIC hmWidth;  
		OLE_YSIZE_HIMETRIC hmHeight;  
		pPic->get_Width(&hmWidth);  
		pPic->get_Height(&hmHeight);  
		double fX,fY;  

		//get image height and width , Origin Size

		fX = (double)pDC->GetDeviceCaps(HORZRES)*(double)hmWidth/((double)pDC->GetDeviceCaps(HORZSIZE)*100.0);  

		fY = (double)pDC->GetDeviceCaps(VERTRES)*(double)hmHeight/((double)pDC->GetDeviceCaps(VERTSIZE)*100.0);  

		//Show Origin Size Of Image...
		BOOL bOrigin  = false;
		if (bOrigin)
		{
			//use render function display image
			if(FAILED(pPic->Render(*pDC,nWidth,nHeight,(DWORD)fX,(DWORD)fY,0,hmHeight,hmWidth,-hmHeight,NULL)))  
			{
				pPic->Release();
				return false;
			}
		}
		else
		{
			CRect rect; GetClientRect(&rect); 
			if(FAILED(pPic->Render(*pDC,nWidth,nHeight,rect.Width(),rect.Height(),0,hmHeight,hmWidth,-hmHeight,NULL)))  
			{
				pPic->Release();
				return false;
			}
		}
		
		pPic->Release();  
	}  
	else  
	{
		return false;  
	}

	return true;
}

void CPlayView::SetImageType(int  nImageType)
{
	 m_nImageType =  nImageType;
}

void CPlayView::SetImageFile(CString sPicture)
{
	 m_sPicture = sPicture;
}

void CPlayView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if (m_pMonitorImage && (m_nImageType==2))
	{
		//Save current alarm image to the directory.
		CString sLine, str, sDateTime, sBts, sCh;
		int pos=0;
		if (m_bHasImage)
		{

			pos = util::split_next(m_pMonitorImage->datetime,str,':',0);
			pos = util::split_next(m_pMonitorImage->datetime,str,'-',pos+1);
			sDateTime+=str;//YY
			pos = util::split_next(m_pMonitorImage->datetime,str,'-',pos+1);
			sDateTime+=str;//MM
			pos = util::split_next(m_pMonitorImage->datetime,str,' ',pos+1);
			sDateTime+=str;//DD
			pos = util::split_next(m_pMonitorImage->datetime,str,'-',pos+1);
			sDateTime+=str;//hh
			pos = util::split_next(m_pMonitorImage->datetime,str,'-',pos+1);
			sDateTime+=str;//mm
			//pos = util::split_next(pMoImage->datetime,str,'-',pos+1);
			str = m_pMonitorImage->datetime.Mid(pos+1);
			sDateTime+=str;//ss

			pos = util::split_next(m_pMonitorImage->bts,str,':',0);
			//pos = util::split_next(pMoImage->bts,str,':',pos+1);
			m_pMonitorImage->bts.Mid(pos+1);
			sBts+=str;

			pos = util::split_next(m_pMonitorImage->channel,str,':',0);
			//pos = util::split_next(pMoImage->channel,str,':',pos+1);
			m_pMonitorImage->channel.Mid(pos+1);
			sCh+=str;

			m_sPicture.Format("%s%s%s_%s_%s.jpg", m_sRealImageDir,"\\", sBts, sCh, sDateTime);
			m_pMonitorImage->savedata(m_sPicture);

			Invalidate();
		}

		 m_bHasImage = m_pMonitorImage->getNextImage();

	}

	CView::OnTimer(nIDEvent);
}

void CPlayView::SetTimerIDEvent(int nIDEvent)
{
	m_nIDEvent = nIDEvent;
}

void CPlayView::StartImgMonitor(void)
{
	m_bHasImage = true;
	SetTimer(m_nIDEvent,cnREAL_IMG_INTERVAL,NULL);
}

void CPlayView::StopImgMonitor(void)
{
	KillTimer(m_nIDEvent);

	//Free current MomitorImage Object
	if (m_pMonitorImage)
	{
		delete m_pMonitorImage;
		m_pMonitorImage = NULL;
	}
}

void CPlayView::SetMonitorImageObj(MonitorImage* pObj)
{
	m_pMonitorImage = pObj;
}
