// ClientDemo3Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "ClientDemo3.h"
#include "ClientDemo3Dlg.h"
#include "MessageText.h"
#include "dhplay.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CClientDemo3Dlg dialog

CClientDemo3Dlg::CClientDemo3Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CClientDemo3Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CClientDemo3Dlg)
	m_strUserName = _T("admin");
	m_strPwd = _T("admin");
	m_nPort = 37777;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_LoginID = 0;
	m_nChannelCount = 0;
	m_hPlayBack = 0;
	m_dwTotalSize = 0;
	m_dwCurValue = 0;
	m_nTimer = 0;
	m_pDownLoadFile = 0;
	m_pDownLoadByTime = 0;
	m_PlayBackType = EM_FILEBACK;
	m_ePlayMode = EM_DIRECTMODE;
}

void CClientDemo3Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CClientDemo3Dlg)
	DDX_Control(pDX, IDC_SLIDER_PLAYPOSITION, m_ctlPlayBack);
	DDX_Control(pDX, IDC_TAB1, m_Tab);
	DDX_Control(pDX, IDC_IPADDRESS_DVRIP, m_ctlDvrIP);
	DDX_Text(pDX, IDC_EDIT_USERNAME, m_strUserName);
	DDX_Text(pDX, IDC_EDIT_DVRPWD, m_strPwd);
	DDX_Text(pDX, IDC_EDIT_PORT, m_nPort);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CClientDemo3Dlg, CDialog)
	//{{AFX_MSG_MAP(CClientDemo3Dlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_LOGIN, OnBtnLogin)
	ON_BN_CLICKED(IDC_BTN_LOGOUT, OnBtnLogout)
	ON_WM_DESTROY()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, OnSelchangeTab1)
	ON_BN_CLICKED(IDC_BUTTON_STOP, OnButtonStop)
	ON_WM_TIMER()
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_PLAYPOSITION, OnReleasedcaptureSliderPlayposition)
	ON_BN_CLICKED(IDC_BUTTON_PLAY, OnButtonPlay)
	ON_BN_CLICKED(IDC_BUTTON_SLOW, OnButtonSlow)
	ON_BN_CLICKED(IDC_BUTTON_FAST, OnButtonFast)
	ON_BN_CLICKED(IDC_BUTTON_FRAMETO, OnButtonFrameto)
	ON_BN_CLICKED(IDC_BUTTON_RESET, OnButtonReset)
	ON_BN_CLICKED(IDC_BUTTON_NORMALSPEED, OnButtonNormalspeed)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CClientDemo3Dlg message handlers

BOOL CClientDemo3Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	g_SetWndStaticText(this);
	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	//Device default IP
	m_ctlDvrIP.SetAddress(20, 2, 2, 116);
	m_ctlPlayBack.SetRange(0, 100);
	m_ctlPlayBack.SetPos(0);
	m_ctlPlayBack.SetSelection(0, 0);
	//Initialize TabControl control
	InitTabControl();
	//Initialize SDK
	InitNetSDK();
	m_nTimer = SetTimer(1, 500, NULL);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CClientDemo3Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CClientDemo3Dlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CClientDemo3Dlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

//Log in user to the device
void CClientDemo3Dlg::OnBtnLogin() 
{
	// TODO: Add your control notification handler code here
	BOOL bValid = UpdateData(TRUE);
	if(bValid)
	{
		int err=0;	//Storage the possible returned errror value
		char *pchDVRIP;
		CString strDvrIP = GetDvrIP();
		pchDVRIP = (LPSTR)(LPCSTR)strDvrIP;
		WORD wDVRPort=(WORD)m_nPort;
		char *pchUserName=(LPSTR)(LPCSTR)m_strUserName;
		char *pchPassword=(LPSTR)(LPCSTR)m_strPwd;
		//Device message
		NET_DEVICEINFO deviceInfo;
		//Call log in interface
		long lRet = CLIENT_Login(pchDVRIP,wDVRPort,pchUserName,pchPassword,&deviceInfo,&err);
		if(0 == lRet)
		{
			//Display log in failed reason
			ShowLoginErrorReason(err);
		}
		else
		{
			m_LoginID = lRet;
			m_nChannelCount = deviceInfo.byChanNum;
			//Set channel menu 
			m_pTabFileDlg->InitComboBox(m_nChannelCount,m_LoginID);
			m_pTabTimeDlg->InitComboBox(m_nChannelCount,m_LoginID);
			GetDlgItem(IDC_BTN_LOGIN)->EnableWindow(FALSE);
			GetDlgItem(IDC_BTN_LOGOUT)->EnableWindow(TRUE);
		}
	}
}

//Log off device user
void CClientDemo3Dlg::OnBtnLogout() 
{
	// TODO: Add your control notification handler code here
	//Close playback
	if(0 != m_hPlayBack)
	{
		ClosePlayBack();
		m_hPlayBack = 0;
	}

	//Reset process bar
	m_dwTotalSize = 0;
	m_dwCurValue = 0;

	//Close download
	if(0 != m_pDownLoadFile)
	{
		CLIENT_StopDownload(m_pDownLoadFile);
		m_pDownLoadFile = 0;
	}

	if(0 != m_pDownLoadByTime)
	{
		CLIENT_StopDownload(m_pDownLoadByTime);
		m_pDownLoadByTime = 0;
	}

	//Log off and then clear
	m_pTabFileDlg->Clean();
	m_pTabTimeDlg->Clean();

	BOOL bRet = CLIENT_Logout(m_LoginID);
	//Log off and then clear log in handle
	if(bRet)
	{
		m_LoginID=0;
		m_nChannelCount = 0;
		//Set channel menu 
		m_pTabFileDlg->InitComboBox(m_nChannelCount,m_LoginID);
		m_pTabTimeDlg->InitComboBox(m_nChannelCount,m_LoginID);
		GetDlgItem(IDC_BTN_LOGIN)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_LOGOUT)->EnableWindow(FALSE);
		Invalidate();
	}
	else
	{
		MessageBox(ConvertString("Logout failed!"), ConvertString("Prompt"));
	}
}

