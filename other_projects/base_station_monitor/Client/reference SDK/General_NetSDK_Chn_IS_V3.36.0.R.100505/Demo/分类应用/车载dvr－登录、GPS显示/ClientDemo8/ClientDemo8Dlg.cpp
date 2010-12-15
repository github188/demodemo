// ClientDemo8Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "ClientDemo8.h"
#include "ClientDemo8Dlg.h"
#include "dhnetsdk.h"
#include "MessageText.h"
#include <Winsock2.h>

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
// CClientDemo8Dlg dialog

CClientDemo8Dlg::CClientDemo8Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CClientDemo8Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CClientDemo8Dlg)
	m_strUserName = _T("admin");
	m_strPwd = _T("admin");
	m_nPort = 7070;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_hServerHandle = 0;
}

void CClientDemo8Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CClientDemo8Dlg)
	DDX_Control(pDX, IDC_LIST_GPSINFO, m_ctlGPSInfo);
	DDX_Control(pDX, IDC_IPADDRESS_IP, m_ctlLocalIP);
	DDX_Text(pDX, IDC_EDIT_USERNAME, m_strUserName);
	DDX_Text(pDX, IDC_EDIT_PWD, m_strPwd);
	DDX_Text(pDX, IDC_EDIT_PORT, m_nPort);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CClientDemo8Dlg, CDialog)
	//{{AFX_MSG_MAP(CClientDemo8Dlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_STARTLISTEN, OnButtonStartlisten)
	ON_BN_CLICKED(IDC_BUTTON_STOPLISTEN, OnButtonStoplisten)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CClientDemo8Dlg message handlers

BOOL CClientDemo8Dlg::OnInitDialog()
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
	//Set initial value 
	CString strLocalIP = GetLocalIP();
	SetDlgItemText(IDC_IPADDRESS_IP,strLocalIP);
	InitListView();
	m_nTimer = SetTimer(1, 1000, NULL);
	//Initial net SDK
	InitNetSDK();
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CClientDemo8Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CClientDemo8Dlg::OnPaint() 
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
HCURSOR CClientDemo8Dlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

//Begin listening 
void CClientDemo8Dlg::OnButtonStartlisten() 
{
	// TODO: Add your control notification handler code here
	BOOL bValid = UpdateData(TRUE);
	if(bValid)
	{
		CString strLocalIP = GetDvrIP();
		char *pIP = (LPSTR)(LPCSTR)strLocalIP;
		WORD wPort= (WORD)m_nPort;
		//Enable listening service 
		long lRet = CLIENT_ListenServer(pIP, wPort, 5000, ServerCallBack, (DWORD)this);	
		if (0 != lRet)
		{
			m_hServerHandle = lRet;
			GetDlgItem(IDC_BUTTON_STARTLISTEN)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_STOPLISTEN)->EnableWindow(TRUE);
		}
		else
		{
			MessageBox(ConvertString("Start interception failed!"), ConvertString("Prompt"));
		}
	}
}

//Stop listening 
void CClientDemo8Dlg::OnButtonStoplisten() 
{
	// TODO: Add your control notification handler code here
	//Log off all GPD connection 
	GPSLogoutAll();
	if(0 != m_hServerHandle)
	{
		BOOL bSuccess = CLIENT_StopListenServer(m_hServerHandle);
		if( !bSuccess )
		{
			MessageBox(ConvertString("Stop interception failed!"), ConvertString("Prompt"));
			return;
		}
	}
	GetDlgItem(IDC_BUTTON_STARTLISTEN)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_STOPLISTEN)->EnableWindow(FALSE);
}

//Initialize net SDK
void CClientDemo8Dlg::InitNetSDK()
{
	//Initialize net sdk. It is to activate all callbacks.
	BOOL bSuccess = CLIENT_Init(DisConnectFunc, (DWORD)this);
	if (!bSuccess)
	{
		MessageBox(ConvertString("Initialize SDK failed!"), ConvertString("Prompt"));
	}
	else
	{
		//Set GPSsubscription callback function
		CLIENT_SetSubcribeGPSCallBack(GPSRev,(DWORD)this);
	}
}

//Callbak function when device disconnted 
void CALLBACK DisConnectFunc(LONG lLoginID, char *pchDVRIP, LONG nDVRPort, DWORD dwUser)
{
	if(0 != dwUser)
	{
		CClientDemo8Dlg *dlg = (CClientDemo8Dlg *)dwUser;
		dlg->DeviceDisConnect(lLoginID, pchDVRIP,nDVRPort);
	}
}

//Process when device disconnected 
void CClientDemo8Dlg::DeviceDisConnect(LONG lLoginID, char *sDVRIP, LONG nDVRPort)
{
	//The codes need to be processed when device disconnected.
	MessageBox(ConvertString("Network disconnected!"), ConvertString("Prompt"));
}


void CClientDemo8Dlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	if(0 != m_LoginID)
	{
		CLIENT_Logout(m_LoginID);
	}
	if(0 != m_hServerHandle)
	{
		CLIENT_StopListenServer(m_hServerHandle);
	}
	//Clear SDK and then release occupieds resources. 
	CLIENT_Cleanup();
}

