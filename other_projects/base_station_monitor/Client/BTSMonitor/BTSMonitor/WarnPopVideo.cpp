// WarnPopVideo.cpp : implementation file
//

#include "stdafx.h"
#include "BTSMonitor.h"
#include "WarnPopVideo.h"
#include "const.h"
#include "PopPlayView.h"
#include "include/iPlay.h"
#include "RuntimeWarning.h"

// CWarnPopVideo dialog

IMPLEMENT_DYNAMIC(CWarnPopVideo, CDialog)

CWarnPopVideo::CWarnPopVideo(CWnd* pParent /*=NULL*/)
	: CDialog(CWarnPopVideo::IDD, pParent)
	, m_nPopIndex(0)
	, m_bFullScreen(false)
	, m_sUUID(_T(""))
	, m_sChannel(_T(""))
	, m_sStartTime(_T(""))
	, m_sEndTime(_T(""))
	, m_bShowing(FALSE)
	, m_btsName(_T(""))
	, m_nTimeOut(0)
{
	m_pPopView = NULL;
	m_pParent = pParent;
}

CWarnPopVideo::~CWarnPopVideo()
{

}

void CWarnPopVideo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_TIMEOUT, m_nTimeOut);
}


BEGIN_MESSAGE_MAP(CWarnPopVideo, CDialog)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDOK, &CWarnPopVideo::OnBnClickedOk)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_SAVE_PASSED_VIDEO, &CWarnPopVideo::OnBnClickedSavePassedVideo)
	ON_BN_CLICKED(IDC_PAUSE_ALARM, &CWarnPopVideo::OnBnClickedPauseAlarm)
END_MESSAGE_MAP()


// CWarnPopVideo message handlers

BOOL CWarnPopVideo::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	CRect rect;
	rect.top	= 0; 
	rect.bottom	= 0;//50;
	rect.left	= 0;//70*i;
	rect.right	= 0;//70*(i+1);

	m_pPopView = (CPopPlayView*)RUNTIME_CLASS(CPopPlayView)->CreateObject();
	// AFX_WS_DEFAULT_VIEW代表(WS_BORDER | WS_VISIBLE | WS_CHILD)
	//m_vvControl.vvInfo[i].vv->Create(NULL, NULL, AFX_WS_DEFAULT_VIEW, rect, this, 1000+i);
	
	DWORD dwStyle = AFX_WS_DEFAULT_VIEW;
	//if   (afxData.bWin4)
	//	dwStyle   &=   ~WS_BORDER; 
	
	m_pPopView->Create(NULL, NULL, dwStyle/*S_VISIBLE | WS_CHILD*/, rect, this, ID_POPVIDEO_VIEW+m_nPopIndex);

	m_pPopView->ShowWindow(SW_SHOW);

	m_pPopView->SendMessage(WM_INITIALUPDATE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CWarnPopVideo::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here

	if ( IsWindow(m_hWnd) && m_pPopView && IsWindow(m_pPopView->m_hWnd) )
	{
		CButton *pBtn = (CButton *)GetDlgItem(IDOK);
		if (pBtn)
		{
			int dlt = 5;
			CRect btnRect, dlgRect;
			pBtn->GetClientRect(&btnRect);
			GetClientRect(&dlgRect);

			m_pPopView->MoveWindow(dlgRect.left,dlgRect.top,dlgRect.Width(), dlgRect.Height()-btnRect.Height()-dlt);
		}

	}
}

//Video Ctrl ID...
void CWarnPopVideo::SetPopVideoIndex(int nIndex)
{
	m_nPopIndex = nIndex;
	CPopPlayView *pView = (CPopPlayView *)m_pPopView;
	if (pView) pView->SetPopViewIdx(nIndex);
}

int CWarnPopVideo::GetPopVideoIndex(void)
{
	return m_nPopIndex;
}

void CWarnPopVideo::OnBnClickedOk()//确认当天告警视频
{
	// TODO: Add your control notification handler code here
	BOOL bDebug = FALSE;
	if (bDebug)
	{
		CString sVVFile;
		CString path="F:\\Projects\\Video\\BTSMonitor\\test\\";
		CString sVideo[] = {"test01.h264","test02.h264","test03.h264","test04.h264","test05.h264","test06.h264","test07.h264"};

		int nActView = m_nPopIndex;
		sVVFile = path + sVideo[m_nPopIndex];
		
		BOOL bPlayFile = TRUE;
		if (bPlayFile)
		{
			PLAY_OpenFile(nActView, sVVFile.GetBuffer());
		
			PLAY_Play(nActView, m_pPopView->m_hWnd);
		}

		return;
	}

	//Acked this warning video.
	CRuntimeWarning *pRunTimeWnd = (CRuntimeWarning*)m_pParent;
	if (pRunTimeWnd)
		pRunTimeWnd->AckedWarning(m_sUUID);

	CBTSMonitorApp *pApp=(CBTSMonitorApp *)AfxGetApp();
	if ( atoi(m_sCategory) == 2 ) //Piture...
	{
		//Retrive this picutre, and show it.
		//pApp->pgkclient->getAlarmImagebyBase64(this->m_sBtsID,this->m_sChannel,this->m_sRoute,
		return;

	}
	else
	//Close Current vedio
	//BOOL bOpenRet = PLAY_CloseStream(nViewIndex);	
	//if(bOpenRet)
	{
		pApp->pgkclient->Stop_Play(m_nPopIndex+cnPOP_VEDIO_INDEX);		

		BOOL bPlay = PLAY_Stop(m_nPopIndex+cnPOP_VEDIO_INDEX);
		
		BOOL bOpenRet = PLAY_CloseStream(m_nPopIndex+cnPOP_VEDIO_INDEX);			
	}

	OnOK();
}

