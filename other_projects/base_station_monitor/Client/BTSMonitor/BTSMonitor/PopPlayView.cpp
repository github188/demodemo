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

	if ( (m_nImageType==2) && (!m_sPicture.IsEmpty()) )
		ShowPicture(pDC,m_sPicture,1,1);
	
	if ( !m_strShowMsg.IsEmpty() )
	{
		CRect rt;
		GetClientRect(&rt);

		pDC->SetBkColor(VIDEO_BACK_COLOR);
		pDC->TextOut(rt.Width()/3, rt.Height()/2, m_strShowMsg);

	}

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
	//��������⣬��ο����Ի����е�CView.aspx.htm ��
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
	CRect rt;
	GetClientRect(&rt);
	CBrush br;
	br.CreateSolidBrush(VIDEO_BACK_COLOR);
	pDC->FillRect(&rt,&br);

	return TRUE;

	//return CView::OnEraseBkgnd(pDC);
}

BOOL CPopPlayView::ShowPicture(CDC*  pDC, CString strPicName, int nWidth, int nHeight)
{
	IStream *pStm;  
	CFileStatus fstatus;  
	CFile file;  
	ULONGLONG cb;  

	//���ļ�������ļ�����Ч��
	if (file.Open(strPicName,CFile::modeRead)&&file.GetStatus(strPicName,fstatus)&&((cb = fstatus.m_size) != -1))  
	{  
		if (cb == 0)
			return false;

		HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, (SIZE_T)cb);  
		LPVOID pvData = NULL;  
		if (hGlobal != NULL)  
		{  
			pvData = GlobalLock(hGlobal);
			if (pvData != NULL)  
			{  
				//file.ReadHuge(pvData, cb);  //6.0�п��������������
				file.Read(pvData, (UINT)cb);  //VC2005.NET�����������
				GlobalUnlock(hGlobal);  
				CreateStreamOnHGlobal(hGlobal, TRUE, &pStm);  
			}
		}
	}
	else
	{
		return false;
	} //���ļ�����


	//��ʾJPEG��GIF��ʽ��ͼƬ��GIFֻ����ʾһ֡����������ʾ������

	//Ҫ��ʾ����GIF��ʹ��ACTIVE�ؼ���

	IPicture *pPic;

	//load image from file stream

	if(SUCCEEDED(OleLoadPicture(pStm,(LONG)fstatus.m_size,TRUE,IID_IPicture,(LPVOID*)&pPic)))

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
void CPopPlayView::SetImageStatus (CString sInfo) 
{
	m_strShowMsg = sInfo;
	Invalidate();
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

			//pos = util::split_next(m_pMonitorImage->datetime,str,'-',0);
			pos = util::split_next(m_pMonitorImage->datetime,str,'-',0);
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
			
			//StopImgMonitor();
			
			m_sImageStatus = "";

			RefreshPlayView();
		}
		else
		{
			CString strError;
			if (-2 == err)
			{
				m_sImageStatus = "��������";
				StopImgMonitor();

			}
			else if (1 == err) //current expired
			{
				m_sImageStatus = "Timeout! Retrieving...";
				if ( m_pMonitorImage->getNextImageSession(m_sUUID, m_sChannelID,&err) )
				{
					strError.Format("��ȡͼƬSessionIDʧ�� BTSID=%s, ChannelID=%s .ErrorCode=%d", m_sUUID, m_sChannelID,err);
					CLogFile::WriteLog(strError);
				}
			}
			else if (2 == err) //˳��λ--�����ڴ��󣬸澯��ѯʱ��ʾ��ѯ����
			{
				m_sImageStatus = "Polling Over!"; //Only Warning Mgr...
				strError.Format("getNextImage()ʧ�� BTSID=%s, ChannelID=%s .ErrorCode=%d", m_sUUID, m_sChannelID,err);
				CLogFile::WriteLog(strError);
			}
			else if (3 == err)
			{
				m_sImageStatus = "Waiting...";
			}
			else if (4 == err) //���ݽ���--�澯��ѯʱ����û������������ͼƬ
			{
				m_sImageStatus = "Condition is not satisfied!";
			}
			else if (0xE2 == err)
			{
				m_sImageStatus = "No Image or Connection is broken!";
			}
			else
			{
				m_sImageStatus = "No Information!"; //"Unknown error!"; //Only Warning Mgr...
				strError.Format("getNextImage()ʧ�� BTSID=%s, ChannelID=%s .δ֪�Ĵ������=%d", m_sUUID, m_sChannelID,err);
				CLogFile::WriteLog(strError);
			}

			RefreshPlayView();

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
	//if (m_pMonitorImage)
	//{
	//	delete m_pMonitorImage;
	//	m_pMonitorImage = NULL;
	//}
}

void CPopPlayView::SetImageType(int  nImageType)
{
	 m_nImageType =  nImageType;
}

void CPopPlayView::SetImageFile(CString sPicture)
{
	 m_sPicture = sPicture;
}
void CPopPlayView::SetPopViewIdx(int nIdx)
{
	m_nViewID = nIdx + cnPOP_VEDIO_INDEX;
}
void CPopPlayView::SetRealImagePara(MonitorImage* pObj, CString sUUID,CString sChannelID, CString sRoute)
{
	m_sUUID			= sUUID;
	m_sChannelID	= sChannelID;
	m_sRoute		= sRoute;
	m_pMonitorImage = pObj;
}
void CPopPlayView::RefreshPlayView(int status)
{
	CString strShowMsg;

	if (m_nImageType == 2)
	{
		strShowMsg = m_sImageStatus;
	}
	else
	{
		if (status == 0)
		{
			CBTSMonitorApp *pApp=(CBTSMonitorApp *)AfxGetApp();
			if ( pApp->pgkclient->m_pArrVideoCtrl[m_nViewID] )
			{
				 //-1 end, 0 not start, 1 running, -2 start error, 2 Time out
				m_nStatus = pApp->pgkclient->m_pArrVideoCtrl[m_nViewID]->status;
				switch(m_nStatus)
				{
				case -1:
				case 0:
					strShowMsg = "No Video";
					break;
				case 1:
					{
						if (pApp->pgkclient->m_pArrVideoCtrl[m_nViewID]->bIsBlocking)
							strShowMsg = "Data Receiving..."; //"Playing Video";
						else
							strShowMsg = "Playing Video";
					}
					break;
				case 2:
					strShowMsg = "Timeout,Connection is disconnected!";
					break;
				case 3:
					strShowMsg = "Connecting... please wait...";
					break;
				default:
					strShowMsg = "No Video";
					break;
				}
			}
			else
				strShowMsg = "No Video";
		}
		else if (status==2)
		{
			strShowMsg = "Timeout,Connection is disconnected!";
		}
		else if (status==3)
		{
			strShowMsg = "Re Connect Server... please wait!";
		}
		else if(status==4)
		{
			strShowMsg = "Data Receiving..."; //"Playing Video";
		}
		else if (status==5)
		{
			strShowMsg = "Connect to Server..."; 
		}
		else if (status==6)
		{
			strShowMsg = "Faild to Connect Server!"; 
		}

	} 

	//if (m_strShowMsg != strShowMsg)
	{
		m_strShowMsg = strShowMsg;
		Invalidate();
	}
}