//Get input IP
CString CClientDemo8Dlg::GetDvrIP()
{
	CString strRet="";
	BYTE nField0,nField1,nField2,nField3;
	m_ctlLocalIP.GetAddress(nField0,nField1,nField2,nField3);
	strRet.Format("%d.%d.%d.%d",nField0,nField1,nField2,nField3);
	return strRet;
}

//GPS information subscription callback
void CALLBACK GPSRev(LONG lLoginID,GPS_Info GpsInfo,DWORD dwUserData)
{
	CClientDemo8Dlg* pDlg = (CClientDemo8Dlg*)dwUserData;
	pDlg->ReceiveGPSMessage(lLoginID,GpsInfo);
}

//Receive GPS information
void CClientDemo8Dlg::ReceiveGPSMessage(LONG lLoginID, GPS_Info GpsInfo)
{
	BOOL bIsTimeOut = m_cs.Lock();
	if(bIsTimeOut)
	{
		GPSInfoPackage info;
		info.lLoginID = lLoginID;
		info.info = GpsInfo;
		m_listGPSInfoPack.AddTail(info);
		m_cs.Unlock();
	}
}

//Listening callback function 
int CALLBACK ServerCallBack(LONG lHandle, char *pIp, WORD wPort, LONG lCommand, void *pParam, DWORD dwParamLen, DWORD dwUserData)
{
	CClientDemo8Dlg* pDlg = (CClientDemo8Dlg*)dwUserData;
	return pDlg->ListenCallBack(lHandle,pIp,wPort,lCommand,pParam,dwParamLen);
}

//Listening callback function 
int CClientDemo8Dlg::ListenCallBack(LONG lHandle, char *pIp, WORD wPort, LONG lCommand, void *pParam, DWORD dwParamLen)
{
	CString strMsg;
	char buffer[256] = {0};
	BOOL bIsTimeOut = FALSE;
	NET_DEVICEINFO deviceInfo = {0};
	int error = 0;
	CString strDvrIP = pIp;
	strncat(buffer,(const char*)pParam,dwParamLen);
	CString strGPS = buffer;
	char *pUserName = (LPSTR)(LPCSTR)m_strUserName;
	char *pPassword=(LPSTR)(LPCSTR)m_strPwd;
	switch(lCommand) {
	case DH_DVR_SERIAL_RETURN:
		{
			//Device sending out serial number callback 
			InsertListViewItem(strDvrIP,strGPS,"","", ConvertString("Login"));
			//Send out allowing registration respond 
			CLIENT_ResponseDevReg(buffer,pIp,wPort,true);
			//Log in device 
			bIsTimeOut = m_cs.Lock();
			if(bIsTimeOut)
			{
				int nRet = GPSLogin(pIp,wPort,pUserName,pPassword,pParam);
				m_cs.Unlock();
				if( nRet > 0)
				{
					ShowLoginErrorReason(error);
				}
				else
				{
					InsertListViewItem(strDvrIP,strGPS,"","", ConvertString("Login"));
				}
			}
		}
		break;
	case DH_DVR_DISCONNECT:
		//Device disconnection callback during the verification
		InsertListViewItem(strDvrIP,strGPS,"","", ConvertString("Disconnection"));
		//log off current GPS
		GPSLogout(pIp);
		break;
	default:
		break;
	}
	return 0;
}

//Display log in failure reason
void CClientDemo8Dlg::ShowLoginErrorReason(int nError)
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

//Log off all GPS module
void CClientDemo8Dlg::GPSLogoutAll()
{
	int nCount = m_listGPSModules.GetCount();
	POSITION pos = m_listGPSModules.GetHeadPosition();	
	for(int i=0;i<nCount;i++)
	{
		DH_ModuleInfo ModuleInfo = (DH_ModuleInfo)m_listGPSModules.GetNext(pos);
		if(0 != ModuleInfo.lLoginID)
		{
			CLIENT_Logout(ModuleInfo.lLoginID);
			ModuleInfo.lLoginID = 0;
		}
	}
	m_listGPSModules.RemoveAll();
}

//Log in device 
int CClientDemo8Dlg::GPSLogin(char *pIP, WORD wPort, char *pUserName, char *pPwd, void *pParam)
{
	//Check current GPS has logged in or not. 
	int nRet = -1;
	BOOL bExit = FALSE;
	NET_DEVICEINFO deviceInfo = {0};
	int error = 0;
	int nCount = m_listGPSModules.GetCount();
	POSITION pos = m_listGPSModules.GetHeadPosition();	
	for(int i=0;i<nCount;i++)
	{
		DH_ModuleInfo ModuleInfo = (DH_ModuleInfo)m_listGPSModules.GetNext(pos);
		if((*pIP) == ModuleInfo.strDvrIP)
		{
			bExit = TRUE;
			break;
		}
	}
	//Add newly connected  GPS module and then log in 
	if(!bExit)
	{
		long lRet = CLIENT_LoginEx(pIP,wPort,pUserName,pPwd,2,pParam,&deviceInfo,&error);
		if(0 != lRet)
		{
			DH_ModuleInfo ModuleInfo;
			ModuleInfo.lLoginID = lRet;
			ModuleInfo.strDvrIP.Format("%s",pIP);
			ModuleInfo.strDvrPwd.Format("%s",pPwd);
			ModuleInfo.strDvrUserName.Format("%s",pUserName);
			ModuleInfo.wPort = wPort;
			m_listGPSModules.AddTail(ModuleInfo);
			//GPS information subscription 
			CLIENT_SubcribeGPS(lRet,TRUE,3600,5);
		}
		else
		{
			//Display log in failure reason 
			nRet = error;
		}
	}
	return nRet;
}