//Initialize net SDK
void CClientDemo3Dlg::InitNetSDK()
{
	BOOL ret = CLIENT_Init(DisConnectFunc, (DWORD)this);
	if (!ret)
	{
		MessageBox(ConvertString("Initialize SDK failed!"), ConvertString("Prompt"));
	}
}

//Callback function when device disconnected
void CALLBACK DisConnectFunc(LONG lLoginID, char *pchDVRIP, LONG nDVRPort, DWORD dwUser)
{
	if(dwUser == 0)
	{
		return;
	}
	CClientDemo3Dlg *dlg = (CClientDemo3Dlg *)dwUser;
	dlg->DeviceDisConnect(lLoginID, pchDVRIP,nDVRPort);
}

//Process when device disconnected.
void CClientDemo3Dlg::DeviceDisConnect(LONG lLoginID, char *sDVRIP, LONG nDVRPort)
{
	//Add device disconnection process code
	MessageBox(ConvertString("Network disconnected!"), ConvertString("Prompt"));
	
}

void CClientDemo3Dlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	if(0 != m_hPlayBack)
	{
		CLIENT_StopPlayBack(m_hPlayBack);
	}

	if(0 != m_pDownLoadFile)
	{
		//Close download
		CLIENT_StopDownload(m_pDownLoadFile);
	}
	if(0 != m_pDownLoadByTime)
	{
		CLIENT_StopDownload(m_pDownLoadByTime);
	}

	//Log off
	if(0 != m_LoginID)
	{
		BOOL bLogout = CLIENT_Logout(m_LoginID);
	}
	
	//Clear SDK and then release occupied resources.
	CLIENT_Cleanup();
}

//Get DVR IP address
CString CClientDemo3Dlg::GetDvrIP()
{
	CString strRet="";
	BYTE nField0,nField1,nField2,nField3;
	m_ctlDvrIP.GetAddress(nField0,nField1,nField2,nField3);
	strRet.Format("%d.%d.%d.%d",nField0,nField1,nField2,nField3);
	return strRet;
}

//Display log in failure reason
void CClientDemo3Dlg::ShowLoginErrorReason(int nError)
{
	if(1 == nError)		MessageBox(ConvertString("Invalid password!"), ConvertString("Prompt"));
	else if(2 == nError)	MessageBox(ConvertString("Invalid account!"), ConvertString("Prompt"));
	else if(3 == nError)	MessageBox(ConvertString("Timeout!"), ConvertString("Prompt"));
	else if(4 == nError)	MessageBox(ConvertString("The user has logged in!"), ConvertString("Prompt"));
	else if(5 == nError)	MessageBox(ConvertString("The user has been locked!"), ConvertString("Prompt"));
	else if(6 == nError)	MessageBox(ConvertString("The user has listed into illegal!"), ConvertString("Prompt"));
	else if(7 == nError)	MessageBox(ConvertString("The system is busy!"), ConvertString("Prompt"));
	else if(9 == nError)	MessageBox(ConvertString("You Can't find the network server!"), ConvertString("Prompt"));
	else	MessageBox(ConvertString("Login failed!"), ConvertString("Prompt"));
}

//Initialize TabControl 
void CClientDemo3Dlg::InitTabControl()
{
	m_pTabFileDlg = new CPlaybackByFile();
	m_pTabTimeDlg = new CPlaybackByTime();
	//Create no-mode dialogue box. Specified tag control is the main window of the no-mode dialogue box.
	m_pTabFileDlg->Create(IDD_PLAYBACK_RECORDFILE,&m_Tab);
	m_pTabTimeDlg->Create(IDD_PLAYBACK_BYTIME,&m_Tab);
	
	m_Tab.InsertItem(0, ConvertString("Playback by file"));
	m_Tab.InsertItem(1, ConvertString("Playback by time"));
	
	m_Tab.SetMinTabWidth(-1);
	m_Tab.SetPadding(CSize(15,8));
	m_Tab.SetCurSel(0);
	DoTab(0);
}

void CClientDemo3Dlg::DoTab(int nTab)
{
	//Confirm nTab value is within the threshold.
	if(nTab>1)
	{
		nTab=1;
	}
	if(nTab<0)
	{
		nTab=0;
	}
	
	BOOL bTab[2];
	bTab[0]=bTab[1]=FALSE;
	bTab[nTab]=TRUE;
	
	//Display or hide dialogue box
	SetDlgState(m_pTabFileDlg,bTab[0]);
	SetDlgState(m_pTabTimeDlg,bTab[1]);
}

void CClientDemo3Dlg::SetDlgState(CWnd *pWnd, BOOL bShow)
{
	if(bShow)
	{
		pWnd->ShowWindow(SW_SHOW);
		//Display in the middle 
		pWnd->CenterWindow();
	}
	else
	{
		pWnd->ShowWindow(SW_HIDE);
	}
}

void CClientDemo3Dlg::OnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	int nSelect = m_Tab.GetCurSel();
	if(nSelect>=0)
	{
		DoTab(nSelect);
	}
	*pResult = 0;
}

//Play in file mode
void CClientDemo3Dlg::PlayBackByFile(NET_RECORDFILE_INFO fileInfo)
{
	m_ctlPlayBack.SetSelection(0,0);
	m_ctlPlayBack.SetPos(0);
	if(0 != m_LoginID)
	{
		//First close video 
		ClosePlayBack();
		m_hPlayBack = 0;

		HWND hPlayBack = GetDlgItem(IDC_SCREEN_PLAYBACK)->m_hWnd;
		long lHandle = CLIENT_PlayBackByRecordFile(m_LoginID,&fileInfo,hPlayBack,PlayCallBack,(DWORD)this);
		if(0 != lHandle)
		{
			m_hPlayBack = lHandle;
			m_ePlayMode = EM_DIRECTMODE;
			SetDlgItemText(IDC_BUTTON_PLAY, ConvertString("Pause"));
			m_PlayBackType = EM_FILEBACK;
		}
		else
		{
			MessageBox(ConvertString("Playback failed!"), ConvertString("Prompt"));
		}
	}
	Invalidate();
}

