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
	m_sPicture.Empty();
	m_nImageType = 0;
	m_pMonitorImage = NULL;
	m_bHasImage=FALSE;
	m_nIDEvent=0;
	m_sRealImageDir.Empty();

}

CPopPlayView::~CPopPlayView()
{
}

BEGIN_MESSAGE_MAP(CPopPlayView, CView)
	ON_WM_SIZE()
	ON_WM_MOUSEACTIVATE()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_ERASEBKGND()
	ON_WM_TIMER()
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
	util::InitApp();
	m_sRealImageDir = util::GetAppPath();
	m_sRealImageDir +="RealImage";

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

BOOL CPopPlayView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	// TODO: Add your message handler code here and/or call default
	CRect rt;
	GetClientRect(&rt);
	CBrush br;
	br.CreateSolidBrush(VIDEO_BACK_COLOR);
	pDC->FillRect(&rt,&br);

	return CView::OnEraseBkgnd(pDC);
}

BOOL CPopPlayView::ShowPicture(CDC*  pDC, CString strPicName, int nWidth, int nHeight)
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
void CPopPlayView::SetTimerIDEvent(int nIDEvent)
{
	m_nIDEvent = nIDEvent;
}
void CPopPlayView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if (m_pMonitorImage && (m_nImageType==2))
	{
		//Save current alarm image to the directory.
		CString sLine, str, sDateTime, sBts, sCh;
		int pos=0;
		int err;
		m_bHasImage = m_pMonitorImage->getNextImage(&err);
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

	}
	CView::OnTimer(nIDEvent);
}

void CPopPlayView::StartImgMonitor(void)
{
	m_bHasImage = true;
	SetTimer(m_nIDEvent,cnREAL_IMG_INTERVAL,NULL);
}

void CPopPlayView::StopImgMonitor(void)
{
	KillTimer(m_nIDEvent);

	//Free current MomitorImage Object
	if (m_pMonitorImage)
	{
		delete m_pMonitorImage;
		m_pMonitorImage = NULL;
	}
}

void CPopPlayView::SetMonitorImageObj(MonitorImage* pObj)
{
	m_pMonitorImage = pObj;
}
void CPopPlayView::SetImageType(int  nImageType)
{
	 m_nImageType =  nImageType;
}

void CPopPlayView::SetImageFile(CString sPicture)
{
	 m_sPicture = sPicture;
}