void CWarnPopVideo::FullScreenPopVideo(void)
{
	if (m_bFullScreen==false) //full screen
	{
		//Hide OK Button
		CButton *pBtnOK = (CButton *)GetDlgItem(IDOK);
		if (pBtnOK)
			pBtnOK->ShowWindow(SW_HIDE);

		//Full Video Screen
		GetWindowPlacement(&m_struOldPlacement);

		//Remove WS_SIZEBOX windows style. or not the window can't be full-creen.
		ModifyStyle(WS_SIZEBOX,0,0);
		
		CRect WindowRect, ClientRect;
		RECT m_FullScreenRect;

		//ReDraw the window. Get the correct edge value.
		GetWindowRect(&WindowRect);
		WindowRect.left+=1;
		WindowRect.right+=1;
		MoveWindow(CRect(0,0,352,288),TRUE);
		
		GetWindowRect(&WindowRect);
		GetClientRect(&ClientRect);
		ClientToScreen(&ClientRect);
	
		//get the dest window rect.
		int x = GetSystemMetrics(SM_CXSCREEN);
		int y = GetSystemMetrics(SM_CYSCREEN);
		m_FullScreenRect.left = WindowRect.left - ClientRect.left;
		m_FullScreenRect.top = WindowRect.top - ClientRect.top;
		m_FullScreenRect.right = WindowRect.right - ClientRect.right + x;
		m_FullScreenRect.bottom = WindowRect.bottom - ClientRect.bottom + y;

//		m_FullScreenRect.left = 0;
//		m_FullScreenRect.top = 0; 
//		m_FullScreenRect.right = GetSystemMetrics(SM_CXSCREEN);
//		m_FullScreenRect.bottom = GetSystemMetrics(SM_CYSCREEN);

		//Move the main window to the dest rect.
		WINDOWPLACEMENT wndpl;
		wndpl.length = sizeof(WINDOWPLACEMENT);
		wndpl.flags = 0;
		wndpl.showCmd = SW_SHOWNORMAL;
		wndpl.rcNormalPosition = m_FullScreenRect;
		SetWindowPlacement(&wndpl);

		//Move the view winow to full-screen.
		RECT rc;
		GetClientRect(&rc);
		m_pPopView->MoveWindow(&rc,TRUE);

		//
		m_bFullScreen = true;

	}
	else //Restore normal screen
	{
		//Show OK Button
		CButton *pBtnOK = (CButton *)GetDlgItem(IDOK);
		if (pBtnOK)
			pBtnOK->ShowWindow(SW_SHOW);
		
		//Restore old position
		ModifyStyle(0,WS_SIZEBOX,0);
				
		SetWindowPlacement(&m_struOldPlacement);
		RECT rc;
		GetClientRect(&rc);
// 		rc.right  -= 1;
// 		rc.bottom -= 132;
		m_pPopView->MoveWindow(&rc,TRUE);

		//SetWindowSize();

		//
		m_bFullScreen = false;

	}

}

void CWarnPopVideo::SetVideoPara(CString sBtsID, CString sUUID,CString sBtsName, CString sChannel,CString sStartTime, CString sEndTime, CString sCategory)
{
	m_sBtsID	 = sBtsID;
	m_sUUID		 = sUUID;
	m_btsName    = sBtsName;
	m_sChannel   = sChannel;
	m_sStartTime = sStartTime;
	m_sEndTime   = sEndTime;
	m_sCategory	 = sCategory;
}

void CWarnPopVideo::OnClose()
{
	// TODO: Add your message handler code here and/or call default	
	//if ( IsShowing() ) 
	//	ShowWindow(SW_HIDE);
	
	ShowWindow(SW_HIDE);
	

	CRuntimeWarning *pRunTimeWnd = (CRuntimeWarning*)m_pParent;
	if (pRunTimeWnd)
		pRunTimeWnd->DecPopVedioCount();

	m_bShowing = FALSE;

	//Close Current vedio
	CBTSMonitorApp *pApp=(CBTSMonitorApp *)AfxGetApp();
	if ( atoi(m_sCategory) == 2 )
	{
		CPopPlayView *pView = ((CPopPlayView*)m_pPopView);
		pView->SetImageType(0); //no type
		pView->StopImgMonitor();
	}
	else
	//BOOL bOpenRet = PLAY_CloseStream(nViewIndex);	
	//if(bOpenRet)
	{
		pApp->pgkclient->PauseVedioThread();
		pApp->pgkclient->Stop_Play(m_nPopIndex+cnPOP_VEDIO_INDEX);		
		pApp->pgkclient->ContinueVedioThread();

		BOOL bPlay = PLAY_Stop(m_nPopIndex+cnPOP_VEDIO_INDEX);
		
		BOOL bOpenRet = PLAY_CloseStream(m_nPopIndex+cnPOP_VEDIO_INDEX);			
	}

	//CDialog::OnClose();
}