//Process status callback
void CALLBACK PlayCallBack(LONG lPlayHandle, DWORD dwTotalSize, DWORD dwDownLoadSize, DWORD dwUser)
{
	if(0 != dwUser)
	{
		CClientDemo3Dlg *dlg = (CClientDemo3Dlg *)dwUser;
		dlg->ReceivePlayPos(lPlayHandle, dwTotalSize, dwDownLoadSize);
	}
}

//Process status process 
void CClientDemo3Dlg::ReceivePlayPos(LONG lPlayHandle, DWORD dwTotalSize, DWORD dwDownLoadSize)
{
	//TRACE("%d   %d\n",dwTotalSize,dwDownLoadSize);
	if(lPlayHandle == m_hPlayBack)
	{
		//Playback process
		if(-1 != dwDownLoadSize)
		{
			m_dwTotalSize = dwTotalSize;
			m_dwCurValue = dwDownLoadSize;
		}
		else
		{
			//TRACE("over\n");
			//Play ends
			m_dwTotalSize = 0;
			m_dwCurValue = 0;
			SetDlgItemText(IDC_BUTTON_PLAY, ConvertString("Play"));
			if(0 != m_hPlayBack)
			{
				CLIENT_StopPlayBack(m_hPlayBack);
				m_hPlayBack = 0;
				Invalidate();
			}
		}
	}
	else if(lPlayHandle == m_pDownLoadFile)
	{
		//Download process in file mode
		if(-1 != dwDownLoadSize)
		{
			m_pTabFileDlg->m_dwTotalSize = dwTotalSize;
			m_pTabFileDlg->m_dwCurValue = dwDownLoadSize;
		}
		else
		{
			m_pTabFileDlg->m_dwTotalSize = 0;
			m_pTabFileDlg->m_dwCurValue = 0;
			if(0 != m_pDownLoadFile)
			{
				CLIENT_StopDownload(m_pDownLoadFile);
				m_pDownLoadFile = 0;
			}
			m_pTabFileDlg->SetDlgItemText(IDC_BUTTON_DOWNLOADBYFILE,ConvertString("Download"));
		}
	}
}

//Stop play
void CClientDemo3Dlg::OnButtonStop() 
{
	// TODO: Add your control notification handler code here
	if(0 != m_hPlayBack)
	{
		ClosePlayBack();
		m_hPlayBack = 0;
		Invalidate();
	}

	SetDlgItemText(IDC_BUTTON_PLAY,ConvertString("Play"));

	m_dwTotalSize = 0;
	m_dwCurValue = 0;
	m_ctlPlayBack.SetPos(0);
	m_ctlPlayBack.SetSelection(0, 0);
}

void CClientDemo3Dlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent == m_nTimer)
	{
		if (m_dwTotalSize != 0 && m_dwCurValue != -1)
		{
			int nOffsetPos = m_dwCurValue*100/m_dwTotalSize;
			m_ctlPlayBack.SetPos(nOffsetPos);
			m_ctlPlayBack.SetSelection(0, nOffsetPos);
			m_ctlPlayBack.Invalidate();
		}
		else
		{
			m_ctlPlayBack.SetPos(0);
			m_ctlPlayBack.SetSelection(0, 0);
			m_ctlPlayBack.Invalidate();
		}
	}
	
	CDialog::OnTimer(nIDEvent);
}

void CClientDemo3Dlg::OnReleasedcaptureSliderPlayposition(NMHDR* pNMHDR, LRESULT* pResult) 
{
	//TRACE("Release\n");
	// TODO: Add your control notification handler code here
	if(0 != m_hPlayBack)
	{
		if(0 != m_nTimer)
		{
			KillTimer(m_nTimer);
			m_nTimer = 0;
		}
		
		int nOffsetPos = m_ctlPlayBack.GetPos();
//		TRACE("Pos:%d\n", nOffsetPos);
		if(m_PlayBackType == EM_FILEBACK)
		{
			unsigned int nOffsetByte = nOffsetPos*m_dwTotalSize/100;
			BOOL bSuccess = CLIENT_SeekPlayBack(m_hPlayBack, 0xFFFFFFFF, nOffsetByte);
			if(m_ePlayMode == EM_SERVERMODE)
			{
				PLAY_ResetBuffer(1, BUF_VIDEO_SRC);
				PLAY_ResetBuffer(1, BUF_AUDIO_SRC);
				PLAY_ResetBuffer(1, BUF_VIDEO_RENDER);
				PLAY_ResetBuffer(1, BUF_AUDIO_RENDER);
			}
		}
		else if(m_PlayBackType == EM_TIMEBACK)
		{
			UINT nOffsetTime = nOffsetPos*m_dwTotalSize/100;
			TRACE("TotalTime:%d,OffsetTime:%d\n", m_dwTotalSize, nOffsetTime);
			BOOL bSuccess = CLIENT_SeekPlayBack(m_hPlayBack, nOffsetTime, 0xFFFFFFFF);
			if(m_ePlayMode == EM_SERVERMODE)
			{
				PLAY_ResetBuffer(1, BUF_VIDEO_SRC);
				PLAY_ResetBuffer(1, BUF_AUDIO_SRC);
				PLAY_ResetBuffer(1, BUF_VIDEO_RENDER);
				PLAY_ResetBuffer(1, BUF_AUDIO_RENDER);
			}
		}

		if(0 == m_nTimer)
		{
			m_nTimer = SetTimer(1, 500, NULL);
		}
	}
	
	*pResult = 0;
}