//Device logg off 
void CClientDemo8Dlg::GPSLogout(char *pIP)
{
	int nCount = m_listGPSModules.GetCount();
	POSITION pos = m_listGPSModules.GetHeadPosition();	
	for(int i=0;i<nCount;i++)
	{
		DH_ModuleInfo ModuleInfo = (DH_ModuleInfo)m_listGPSModules.GetNext(pos);
		if((*pIP) == ModuleInfo.strDvrIP)
		{
			//Log off 
			if(0 != ModuleInfo.lLoginID)
			{
				CLIENT_Logout(ModuleInfo.lLoginID);
			}
			break;
		}
	}
}

//Initialize ListView
void CClientDemo8Dlg::InitListView()
{
	m_ctlGPSInfo.SetExtendedStyle(m_ctlGPSInfo.GetExtendedStyle() | LVS_EX_HEADERDRAGDROP | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_ctlGPSInfo.InsertColumn(0, ConvertString("Dev.IP"), LVCFMT_CENTER,120,-1);
	m_ctlGPSInfo.InsertColumn(1, ConvertString("Dev.Sequence"), LVCFMT_CENTER,120,0);
	m_ctlGPSInfo.InsertColumn(2, ConvertString("Longitude"), LVCFMT_CENTER,100,1);
	m_ctlGPSInfo.InsertColumn(3, ConvertString("Latitude"), LVCFMT_CENTER,100,2);
	m_ctlGPSInfo.InsertColumn(4, ConvertString("State"), LVCFMT_CENTER,100,3);
}

//Insert row into ListView
void CClientDemo8Dlg::InsertListViewItem(CString strDvrIP, CString strGPS, CString strX, CString strY,CString strStatus)
{
	int nIndex = m_ctlGPSInfo.GetItemCount();
	m_ctlGPSInfo.InsertItem(LVIF_TEXT|LVIF_STATE,nIndex,strDvrIP,0,LVIS_SELECTED,0,0);

	m_ctlGPSInfo.SetItemText(nIndex,1,strGPS);
	m_ctlGPSInfo.SetItemText(nIndex,2,strX);
	m_ctlGPSInfo.SetItemText(nIndex,3,strY);
	m_ctlGPSInfo.SetItemText(nIndex,4,strStatus);
}

//GPS information package in schedule.
void CClientDemo8Dlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	BOOL bIsTimeOut = m_cs.Lock();
	if(bIsTimeOut)
	{
		int nCount = m_listGPSInfoPack.GetCount();
		POSITION pos = m_listGPSInfoPack.GetHeadPosition();
		for(int i=0;i<nCount;i++)
		{
			//Get GPS package
			GPSInfoPackage& info = m_listGPSInfoPack.GetNext(pos);
			long lLoginID = info.lLoginID;
			int nCountModules = m_listGPSModules.GetCount();
			POSITION posModules = m_listGPSModules.GetHeadPosition();	
			for(int i=0;i<nCountModules;i++)
			{
				DH_ModuleInfo ModuleInfo = (DH_ModuleInfo)m_listGPSModules.GetNext(posModules);
				if(lLoginID == ModuleInfo.lLoginID)
				{
					//Display in ListView
					CString strGPS = info.info.DvrSerial;
					CString strX;
					strX.Format("%f",info.info.longitude);
					CString strY;
					strY.Format("%f",info.info.latidude);
					InsertListViewItem(ModuleInfo.strDvrIP,strGPS,strX,strY,"online");
					break;
				}
			}
		}
		m_listGPSInfoPack.RemoveAll();
		m_cs.Unlock();
	}
	
	CDialog::OnTimer(nIDEvent);
}

//Get current device IP address
CString CClientDemo8Dlg::GetLocalIP()
{
	CString strIP;
	WORD wVer = MAKEWORD(2,0);
	WSADATA wsa;
	char sHost[228];
	CString ssql;
	PHOSTENT hostinfo;
	int i = 0;
    
	if(WSAStartup(wVer,&wsa) == 0)
	{
		gethostname(sHost,228);
	}   
	if((hostinfo = gethostbyname(sHost)) != NULL)
	{
		while(hostinfo->h_addr_list[i])
		{
			strIP = inet_ntoa(*(struct in_addr *)hostinfo->h_addr_list[i++]);
		}
	}
	WSACleanup();

	return strIP;
}











































































































































































































































