BOOL CWarnPopVideo::IsShowing(void)
{
	return m_bShowing;
}

void CWarnPopVideo::PlayVideo(void)
{
	m_bShowing = TRUE;
	CRuntimeWarning *pRunTimeWnd = (CRuntimeWarning*) GetParent();
	if (pRunTimeWnd)
		pRunTimeWnd->IncPopVedioCount();

	//set the warning pop title
	CString sTitle;
	sTitle = m_btsName + " 开始时间:" + m_sStartTime;
	SetWindowText( sTitle );

	CBTSMonitorApp *pApp=(CBTSMonitorApp *)AfxGetApp();

	CPopPlayView *pView = (CPopPlayView*)m_pPopView;
	if (m_sCategory=="2") //picture..
	{
		CString sRoute = pApp->pgkclient->btsmanager.GetRouteByUUID(m_sBtsID);
		//start Image Monitoring....
		int err=0;
		CString sError;		
		MonitorImage *pMoImage = pApp->pgkclient->getRealImagebyBase64(m_sBtsID,m_sChannel,sRoute,&err);
		switch(err)
		{
		case -2: //:返回         -2:参数错误
			{
				sError.Format("%s-%s-%s:%d",m_sBtsID, m_sChannel,"参数错误", err);
				CLogFile::WriteLog(sError);
				//AfxMessageBox("参数错误!");
			}
			break;
		case 0:
			sError = "Image Receiving...";
			break;
		case 1://基站未找到
			{
				sError.Format("%s-%s-%s:%d",m_sBtsID, m_sChannel,"基站未找到", err);
				CLogFile::WriteLog(sError);
				//AfxMessageBox("基站未找到!");
			}
            break;
		case 2://正在传输其他图片  <--------需要界面提示
			{
				sError.Format("%s-%s-%s:%d",m_sBtsID, m_sChannel,"正在传输其他图片", err);
				CLogFile::WriteLog(sError);
				//AfxMessageBox("正在传输其他图片!");
			}
			break;
		case 0xFF:
			{
				//Socket Error , Receive or Write failed...
				//Need to Re connect socket.
				sError.Format("%s-%s-%s:%d",m_sBtsID, m_sChannel,"图片获取指令发送失败！", err);
				CLogFile::WriteLog(sError);

			}
			break;
		default:
			{
				sError.Format("%s:%d","未知的错误类型", err);
				CLogFile::WriteLog(sError);
				//AfxMessageBox("未知的错误类型!");
			}
		}

		if (err) 
		{
			//Only Show Message To user...

			//pView->SetRealImagePara(pMoImage ,sUUID,sChannelID,sRoute);
			pView->SetImageType(2);//Picture...
			pView->SetImageFile(""); 
			//pView->StartImgMonitor();
			pView->SetImageStatus(sError);

			return;
		}
		//.................
		if (pMoImage)
		{
			pView->SetRealImagePara(pMoImage ,m_sBtsID,m_sChannel,sRoute);
			pView->SetImageType(2);//Picture...
			pView->SetImageFile(""); 
			pView->SetImageStatus("Image Receiving...");
			pView->SetTimerIDEvent(m_nPopIndex+ID_REAL_POP_IMG_TIMER);
			pView->StartImgMonitor();
		}
	}
	else
	{
		PLAY_SetStreamOpenMode(m_nPopIndex+cnPOP_VEDIO_INDEX, STREAME_REALTIME);
		BOOL bOpenRet = PLAY_OpenStream(m_nPopIndex+cnPOP_VEDIO_INDEX,0,0,1024*900);
		if(bOpenRet)
		{
			PLAY_Play(m_nPopIndex+cnPOP_VEDIO_INDEX, m_pPopView->m_hWnd);

			//Play Remote Vedio runatime			
			pApp->pgkclient->real_play(m_sBtsID, m_sChannel, play_video, m_nPopIndex+cnPOP_VEDIO_INDEX);			
		}
	}
		
}

void CWarnPopVideo::OnBnClickedSavePassedVideo()
{
	// TODO: Add your control notification handler code here
}

void CWarnPopVideo::OnBnClickedPauseAlarm()
{
	// TODO: Add your control notification handler code here
	CBTSMonitorApp *pApp=(CBTSMonitorApp *)AfxGetApp();

	UpdateData();
	CString sTimeOut;
	sTimeOut.Format("%d",m_nTimeOut);
	if ( 0==pApp->pgkclient->stop_Alarm(m_sBtsID,sTimeOut))
		AfxMessageBox("暂停告警操作成功!");
	else
		AfxMessageBox("暂停告警操作失败!");

}