//Download in file mode 
BOOL CClientDemo3Dlg::DownLoadByFile(NET_RECORDFILE_INFO fileInfo,BOOL bLoad)
{
	BOOL bRet = FALSE;
	if(bLoad)
	{
		//Download 
		if(0 != m_LoginID)
		{
			if(0 != m_pDownLoadFile)
			{
				//Close download
				BOOL bSuccess = CLIENT_StopDownload(m_pDownLoadFile);
				if(bSuccess)
				{
					m_pDownLoadFile = 0;
				}
				else
				{
					MessageBox(ConvertString("Stop download failed!"), ConvertString("Prompt"));
				}
			}
			
			CFileDialog dlg(FALSE,"*.dav","download.dav",OFN_OVERWRITEPROMPT|OFN_HIDEREADONLY,
				"JPEG(*.JPEG;JPG)|*.JPEG;JPG|All File(*.dav)|*.*||",this);
			if(dlg.DoModal() == IDOK)
			{
				CString strFilePathName = dlg.GetPathName();
				long lRet = CLIENT_DownloadByRecordFile(m_LoginID, &fileInfo, 
					(LPSTR)(LPCSTR)strFilePathName,PlayCallBack, (DWORD)this);
				if(0 != lRet)
				{
					m_pDownLoadFile = lRet;
					bRet = TRUE;
				}
				else
				{
					LastError();
					MessageBox(ConvertString("Does not support playback and download simultaneously in one channel! please select another channel to download!"), ConvertString("Prompt"));
				}
			}
		}
	}
	else
	{
		//Stop download
		if(0 != m_pDownLoadFile)
		{
			BOOL bSuccess = CLIENT_StopDownload(m_pDownLoadFile);
			if(bSuccess)
			{
				m_pDownLoadFile = 0;
				bRet = TRUE;
			}
			else
			{
				MessageBox(ConvertString("Stop download failed!"), ConvertString("Prompt"));
			}
		}
	}
	return bRet;
}

//Display function execution failure reason
void CClientDemo3Dlg::LastError()
{
DWORD dwError = CLIENT_GetLastError();
	switch(dwError)
	{
	case NET_NOERROR:					GetDlgItem(IDC_LAST_ERROR)->SetWindowText(ConvertString(ERROR_NET_NOERROR));
		break;
	case NET_ERROR:						GetDlgItem(IDC_LAST_ERROR)->SetWindowText(ConvertString(ERROR_NET_ERROR));
		break;
	case NET_SYSTEM_ERROR:				GetDlgItem(IDC_LAST_ERROR)->SetWindowText(ConvertString(ERROR_NET_SYSTEM_ERROR));
		break;
	case NET_NETWORK_ERROR:				GetDlgItem(IDC_LAST_ERROR)->SetWindowText(ConvertString(ERROR_NET_NETWORK_ERROR));
		break;
	case NET_DEV_VER_NOMATCH:			GetDlgItem(IDC_LAST_ERROR)->SetWindowText(ConvertString(ERROR_NET_DEV_VER_NOMATCH));
		break;
	case NET_INVALID_HANDLE:			GetDlgItem(IDC_LAST_ERROR)->SetWindowText(ConvertString(ERROR_NET_INVALID_HANDLE));
		break;
	case NET_OPEN_CHANNEL_ERROR:		GetDlgItem(IDC_LAST_ERROR)->SetWindowText(ConvertString(ERROR_NET_OPEN_CHANNEL_ERROR));
		break;
	case NET_CLOSE_CHANNEL_ERROR:		GetDlgItem(IDC_LAST_ERROR)->SetWindowText(ConvertString(ERROR_NET_CLOSE_CHANNEL_ERROR));
		break;
	case NET_ILLEGAL_PARAM:				GetDlgItem(IDC_LAST_ERROR)->SetWindowText(ConvertString(ERROR_NET_ILLEGAL_PARAM));
		break;
	case NET_SDK_INIT_ERROR:			GetDlgItem(IDC_LAST_ERROR)->SetWindowText(ConvertString(ERROR_NET_SDK_INIT_ERROR));
		break;
	case NET_SDK_UNINIT_ERROR:			GetDlgItem(IDC_LAST_ERROR)->SetWindowText(ConvertString(ERROR_NET_SDK_UNINIT_ERROR));
		break;
	case NET_RENDER_OPEN_ERROR:			GetDlgItem(IDC_LAST_ERROR)->SetWindowText(ConvertString(ERROR_NET_RENDER_OPEN_ERROR));
		break;
	case NET_DEC_OPEN_ERROR:			GetDlgItem(IDC_LAST_ERROR)->SetWindowText(ConvertString(ERROR_NET_DEC_OPEN_ERROR));
		break;
	case NET_DEC_CLOSE_ERROR:			GetDlgItem(IDC_LAST_ERROR)->SetWindowText(ConvertString(ERROR_NET_DEC_CLOSE_ERROR));
		break;
	case NET_MULTIPLAY_NOCHANNEL:		GetDlgItem(IDC_LAST_ERROR)->SetWindowText(ConvertString(ERROR_NET_MULTIPLAY_NOCHANNEL));
		break;
	case NET_TALK_INIT_ERROR:			GetDlgItem(IDC_LAST_ERROR)->SetWindowText(ConvertString(ERROR_NET_TALK_INIT_ERROR));
		break;
	case NET_TALK_NOT_INIT:				GetDlgItem(IDC_LAST_ERROR)->SetWindowText(ConvertString(ERROR_NET_TALK_NOT_INIT));
		break;	
	case NET_TALK_SENDDATA_ERROR:		GetDlgItem(IDC_LAST_ERROR)->SetWindowText(ConvertString(ERROR_NET_TALK_SENDDATA_ERROR));
		break;
	case NET_NO_TALK_CHANNEL:			GetDlgItem(IDC_LAST_ERROR)->SetWindowText(ConvertString(ERROR_NET_NO_TALK_CHANNEL));
		break;
	case NET_NO_AUDIO:					GetDlgItem(IDC_LAST_ERROR)->SetWindowText(ConvertString(ERROR_NET_NO_AUDIO));
		break;								
	case NET_REAL_ALREADY_SAVING:		GetDlgItem(IDC_LAST_ERROR)->SetWindowText(ConvertString(ERROR_NET_REAL_ALREADY_SAVING));
		break;
	case NET_NOT_SAVING:				GetDlgItem(IDC_LAST_ERROR)->SetWindowText(ConvertString(ERROR_NET_NOT_SAVING));
		break;
	case NET_OPEN_FILE_ERROR:			GetDlgItem(IDC_LAST_ERROR)->SetWindowText(ConvertString(ERROR_NET_OPEN_FILE_ERROR));
		break;
	case NET_PTZ_SET_TIMER_ERROR:		GetDlgItem(IDC_LAST_ERROR)->SetWindowText(ConvertString(ERROR_NET_PTZ_SET_TIMER_ERROR));
		break;
	case NET_RETURN_DATA_ERROR:			GetDlgItem(IDC_LAST_ERROR)->SetWindowText(ConvertString(ERROR_NET_RETURN_DATA_ERROR));
		break;
	case NET_INSUFFICIENT_BUFFER:		GetDlgItem(IDC_LAST_ERROR)->SetWindowText(ConvertString(ERROR_NET_INSUFFICIENT_BUFFER));
		break;
	case NET_NOT_SUPPORTED:				GetDlgItem(IDC_LAST_ERROR)->SetWindowText(ConvertString(ERROR_NET_NOT_SUPPORTED));
		break;
	case NET_NO_RECORD_FOUND:			GetDlgItem(IDC_LAST_ERROR)->SetWindowText(ConvertString(ERROR_NET_NO_RECORD_FOUND));
		break;	
	case NET_LOGIN_ERROR_PASSWORD:		GetDlgItem(IDC_LAST_ERROR)->SetWindowText(ConvertString(ERROR_NET_LOGIN_ERROR_PASSWORD));
		break;
	case NET_LOGIN_ERROR_USER:			GetDlgItem(IDC_LAST_ERROR)->SetWindowText(ConvertString(ERROR_NET_LOGIN_ERROR_USER));
		break;
	case NET_LOGIN_ERROR_TIMEOUT:		GetDlgItem(IDC_LAST_ERROR)->SetWindowText(ConvertString(ERROR_NET_LOGIN_ERROR_TIMEOUT));
		break;
	case NET_LOGIN_ERROR_RELOGGIN:		GetDlgItem(IDC_LAST_ERROR)->SetWindowText(ConvertString(ERROR_NET_LOGIN_ERROR_RELOGGIN));
		break;
	case NET_LOGIN_ERROR_LOCKED:		GetDlgItem(IDC_LAST_ERROR)->SetWindowText(ConvertString(ERROR_NET_LOGIN_ERROR_LOCKED));
		break;
	case NET_LOGIN_ERROR_BLACKLIST:		GetDlgItem(IDC_LAST_ERROR)->SetWindowText(ConvertString(ERROR_NET_LOGIN_ERROR_BLACKLIST));
		break;
	case NET_LOGIN_ERROR_BUSY:			GetDlgItem(IDC_LAST_ERROR)->SetWindowText(ConvertString(ERROR_NET_LOGIN_ERROR_BUSY));
		break;
	case NET_LOGIN_ERROR_CONNECT:		GetDlgItem(IDC_LAST_ERROR)->SetWindowText(ConvertString(ERROR_NET_LOGIN_ERROR_CONNECT));
		break;
	case NET_LOGIN_ERROR_NETWORK:		GetDlgItem(IDC_LAST_ERROR)->SetWindowText(ConvertString(ERROR_NET_LOGIN_ERROR_NETWORK));
		break;								
	case NET_RENDER_SOUND_ON_ERROR:		GetDlgItem(IDC_LAST_ERROR)->SetWindowText(ConvertString(ERROR_NET_RENDER_SOUND_ON_ERROR));
		break;
	case NET_RENDER_SOUND_OFF_ERROR:	GetDlgItem(IDC_LAST_ERROR)->SetWindowText(ConvertString(ERROR_NET_RENDER_SOUND_OFF_ERROR));
		break;
	case NET_RENDER_SET_VOLUME_ERROR:	GetDlgItem(IDC_LAST_ERROR)->SetWindowText(ConvertString(ERROR_NET_RENDER_SET_VOLUME_ERROR));
		break;
	case NET_RENDER_ADJUST_ERROR:		GetDlgItem(IDC_LAST_ERROR)->SetWindowText(ConvertString(ERROR_NET_RENDER_ADJUST_ERROR));
		break;
	case NET_RENDER_PAUSE_ERROR:		GetDlgItem(IDC_LAST_ERROR)->SetWindowText(ConvertString(ERROR_NET_RENDER_PAUSE_ERROR));
		break;
	case NET_RENDER_SNAP_ERROR:			GetDlgItem(IDC_LAST_ERROR)->SetWindowText(ConvertString(ERROR_NET_RENDER_SNAP_ERROR));
		break;
	case NET_RENDER_STEP_ERROR:			GetDlgItem(IDC_LAST_ERROR)->SetWindowText(ConvertString(ERROR_NET_RENDER_STEP_ERROR));
		break;
	case NET_RENDER_FRAMERATE_ERROR:	GetDlgItem(IDC_LAST_ERROR)->SetWindowText(ConvertString(ERROR_NET_RENDER_FRAMERATE_ERROR));
		break;
	case NET_CONFIG_DEVBUSY:			GetDlgItem(IDC_LAST_ERROR)->SetWindowText(ConvertString(ERROR_NET_CONFIG_DEVBUSY));
		break;
	case NET_CONFIG_DATAILLEGAL:		GetDlgItem(IDC_LAST_ERROR)->SetWindowText(ConvertString(ERROR_NET_CONFIG_DATAILLEGAL));
		break;							
	case NET_NO_INIT:					GetDlgItem(IDC_LAST_ERROR)->SetWindowText(ConvertString(ERROR_NET_NO_INIT));
		break;
	case NET_DOWNLOAD_END:				GetDlgItem(IDC_LAST_ERROR)->SetWindowText(ConvertString(ERROR_NET_DOWNLOAD_END));
		break;
	default:							GetDlgItem(IDC_LAST_ERROR)->SetWindowText(ConvertString(ERROR_NET_ERROR));						
	}
}

//Playback in accordance with time
void CClientDemo3Dlg::PlayBackByTime(int nChannelID, LPNET_TIME lpStartTime, LPNET_TIME lpStopTime)
{
	m_ctlPlayBack.SetSelection(0,0);
	m_ctlPlayBack.SetPos(0);
	if(0 != m_LoginID)
	{
		//First close video 
		ClosePlayBack();
		m_hPlayBack = 0;
		
		HWND hPlayBack = GetDlgItem(IDC_SCREEN_PLAYBACK)->m_hWnd;
		long lHandle = CLIENT_PlayBackByTime(m_LoginID,nChannelID,lpStartTime,lpStopTime,hPlayBack,PlayCallBack,(DWORD)this);
		if(0 != lHandle)
		{
			m_hPlayBack = lHandle;
			m_ePlayMode = EM_DIRECTMODE;
			SetDlgItemText(IDC_BUTTON_PLAY, ConvertString("Pause"));
			m_PlayBackType = EM_TIMEBACK;
		}
		else
		{
			MessageBox(ConvertString("Playback failed!"), ConvertString("Prompt"));
		}
		Invalidate();
	}
	else
	{
		MessageBox(ConvertString("Please login first!"), ConvertString("Prompt"));
	}
}

//Turn NET_TIME type to CTime
CTime CClientDemo3Dlg::ConvertNetTimeToCTime(LPNET_TIME lpNetTime)
{
	int nYear = (int)lpNetTime->dwYear;
	int nMonth = (int)lpNetTime->dwMonth;
	int nDay = (int)lpNetTime->dwDay;
	int nHour = (int)lpNetTime->dwHour;
	int nMinute = (int)lpNetTime->dwMinute;
	int nSecond = (int)lpNetTime->dwSecond;
	CTime time(nYear,nMonth,nDay,nHour,nMinute,nSecond);
	return time;
}

//Download callback in time mode
void CALLBACK TimeDownLoadPos(LONG lPlayHandle, DWORD dwTotalSize, DWORD dwDownLoadSize, int index, NET_RECORDFILE_INFO recordfileinfo, DWORD dwUser)
{
	CClientDemo3Dlg * dlg = (CClientDemo3Dlg *)dwUser;
	dlg->TimeDownLoadCallBack(lPlayHandle, dwTotalSize, dwDownLoadSize, index, recordfileinfo);
}

//Download process in time mode 
void CClientDemo3Dlg::TimeDownLoadCallBack(LONG lPlayHandle, DWORD dwTotalSize, DWORD dwDownLoadSize, int index, NET_RECORDFILE_INFO recordfileinfo)
{
	if(lPlayHandle == m_pDownLoadByTime)
	{
		if(-1 != dwDownLoadSize)
		{
			m_pTabTimeDlg->m_dwTotalSize = dwTotalSize;
			m_pTabTimeDlg->m_dwCurValue = dwDownLoadSize;
		}
		else
		{
			m_pTabTimeDlg->m_dwTotalSize = 0;
			m_pTabTimeDlg->m_dwCurValue = 0;
			if(0 != m_pDownLoadByTime)
			{
				CLIENT_StopDownload(m_pDownLoadByTime);
				m_pDownLoadByTime = 0;
			}
			m_pTabTimeDlg->SetDlgItemText(IDC_BUTTON_DOWNLOAD, ConvertString("Download"));
		}
	}
}


//Download in accordance with time
BOOL CClientDemo3Dlg::DownLoadByTime(int nChannelId, LPNET_TIME tmStart, LPNET_TIME tmEnd, BOOL bLoad)
{
	BOOL bRet =FALSE;
	if(0 == m_LoginID)
	{
		MessageBox(ConvertString("Please login first"), ConvertString("Prompt"));
		return bRet;
	}
	if(bLoad)
	{
		//Download
		if(0 != m_LoginID)
		{
			if(0 != m_pDownLoadByTime)
			{
				//Close download
				BOOL bSuccess = CLIENT_StopDownload(m_pDownLoadByTime);
				if(bSuccess)
				{
					m_pDownLoadByTime = 0;
				}
				else
				{
					MessageBox(ConvertString("Stop download failed!"), ConvertString("Prompt"));
				}
			}
			
			CFileDialog dlg(FALSE,"*.dav","download.dav",OFN_OVERWRITEPROMPT|OFN_HIDEREADONLY,
				"All File(*.dav)|*.*||",this);
			if(dlg.DoModal() == IDOK)
			{
				CString strFilePathName = dlg.GetPathName();
				long lRet = CLIENT_DownloadByTime(m_LoginID, nChannelId, 0, 
					tmStart, tmEnd, (LPSTR)(LPCSTR)strFilePathName,TimeDownLoadPos, (DWORD)this);
				if(0 != lRet)
				{
					m_pDownLoadByTime = lRet;
					bRet = TRUE;
				}
				else
				{
					LastError();
					MessageBox(ConvertString("Does not support playback and download simultaneously in one channel! please select another channel to download!"), ConvertString("Prompt"));
				}
			}
		}
	}
	else
	{
		//Stop download
		if(0 != m_pDownLoadByTime)
		{
			BOOL bSuccess = CLIENT_StopDownload(m_pDownLoadByTime);
			if(bSuccess)
			{
				m_pDownLoadByTime = 0;
				bRet = TRUE;
			}
			else
			{
				MessageBox(ConvertString("Stop download failed!"), ConvertString("Prompt"));
			}
		}
	}
	return bRet;
}

//Play and pause
void CClientDemo3Dlg::OnButtonPlay() 
{
	// TODO: Add your control notification handler code here
	if((0 != m_hPlayBack) && (0 != m_LoginID))
	{
		CString strText;
		GetDlgItemText(IDC_BUTTON_PLAY,strText);
		if(strText ==  ConvertString("Play"))
		{
			BOOL bSuccess = FALSE;
			if(m_ePlayMode == EM_DIRECTMODE)
			{
				bSuccess = CLIENT_PausePlayBack(m_hPlayBack,FALSE);
			}
			else if(m_ePlayMode == EM_SERVERMODE)
			{
				bSuccess = PLAY_Pause(1,FALSE);
			}
			if(bSuccess)
			{
				SetDlgItemText(IDC_BUTTON_PLAY, ConvertString("Pause"));
			}
			else
			{
				MessageBox( ConvertString("Play failed!"), ConvertString("Prompt"));
			}
		}
		else
		{
			//Pause
			BOOL bSuccess = FALSE;
			if(m_ePlayMode == EM_DIRECTMODE)
			{
				bSuccess = CLIENT_PausePlayBack(m_hPlayBack,TRUE);
			}
			else if(m_ePlayMode == EM_SERVERMODE)
			{
				bSuccess = PLAY_Pause(1,TRUE);
			}
			if(bSuccess)
			{
				SetDlgItemText(IDC_BUTTON_PLAY, ConvertString("Play"));
			}
			else
			{
				MessageBox(ConvertString("Pause failed!"), ConvertString("Prompt"));
			}
		}
	}
	else
	{
		MessageBox(ConvertString("Please play first!"), ConvertString("Prompt"));
	}
}

//Slow forward
void CClientDemo3Dlg::OnButtonSlow() 
{
	// TODO: Add your control notification handler code here
	if((0 != m_hPlayBack) && (0 != m_LoginID))
	{
		BOOL bSuccess = TRUE;
		if(m_ePlayMode == EM_DIRECTMODE)
		{
			bSuccess = CLIENT_SlowPlayBack(m_hPlayBack);
		}
		else if(m_ePlayMode == EM_SERVERMODE)
		{
			bSuccess = PLAY_Slow(1);
		}
		if(!bSuccess)
		{
			MessageBox(ConvertString("Slow failed!"), ConvertString("Prompt"));
		}
	}
	else
	{
		MessageBox(ConvertString("Please play first!"), ConvertString("Prompt"));
	}
}

//fast forward
void CClientDemo3Dlg::OnButtonFast() 
{
	// TODO: Add your control notification handler code here
	if((0 != m_hPlayBack) && (0 != m_LoginID))
	{
		BOOL bSuccess = TRUE;
		if(m_ePlayMode == EM_DIRECTMODE)
		{
			bSuccess = CLIENT_FastPlayBack(m_hPlayBack);
		}
		else if(m_ePlayMode == EM_SERVERMODE)
		{
			bSuccess = PLAY_Fast(1);
		}
		if(!bSuccess)
		{
			MessageBox(ConvertString("Fast failed!"), ConvertString("Prompt"));
		}
	}
	else
	{
		MessageBox(ConvertString("Please play first!"), ConvertString("Prompt"));
	}
}

//Frame
void CClientDemo3Dlg::OnButtonFrameto() 
{
	// TODO: Add your control notification handler code here
	if((0 != m_hPlayBack) && (0 != m_LoginID))
	{
		BOOL bSuccess = TRUE;
		if(m_ePlayMode == EM_DIRECTMODE)
		{
			bSuccess = CLIENT_StepPlayBack(m_hPlayBack,FALSE);
		}
		else if(m_ePlayMode == EM_SERVERMODE)
		{
			bSuccess = PLAY_OneByOne(1);
		}
		if(bSuccess)
		{
			GetDlgItem(IDC_BUTTON_RESET)->EnableWindow(TRUE);
		}
		else
		{
			MessageBox(ConvertString("Play by Single Frame failed!"), ConvertString("Prompt"));
		}
	}
	else
	{
		MessageBox(ConvertString("Please play first!"), ConvertString("Prompt"));
	}
}

//Reset
void CClientDemo3Dlg::OnButtonReset() 
{
	// TODO: Add your control notification handler code here
	if((0 != m_hPlayBack) && (0 != m_LoginID))
	{
		BOOL bSuccess = TRUE;
		if(m_ePlayMode == EM_DIRECTMODE)
		{
			bSuccess = CLIENT_StepPlayBack(m_hPlayBack,TRUE);
		}
		else if(m_ePlayMode == EM_SERVERMODE)
		{
			HWND hPlayBack = GetDlgItem(IDC_SCREEN_PLAYBACK)->m_hWnd;
			bSuccess = PLAY_Play(1,hPlayBack);
		}
		if(bSuccess)
		{
			GetDlgItem(IDC_BUTTON_RESET)->EnableWindow(FALSE);
		}
		else
		{
			MessageBox(ConvertString("Reset failed!"), ConvertString("Prompt"));
		}
	}
	else
	{
		MessageBox(ConvertString("Please play first!"), ConvertString("Prompt"));
	}
}

//Data callback
int CALLBACK DataCallBack(LONG lRealHandle, DWORD dwDataType, BYTE *pBuffer, DWORD dwBufSize, DWORD dwUser)
{
	TRACE("lRealHandle=%d;dwUser=%d\n",lRealHandle,dwUser);
	int nRet = 1;
	if(dwUser == 0)
	{
		return nRet;
	}
	CClientDemo3Dlg *dlg = (CClientDemo3Dlg *)dwUser;
	nRet = dlg->ReceiveRealData(lRealHandle,dwDataType, pBuffer, dwBufSize);
	return nRet;
}

//Data callback mode 
int CClientDemo3Dlg::ReceiveRealData(LONG lRealHandle, DWORD dwDataType, BYTE *pBuffer, DWORD dwBufSize)
{
	int nRet = 1;
	if(lRealHandle == m_hPlayBack)
	{	
		BOOL bSuccess = TRUE;
		switch(dwDataType) {
		case 0:
			//Original data 
			bSuccess = PLAY_InputData(1,pBuffer,dwBufSize);
			if(!bSuccess)
			{
				nRet = 0;
			}
			break;
		case 1:
			//Standard video data 
			
			break;
		case 2:
			//yuv data
			
			break;
		case 3:
			//pcm audio data 
			
			break;
		case 4:
			//Original audio data 
			
			break;
		default:
			break;
		}	
	}
	return nRet;
}

//Playback in accordance with file in data callback mode.
void CClientDemo3Dlg::PlayBackByFileServerMode(NET_RECORDFILE_INFO fileInfo)
{
	m_ctlPlayBack.SetSelection(0,0);
	m_ctlPlayBack.SetPos(0);
	if(0 != m_LoginID)
	{
		//First close video 
		ClosePlayBack();
		m_hPlayBack = 0;
		
		//Set stream play mode
		PLAY_SetStreamOpenMode(1,STREAME_FILE);
		//Open stream
		BOOL bOpenRet = PLAY_OpenStream(1,0,0,1024*100);
		if(bOpenRet)
		{
			//Begin play
			HWND hPlayBack = GetDlgItem(IDC_SCREEN_PLAYBACK)->m_hWnd;
			BOOL bPlayRet = PLAY_Play(1,hPlayBack);
			if(bPlayRet)
			{
				//Playabck
				long lHandle = CLIENT_PlayBackByRecordFileEx(m_LoginID,&fileInfo,0,PlayCallBack,(DWORD)this,DataCallBack,(DWORD)this);
				if(0 != lHandle)
				{
					m_hPlayBack = lHandle;
					m_ePlayMode = EM_SERVERMODE;
					SetDlgItemText(IDC_BUTTON_PLAY, ConvertString("Pause"));
					m_PlayBackType = EM_FILEBACK;
				}
				else
				{
					PLAY_Stop(1);
					PLAY_CloseStream(1);
					MessageBox(ConvertString("Playback failed!"), ConvertString("Prompt"));
				}
			}
			else
			{
				PLAY_CloseStream(1);
			}
		}
	}
	Invalidate();
}

//Playback in accordance with time in data callback mode.
void CClientDemo3Dlg::PlayBackByTimeServerMode(int nChannelID, LPNET_TIME lpStartTime, LPNET_TIME lpStopTime)
{
	//m_ePlayMode = EM_SERVERMODE;
	m_ctlPlayBack.SetSelection(0,0);
	m_ctlPlayBack.SetPos(0);
	if(0 != m_LoginID)
	{
		//First close video 
		ClosePlayBack();
		m_hPlayBack = 0;

		//Set stream play mode
		PLAY_SetStreamOpenMode(1,STREAME_FILE);
		//Open stream
		BOOL bOpenRet = PLAY_OpenStream(1,0,0,1024*100);
		if(bOpenRet)
		{
			//Begin play
			HWND hPlayBack = GetDlgItem(IDC_SCREEN_PLAYBACK)->m_hWnd;
			BOOL bPlayRet = PLAY_Play(1,hPlayBack);
			if(bPlayRet)
			{
				//Playback
				long lHandle = CLIENT_PlayBackByTimeEx(m_LoginID,nChannelID,lpStartTime,lpStopTime,0,PlayCallBack,(DWORD)this,DataCallBack,(DWORD)this);
				if(0 != lHandle)
				{
					m_hPlayBack = lHandle;
					m_ePlayMode = EM_SERVERMODE;
					SetDlgItemText(IDC_BUTTON_PLAY, ConvertString("Pause"));
					m_PlayBackType = EM_TIMEBACK;
				}
				else
				{
					PLAY_Stop(1);
					PLAY_CloseStream(1);
					MessageBox(ConvertString("Playback failed!"), ConvertString("Prompt"));
				}
			}
			else
			{
				PLAY_CloseStream(1);
			}
		}
		Invalidate();
	}
	else
	{
		MessageBox(ConvertString("Please login first!"), ConvertString("Prompt"));
	}
}

//Close video 
void CClientDemo3Dlg::ClosePlayBack()
{
	if(m_ePlayMode == EM_DIRECTMODE)
	{
		//Close video directly
		if(0 != m_hPlayBack)
		{
			CLIENT_StopPlayBack(m_hPlayBack);
			m_hPlayBack = 0;
		}
	}
	else if(m_ePlayMode == EM_SERVERMODE)
	{
		//Close video in data callback mode
		if(0 != m_hPlayBack)
		{
			BOOL bSuccess = CLIENT_StopPlayBack(m_hPlayBack);
			m_hPlayBack = 0;
			//Then close PLAY_Play
			PLAY_Stop(1);
			//At last close PLAY_OpenStream
			PLAY_CloseStream(1);
		}
	}
}

//Click mouse event in process bar
void CClientDemo3Dlg::ExSliderButtonDown()
{
	//TRACE("down\n");
	if(0 != m_nTimer)
	{
		KillTimer(m_nTimer);
		m_nTimer = 0;
	}
}

//Mouse release event in process bar
void CClientDemo3Dlg::ExSliderButtonUp()
{
	//TRACE("up\n");
	if(0 == m_nTimer)
	{
		m_nTimer = SetTimer(1, 500, NULL);
	}
}

void CClientDemo3Dlg::OnButtonNormalspeed() 
{
	// TODO: Add your control notification handler code here
	if((0 != m_LoginID) && (0 != m_hPlayBack))
	{
		BOOL bSuccess = TRUE;
		if(m_ePlayMode == EM_DIRECTMODE)
		{
			bSuccess = CLIENT_NormalPlayBack(m_hPlayBack);
		}
		else if(m_ePlayMode == EM_SERVERMODE)
		{
			HWND hPlayBack = GetDlgItem(IDC_SCREEN_PLAYBACK)->m_hWnd;
			bSuccess = PLAY_Play(1, hPlayBack);
		}
		if(!bSuccess)
		{
			MessageBox(ConvertString("Recover normal speed failed!"), ConvertString("Prompt"));
		}
	}
	else
	{
		MessageBox(ConvertString("Please play first!"), ConvertString("Prompt"));
	}
}



















































































































































































































































































































