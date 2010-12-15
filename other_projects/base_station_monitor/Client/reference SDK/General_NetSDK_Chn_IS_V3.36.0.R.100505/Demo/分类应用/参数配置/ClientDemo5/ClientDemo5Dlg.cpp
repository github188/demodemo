// ClientDemo5Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "ClientDemo5.h"
#include "ClientDemo5Dlg.h"
#include "DlgDVR.h"
#include <string>
using namespace std;

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
// CClientDemo5Dlg dialog

CClientDemo5Dlg::CClientDemo5Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CClientDemo5Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CClientDemo5Dlg)
	m_nPort = 37777;
	m_strUserName = _T("admin");
	m_strPwd = _T("admin");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_LoginID = 0;
	m_nDevProtoVer = 4;
	m_dlgDeviceType = NULL;
	m_nVideoStandard = 0; 
	memset(&m_stuEncodeInfo, 0, sizeof(CFG_ENCODE_INFO));
	memset(&m_PTZInfo, 0, sizeof(CFG_PTZ_INFO));
	memset(&m_RecordInfo, 0, sizeof(CFG_RECORD_INFO));
	memset(&m_AlarmInfo, 0, sizeof(CFG_ALARMIN_INFO));
	memset(&m_MotionInfo, 0, sizeof(CFG_MOTION_INFO));
	memset(&m_BlindInfo, 0, sizeof(CFG_SHELTER_INFO));
	memset(&m_VideoLostInfo, 0, sizeof(CFG_VIDEOLOST_INFO));
	memset(&m_VideoCoveInfo, 0, sizeof(CFG_VIDEO_COVER));
}

void CClientDemo5Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CClientDemo5Dlg)
	DDX_Control(pDX, IDC_COMBO_CONFIGTYPE, m_ctlRevertType);
	DDX_Control(pDX, IDC_TAB_MAIN, m_ctlMainTab);
	DDX_Control(pDX, IDC_IPADDRESS_DVRIP, m_ctlDvrIP);
	DDX_Text(pDX, IDC_EDIT_PORT, m_nPort);
	DDX_Text(pDX, IDC_EDIT_USERNAME, m_strUserName);
	DDX_Text(pDX, IDC_EDIT_DVRPWD, m_strPwd);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CClientDemo5Dlg, CDialog)
	//{{AFX_MSG_MAP(CClientDemo5Dlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_LOGIN, OnBtnLogin)
	ON_BN_CLICKED(IDC_BTN_LOGOUT, OnBtnLogout)
	ON_WM_DESTROY()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_MAIN, OnSelchangeTabMain)
	ON_BN_CLICKED(IDC_BUTTON_REVERT, OnButtonRevert)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CClientDemo5Dlg message handlers

BOOL CClientDemo5Dlg::OnInitDialog()
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
	m_ctlDvrIP.SetAddress(10,6,9,53);
	//Initialize TabControl control
	InitTabControl();
	InitCombo();
	//Initialize SDK
	InitNetSDK();

//	SetTimer(1, 10000, NULL);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CClientDemo5Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CClientDemo5Dlg::OnPaint() 
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
HCURSOR CClientDemo5Dlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}
BOOL CClientDemo5Dlg::AddPage(CString strTitle, CDialog* dlgPage, UINT uID)
{
	if(NULL == dlgPage)
	{
		return FALSE;
	}
	
	DeletePage();

	m_dlgDeviceType = dlgPage;
	
	CRect childRect;
	m_ctlMainTab.GetClientRect(childRect);
	childRect.top += 20;
	childRect.bottom -= 4;
	childRect.left += 4;
	childRect.right -= 4;

	m_dlgDeviceType->Create(uID, &m_ctlMainTab);
	m_dlgDeviceType->MoveWindow(childRect);
	m_ctlMainTab.InsertItem(8, ConvertString(strTitle));

	return TRUE;
}
BOOL CClientDemo5Dlg::DeletePage()
{
	if(NULL != m_dlgDeviceType)
	{
		int nSel = m_ctlMainTab.GetCurSel();
		m_dlgDeviceType->ShowWindow(FALSE);
		m_dlgDeviceType->CloseWindow();
		m_ctlMainTab.DeleteItem(8);
		delete m_dlgDeviceType;
		m_dlgDeviceType = NULL;

		if(nSel == 8)
		{
			m_ctlMainTab.SetCurSel(7);
			DoTab(7);
		}
		else
		{
			DoTab(nSel);
		}
		Invalidate(TRUE);
		return TRUE;
	}

	return FALSE;
}

//Register user to device 
#define TYPEMAP(a,b) case a:return (b)

CClientDemo5Dlg::PAGETYPE CClientDemo5Dlg::FromDeviceType(NET_DEVICE_TYPE eDeviceType)
{
	switch(eDeviceType)
	{
		TYPEMAP(NET_PRODUCT_NONE, NONE);
		TYPEMAP(NET_DVR_NONREALTIME_MACE, NONE);			// 非实时MACE
		TYPEMAP(NET_DVR_NONREALTIME, NONE);					// 非实时
		TYPEMAP(NET_NVS_MPEG1, NONE);						// 网络视频服务器
		TYPEMAP(NET_DVR_MPEG1_2, DVR);						// MPEG1二路录像机
		TYPEMAP(NET_DVR_MPEG1_8, DVR);						// MPEG1八路录像机
		TYPEMAP(NET_DVR_MPEG4_8, DVR);						// MPEG4八路录像机
		TYPEMAP(NET_DVR_MPEG4_16, DVR);					// MPEG4十六路录像机
		TYPEMAP(NET_DVR_MPEG4_SX2, DVR);					// LB系列录像机
		TYPEMAP(NET_DVR_MEPG4_ST2, DVR);					// GB系列录像机
		TYPEMAP(NET_DVR_MEPG4_SH2, DVR);					// HB系列录像机
		TYPEMAP(NET_DVR_MPEG4_GBE, DVR);			    	// GBE系列录像机
		TYPEMAP(NET_DVR_MPEG4_NVSII, DVR);					// II代网络视频服务器
		TYPEMAP(NET_DVR_STD_NEW, DVR);						// 新标准配置协议
		TYPEMAP(NET_DVR_DDNS, DVR);						// DDNS服务器
		TYPEMAP(NET_DVR_ATM, DVR);							// ATM机
		TYPEMAP(NET_NB_SERIAL, NONE);						// 二代非实时NB系列机器
		TYPEMAP(NET_LN_SERIAL, NONE);						// LN系列产品
		TYPEMAP(NET_BAV_SERIAL, NONE);						// BAV系列产品
		TYPEMAP(NET_SDIP_SERIAL, NONE);						// SDIP系列产品
		TYPEMAP(NET_IPC_SERIAL, IPC);						// IPC系列产品
		TYPEMAP(NET_NVS_B, NONE);							// NVS B系列
		TYPEMAP(NET_NVS_C, NONE);							// NVS H系列
		TYPEMAP(NET_NVS_S, NONE);							// NVS S系列
		TYPEMAP(NET_NVS_E, NONE);							// NVS E系列
		TYPEMAP(NET_DVR_NEW_PROTOCOL, DVR);				// 从QueryDevState中查询设备类型, NONE);以字符串格式
		TYPEMAP(NET_NVD_SERIAL, NONE);						// 解码器
		TYPEMAP(NET_DVR_N5, DVR);							// N5
		TYPEMAP(NET_DVR_MIX_DVR, DVR);						// 混合DVR
		TYPEMAP(NET_SVR_SERIAL, NONE);						// SVR系列
		TYPEMAP(NET_SVR_BS, NONE);							// SVR-BS
		default:return NONE;
	}
}
void CClientDemo5Dlg::OnBtnLogin() 
{
	// TODO: Add your control notification handler code here
	BOOL bValid = UpdateData(TRUE);
	if(bValid)
	{
		int err=0;	//Stroage the returned error value
		char *pchDVRIP;
		CString strDvrIP = GetDvrIP();
		pchDVRIP = (LPSTR)(LPCSTR)strDvrIP;
		WORD wDVRPort=(WORD)m_nPort;
		char *pchUserName=(LPSTR)(LPCSTR)m_strUserName;
		char *pchPassword=(LPSTR)(LPCSTR)m_strPwd;
		//Device informaiton
		NET_DEVICEINFO deviceInfo;
		//Call log in interface
		long lRet = CLIENT_Login(pchDVRIP,wDVRPort,pchUserName,pchPassword,&deviceInfo,&err);
		if(0 != lRet)
		{
			m_LoginID = lRet;
			GetDlgItem(IDC_BTN_LOGIN)->EnableWindow(FALSE);
			GetDlgItem(IDC_BTN_LOGOUT)->EnableWindow(TRUE);

			//判断是否为三代协议设备
			int len = 0;
			DH_DEV_ENABLE_INFO stDevEn = {0};
			BOOL bRet = CLIENT_QuerySystemInfo(m_LoginID, ABILITY_DEVALL_INFO, (char*)&stDevEn, sizeof(DH_DEV_ENABLE_INFO), &len);
			if (bRet && len == sizeof(DH_DEV_ENABLE_INFO))
			{
				if (stDevEn.IsFucEnable[EN_JSON_CONFIG]!=0 && deviceInfo.byChanNum > 16)
				{
					m_b3ProDev = TRUE;
				}
				else
				{
					m_b3ProDev = FALSE;
				}
			}
			else
			{
				m_b3ProDev = FALSE;
			}
			
			//Get protocol version number 
			int nProtoVer = 0;
			int nRet = 0;
			BOOL bSuccess = CLIENT_QueryDevState(m_LoginID, DH_DEVSTATE_PROTOCAL_VER, (char*)&nProtoVer, sizeof(int), &nRet, 1000);
			if (bSuccess)
			{
				if (nProtoVer >= 5)		//This device is 2.6 basic line or higher 
				{
					m_nDevProtoVer = nProtoVer;
				}
			}

			//Device property 
			GetDeviceConfigInfo();
			//Video channel property 
			GetChannelConfigInfo();
			//COM property 
			GetCommConfigInfo();
			//Record configuration 
			GetRecordConfigInfo();
			//Network configuration 
			GetNetWorkConfigInfo();
			//Alarm property 
			GetAlarmConfigInfo();
			//Multiple privacy mask zones 
			GetCoverConfigInfo();
			//Strategy configuraiton
			GetStrategyConfigInfo();
			//
			PAGETYPE pageType = FromDeviceType((NET_DEVICE_TYPE)deviceInfo.byDVRType);
			if(pageType == IPC)
			{
				CDlgIPC* dlgIPC = new CDlgIPC;
				this->AddPage("IPC configure", dlgIPC, IDD_DIALOG_IPC);
				dlgIPC->PostMessage(WM_COMMAND, IDC_BUTTON_READ, 0);
				dlgIPC->SetLoginHandle(lRet, m_b3ProDev);
			}
			else if(pageType == DVR)
			{
				CDlgDVR* dlgDVR = new CDlgDVR;
				this->AddPage("DVR configure", dlgDVR, IDD_DIALOG_DVR);
				dlgDVR->PostMessage(WM_COMMAND, IDC_BUTTON_READ, 0);
				dlgDVR->SetLoginHandle(lRet, m_b3ProDev);
			}
		}
		else
		{
			m_LoginID = 0;
			//Display log in failure reason 
			ShowLoginErrorReason(err);
		}
	}
}

//Logout device user 
void CClientDemo5Dlg::OnBtnLogout() 
{
	// TODO: Add your control notification handler code here
	DeletePage();
	BOOL bRet = CLIENT_Logout(m_LoginID);
	//Clear handle as o after logout 
	if(bRet)
	{
		m_LoginID=0;
		GetDlgItem(IDC_BTN_LOGIN)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_LOGOUT)->EnableWindow(FALSE);
		//Device property 
		m_serverDlg.CleanAll();
		//Video channel property 
		m_channelDlg.CleanAll();
		//COM property 
		m_commDlg.CleanAll();
		//Record configuration 
		m_recordDlg.CleanAll();
		//Network configuration 
		m_networkDlg.CleanAll();
		//Alarm property 
		m_alarmDlg.CleanAll();
		//Multiple privacy mask zones 
		m_coverDlg.CleanAll();
		//Configuration strategy 
		m_StrategyDlg.CleanAll();
	}
	else
	{
		MessageBox(ConvertString("Login failed!"), ConvertString("Prompt"));
	}
}

BOOL CALLBACK MessCallBackFunc(LONG lCommand, LONG lLoginID, char *pBuf, DWORD dwBufLen, char *pchDVRIP, LONG nDVRPort, DWORD dwUser)
{
	return TRUE;
}

//Initialize SDK
void CClientDemo5Dlg::InitNetSDK()
{
	BOOL bSuccess = CLIENT_Init(DisConnectFunc, (DWORD)this);
	if (bSuccess)
	{
		CLIENT_SetDVRMessCallBack(MessCallBackFunc, (DWORD)this);
	}
	else
	{
		MessageBox(ConvertString("Initialize SDK failed!"), ConvertString("Prompt"));
	}
}

//Call function when device disconnected
void CALLBACK DisConnectFunc(LONG lLoginID, char *pchDVRIP, LONG nDVRPort, DWORD dwUser)
{
	if(dwUser == 0)
	{
		return;
	}
	CClientDemo5Dlg *dlg = (CClientDemo5Dlg *)dwUser;
	dlg->DeviceDisConnect(lLoginID, pchDVRIP,nDVRPort);
}

//Disposal when device disconnected 
void CClientDemo5Dlg::DeviceDisConnect(LONG lLoginID, char *sDVRIP, LONG nDVRPort)
{
	//Add device disconnection process code 
	MessageBox(ConvertString("Network disconnected!"), ConvertString("Prompt"));
	
}

void CClientDemo5Dlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	// Logout 
	if(0 != m_LoginID)
	{
		DeletePage();
		BOOL bLogout = CLIENT_Logout(m_LoginID);
	}
	//Clear SDK, release resources  
	CLIENT_Cleanup();
}

//Get DVR IP
CString CClientDemo5Dlg::GetDvrIP()
{
	CString strRet="";
	BYTE nField0,nField1,nField2,nField3;
	m_ctlDvrIP.GetAddress(nField0,nField1,nField2,nField3);
	strRet.Format("%d.%d.%d.%d",nField0,nField1,nField2,nField3);
	return strRet;
}

//Display log in failure reason 
void CClientDemo5Dlg::ShowLoginErrorReason(int nError)
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

//Initialize TabControl control
void CClientDemo5Dlg::InitTabControl()
{
	CRect childRect;
	m_ctlMainTab.GetClientRect(childRect);
	childRect.top += 20;
	childRect.bottom -= 4;
	childRect.left += 4;
	childRect.right -= 4;

	//Create non-mode dialogue box. Specified tag is non-mode dialogue box father window.
	m_serverDlg.Create(IDD_CONFIG_SERVER, &m_ctlMainTab);
	m_serverDlg.MoveWindow(childRect);
	m_channelDlg.Create(IDD_CONFIG_CHANNEL, &m_ctlMainTab);
	m_channelDlg.MoveWindow(childRect);
	m_commDlg.Create(IDD_CONFIG_COMM, &m_ctlMainTab);
	m_commDlg.MoveWindow(childRect);
	m_alarmDlg.Create(IDD_CONFIG_ALARM, &m_ctlMainTab);
	m_alarmDlg.MoveWindow(childRect);
	m_recordDlg.Create(IDD_CONFIG_RECORD, &m_ctlMainTab);
	m_recordDlg.MoveWindow(childRect);
	m_networkDlg.Create(IDD_CONFIG_NETWORK, &m_ctlMainTab);
	m_networkDlg.MoveWindow(childRect);
	m_coverDlg.Create(IDD_CONFIG_COVER, &m_ctlMainTab);
	m_coverDlg.MoveWindow(childRect);
	m_StrategyDlg.Create(IDD_CONFIG_STRATEGY,&m_ctlMainTab);
	m_StrategyDlg.MoveWindow(childRect);
	m_configFile.Create(IDD_DIALOG_CONFIG_FILE,&m_ctlMainTab);
	m_configFile.MoveWindow(childRect);
	
	m_ctlMainTab.InsertItem(0, ConvertString("Device Attribute"));
	m_ctlMainTab.InsertItem(1, ConvertString("Image Channel Attribute"));
	m_ctlMainTab.InsertItem(2, ConvertString("COM Attribute"));
	m_ctlMainTab.InsertItem(3, ConvertString("Record Config"));
	m_ctlMainTab.InsertItem(4, ConvertString("Network Config"));
	m_ctlMainTab.InsertItem(5, ConvertString("Alarm Attribute"));
	m_ctlMainTab.InsertItem(6, ConvertString("Multi-area Shelter"));
	m_ctlMainTab.InsertItem(7, ConvertString("strategy"));

	m_ctlMainTab.SetCurSel(0);
	DoTab(0);
}

void CClientDemo5Dlg::DoTab(int nTab)
{
	//Check nTab value is within the threshold
	if(nTab>8)
	{
		nTab=8;
	}
	if(nTab<0)
	{
		nTab=0;
	}
	
	BOOL bTab[9];
	bTab[0]=bTab[1]=bTab[2]=bTab[3]=bTab[4]=bTab[5]=bTab[6]=bTab[7]=bTab[8]=FALSE;
	bTab[nTab]=TRUE;
	
	//Display or hide dialogue box 
	SetDlgState(&m_serverDlg,bTab[0]);
	SetDlgState(&m_channelDlg,bTab[1]);
	SetDlgState(&m_commDlg,bTab[2]);
	SetDlgState(&m_recordDlg,bTab[3]);
	SetDlgState(&m_networkDlg,bTab[4]);
	SetDlgState(&m_alarmDlg,bTab[5]);
	SetDlgState(&m_coverDlg,bTab[6]);
	SetDlgState(&m_StrategyDlg,bTab[7]);
	if(NULL != m_dlgDeviceType)
	{
		SetDlgState(m_dlgDeviceType,bTab[8]);
	}
}

void CClientDemo5Dlg::SetDlgState(CWnd *pWnd, BOOL bShow)
{
	if(bShow)
	{
		pWnd->ShowWindow(SW_SHOW);
	}
	else
	{
		pWnd->ShowWindow(SW_HIDE);
	}
}

void CClientDemo5Dlg::OnSelchangeTabMain(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	int nSelect = m_ctlMainTab.GetCurSel();
	if(nSelect>=0)
	{
		DoTab(nSelect);
	}
	*pResult = 0;
}

//Get device property configuration information 
void CClientDemo5Dlg::GetDeviceConfigInfo()
{
	if(0 != m_LoginID)
	{
		DWORD dwRetLen = 0;
		int nWaitTime = 1000;
		DH_QUICK_QUERY_CFG_EN stQuickQueryEn = {0};
		BOOL bSuccess = CLIENT_QuerySystemInfo(m_LoginID, ABILITY_QUICK_QUERY_CFG, (char *)&stQuickQueryEn, sizeof(DH_QUICK_QUERY_CFG_EN), (int *)&dwRetLen);
		if (bSuccess && dwRetLen == sizeof(DH_QUICK_QUERY_CFG_EN)) 
		{
			if (stQuickQueryEn.IsQuickQueryEnable == 1) 
			{
				nWaitTime = 60000;
			}
		}
		DHDEV_SYSTEM_ATTR_CFG sysConfigInfo = {0};
		bSuccess = CLIENT_GetDevConfig(m_LoginID, DH_DEV_DEVICECFG, 0,
			&sysConfigInfo, sizeof(DHDEV_SYSTEM_ATTR_CFG), &dwRetLen, nWaitTime);
		if (bSuccess && dwRetLen == sizeof(DHDEV_SYSTEM_ATTR_CFG))
		{
			//Initialize dialogue box
			m_serverDlg.InitDlgInfo(&sysConfigInfo);
		}
	}
	else
	{
		MessageBox(ConvertString("Login first!"), ConvertString("Prompt"));
	}
}

//Set device property configuration information 
void CClientDemo5Dlg::SetDeviceConfigInfo(LPDHDEV_SYSTEM_ATTR_CFG sysConfigInfo)
{
	if(0 != m_LoginID && sysConfigInfo != NULL)
	{
		//First read informaiton 
		DWORD dwRetLen = 0;
		int nWaitTime = 1000;
		DH_QUICK_QUERY_CFG_EN stQuickQueryEn = {0};
		BOOL bSuccess = CLIENT_QuerySystemInfo(m_LoginID, ABILITY_QUICK_QUERY_CFG, (char *)&stQuickQueryEn, sizeof(DH_QUICK_QUERY_CFG_EN), (int *)&dwRetLen);
		if (bSuccess && dwRetLen == sizeof(DH_QUICK_QUERY_CFG_EN)) 
		{
			if (stQuickQueryEn.IsQuickQueryEnable == 1) 
			{
				nWaitTime = 60000;
			}
		}
		DHDEV_SYSTEM_ATTR_CFG sysDevInfo = {0};
		bSuccess = CLIENT_GetDevConfig(m_LoginID, DH_DEV_DEVICECFG, 0,
			&sysDevInfo, sizeof(DHDEV_SYSTEM_ATTR_CFG), &dwRetLen, nWaitTime);
		if (bSuccess && dwRetLen == sizeof(DHDEV_SYSTEM_ATTR_CFG))
		{
			//Modify information 
			sysDevInfo.wDevNo = sysConfigInfo->wDevNo;
			sysDevInfo.byOverWrite = sysConfigInfo->byOverWrite;
			sysDevInfo.byRecordLen = sysConfigInfo->byRecordLen;
			sysDevInfo.byVideoStandard = sysConfigInfo->byVideoStandard;
			sysDevInfo.byDateFormat = sysConfigInfo->byDateFormat;
			sysDevInfo.byDateSprtr = sysConfigInfo->byDateSprtr;
			sysDevInfo.byTimeFmt = sysConfigInfo->byTimeFmt;

			//Save information 
			BOOL bSuccess = CLIENT_SetDevConfig(m_LoginID, DH_DEV_DEVICECFG, 0,
				&sysDevInfo, sizeof(DHDEV_SYSTEM_ATTR_CFG));
			if (bSuccess)
			{
				MessageBox(ConvertString("Save config info successfully!"), ConvertString("Prompt"));
			}
			else
			{
				MessageBox(ConvertString("Save config info failed!"), ConvertString("Prompt"));
			}
		}
	}
	else
	{
		MessageBox(ConvertString("Login first!"), ConvertString("Prompt"));
	}
}

//Get video channel property 
void CClientDemo5Dlg::GetChannelConfigInfo()
{
	if(0 != m_LoginID)
	{
		DWORD dwRetLen = 0;
		int nWaitTime = 1000;
		DH_QUICK_QUERY_CFG_EN stQuickQueryEn = {0};
		BOOL bSuccess = CLIENT_QuerySystemInfo(m_LoginID, ABILITY_QUICK_QUERY_CFG, (char *)&stQuickQueryEn, sizeof(DH_QUICK_QUERY_CFG_EN), (int *)&dwRetLen);
		if (bSuccess && dwRetLen == sizeof(DH_QUICK_QUERY_CFG_EN)) 
		{
			if (stQuickQueryEn.IsQuickQueryEnable == 1) 
			{
				nWaitTime = 60000;
			}
		}
		DHDEV_SYSTEM_ATTR_CFG sysConfigInfo = {0};
		bSuccess = CLIENT_GetDevConfig(m_LoginID, DH_DEV_DEVICECFG, 0,
			&sysConfigInfo, sizeof(DHDEV_SYSTEM_ATTR_CFG), &dwRetLen, nWaitTime);
		if (bSuccess && dwRetLen == sizeof(DHDEV_SYSTEM_ATTR_CFG))
		{
			BYTE nChannelCount = sysConfigInfo.byVideoCaptureNum;
			m_nVideoStandard = sysConfigInfo.byVideoStandard;

			//Video channel property 
			DHDEV_CHANNEL_CFG *pChannelInfo = new DHDEV_CHANNEL_CFG[nChannelCount];
			memset(pChannelInfo, 0, nChannelCount*sizeof(DHDEV_CHANNEL_CFG));
			if(m_b3ProDev == FALSE)
			{
				bSuccess = CLIENT_GetDevConfig(m_LoginID, DH_DEV_CHANNELCFG, -1,
					pChannelInfo, nChannelCount * sizeof(DHDEV_CHANNEL_CFG), &dwRetLen);
				if (bSuccess && dwRetLen == nChannelCount * sizeof(DHDEV_CHANNEL_CFG))
				{
					//Read DSP information
					DHDEV_DSP_ENCODECAP stDspInfo = {0};
					bSuccess = CLIENT_QueryDevState(m_LoginID, DH_DEVSTATE_DSP, (char*)&stDspInfo, sizeof(DHDEV_DSP_ENCODECAP), (int*)&dwRetLen, 2000);
					if (bSuccess && dwRetLen == sizeof(DHDEV_DSP_ENCODECAP))
					{
						//Initialize dialogue box 
						m_channelDlg.InitDlgInfo(pChannelInfo, nChannelCount, sysConfigInfo.byVideoStandard, stDspInfo, m_nDevProtoVer, m_b3ProDev);
					}
				}
			}
			else
			{
				//Read DSP information
				DHDEV_DSP_ENCODECAP stDspInfo = {0};
				bSuccess = CLIENT_QueryDevState(m_LoginID, DH_DEVSTATE_DSP, (char*)&stDspInfo, sizeof(DHDEV_DSP_ENCODECAP), (int*)&dwRetLen, 2000);
				if (bSuccess && dwRetLen == sizeof(DHDEV_DSP_ENCODECAP))
				{
					//Initialize dialogue box 
					m_channelDlg.InitDlgInfo(pChannelInfo, nChannelCount, sysConfigInfo.byVideoStandard, stDspInfo, m_nDevProtoVer, m_b3ProDev);
				}
			}
			delete[] pChannelInfo;
		}
	}
	else
	{
		MessageBox(ConvertString("Login first!"), ConvertString("Prompt"));
	}
}

void CClientDemo5Dlg::GetOneChannelConfigInfo(DHDEV_CHANNEL_CFG *pInfo, int nCurChannel)
{
	if(0 != m_LoginID)
	{
		if (pInfo == NULL)
		{
			return;
		}
		
		char *szOutBuffer = new char[32*1024];
		if (szOutBuffer == NULL)
		{
			return;	
		}
		if (nCurChannel < 0 || nCurChannel >= 32)
		{
			return;
		}
		
		int i = nCurChannel;
		int nerror = 0;
		
		BOOL bSuccess = CLIENT_GetNewDevConfig(m_LoginID, CFG_CMD_ENCODE, i, szOutBuffer, 32*1024, &nerror, 3000);		
		if (bSuccess)
		{
			memset(pInfo, 0, sizeof(DHDEV_CHANNEL_CFG));
			memset(&m_stuEncodeInfo, 0, sizeof(CFG_ENCODE_INFO));
			int nRetLen = 0;
			
			//解析
			BOOL bRet = CLIENT_ParseData(CFG_CMD_ENCODE, (char *)szOutBuffer, &m_stuEncodeInfo, sizeof(CFG_ENCODE_INFO), &nRetLen);
			if (bRet)
			{
				
				char Name[64] = {0} ;
				Change_Utf8_Assic((unsigned char *)m_stuEncodeInfo.szChnName, Name);
				strcpy(pInfo->szChannelName, Name);	
				
				int k = 0;
				for (k = 0; k < 3; k++)
				{	
					//主码流
					pInfo->stMainVideoEncOpt[k].byAudioEnable = m_stuEncodeInfo.stuMainStream[k].bAudioEnable;	
					pInfo->stMainVideoEncOpt[k].byVideoEnable = m_stuEncodeInfo.stuMainStream[k].bVideoEnable;
					pInfo->stMainVideoEncOpt[k].byEncodeMode = m_stuEncodeInfo.stuMainStream[k].stuVideoFormat.emCompression;	
					pInfo->stMainVideoEncOpt[k].byImageSize = (BYTE)ResolutionInttoSize(m_stuEncodeInfo.stuMainStream[k].stuVideoFormat.nWidth, m_stuEncodeInfo.stuMainStream[k].stuVideoFormat.nHeight);
					
					pInfo->stMainVideoEncOpt[k].byBitRateControl = m_stuEncodeInfo.stuMainStream[k].stuVideoFormat.emBitRateControl;
					pInfo->stMainVideoEncOpt[k].wLimitStream = m_stuEncodeInfo.stuMainStream[k].stuVideoFormat.nBitRate;
					pInfo->stMainVideoEncOpt[k].byFramesPerSec = m_stuEncodeInfo.stuMainStream[k].stuVideoFormat.nFrameRate;
					pInfo->stMainVideoEncOpt[k].bIFrameInterval = m_stuEncodeInfo.stuMainStream[k].stuVideoFormat.nIFrameInterval;
					pInfo->stMainVideoEncOpt[k].byImageQlty = m_stuEncodeInfo.stuMainStream[k].stuVideoFormat.emImageQuality;
					
					//辅码流
					pInfo->stAssiVideoEncOpt[k].byAudioEnable = m_stuEncodeInfo.stuExtraStream[k].bAudioEnable;	
					pInfo->stAssiVideoEncOpt[k].byVideoEnable = m_stuEncodeInfo.stuExtraStream[k].bVideoEnable;
					pInfo->stAssiVideoEncOpt[k].byEncodeMode = m_stuEncodeInfo.stuExtraStream[k].stuVideoFormat.emCompression;	
					pInfo->stAssiVideoEncOpt[k].byImageSize = (BYTE)ResolutionInttoSize(m_stuEncodeInfo.stuExtraStream[k].stuVideoFormat.nWidth, m_stuEncodeInfo.stuExtraStream[k].stuVideoFormat.nHeight);
					
					pInfo->stAssiVideoEncOpt[k].byBitRateControl = m_stuEncodeInfo.stuExtraStream[k].stuVideoFormat.emBitRateControl;
					pInfo->stAssiVideoEncOpt[k].wLimitStream = m_stuEncodeInfo.stuExtraStream[k].stuVideoFormat.nBitRate;
					pInfo->stAssiVideoEncOpt[k].byFramesPerSec = m_stuEncodeInfo.stuExtraStream[k].stuVideoFormat.nFrameRate;
					pInfo->stAssiVideoEncOpt[k].bIFrameInterval = m_stuEncodeInfo.stuExtraStream[k].stuVideoFormat.nIFrameInterval;
					pInfo->stAssiVideoEncOpt[k].byImageQlty = m_stuEncodeInfo.stuExtraStream[k].stuVideoFormat.emImageQuality;
				}
				
				//ChannelOSD
				pInfo->stChannelOSD.rgbaFrontground |= m_stuEncodeInfo.stuChnTitle.stuFrontColor.nRed;
				pInfo->stChannelOSD.rgbaFrontground <<= 8;
				pInfo->stChannelOSD.rgbaFrontground |= m_stuEncodeInfo.stuChnTitle.stuFrontColor.nGreen;	
				pInfo->stChannelOSD.rgbaFrontground <<= 8;
				pInfo->stChannelOSD.rgbaFrontground |= m_stuEncodeInfo.stuChnTitle.stuFrontColor.nBlue;	
				pInfo->stChannelOSD.rgbaFrontground <<= 8;
				pInfo->stChannelOSD.rgbaFrontground |= m_stuEncodeInfo.stuChnTitle.stuFrontColor.nAlpha;
				
				pInfo->stChannelOSD.rgbaBackground |= m_stuEncodeInfo.stuChnTitle.stuBackColor.nRed;	
				pInfo->stChannelOSD.rgbaBackground <<= 8;
				pInfo->stChannelOSD.rgbaBackground |= m_stuEncodeInfo.stuChnTitle.stuBackColor.nGreen;	
				pInfo->stChannelOSD.rgbaBackground <<= 8;
				pInfo->stChannelOSD.rgbaBackground |= m_stuEncodeInfo.stuChnTitle.stuBackColor.nBlue;	
				pInfo->stChannelOSD.rgbaBackground <<= 8;
				pInfo->stChannelOSD.rgbaBackground |= m_stuEncodeInfo.stuChnTitle.stuBackColor.nAlpha;
				
				pInfo->stChannelOSD.rcRect.left = m_stuEncodeInfo.stuChnTitle.stuRect.nLeft;
				pInfo->stChannelOSD.rcRect.top = m_stuEncodeInfo.stuChnTitle.stuRect.nTop;
				pInfo->stChannelOSD.rcRect.right = m_stuEncodeInfo.stuChnTitle.stuRect.nRight;
				pInfo->stChannelOSD.rcRect.bottom = m_stuEncodeInfo.stuChnTitle.stuRect.nBottom;
				pInfo->stChannelOSD.bShow = m_stuEncodeInfo.stuChnTitle.bShowEnable;
				
				//TimeOSD
				pInfo->stTimeOSD.rgbaFrontground |= m_stuEncodeInfo.stuTimeTitle.stuFrontColor.nRed;
				pInfo->stTimeOSD.rgbaFrontground <<= 8;
				pInfo->stTimeOSD.rgbaFrontground |= m_stuEncodeInfo.stuTimeTitle.stuFrontColor.nGreen;	
				pInfo->stTimeOSD.rgbaFrontground <<= 8;
				pInfo->stTimeOSD.rgbaFrontground |= m_stuEncodeInfo.stuTimeTitle.stuFrontColor.nBlue;	
				pInfo->stTimeOSD.rgbaFrontground <<= 8;
				pInfo->stTimeOSD.rgbaFrontground |= m_stuEncodeInfo.stuTimeTitle.stuFrontColor.nAlpha;
				
				pInfo->stTimeOSD.rgbaBackground |= m_stuEncodeInfo.stuTimeTitle.stuBackColor.nRed;	
				pInfo->stTimeOSD.rgbaBackground <<= 8;
				pInfo->stTimeOSD.rgbaBackground |= m_stuEncodeInfo.stuTimeTitle.stuBackColor.nGreen;	
				pInfo->stTimeOSD.rgbaBackground <<= 8;
				pInfo->stTimeOSD.rgbaBackground |= m_stuEncodeInfo.stuTimeTitle.stuBackColor.nBlue;	
				pInfo->stTimeOSD.rgbaBackground <<= 8;
				pInfo->stTimeOSD.rgbaBackground |= m_stuEncodeInfo.stuTimeTitle.stuBackColor.nAlpha;
				
				pInfo->stTimeOSD.rcRect.left = m_stuEncodeInfo.stuTimeTitle.stuRect.nLeft;
				pInfo->stTimeOSD.rcRect.top = m_stuEncodeInfo.stuTimeTitle.stuRect.nTop;
				pInfo->stTimeOSD.rcRect.right = m_stuEncodeInfo.stuTimeTitle.stuRect.nRight;
				pInfo->stTimeOSD.rcRect.bottom = m_stuEncodeInfo.stuTimeTitle.stuRect.nBottom;
				pInfo->stTimeOSD.bShow = m_stuEncodeInfo.stuTimeTitle.bShowEnable;
				
				//color
				pInfo->stColorCfg[0].byBrightness = m_stuEncodeInfo.stuVideoColor.nBrightness;
				pInfo->stColorCfg[0].byContrast = m_stuEncodeInfo.stuVideoColor.nContrast;
				pInfo->stColorCfg[0].bySaturation = m_stuEncodeInfo.stuVideoColor.nSaturation;
				pInfo->stColorCfg[0].byHue = m_stuEncodeInfo.stuVideoColor.nHue;
				pInfo->stColorCfg[0].byGain = m_stuEncodeInfo.stuVideoColor.nGain;
				pInfo->stColorCfg[0].byGainEn = m_stuEncodeInfo.stuVideoColor.bGainEn;
			}
			else
			{
				MessageBox(ConvertString("Parse data error"), ConvertString("Prompt"));
			}
		}
		else
		{
			MessageBox(ConvertString("Get encode data failed!"), ConvertString("Prompt"));

		}
		if (szOutBuffer)
		{
			delete []szOutBuffer;
			szOutBuffer = NULL;
		}
	}
	else
	{
		MessageBox(ConvertString("Login first!"), ConvertString("Prompt"));
	}	

}

//Save video channel property configuration information 
void CClientDemo5Dlg::SetChannelConfigInfo(DHDEV_CHANNEL_CFG *pChannelInfo, int nLength)
{
	if(0 != m_LoginID && pChannelInfo != NULL)
	{
		//Save information
		BOOL bSuccess = FALSE;
		if(m_b3ProDev == FALSE)
		{
			bSuccess = CLIENT_SetDevConfig(m_LoginID, DH_DEV_CHANNELCFG, -1,
				pChannelInfo, nLength*sizeof(DHDEV_CHANNEL_CFG));
		}
		if (bSuccess)
		{
			MessageBox(ConvertString("Save config info successfully!"), ConvertString("Prompt"));
		}
		else
		{
			MessageBox(ConvertString("Save config info failed!"), ConvertString("Prompt"));
		}
	}
	else
	{
		MessageBox(ConvertString("Login first!"), ConvertString("Prompt"));
	}
}

void CClientDemo5Dlg::SetOneChannelConfigInfo(DHDEV_CHANNEL_CFG *pInfo, int nCurChannel)
{
	if(0 != m_LoginID && pInfo != NULL)
	{	
		int nerror = 0;
		int nFailCount = 0;
		int nrestart = 0;
		int i = nCurChannel;
		DWORD deTime = GetTickCount();

		char *szOutBuffer = new char[32*1024];
		if (szOutBuffer == NULL)
		{
			return;	
		}
		
		//组包
		m_stuEncodeInfo.nChannelID = nCurChannel;
		memset(m_stuEncodeInfo.szChnName, 0, MAX_CHANNELNAME_LEN);
		Change_Assic_UTF8(pInfo->szChannelName, strlen(pInfo->szChannelName), m_stuEncodeInfo.szChnName, MAX_CHANNELNAME_LEN);
		
		
		int k = 0;
		for (k = 0; k < 3; k++)
		{	
			//主码流
			m_stuEncodeInfo.stuMainStream[k].bAudioEnable = pInfo->stMainVideoEncOpt[k].byAudioEnable;	
			m_stuEncodeInfo.stuMainStream[k].bVideoEnable = pInfo->stMainVideoEncOpt[k].byVideoEnable;
			m_stuEncodeInfo.stuMainStream[k].stuVideoFormat.emCompression = (CFG_VIDEO_COMPRESSION)pInfo->stMainVideoEncOpt[k].byEncodeMode;
			ResolutionSizetoInt(pInfo->stMainVideoEncOpt[k].byImageSize, &m_stuEncodeInfo.stuMainStream[k].stuVideoFormat.nWidth, &m_stuEncodeInfo.stuMainStream[k].stuVideoFormat.nHeight);
			
			m_stuEncodeInfo.stuMainStream[k].stuVideoFormat.emBitRateControl = (CFG_BITRATE_CONTROL)pInfo->stMainVideoEncOpt[k].byBitRateControl;
			m_stuEncodeInfo.stuMainStream[k].stuVideoFormat.nBitRate = pInfo->stMainVideoEncOpt[k].wLimitStream;
			m_stuEncodeInfo.stuMainStream[k].stuVideoFormat.nFrameRate = pInfo->stMainVideoEncOpt[k].byFramesPerSec;
			m_stuEncodeInfo.stuMainStream[k].stuVideoFormat.nIFrameInterval = pInfo->stMainVideoEncOpt[k].bIFrameInterval;
			m_stuEncodeInfo.stuMainStream[k].stuVideoFormat.emImageQuality = (CFG_IMAGE_QUALITY)pInfo->stMainVideoEncOpt[k].byImageQlty;
			
			//辅码流
			m_stuEncodeInfo.stuExtraStream[k].bAudioEnable = pInfo->stAssiVideoEncOpt[k].byAudioEnable;	
			m_stuEncodeInfo.stuExtraStream[k].bVideoEnable = pInfo->stAssiVideoEncOpt[k].byVideoEnable;
			m_stuEncodeInfo.stuExtraStream[k].stuVideoFormat.emCompression = (CFG_VIDEO_COMPRESSION)pInfo->stAssiVideoEncOpt[k].byEncodeMode;	
			ResolutionSizetoInt(pInfo->stAssiVideoEncOpt[k].byImageSize, &m_stuEncodeInfo.stuExtraStream[k].stuVideoFormat.nWidth, &m_stuEncodeInfo.stuExtraStream[k].stuVideoFormat.nHeight);
			
			m_stuEncodeInfo.stuExtraStream[k].stuVideoFormat.emBitRateControl = (CFG_BITRATE_CONTROL)pInfo->stAssiVideoEncOpt[k].byBitRateControl;
			m_stuEncodeInfo.stuExtraStream[k].stuVideoFormat.nBitRate = pInfo->stAssiVideoEncOpt[k].wLimitStream;
			m_stuEncodeInfo.stuExtraStream[k].stuVideoFormat.nFrameRate = pInfo->stAssiVideoEncOpt[k].byFramesPerSec;
			m_stuEncodeInfo.stuExtraStream[k].stuVideoFormat.nIFrameInterval = pInfo->stAssiVideoEncOpt[k].bIFrameInterval;
			m_stuEncodeInfo.stuExtraStream[k].stuVideoFormat.emImageQuality = (CFG_IMAGE_QUALITY)pInfo->stAssiVideoEncOpt[k].byImageQlty;
		}
		
		//ChannelOSD
		m_stuEncodeInfo.stuChnTitle.stuFrontColor.nRed = (pInfo->stChannelOSD.rgbaFrontground>>24)&0xFF;
		m_stuEncodeInfo.stuChnTitle.stuFrontColor.nGreen = (pInfo->stChannelOSD.rgbaFrontground>>16)&0xFF;	
		m_stuEncodeInfo.stuChnTitle.stuFrontColor.nBlue = (pInfo->stChannelOSD.rgbaFrontground>>8)&0xFF;	
		m_stuEncodeInfo.stuChnTitle.stuFrontColor.nAlpha = pInfo->stChannelOSD.rgbaFrontground&0xFF;
		
		m_stuEncodeInfo.stuChnTitle.stuBackColor.nRed = (pInfo->stChannelOSD.rgbaBackground>>24)&0xFF;
		m_stuEncodeInfo.stuChnTitle.stuBackColor.nGreen = (pInfo->stChannelOSD.rgbaBackground>>16)&0xFF;	
		m_stuEncodeInfo.stuChnTitle.stuBackColor.nBlue = (pInfo->stChannelOSD.rgbaBackground>>8)&0xFF;	
		m_stuEncodeInfo.stuChnTitle.stuBackColor.nAlpha = pInfo->stChannelOSD.rgbaBackground&0xFF;
		
		m_stuEncodeInfo.stuChnTitle.stuRect.nLeft = pInfo->stChannelOSD.rcRect.left;
		m_stuEncodeInfo.stuChnTitle.stuRect.nTop = pInfo->stChannelOSD.rcRect.top;
		m_stuEncodeInfo.stuChnTitle.stuRect.nRight = pInfo->stChannelOSD.rcRect.right;
		m_stuEncodeInfo.stuChnTitle.stuRect.nBottom = pInfo->stChannelOSD.rcRect.bottom;
		m_stuEncodeInfo.stuChnTitle.bShowEnable = pInfo->stChannelOSD.bShow;
		
		//TimeOSD
		m_stuEncodeInfo.stuTimeTitle.stuFrontColor.nRed = (pInfo->stTimeOSD.rgbaFrontground>>24)&0xFF;
		m_stuEncodeInfo.stuTimeTitle.stuFrontColor.nGreen = (pInfo->stTimeOSD.rgbaFrontground>>16)&0xFF;	
		m_stuEncodeInfo.stuTimeTitle.stuFrontColor.nBlue = (pInfo->stTimeOSD.rgbaFrontground>>8)&0xFF;	
		m_stuEncodeInfo.stuTimeTitle.stuFrontColor.nAlpha = pInfo->stTimeOSD.rgbaFrontground&0xFF;
		
		m_stuEncodeInfo.stuTimeTitle.stuBackColor.nRed = (pInfo->stTimeOSD.rgbaBackground>>24)&0xFF;
		m_stuEncodeInfo.stuTimeTitle.stuBackColor.nGreen = (pInfo->stTimeOSD.rgbaBackground>>16)&0xFF;	
		m_stuEncodeInfo.stuTimeTitle.stuBackColor.nBlue = (pInfo->stTimeOSD.rgbaBackground>>8)&0xFF;	
		m_stuEncodeInfo.stuTimeTitle.stuBackColor.nAlpha = pInfo->stTimeOSD.rgbaBackground&0xFF;
		
		m_stuEncodeInfo.stuTimeTitle.stuRect.nLeft = pInfo->stTimeOSD.rcRect.left;
		m_stuEncodeInfo.stuTimeTitle.stuRect.nTop = pInfo->stTimeOSD.rcRect.top;
		m_stuEncodeInfo.stuTimeTitle.stuRect.nRight = pInfo->stTimeOSD.rcRect.right;
		m_stuEncodeInfo.stuTimeTitle.stuRect.nBottom = pInfo->stTimeOSD.rcRect.bottom;
		m_stuEncodeInfo.stuTimeTitle.bShowEnable = pInfo->stTimeOSD.bShow;
		
		//color
		m_stuEncodeInfo.stuVideoColor.nBrightness = pInfo->stColorCfg[0].byBrightness;
		m_stuEncodeInfo.stuVideoColor.nContrast = pInfo->stColorCfg[0].byContrast;
		m_stuEncodeInfo.stuVideoColor.nSaturation = pInfo->stColorCfg[0].bySaturation;
		m_stuEncodeInfo.stuVideoColor.nHue = pInfo->stColorCfg[0].byHue;
		m_stuEncodeInfo.stuVideoColor.nGain = pInfo->stColorCfg[0].byGain;
		m_stuEncodeInfo.stuVideoColor.bGainEn = pInfo->stColorCfg[0].byGainEn;

		BOOL bRet = CLIENT_PacketData(CFG_CMD_ENCODE, (char *)&m_stuEncodeInfo, sizeof(CFG_ENCODE_INFO), szOutBuffer, 32*1024);
		if (bRet == FALSE)
		{
			MessageBox(ConvertString("Pack data failed!"), ConvertString("Prompt"));
		}
		else
		{
			BOOL bSuccess = CLIENT_SetNewDevConfig(m_LoginID, CFG_CMD_ENCODE, i, szOutBuffer, 32*1024, &nerror, &nrestart, 3000);		
			if (bSuccess)
			{
				if (nrestart == 1)
				{
					MessageBox(ConvertString("Save config info successfully!devide need restart!"), ConvertString("Prompt"));
				}
			}
			else
			{
				MessageBox(ConvertString("Save config info failed!"), ConvertString("Prompt"));
			}		
		}

		if (szOutBuffer)
		{
			delete []szOutBuffer;
			szOutBuffer = NULL;
		}	
	}
	else
	{
		MessageBox(ConvertString("Login first!"), ConvertString("Prompt"));
	}
}
//Get COM property configuration information
void CClientDemo5Dlg::GetCommConfigInfo()
{
	if(0 != m_LoginID)
	{
		DWORD dwRetLen = 0;
		int nWaitTime = 1000;
		DH_QUICK_QUERY_CFG_EN stQuickQueryEn = {0};
		BOOL bSuccess = CLIENT_QuerySystemInfo(m_LoginID, ABILITY_QUICK_QUERY_CFG, (char *)&stQuickQueryEn, sizeof(DH_QUICK_QUERY_CFG_EN), (int *)&dwRetLen);
		if (bSuccess && dwRetLen == sizeof(DH_QUICK_QUERY_CFG_EN)) 
		{
			if (stQuickQueryEn.IsQuickQueryEnable == 1) 
			{
				nWaitTime = 60000;
			}
		}
		
		DHDEV_SYSTEM_ATTR_CFG sysConfigInfo = {0};
		bSuccess = CLIENT_GetDevConfig(m_LoginID, DH_DEV_DEVICECFG, 0,
			&sysConfigInfo, sizeof(DHDEV_SYSTEM_ATTR_CFG), &dwRetLen, nWaitTime);
		if (bSuccess && dwRetLen == sizeof(DHDEV_SYSTEM_ATTR_CFG))
		{
			DHDEV_COMM_CFG_EX CommExInfo = {0};
			//COM amount
			BYTE nCommCount = sysConfigInfo.byComIONum;
			//Channel amount
			BYTE nChannelCount = sysConfigInfo.byVideoCaptureNum;
			
			//COM property
			DHDEV_COMM_CFG CommInfo = {0};
			bSuccess = CLIENT_GetDevConfig(m_LoginID, DH_DEV_COMMCFG, 0,
				&CommInfo, sizeof(DHDEV_COMM_CFG), &dwRetLen);
			if (bSuccess && dwRetLen == sizeof(DHDEV_COMM_CFG))
			{
				int i = 0;
				for (i = 0; i < DH_MAX_DECPRO_LIST_SIZE; i++)
				{
					strcpy((char *)&CommExInfo.DecProName[i], (char *)&CommInfo.DecProName[i]);
				}
				for (i = 0; i < DH_MAX_232FUNCS; i++)
				{
					strcpy((char *)&CommExInfo.s232FuncName[i], (char *)&CommInfo.s232FuncName[i]);
				}
				
				CommExInfo.dw232FuncNameNum = CommInfo.dw232FuncNameNum;
				CommExInfo.dwDecProListNum = CommInfo.dwDecProListNum;
				memcpy(CommExInfo.st232, CommInfo.st232, DH_MAX_232_NUM*sizeof(DH_RS232_CFG));
				memcpy(CommExInfo.stDecoder, CommInfo.stDecoder, DH_MAX_DECODER_NUM*sizeof(DH_485_CFG));			
			}
			
			if (m_b3ProDev == TRUE)
			{
				int nerror = 0;
				int nRetLen = 0;
				char *szOutBuffer = new char[32*1024];
				if (szOutBuffer == NULL)
				{
					return;	
				}
				for (int i = 0; i < nChannelCount; i++)
				{
					BOOL bSuccess = CLIENT_GetNewDevConfig(m_LoginID, CFG_CMD_PTZ, i, szOutBuffer, 32*1024, &nerror, 3000);		
					if (bSuccess)
					{
						//解析
						BOOL bRet = CLIENT_ParseData(CFG_CMD_PTZ, (char *)szOutBuffer, &m_PTZInfo, sizeof(CFG_PTZ_INFO), &nRetLen);
						if (bRet)
						{
							CommExInfo.stDecoder[i].wProtocol = m_PTZInfo.nProName;
							CommExInfo.stDecoder[i].wDecoderAddress = m_PTZInfo.nDecoderAddress;	
							CommExInfo.stDecoder[i].bPTZType = m_PTZInfo.nPTZType;	
							CommExInfo.stDecoder[i].byMartixID = m_PTZInfo.nMartixID;	
							CommExInfo.stDecoder[i].struComm.byBaudRate = m_PTZInfo.struComm.byBaudRate;	
							CommExInfo.stDecoder[i].struComm.byDataBit = m_PTZInfo.struComm.byDataBit;	
							CommExInfo.stDecoder[i].struComm.byStopBit = m_PTZInfo.struComm.byStopBit;	
							CommExInfo.stDecoder[i].struComm.byParity = m_PTZInfo.struComm.byParity;
						}
						else
						{
							MessageBox(ConvertString("Parse data failed!"), ConvertString("Prompt"));
						}
					}
					else
					{
						MessageBox(ConvertString("GetConfig failed(Comm)!"), ConvertString("Prompt"));	
					}
				}	
				
				if (szOutBuffer)
				{
					delete []szOutBuffer;
					szOutBuffer = NULL;
				}
			}
			
			//Initialize dialogue box
			m_commDlg.InitDlgInfo(&CommExInfo,nCommCount,nChannelCount);
		}
	}
	else
	{
		MessageBox(ConvertString("Login first!"), ConvertString("Prompt"));
	}
}

//Save COM property configuration information 
void CClientDemo5Dlg::SetCommConfigInfo(DHDEV_COMM_CFG_EX *pCommInfo, int nCurChannel)
{
	if(0 != m_LoginID && pCommInfo != NULL)
	{
		DHDEV_COMM_CFG CommInfo = {0};
		int i = 0;
		for (i = 0; i < DH_MAX_DECPRO_LIST_SIZE; i++)
		{
			strcpy((char *)&CommInfo.DecProName[i], (char *)&pCommInfo->DecProName[i]);
		}
		for (i = 0; i < DH_MAX_232FUNCS; i++)
		{
			strcpy((char *)&CommInfo.s232FuncName[0], (char *)&pCommInfo->s232FuncName[0]);
		}
		
		CommInfo.dw232FuncNameNum = pCommInfo->dw232FuncNameNum;
		CommInfo.dwDecProListNum = pCommInfo->dwDecProListNum;
		memcpy(CommInfo.st232, pCommInfo->st232, DH_MAX_232_NUM*sizeof(DH_RS232_CFG));
		memcpy(CommInfo.stDecoder, pCommInfo->stDecoder, DH_MAX_DECODER_NUM*sizeof(DH_485_CFG));
		
		//Save information 
		BOOL bSuccess = CLIENT_SetDevConfig(m_LoginID, DH_DEV_COMMCFG, 0,
			&CommInfo, sizeof(DHDEV_COMM_CFG));
		if (bSuccess)
		{
			MessageBox(ConvertString("Save config info successfully!"), ConvertString("Prompt"));
		}
		else
		{
			MessageBox(ConvertString("Save config info failed!"), ConvertString("Prompt"));
		}
		
		if (m_b3ProDev == TRUE)
		{
			int nerror = 0;
			int nrestart = 0;
			int i = nCurChannel;

			char *szOutBuffer = new char[32*1024];
			if (szOutBuffer == NULL)
			{
				return;	
			}
			
			//	for (int i = 0; i < 32; i++)
			{
				//组包

				m_PTZInfo.nChannelID = nCurChannel;
				m_PTZInfo.nProName = pCommInfo->stDecoder[nCurChannel].wProtocol;
				m_PTZInfo.nDecoderAddress = pCommInfo->stDecoder[nCurChannel].wDecoderAddress;	
				m_PTZInfo.nPTZType = pCommInfo->stDecoder[nCurChannel].bPTZType;	
				m_PTZInfo.nMartixID = pCommInfo->stDecoder[nCurChannel].byMartixID;	
				m_PTZInfo.struComm.byBaudRate = pCommInfo->stDecoder[nCurChannel].struComm.byBaudRate;	
				m_PTZInfo.struComm.byDataBit = pCommInfo->stDecoder[nCurChannel].struComm.byDataBit;	
				m_PTZInfo.struComm.byStopBit = pCommInfo->stDecoder[nCurChannel].struComm.byStopBit;	
				m_PTZInfo.struComm.byParity = pCommInfo->stDecoder[nCurChannel].struComm.byParity;

				BOOL bRet = CLIENT_PacketData(CFG_CMD_PTZ, (char *)&m_PTZInfo, sizeof(CFG_PTZ_INFO), szOutBuffer, 32*1024);
				if (bRet == FALSE)
				{
					MessageBox(ConvertString("Pack data error"), ConvertString("Prompt"));
				}
				else
				{
					BOOL bSuccess = CLIENT_SetNewDevConfig(m_LoginID, CFG_CMD_PTZ, i, szOutBuffer, 32*1024, &nerror, &nrestart, 3000);		
					if (bSuccess)
					{
						if (nrestart == 1)
						{
							MessageBox(ConvertString("Save config info successfully!devide need restart!"), ConvertString("Prompt"));
						}
					}
					else
					{
						MessageBox(ConvertString("SetConfig failed"), ConvertString("Prompt"));	
					}
					
				}

				if (szOutBuffer)
				{
					delete []szOutBuffer;
					szOutBuffer = NULL;
				}	
			}
		}
	}
	else
	{
		MessageBox(ConvertString("Login first!"), ConvertString("Prompt"));
	}
}

//Get record configuration information 
void CClientDemo5Dlg::GetRecordConfigInfo()
{
	if(0 != m_LoginID)
	{
		DWORD dwRetLen = 0;
		int nWaitTime = 1000;
		DH_QUICK_QUERY_CFG_EN stQuickQueryEn = {0};
		BOOL bSuccess = CLIENT_QuerySystemInfo(m_LoginID, ABILITY_QUICK_QUERY_CFG, (char *)&stQuickQueryEn, sizeof(DH_QUICK_QUERY_CFG_EN), (int *)&dwRetLen);
		if (bSuccess && dwRetLen == sizeof(DH_QUICK_QUERY_CFG_EN)) 
		{
			if (stQuickQueryEn.IsQuickQueryEnable == 1) 
			{
				nWaitTime = 60000;
			}
		}
		DHDEV_SYSTEM_ATTR_CFG sysConfigInfo = {0};
		bSuccess = CLIENT_GetDevConfig(m_LoginID, DH_DEV_DEVICECFG, 0,
			&sysConfigInfo, sizeof(DHDEV_SYSTEM_ATTR_CFG), &dwRetLen, nWaitTime);
		if (bSuccess && dwRetLen == sizeof(DHDEV_SYSTEM_ATTR_CFG))
		{
			//Channel amount 
			BYTE nChannelCount = sysConfigInfo.byVideoCaptureNum;
			//Record property 
			DHDEV_RECORD_CFG *pRecordInfo = new DHDEV_RECORD_CFG[nChannelCount];

			if(m_b3ProDev == FALSE)
			{
				bSuccess = CLIENT_GetDevConfig(m_LoginID, DH_DEV_RECORDCFG, -1/*all channel*/,
					pRecordInfo, nChannelCount*sizeof(DHDEV_RECORD_CFG), &dwRetLen);

				if (bSuccess && dwRetLen == nChannelCount * sizeof(DHDEV_RECORD_CFG))
				{
					//Initialize dialogue box 
					m_recordDlg.InitDlgInfo(pRecordInfo,nChannelCount, m_nDevProtoVer, m_b3ProDev);
				}
			}
			else
			{
				m_recordDlg.InitDlgInfo(pRecordInfo,nChannelCount, m_nDevProtoVer, m_b3ProDev);
			}
			delete[] pRecordInfo;
		}
	}
	else
	{
		MessageBox(ConvertString("Login first!"), ConvertString("Prompt"));
	}
}

void CClientDemo5Dlg::GetOneRecordConfigInfo(DHDEV_RECORD_CFG *pInfo, int nCurChannel)
{
	if(0 != m_LoginID || pInfo != NULL)
	{
		int nerror = 0;
		int i = nCurChannel;

		char *szOutBuffer = new char[32*1024];
		if (szOutBuffer == NULL)
		{
			return;	
		}
		memset(szOutBuffer, 0, 32*1024);
		//	for (int i = 0; i < nChannelCount; i++)
		{
			int nRetLen = 0;
			BOOL bSuccess = CLIENT_GetNewDevConfig(m_LoginID, CFG_CMD_RECORD, i, szOutBuffer, 32 * 1024, &nerror, 3000);		
			if (bSuccess)
			{
				BOOL bRet = CLIENT_ParseData(CFG_CMD_RECORD, szOutBuffer, (char *)&m_RecordInfo, sizeof(CFG_RECORD_INFO), &nRetLen);
				if (bRet == FALSE)
				{
					MessageBox(ConvertString("Parse data error"), ConvertString("Prompt"));
				}
				else
				{
					memset(pInfo, 0, sizeof(DHDEV_RECORD_CFG));
					//解析
					
					pInfo->byPreRecordLen = m_RecordInfo.nPreRecTime;
					pInfo->byRedundancyEn = m_RecordInfo.bRedundancyEn;	
					pInfo->byRecordType = m_RecordInfo.nStreamType;	
					
					for (int w = 0; w < DH_N_WEEKS; w++)
					{
						for (int t = 0; t < DH_N_TSECT; t++)
						{
							memcpy(&pInfo->stSect[w][t], &m_RecordInfo.stuTimeSection[w][t], sizeof(CFG_TIME_SECTION));
						}
					}		
				}
			}
			else
			{
				MessageBox(ConvertString("GetConfig failed(record)!"), ConvertString("Prompt"));	
			}
		}
		if (szOutBuffer)
		{
			delete []szOutBuffer;
			szOutBuffer = NULL;
		}	
	}
	else
	{
		MessageBox(ConvertString("Login first!"), ConvertString("Prompt"));
	}
}


//Set record configuration information 
void CClientDemo5Dlg::SetRecordConfigInfo(DHDEV_RECORD_CFG *pRecordInfo, BYTE nLength)
{
	if(0 != m_LoginID && pRecordInfo != NULL)
	{
		//Save information 
		BOOL bSuccess = FALSE;
		if(m_b3ProDev == FALSE)
		{
			bSuccess = CLIENT_SetDevConfig(m_LoginID, DH_DEV_RECORDCFG, -1,
				pRecordInfo, nLength*sizeof(DHDEV_RECORD_CFG));
			
			if (bSuccess)
			{
				MessageBox(ConvertString("Save config info successfully!"), ConvertString("Prompt"));
			}
			else
			{
				MessageBox(ConvertString("Save config info failed!"), ConvertString("Prompt"));
			}
		}
	}
	else
	{
		MessageBox(ConvertString("Login first!"), ConvertString("Prompt"));
	}
}

void CClientDemo5Dlg::SetOneRecordConfigInfo(DHDEV_RECORD_CFG *pInfo, int nCurChannel)
{
	if(0 != m_LoginID && pInfo != NULL)
	{
		//Save information 
		
		char *szInBuffer = new char[32*1024];
		if (szInBuffer == NULL)
		{
			return;	
		}
		memset(szInBuffer, 0, 32*1024);
		
		int nerror = 0;
		int nrestart = 0;
		int i = nCurChannel;
		//	for (int i = 0; i < nLength; i++)
		{	
			memset(szInBuffer, 0, 32*1024);
			
			DWORD deTime = GetTickCount();
			//组包
			m_RecordInfo.nChannelID = nCurChannel;
			m_RecordInfo.nPreRecTime = pInfo->byPreRecordLen;
			m_RecordInfo.bRedundancyEn = pInfo->byRedundancyEn;	
			m_RecordInfo.nStreamType = pInfo->byRecordType;	
			
			for (int w = 0; w < DH_N_WEEKS; w++)
			{
				for (int t = 0; t < DH_N_TSECT; t++)
				{
					memcpy(&m_RecordInfo.stuTimeSection[w][t], &pInfo->stSect[w][t], sizeof(CFG_TIME_SECTION));
				}
			}
			BOOL bRet = CLIENT_PacketData(CFG_CMD_RECORD, (char *)&m_RecordInfo, sizeof(CFG_RECORD_INFO), szInBuffer, 32*1024);
			if (bRet == FALSE)
			{
				MessageBox(ConvertString("Pack data error"), ConvertString("Prompt"));
			}
			else
			{
				BOOL bSuccess = CLIENT_SetNewDevConfig(m_LoginID, CFG_CMD_RECORD, i, szInBuffer, 32*1024, &nerror, &nrestart, 3000);		
				if (bSuccess)
				{
					if (nrestart == 1)
					{
						MessageBox(ConvertString("Save config info successfully!devide need restart!"), ConvertString("Prompt"));
					}
				}
				else
				{
					MessageBox(ConvertString("SetConfig failed"), ConvertString("Prompt"));	
				}	
			}	
		}
		
		if (szInBuffer)
		{
			delete []szInBuffer;
			szInBuffer = NULL;
		}
	}
	else
	{
		MessageBox(ConvertString("Login first!"), ConvertString("Prompt"));
	}
	
}
//Get network configuration information 
void CClientDemo5Dlg::GetNetWorkConfigInfo()
{
	if(0 != m_LoginID)
	{
		DWORD dwRetLen = 0;
		int nWaitTime = 1000;
		DH_QUICK_QUERY_CFG_EN stQuickQueryEn = {0};
		BOOL bSuccess = CLIENT_QuerySystemInfo(m_LoginID, ABILITY_QUICK_QUERY_CFG, (char *)&stQuickQueryEn, sizeof(DH_QUICK_QUERY_CFG_EN), (int *)&dwRetLen);
		if (bSuccess && dwRetLen == sizeof(DH_QUICK_QUERY_CFG_EN)) 
		{
			if (stQuickQueryEn.IsQuickQueryEnable == 1) 
			{
				nWaitTime = 60000;
			}
		}
		DHDEV_SYSTEM_ATTR_CFG sysConfigInfo = {0};
		bSuccess = CLIENT_GetDevConfig(m_LoginID, DH_DEV_DEVICECFG, 0,
			&sysConfigInfo, sizeof(DHDEV_SYSTEM_ATTR_CFG), &dwRetLen, nWaitTime);
		if (bSuccess && dwRetLen == sizeof(DHDEV_SYSTEM_ATTR_CFG))
		{
			//Network port amount 
			BYTE nNetIOCount = sysConfigInfo.byNetIONum;
			//Network configuration
			DHDEV_NET_CFG NetIOInfo = {0};
			bSuccess = CLIENT_GetDevConfig(m_LoginID, DH_DEV_NETCFG, 0,
				&NetIOInfo, sizeof(DHDEV_NET_CFG), &dwRetLen);
			if (bSuccess && dwRetLen == sizeof(DHDEV_NET_CFG))
			{
				DHDEV_MAIL_CFG stuMailInfo = {0};
				bSuccess = CLIENT_GetDevConfig(m_LoginID, DH_DEV_MAIL_CFG, 0, &stuMailInfo, sizeof(DHDEV_MAIL_CFG), &dwRetLen, nWaitTime);
				if (bSuccess && dwRetLen == sizeof(DHDEV_MAIL_CFG))
				{
					//Initialize dialogue box 
					m_networkDlg.InitDlgInfo(&NetIOInfo, nNetIOCount, &stuMailInfo);
				}
			}
		}
	}
	else
	{
		MessageBox(ConvertString("Login first!"), ConvertString("Prompt"));
	}
}

//Save network configuration information 
void CClientDemo5Dlg::SetNetWorkConfigInfo(DHDEV_NET_CFG *pNetIOInfo, DHDEV_MAIL_CFG *pMailInfo)
{
	if(0 != m_LoginID && pNetIOInfo != NULL)
	{
		//Save information 
		int nWaittime = 3000;
		BOOL bSuccess = CLIENT_SetDevConfig(m_LoginID, DH_DEV_NETCFG, 0,
			pNetIOInfo, sizeof(DHDEV_NET_CFG), nWaittime);
		if (bSuccess)
		{
			bSuccess = CLIENT_SetDevConfig(m_LoginID, DH_DEV_MAIL_CFG, 0, pMailInfo, sizeof(DHDEV_MAIL_CFG), nWaittime);
			if (bSuccess)
			{
				MessageBox(ConvertString("Save config info successfully!"), ConvertString("Prompt"));
			}
			else
			{
				MessageBox(ConvertString("Save config info failed!"), ConvertString("Prompt"));
			}
		}
		else
		{
			MessageBox(ConvertString("Save config info failed!"), ConvertString("Prompt"));
		}
	}
	else
	{
		MessageBox(ConvertString("Login first!"), ConvertString("Prompt"));
	}
}

//Get alarm property configuration information 
void CClientDemo5Dlg::GetAlarmConfigInfo()
{	
	if(0 != m_LoginID)
	{
		DWORD dwRetLen = 0;
		int nWaitTime = 1000;
		DH_QUICK_QUERY_CFG_EN stQuickQueryEn = {0};
		BOOL bSuccess = CLIENT_QuerySystemInfo(m_LoginID, ABILITY_QUICK_QUERY_CFG, (char *)&stQuickQueryEn, sizeof(DH_QUICK_QUERY_CFG_EN), (int *)&dwRetLen);
		if (bSuccess && dwRetLen == sizeof(DH_QUICK_QUERY_CFG_EN)) 
		{
			if (stQuickQueryEn.IsQuickQueryEnable == 1) 
			{
				nWaitTime = 60000;
			}
		}
		DHDEV_SYSTEM_ATTR_CFG sysConfigInfo = {0};
	
	    bSuccess = CLIENT_GetDevConfig(m_LoginID, DH_DEV_DEVICECFG, 0,
			&sysConfigInfo, sizeof(DHDEV_SYSTEM_ATTR_CFG), &dwRetLen, nWaitTime);
		if (bSuccess && dwRetLen == sizeof(DHDEV_SYSTEM_ATTR_CFG))
		{
			//Alarm input channel amount 
			BYTE nAlarmInCount = sysConfigInfo.byAlarmInNum>32?32:sysConfigInfo.byAlarmInNum;
			//Alarm output channel amount 
			BYTE nAlarmOutCount = sysConfigInfo.byAlarmOutNum>32?32:sysConfigInfo.byAlarmOutNum;
			//Channel amount 
			BYTE nChannelCount = sysConfigInfo.byVideoCaptureNum>32?32:sysConfigInfo.byVideoCaptureNum;


			DHDEV_ALARM_SCHEDULE_EX *pAlarmInfo = new DHDEV_ALARM_SCHEDULE_EX;
			if (pAlarmInfo == NULL)
			{
				return;
			}

			DHDEV_ALARM_SCHEDULE_EX &AlarmInfo = *pAlarmInfo;
			memset(&AlarmInfo, 0, sizeof(DHDEV_ALARM_SCHEDULE_EX));

			if(m_b3ProDev == FALSE)
			{
				bSuccess = CLIENT_GetDevConfig(m_LoginID, DH_DEV_LOCALALARM_CFG, -1,
					&AlarmInfo.struLocalAlmIn[0], sizeof(DH_ALARMIN_CFG_EX) * nAlarmInCount, &dwRetLen);

				bSuccess = CLIENT_GetDevConfig(m_LoginID, DH_DEV_NETALARM_CFG, -1,
					&AlarmInfo.struNetAlmIn[0], sizeof(DH_ALARMIN_CFG_EX) * nAlarmInCount, &dwRetLen);
				
				bSuccess &= CLIENT_GetDevConfig(m_LoginID, DH_DEV_MOTIONALARM_CFG, -1,
					&AlarmInfo.struMotion[0], sizeof(DH_MOTION_DETECT_CFG_EX) * nChannelCount, &dwRetLen);
				
				bSuccess &= CLIENT_GetDevConfig(m_LoginID, DH_DEV_VIDEOLOSTALARM_CFG, -1,
					&AlarmInfo.struVideoLost[0], sizeof(DH_VIDEO_LOST_CFG_EX) * nChannelCount, &dwRetLen);
				
				bSuccess &= CLIENT_GetDevConfig(m_LoginID, DH_DEV_BLINDALARM_CFG,-1,
					&AlarmInfo.struBlind[0], sizeof(DH_BLIND_CFG_EX) * nChannelCount, &dwRetLen, 3000);	

				DH_DISK_ALARM_CFG_EX		DiskAlarm = {0};		
				DH_NETBROKEN_ALARM_CFG_EX	NetBrokenAlarm = {0};
				bSuccess = CLIENT_GetDevConfig(m_LoginID, DH_DEV_DISKALARM_CFG, 0,
					&DiskAlarm, sizeof(DH_DISK_ALARM_CFG_EX), &dwRetLen);
				if (bSuccess && dwRetLen == sizeof(DH_DISK_ALARM_CFG_EX))
				{
					AlarmInfo.struDiskAlarm = DiskAlarm;
				}
				bSuccess = CLIENT_GetDevConfig(m_LoginID, DH_DEV_NETBROKENALARM_CFG, 0,
					&NetBrokenAlarm, sizeof(DH_NETBROKEN_ALARM_CFG_EX), &dwRetLen);
				if (bSuccess && dwRetLen == sizeof(DH_NETBROKEN_ALARM_CFG_EX))
				{
					AlarmInfo.struNetBrokenAlarm = NetBrokenAlarm;
				}
			}
			
			m_alarmDlg.InitDlgInfo(&AlarmInfo,nAlarmInCount,nAlarmOutCount,nChannelCount,m_b3ProDev);
		
			if (pAlarmInfo != NULL)
			{
				delete pAlarmInfo;
				pAlarmInfo = NULL;
			}
		}
	}
	else
	{
		MessageBox(ConvertString("Login first!"), ConvertString("Prompt"));
	}
	
}

void CClientDemo5Dlg::GetAlarmInConfigInfo(DH_ALARMIN_CFG_EX *pInfo, int nCurChannel)
{
	if (pInfo == NULL)
	{
		return;
	}
	
	char *szOutBuffer = new char[32*1024];
	if (szOutBuffer == NULL)
	{
		return;	
	}
	memset(szOutBuffer, 0, 32*1024);
	memset(pInfo, 0, sizeof(DH_ALARMIN_CFG_EX));

	if (nCurChannel < 0 || nCurChannel >= 32)
	{
		return;
	}
	
	int nalarmerror = 0;
	int i = nCurChannel;
	//	for (i = 0; i < nAlarmInCount; i++)
	{
		DWORD deTime = GetTickCount();
		//alarm
		BOOL bSuccess = CLIENT_GetNewDevConfig(m_LoginID, CFG_CMD_ALARMINPUT, i, szOutBuffer, 32*1024, &nalarmerror, 3000);		
		if (bSuccess)
		{
			int nRetLen = 0;
			BOOL bRet = CLIENT_ParseData(CFG_CMD_ALARMINPUT, szOutBuffer, (char *)&m_AlarmInfo, sizeof(CFG_ALARMIN_INFO), &nRetLen);
			int ncqs = sizeof(m_AlarmInfo);
			if (bRet == FALSE)
			{
				MessageBox(ConvertString("Parse data error"), ConvertString("Prompt"));
			}
			else
			{
				//解析
				pInfo->byAlarmEn = m_AlarmInfo.bEnable;			
				pInfo->byAlarmType = m_AlarmInfo.nAlarmType;	
					
				int w = 0;
				int t = 0;
				for (w = 0; w < WEEK_DAY_NUM; w++)
				{
					for (t = 0; t < MAX_REC_TSECT; t++)
					{
						memcpy(&pInfo->stSect[w][t], &m_AlarmInfo.stuTimeSection[w][t], sizeof(CFG_TIME_SECTION));
					}
				}

				ParseEventHandle(m_AlarmInfo.stuEventHandler, &pInfo->struHandle);
			}	
		}
		else
		{
			MessageBox(ConvertString("GetConfig failed(Alarm)!"), ConvertString("Prompt"));	
		}
	}	
	
	if (szOutBuffer)
	{
		delete []szOutBuffer;
		szOutBuffer = NULL;
	}
}


void CClientDemo5Dlg::GetMotionAlarmConfigInfo(DH_MOTION_DETECT_CFG_EX *pInfo, int nCurChannel)
{
	if (pInfo == NULL)
	{
		return;
	}
	
	char *szOutBuffer = new char[32*1024];
	if (szOutBuffer == NULL)
	{
		return;	
	}
	if (nCurChannel < 0 || nCurChannel >= 32)
	{
		return;
	}
	
	int nalarmerror = 0;
	int i = nCurChannel;
	//	for (i = 0; i < nAlarmInCount; i++)
	{
		//motionalarm
		BOOL bSuccess = CLIENT_GetNewDevConfig(m_LoginID, CFG_CMD_MOTIONDETECT, i, szOutBuffer, 32*1024, &nalarmerror, 3000);		
		if (bSuccess)
		{
			memset(pInfo, 0, sizeof(DH_MOTION_DETECT_CFG_EX));
			int nRetLen = 0;

			BOOL bRet = CLIENT_ParseData(CFG_CMD_MOTIONDETECT, szOutBuffer, &m_MotionInfo, sizeof(CFG_MOTION_INFO), &nRetLen);
			if (bRet == FALSE)
			{
				MessageBox(ConvertString("Parse data error"), ConvertString("Prompt"));
			}
			else
			{
				pInfo->byMotionEn = m_MotionInfo.bEnable;			
				pInfo->wSenseLevel = m_MotionInfo.nSenseLevel;			
				pInfo->wMotionRow = m_MotionInfo.nMotionRow;			
				pInfo->wMotionCol = m_MotionInfo.nMotionCol;	

				int w = 0;
				int t = 0;
				for (w = 0; w < pInfo->wMotionRow; w++)
				{
					for (t = 0; t < pInfo->wMotionCol; t++)
					{
						pInfo->byDetected[w][t] = m_MotionInfo.byRegion[w][t];
					}
				}
			
				for (w = 0; w < DH_N_WEEKS; w++)
				{
					for (t = 0; t < DH_N_TSECT; t++)
					{	
						memcpy(&pInfo->stSect[w][t], &m_MotionInfo.stuTimeSection[w][t], sizeof(CFG_TIME_SECTION));
					}
				}
				
				ParseEventHandle(m_MotionInfo.stuEventHandler, &pInfo->struHandle);
			}
		}
		else
		{
			MessageBox(ConvertString("GetConfig failed(MotionAlarm)!"), ConvertString("Prompt"));	
		}
	}

	if (szOutBuffer)
	{
		delete []szOutBuffer;
		szOutBuffer = NULL;
	}
	
}

void CClientDemo5Dlg::GetBlindAlarmConfigInfo(DH_BLIND_CFG_EX *pInfo, int nCurChannel)
{
	if (pInfo == NULL)
	{
		return;
	}
	
	char *szOutBuffer = new char[32*1024];
	if (szOutBuffer == NULL)
	{
		return;	
	}
	if (nCurChannel < 0 || nCurChannel >= 32)
	{
		return;
	}
	
	int nalarmerror = 0;
	int i = nCurChannel;
	//	for (i = 0; i < nChannelCount; i++)
	{
		//blindalarm
		BOOL bSuccess = CLIENT_GetNewDevConfig(m_LoginID, CFG_CMD_VIDEOBLIND, i, szOutBuffer, 32*1024, &nalarmerror, 3000);		
		if (bSuccess)
		{
			memset(pInfo, 0, sizeof(DH_BLIND_CFG_EX));
			//解析
			int nRetLen = 0;
			BOOL bRet = CLIENT_ParseData(CFG_CMD_VIDEOBLIND, szOutBuffer, (char *)&m_BlindInfo, sizeof(CFG_SHELTER_INFO), &nRetLen);
			if (bRet == FALSE)
			{
				MessageBox(ConvertString("Parse data error"), ConvertString("Prompt"));
			}
			else
			{
				pInfo->byBlindEnable = m_BlindInfo.bEnable;			
				pInfo->byBlindLevel = m_BlindInfo.nSenseLevel;			
				
				for (int w = 0; w < DH_N_WEEKS; w++)
				{
					for (int t = 0; t < DH_N_TSECT; t++)
					{	
						memcpy(&pInfo->stSect[w][t], &m_BlindInfo.stuTimeSection[w][t], sizeof(CFG_TIME_SECTION));
					}
				}
		
				ParseEventHandle(m_BlindInfo.stuEventHandler, &pInfo->struHandle);		
			}				
		}
		else
		{
			MessageBox(ConvertString("GetConfig failed(BlindAlarm)!"), ConvertString("Prompt"));	
		}
					
	}
							
	if (szOutBuffer)
	{
		delete []szOutBuffer;
		szOutBuffer = NULL;
	}
				
}

void CClientDemo5Dlg::GetLossAlarmConfigInfo(DH_VIDEO_LOST_CFG_EX *pInfo, int nCurChannel)
{
	if (pInfo == NULL)
	{
		return;
	}
	
	char *szOutBuffer = new char[32*1024];
	if (szOutBuffer == NULL)
	{
		return;	
	}
	if (nCurChannel < 0 || nCurChannel >= 32)
	{
		return;
	}
	
	int nalarmerror = 0;
	int i = nCurChannel;
	{
		//lossalarm
		BOOL bSuccess = CLIENT_GetNewDevConfig(m_LoginID, CFG_CMD_VIDEOLOST, i, szOutBuffer, 32*1024, &nalarmerror, 3000);		
		if (bSuccess)
		{	
			memset(pInfo, 0, sizeof(DH_VIDEO_LOST_CFG_EX));
			int nRetLen = 0;
			//解析
			BOOL bRet = CLIENT_ParseData(CFG_CMD_VIDEOLOST, szOutBuffer, (char *)&m_VideoLostInfo, sizeof(CFG_VIDEOLOST_INFO), &nRetLen);
			if (bRet == FALSE)
			{
				MessageBox(ConvertString("Parse data error"), ConvertString("Prompt"));
			}
			else
			{
				pInfo->byAlarmEn = m_VideoLostInfo.bEnable;			
				
				for (int w = 0; w < DH_N_WEEKS; w++)
				{
					for (int t = 0; t < DH_N_TSECT; t++)
					{	
						memcpy(&pInfo->stSect[w][t], &m_VideoLostInfo.stuTimeSection[w][t], sizeof(CFG_TIME_SECTION));
					}
				}
				ParseEventHandle(m_VideoLostInfo.stuEventHandler, &pInfo->struHandle);	
			}
		}
		else
		{
			MessageBox(ConvertString("GetConfig failed!"), ConvertString("Prompt"));	
		}
				}
	if (szOutBuffer)
	{
		delete []szOutBuffer;
		szOutBuffer = NULL;
	}
	
}

void CClientDemo5Dlg::SetAlarmConfigInfo(DHDEV_ALARM_SCHEDULE_EX *pAlarmInfo)
{
	if(0 != m_LoginID && pAlarmInfo != NULL)
	{
		DWORD dwRetLen = 0;
		DHDEV_SYSTEM_ATTR_CFG sysConfigInfo = {0};
		BOOL bAllSuccess = TRUE;	
		BOOL bSuccess = CLIENT_GetDevConfig(m_LoginID, DH_DEV_DEVICECFG, 0,
			&sysConfigInfo, sizeof(DHDEV_SYSTEM_ATTR_CFG), &dwRetLen);
		if (bSuccess && dwRetLen == sizeof(DHDEV_SYSTEM_ATTR_CFG))
		{
			//Alarm input channel amount 
			BYTE nAlarmInCount = sysConfigInfo.byAlarmInNum>32?32:sysConfigInfo.byAlarmInNum;
			//Alarm output channel amount 
			BYTE nAlarmOutCount = sysConfigInfo.byAlarmOutNum>32?32:sysConfigInfo.byAlarmOutNum;
			//Channel amount 
			BYTE nChannelCount = sysConfigInfo.byVideoCaptureNum>32?32:sysConfigInfo.byVideoCaptureNum;
			
			if(m_b3ProDev == FALSE)
			{
				bSuccess &= CLIENT_SetDevConfig(m_LoginID, DH_DEV_LOCALALARM_CFG, -1,
					&pAlarmInfo->struLocalAlmIn[0], sizeof(DH_ALARMIN_CFG_EX) * nAlarmInCount, 3000);	
				
				bSuccess &= CLIENT_SetDevConfig(m_LoginID, DH_DEV_MOTIONALARM_CFG, -1,
					& pAlarmInfo->struMotion[0], sizeof(DH_MOTION_DETECT_CFG_EX) * nChannelCount);
				
				bSuccess &= CLIENT_SetDevConfig(m_LoginID, DH_DEV_VIDEOLOSTALARM_CFG, -1,
					& pAlarmInfo->struVideoLost[0], sizeof(DH_VIDEO_LOST_CFG_EX) * nChannelCount);
				
				bSuccess &= CLIENT_SetDevConfig(m_LoginID, DH_DEV_BLINDALARM_CFG, -1,
					&pAlarmInfo->struBlind[0], sizeof(DH_BLIND_CFG_EX) * nChannelCount);
				
				DH_DISK_ALARM_CFG_EX		DiskAlarm = {0};		
				DH_NETBROKEN_ALARM_CFG_EX	NetBrokenAlarm = {0};
				
				DiskAlarm = pAlarmInfo->struDiskAlarm;
				bSuccess &= CLIENT_SetDevConfig(m_LoginID, DH_DEV_DISKALARM_CFG, 0,
					&DiskAlarm, sizeof(DH_DISK_ALARM_CFG_EX));
				
				NetBrokenAlarm = pAlarmInfo->struNetBrokenAlarm;
				bSuccess &= CLIENT_SetDevConfig(m_LoginID, DH_DEV_NETBROKENALARM_CFG, 0,
					&NetBrokenAlarm, sizeof(DH_NETBROKEN_ALARM_CFG_EX));
				
				if (bSuccess)
				{
					MessageBox(ConvertString("Save config info successfully!"), ConvertString("Prompt"));
				}
				else
				{
					MessageBox(ConvertString("Save config info failed!"), ConvertString("Prompt"));
				}
			}	
		}
	}
	else
	{
		MessageBox(ConvertString("Login first!"), ConvertString("Prompt"));
	}
}


void CClientDemo5Dlg::SetAlarmInConfigInfo(DH_ALARMIN_CFG_EX *pInfo, int nCurChannel)
{
	char *szOutBuffer = new char[32*1024];
	if (szOutBuffer == NULL)
	{
		return;	
	}
				
	int nalarmerror = 0;	
	int nrestart = 0;
				
	int i = nCurChannel;
				
	//for (i = 0; i < nAlarmInCount; i++)
	if(0 != m_LoginID && pInfo != NULL)
	{
		//alarm		
		memset(szOutBuffer, 0, 32*1024);
		//组包
		m_AlarmInfo.nChannelID = nCurChannel;
		m_AlarmInfo.bEnable = pInfo->byAlarmEn;
		m_AlarmInfo.nAlarmType = pInfo->byAlarmType;

		for (int w = 0; w < DH_N_WEEKS; w++)
		{
			for (int t = 0; t < DH_N_TSECT; t++)
			{	
				memcpy(&m_AlarmInfo.stuTimeSection[w][t], &pInfo->stSect[w][t], sizeof(CFG_TIME_SECTION));
			}
		}
		
		BuildEventHandle(pInfo->struHandle, &m_AlarmInfo.stuEventHandler);

		BOOL bRet = CLIENT_PacketData(CFG_CMD_ALARMINPUT, &m_AlarmInfo, sizeof(CFG_ALARMIN_INFO), szOutBuffer, 32*1024);
		if (bRet == FALSE)
		{
			MessageBox(ConvertString("Pack data error"), ConvertString("Prompt"));
		}
		else
		{	
			BOOL bSuccess = CLIENT_SetNewDevConfig(m_LoginID, CFG_CMD_ALARMINPUT, i, (char *)szOutBuffer, 32*1024, &nalarmerror, &nrestart, 3000);		
			if (bSuccess)
			{
				if (nrestart == 1)
				{
					MessageBox(ConvertString("Save config info successfully!devide need restart!"), ConvertString("Prompt"));
				}	
			}
			else
			{
				MessageBox(ConvertString("SetConfig failed(alarm)!"), ConvertString("Prompt"));	
			}	
		}	
	}
	else
	{
		MessageBox(ConvertString("Login first!"), ConvertString("Prompt"));
	}

	if (szOutBuffer)
	{
		delete []szOutBuffer;
		szOutBuffer = NULL;
	}
}

void CClientDemo5Dlg::SetMotionAlarmConfigInfo(DH_MOTION_DETECT_CFG_EX *pInfo, int nCurChannel)
{
	char *szOutBuffer = new char[32*1024];
	if (szOutBuffer == NULL)
	{
		return;	
	}
				
	int nalarmerror = 0;	
	int nrestart = 0;
				
	int i = nCurChannel;
				
	//for (i = 0; i < nAlarmInCount; i++)
	if(0 != m_LoginID && pInfo != NULL)
	{
		//motionalarm
		memset(szOutBuffer, 0, 32*1024);
		//组包
		m_MotionInfo.nChannelID = nCurChannel;
		m_MotionInfo.bEnable = pInfo->byMotionEn;
		m_MotionInfo.nSenseLevel = pInfo->wSenseLevel;
		
		int w = 0;
		int t = 0;
		for (w = 0; w < pInfo->wMotionRow; w++)
		{
			for (t = 0; t < pInfo->wMotionCol; t++)
			{
				m_MotionInfo.byRegion[w][t] = pInfo->byDetected[w][t];
			}
		}
		
		for (w = 0; w < DH_N_WEEKS; w++)
		{
			for (t = 0; t < DH_N_TSECT; t++)
			{	
				memcpy(&m_MotionInfo.stuTimeSection[w][t], &pInfo->stSect[w][t], sizeof(CFG_TIME_SECTION));
			}
		}
		
		BuildEventHandle(pInfo->struHandle, &m_MotionInfo.stuEventHandler);
		
		BOOL bRet = CLIENT_PacketData(CFG_CMD_MOTIONDETECT, &m_MotionInfo, sizeof(CFG_MOTION_INFO), szOutBuffer, 32*1024);
		if (bRet == FALSE)
		{
			MessageBox(ConvertString("Pack data error"), ConvertString("Prompt"));
		}
		else
		{
			BOOL bSuccess = CLIENT_SetNewDevConfig(m_LoginID, CFG_CMD_MOTIONDETECT, i, (char *)szOutBuffer, 32*1024, &nalarmerror, &nrestart, 3000);		
			if (bSuccess)
			{
				if (nrestart == 1)
				{
					MessageBox(ConvertString("Save config info successfully!devide need restart!"), ConvertString("Prompt"));
				}
				
			}
			else
			{
				MessageBox(ConvertString("SetConfig failed(motionalarm)!"), ConvertString("Prompt"));	
			}
			
		}	
	}
	else
	{
		MessageBox(ConvertString("Login first!"), ConvertString("Prompt"));
	}

	if (szOutBuffer)
	{
		delete []szOutBuffer;
		szOutBuffer = NULL;
	}
}


void CClientDemo5Dlg::SetBlindAlarmConfigInfo(DH_BLIND_CFG_EX *pInfo, int nCurChannel)
{
	char *szOutBuffer = new char[32*1024];
	if (szOutBuffer == NULL)
	{
		return;	
	}
				
	int nalarmerror = 0;	
	int nrestart = 0;
				
	int i = nCurChannel;
				
	//for (i = 0; i < nAlarmInCount; i++)
	if(0 != m_LoginID && pInfo != NULL)
	{
		//blindalarm
		
		memset(szOutBuffer, 0, 32*1024);
		//组包
		m_BlindInfo.nChannelID = nCurChannel;
		m_BlindInfo.bEnable = pInfo->byBlindEnable;
		m_BlindInfo.nSenseLevel = pInfo->byBlindLevel;
		int w = 0; 
		int t = 0;
		for (w = 0; w < DH_N_WEEKS; w++)
		{
			for (t = 0; t < DH_N_TSECT; t++)
			{	
				memcpy(&m_BlindInfo.stuTimeSection[w][t], &pInfo->stSect[w][t], sizeof(CFG_TIME_SECTION));
			}
		}
		
		BuildEventHandle(pInfo->struHandle, &m_BlindInfo.stuEventHandler);
		
		BOOL bRet = CLIENT_PacketData(CFG_CMD_VIDEOBLIND, &m_BlindInfo, sizeof(CFG_SHELTER_INFO), szOutBuffer, 32*1024);
		if (bRet == FALSE)
		{
			MessageBox(ConvertString("Pack data error"), ConvertString("Prompt"));
		}
		else
		{
			BOOL bSuccess = CLIENT_SetNewDevConfig(m_LoginID, CFG_CMD_VIDEOBLIND, i, (char *)szOutBuffer, 32*1024, &nalarmerror, &nrestart, 3000);		
			if (bSuccess)
			{
				if (nrestart == 1)
				{
					MessageBox(ConvertString("Save config info successfully!devide need restart!"), ConvertString("Prompt"));
				}
				
			}
			else
			{
				MessageBox(ConvertString("SetConfig failed(blindalarm)!"), ConvertString("Prompt"));	
			}	
			
		}
	}
	else
	{
		MessageBox(ConvertString("Login first!"), ConvertString("Prompt"));
	}
	
	if (szOutBuffer)
	{
		delete []szOutBuffer;
		szOutBuffer = NULL;
	}
	
}

void CClientDemo5Dlg::SetLossAlarmConfigInfo(DH_VIDEO_LOST_CFG_EX *pInfo, int nCurChannel)
{
	char *szOutBuffer = new char[32*1024];
	if (szOutBuffer == NULL)
	{
		return;	
	}
				
	int nalarmerror = 0;	
	int nrestart = 0;
				
	int i = nCurChannel;

	if(0 != m_LoginID && pInfo != NULL)
	{
		//lossalarm
		memset(szOutBuffer, 0, 32*1024);
		//组包
		m_VideoLostInfo.nChannelID = nCurChannel;
		m_VideoLostInfo.bEnable = pInfo->byAlarmEn;
		
		int w = 0; 
		int t = 0;
		for (w = 0; w < DH_N_WEEKS; w++)
		{
			for (t = 0; t < DH_N_TSECT; t++)
			{	
				memcpy(&m_VideoLostInfo.stuTimeSection[w][t], &pInfo->stSect[w][t], sizeof(CFG_TIME_SECTION));
			}
		}
		
		BuildEventHandle(pInfo->struHandle, &m_VideoLostInfo.stuEventHandler);
		
		BOOL bRet = CLIENT_PacketData(CFG_CMD_VIDEOLOST, &m_VideoLostInfo, sizeof(CFG_VIDEOLOST_INFO), szOutBuffer, 32*1024);
		if (bRet == FALSE)
		{
			MessageBox(ConvertString("Pack data error"), ConvertString("Prompt"));
		}
		else
		{
			BOOL bSuccess = CLIENT_SetNewDevConfig(m_LoginID, CFG_CMD_VIDEOLOST, i, (char *)szOutBuffer, 32*1024, &nalarmerror, &nrestart, 3000);		
			if (bSuccess)
			{
				if (nrestart == 1)
				{
					MessageBox(ConvertString("Save config info successfully!devide need restart!"), ConvertString("Prompt"));
				}
				
			}
			else
			{
				MessageBox(ConvertString("SetConfig failed(lossalarm)!"), ConvertString("Prompt"));	
			}	
			
		}	
	}
	else
	{
		MessageBox(ConvertString("Login first!"), ConvertString("Prompt"));
	}
	
	if (szOutBuffer)
	{
		delete []szOutBuffer;
		szOutBuffer = NULL;
	}	
}


void CClientDemo5Dlg::OnButtonRevert() 
{
	// TODO: Add your control notification handler code here
	if(0 != m_LoginID)
	{
		int nIndex = m_ctlRevertType.GetCurSel();
		DWORD dwFlag = m_ctlRevertType.GetItemData(nIndex);
		BOOL bSuccess = CLIENT_ControlDevice(m_LoginID,DH_CTRL_RESTOREDEFAULT,&dwFlag,10000);
		if(bSuccess)
		{
			MessageBox(ConvertString("Set successfully!"), ConvertString("Prompt"));
			//Device property 
			GetDeviceConfigInfo();
			//Video channel property 
			GetChannelConfigInfo();
			//COM property 
			GetCommConfigInfo();
			//Record configuration 
			GetRecordConfigInfo();
			//Network configuration 
			GetNetWorkConfigInfo();
			//Alarm configuration 
			GetAlarmConfigInfo();
		}
		else
		{
			MessageBox(ConvertString("Set failed! It is possible that the device doesn't support the function!"), ConvertString("Prompt"));
		}
	}
	else
	{
		MessageBox(ConvertString("Login first!"), ConvertString("Prompt"));
	}
}

void CClientDemo5Dlg::InitCombo()
{
	int nIndex = m_ctlRevertType.AddString(ConvertString("Common Setting"));
	m_ctlRevertType.SetItemData(nIndex,DH_RESTORE_COMMON);
	nIndex = m_ctlRevertType.AddString(ConvertString("Coding Setting"));
	m_ctlRevertType.SetItemData(nIndex,DH_RESTORE_CODING);
	nIndex = m_ctlRevertType.AddString(ConvertString("Record Setting"));
	m_ctlRevertType.SetItemData(nIndex,DH_RESTORE_VIDEO);
	nIndex = m_ctlRevertType.AddString(ConvertString("COM Setting"));
	m_ctlRevertType.SetItemData(nIndex,DH_RESTORE_COMM);
	nIndex = m_ctlRevertType.AddString(ConvertString("Network Setting"));
	m_ctlRevertType.SetItemData(nIndex,DH_RESTORE_NETWORK);
	nIndex = m_ctlRevertType.AddString(ConvertString("Alarm Setting"));
	m_ctlRevertType.SetItemData(nIndex,DH_RESTORE_ALARM);
	nIndex = m_ctlRevertType.AddString(ConvertString("Video Detecting"));
	m_ctlRevertType.SetItemData(nIndex,DH_RESTORE_VIDEODETECT);
	nIndex = m_ctlRevertType.AddString(ConvertString("PTZ Control"));
	m_ctlRevertType.SetItemData(nIndex,DH_RESTORE_PTZ);
	nIndex = m_ctlRevertType.AddString(ConvertString("Ouput Mode"));
	m_ctlRevertType.SetItemData(nIndex,DH_RESTORE_OUTPUTMODE);
	nIndex = m_ctlRevertType.AddString(ConvertString("Channel Name"));
	m_ctlRevertType.SetItemData(nIndex,DH_RESTORE_CHANNELNAME);
	nIndex = m_ctlRevertType.AddString(ConvertString("Restore all config"));
	m_ctlRevertType.SetItemData(nIndex,DH_RESTORE_ALL);
	nIndex = m_ctlRevertType.AddString(ConvertString("Common Setting|Video Setting"));
	m_ctlRevertType.SetItemData(nIndex,DH_RESTORE_COMMON | DH_RESTORE_VIDEO);
	nIndex = m_ctlRevertType.AddString(ConvertString("COM Setting|Alarm Setting"));
	m_ctlRevertType.SetItemData(nIndex,DH_RESTORE_COMM | DH_RESTORE_ALARM);
	nIndex = m_ctlRevertType.AddString(ConvertString("PTZ Control|Channel Name"));
	m_ctlRevertType.SetItemData(nIndex,DH_RESTORE_PTZ | DH_RESTORE_CHANNELNAME);
	m_ctlRevertType.SetCurSel(0);
}

//Get multiple privacy mask zones information 
void CClientDemo5Dlg::GetCoverConfigInfo()
{
	if(0 != m_LoginID)
	{
		DWORD dwRetLen = 0;
		int nWaitTime = 1000;
		DH_QUICK_QUERY_CFG_EN stQuickQueryEn = {0};
		BOOL bSuccess = CLIENT_QuerySystemInfo(m_LoginID, ABILITY_QUICK_QUERY_CFG, (char *)&stQuickQueryEn, sizeof(DH_QUICK_QUERY_CFG_EN), (int *)&dwRetLen);
		if (bSuccess && dwRetLen == sizeof(DH_QUICK_QUERY_CFG_EN)) 
		{
			if (stQuickQueryEn.IsQuickQueryEnable == 1) 
			{
				nWaitTime = 60000;
			}
		}
		DHDEV_SYSTEM_ATTR_CFG sysConfigInfo = {0};
		bSuccess = CLIENT_GetDevConfig(m_LoginID, DH_DEV_DEVICECFG, 0,
			&sysConfigInfo, sizeof(DHDEV_SYSTEM_ATTR_CFG), &dwRetLen, nWaitTime);
		if (bSuccess && dwRetLen == sizeof(DHDEV_SYSTEM_ATTR_CFG))
		{
			BYTE nChannelCount = sysConfigInfo.byVideoCaptureNum;

			DHDEV_VIDEOCOVER_CFG *pVideoCover = new DHDEV_VIDEOCOVER_CFG[nChannelCount];
			memset(pVideoCover, 0, nChannelCount*sizeof(DHDEV_VIDEOCOVER_CFG));
			if(m_b3ProDev == FALSE)
			{
				bSuccess = CLIENT_GetDevConfig(m_LoginID, DH_DEV_VIDEO_COVER, -1, 
					pVideoCover, nChannelCount * sizeof(DHDEV_VIDEOCOVER_CFG), &dwRetLen);
				if (bSuccess && dwRetLen == nChannelCount * sizeof(DHDEV_VIDEOCOVER_CFG))
				{
					//Initialize dialogue box
					m_coverDlg.InitDlgInfo(pVideoCover,nChannelCount, m_b3ProDev);
				}
				else
				{
					MessageBox(ConvertString("Get Multi-area Shelter info failed! It is possible that the device doesn't support the function!"), ConvertString("Prompt"));
				}
			}
			else
			{
				m_coverDlg.InitDlgInfo(pVideoCover,nChannelCount, m_b3ProDev);
			}	
			delete[] pVideoCover;
		}	
	}
	else
	{
		MessageBox(ConvertString("Login first!"), ConvertString("Prompt"));
	}
}

void CClientDemo5Dlg::GetOneCoverConfigInfo(DHDEV_VIDEOCOVER_CFG *pInfo, int nCurChannel)
{
	if(0 != m_LoginID || pInfo != NULL)
	{	
		char *szOutBuffer = new char[32*1024];
		if (szOutBuffer == NULL)
		{	
			return;	
		}
		int nerror = 0;
		int i = nCurChannel;
		//	for (int i = 0; i < nChannelCount; i++)
		{
			BOOL bSuccess = CLIENT_GetNewDevConfig(m_LoginID, CFG_CMD_ENCODE, i, szOutBuffer, 32*1024, &nerror, 3000);		
			if (bSuccess)
			{
				memset(pInfo, 0, sizeof(DHDEV_VIDEOCOVER_CFG));
				CFG_ENCODE_INFO stuEncodeInfo = {0};
				//解析
				int nRetLen = 0;
				BOOL bRet = CLIENT_ParseData(CFG_CMD_ENCODE, szOutBuffer, (char *)&stuEncodeInfo, sizeof(CFG_ENCODE_INFO), &nRetLen);
				if (bRet == FALSE)
				{
					MessageBox(ConvertString("Parser data error"), ConvertString("Prompt"));
				}
				else
				{
					memcpy(&m_VideoCoveInfo, &stuEncodeInfo.stuVideoCover, sizeof(CFG_VIDEO_COVER));
					char Name[64] = {0} ;
					Change_Utf8_Assic((unsigned char *)stuEncodeInfo.szChnName, Name);
					strcpy(pInfo->szChannelName, Name);	
					
					pInfo->bTotalBlocks = m_VideoCoveInfo.nTotalBlocks;	
					pInfo->bCoverCount = m_VideoCoveInfo.nCurBlocks;

					for (int k = 0; k < pInfo->bTotalBlocks; k++)
					{
						pInfo->CoverBlock[k].bBlockType = m_VideoCoveInfo.stuCoverBlock[k].nBlockType;
						pInfo->CoverBlock[k].bPriview = m_VideoCoveInfo.stuCoverBlock[k].nPreviewBlend;	
						pInfo->CoverBlock[k].bEncode = m_VideoCoveInfo.stuCoverBlock[k].nBlockType;
						
						pInfo->CoverBlock[k].nColor |= m_VideoCoveInfo.stuCoverBlock[k].stuColor.nRed;	
						pInfo->CoverBlock[k].nColor <<= 8;
						pInfo->CoverBlock[k].nColor |= m_VideoCoveInfo.stuCoverBlock[k].stuColor.nGreen;	
						pInfo->CoverBlock[k].nColor <<= 8;
						pInfo->CoverBlock[k].nColor |= m_VideoCoveInfo.stuCoverBlock[k].stuColor.nBlue;	
						pInfo->CoverBlock[k].nColor <<= 8;
						pInfo->CoverBlock[k].nColor |= m_VideoCoveInfo.stuCoverBlock[k].stuColor.nAlpha;	

						pInfo->CoverBlock[k].rcBlock.left = m_VideoCoveInfo.stuCoverBlock[k].stuRect.nLeft;
						pInfo->CoverBlock[k].rcBlock.top = m_VideoCoveInfo.stuCoverBlock[k].stuRect.nTop;	
						pInfo->CoverBlock[k].rcBlock.right = m_VideoCoveInfo.stuCoverBlock[k].stuRect.nRight;	
						pInfo->CoverBlock[k].rcBlock.bottom = m_VideoCoveInfo.stuCoverBlock[k].stuRect.nBottom;			
					}	
				}	
			}
			else
			{
				MessageBox(ConvertString("GetConfig failed(cover)!"), ConvertString("Prompt"));	
			}
		}
		
		if (szOutBuffer)
		{
			delete []szOutBuffer;
			szOutBuffer = NULL;
		}
	}	
	else
	{
		MessageBox(ConvertString("Login first!"), ConvertString("Prompt"));
	}		
}

void CClientDemo5Dlg::SetCoverConfigInfo(DHDEV_VIDEOCOVER_CFG *pCoverInfo, int nLength)
{
	if(0 != m_LoginID && pCoverInfo != NULL)
	{
		//Save information
		BOOL bSuccess = FALSE;
		if(m_b3ProDev == FALSE)
		{
			bSuccess = CLIENT_SetDevConfig(m_LoginID, DH_DEV_VIDEO_COVER, -1,
				pCoverInfo, nLength*sizeof(DHDEV_VIDEOCOVER_CFG));
			
			if (bSuccess)
			{
				MessageBox(ConvertString("Save config info successfully!"), ConvertString("Prompt"));
			}
			else
			{
				MessageBox(ConvertString("Save config info failed!"), ConvertString("Prompt"));
			}
		}
	}	
	else
	{
		MessageBox(ConvertString("Login first!"), ConvertString("Prompt"));
	}
}

void CClientDemo5Dlg::SetOneCoverConfigInfo(DHDEV_VIDEOCOVER_CFG *pInfo, int nCurChannel)
{
	if(0 != m_LoginID && pInfo != NULL)
	{
		char *szInBuffer = new char[32*1024];
		if (szInBuffer == NULL)
		{
			return;	
		}
		int nerror = 0;
		int nrestart = 0;
		int i = nCurChannel;
		//	for (int i = 0; i < nLength; i++)
		{
			memset(szInBuffer, 0, 32*1024);
			//组包
			m_VideoCoveInfo.nTotalBlocks = pInfo->bTotalBlocks;	
			m_VideoCoveInfo.nCurBlocks = pInfo->bCoverCount;
			
			for (int k = 0; k < pInfo->bTotalBlocks; k++)
			{
				m_VideoCoveInfo.stuCoverBlock[k].nBlockType = pInfo->CoverBlock[k].bBlockType;
				m_VideoCoveInfo.stuCoverBlock[k].nPreviewBlend = pInfo->CoverBlock[k].bPriview;	
				m_VideoCoveInfo.stuCoverBlock[k].nBlockType = pInfo->CoverBlock[k].bEncode;
				
				m_VideoCoveInfo.stuCoverBlock[k].stuColor.nRed = (pInfo->CoverBlock[k].nColor>>24)&0xff;	
				m_VideoCoveInfo.stuCoverBlock[k].stuColor.nGreen = (pInfo->CoverBlock[k].nColor>>16)&0xff;		
				m_VideoCoveInfo.stuCoverBlock[k].stuColor.nBlue = (pInfo->CoverBlock[k].nColor>>8)&0xff;	
				m_VideoCoveInfo.stuCoverBlock[k].stuColor.nAlpha = pInfo->CoverBlock[k].nColor&0xff;;	
				
				m_VideoCoveInfo.stuCoverBlock[k].stuRect.nLeft = pInfo->CoverBlock[k].rcBlock.left;
				m_VideoCoveInfo.stuCoverBlock[k].stuRect.nTop = pInfo->CoverBlock[k].rcBlock.top;	
				m_VideoCoveInfo.stuCoverBlock[k].stuRect.nRight = pInfo->CoverBlock[k].rcBlock.right;	
				m_VideoCoveInfo.stuCoverBlock[k].stuRect.nBottom = pInfo->CoverBlock[k].rcBlock.bottom;			
			}

			DHDEV_CHANNEL_CFG stuInfo = {0};
			GetOneChannelConfigInfo(&stuInfo, i);
			memcpy(&m_stuEncodeInfo.stuVideoCover, &m_VideoCoveInfo, sizeof(CFG_VIDEO_COVER));

			BOOL bRet = CLIENT_PacketData(CFG_CMD_ENCODE, (char *)&m_stuEncodeInfo, sizeof(CFG_ENCODE_INFO), szInBuffer, 32*1024);
			if (bRet == FALSE)
			{
				MessageBox(ConvertString("Pack data error"), ConvertString("Prompt"));
			}
			else
			{
				BOOL bSuccess = CLIENT_SetNewDevConfig(m_LoginID, CFG_CMD_ENCODE, i, szInBuffer, 32*1024, &nerror, &nrestart, 3000);		
				if (bSuccess)
				{
					if (nrestart == 1)
					{
						MessageBox(ConvertString("Save config info successfully!devide need restart!"), ConvertString("Prompt"));
					}
				}
				else
				{
					
					MessageBox(ConvertString("SetConfig failed"), ConvertString("Prompt"));	
				}
				
			}
		}
		
		if (szInBuffer)
		{
			delete []szInBuffer;
			szInBuffer = NULL;
		}
	}
	
	else
	{
		MessageBox(ConvertString("Login first!"), ConvertString("Prompt"));
	}
}

void CClientDemo5Dlg::GetStrategyConfigInfo()
{
	if(m_LoginID != 0)
	{
		DWORD dwRetLen =0;
		int nWaitTime = 1000;
		DH_QUICK_QUERY_CFG_EN stQuickQueryEn = {0};
		BOOL bSuccess = CLIENT_QuerySystemInfo(m_LoginID, ABILITY_QUICK_QUERY_CFG, (char *)&stQuickQueryEn, sizeof(DH_QUICK_QUERY_CFG_EN), (int *)&dwRetLen);
		if (bSuccess && dwRetLen == sizeof(DH_QUICK_QUERY_CFG_EN)) 
		{
			if (stQuickQueryEn.IsQuickQueryEnable == 1) 
			{
				nWaitTime = 60000;
			}
		}
		DHDEV_TRANSFER_STRATEGY_CFG stuTrans;
		bSuccess = CLIENT_GetDevConfig(m_LoginID, DH_DEV_TRANS_STRATEGY, 0, (VOID*)&stuTrans, sizeof(DHDEV_TRANSFER_STRATEGY_CFG), &dwRetLen, nWaitTime);
		if( bSuccess && dwRetLen == sizeof(DHDEV_TRANSFER_STRATEGY_CFG))
		{
			m_StrategyDlg.m_StrategyTrans.InitDlgInfo(&stuTrans);
		}
		else
		{
			MessageBox(ConvertString("get net transmission strategy failed!"), ConvertString("Prompt"));
		}
		
		DHDEV_DOWNLOAD_STRATEGY_CFG stuDownload;
		bSuccess = CLIENT_GetDevConfig(m_LoginID,DH_DEV_DOWNLOAD_STRATEGY,0,(VOID*)&stuDownload,sizeof(DHDEV_DOWNLOAD_STRATEGY_CFG),&dwRetLen);
		if(bSuccess && dwRetLen == sizeof(DHDEV_DOWNLOAD_STRATEGY_CFG))
		{
			m_StrategyDlg.m_StrategyDownload.InitDlgInfo(&stuDownload);
		}
		else
		{
			MessageBox(ConvertString("get record transmission strategy failed!"), ConvertString("Prompt"));
		}
	}
	else
	{
		MessageBox(ConvertString("Login first!"), ConvertString("Prompt"));
	}
}

void CClientDemo5Dlg::SetStrategyConfigInfo(DHDEV_TRANSFER_STRATEGY_CFG* pTransInfo,
											DHDEV_DOWNLOAD_STRATEGY_CFG* pDownloadInfo)
{
	if(0 != m_LoginID && pTransInfo != NULL && pTransInfo != NULL)
	{
		//Save information 
		BOOL bSuccess = CLIENT_SetDevConfig(m_LoginID, DH_DEV_TRANS_STRATEGY, -1,
			pTransInfo, sizeof(DHDEV_TRANSFER_STRATEGY_CFG));
		if (bSuccess)
		{
			MessageBox(ConvertString("save net transmission strategy successfully!"), ConvertString("Prompt"));
		}
		else
		{
			MessageBox(ConvertString("save net transmission strategy failed!"), ConvertString("Prompt"));
		}
		bSuccess = CLIENT_SetDevConfig(m_LoginID, DH_DEV_DOWNLOAD_STRATEGY, -1,
			pDownloadInfo, sizeof(DHDEV_DOWNLOAD_STRATEGY_CFG));
		if (bSuccess)
		{
			MessageBox(ConvertString("save record transmission strategy successfully!"), ConvertString("Prompt"));
		}
		else
		{
			MessageBox(ConvertString("save record transmission strategy failed!"), ConvertString("Prompt"));
		}
	}
	else
	{
		MessageBox(ConvertString("Login first!"), ConvertString("Prompt"));
	}
}

//Configuration file 
LONG CClientDemo5Dlg::ImportConfigFile(char* szPathName, fDownLoadPosCallBack cbUploadPos, DWORD dwUser)
{
	if(NULL == szPathName || NULL == cbUploadPos )
	{
		return -1;
	}

	return CLIENT_ImportConfigFile(m_LoginID, szPathName, cbUploadPos, dwUser, 0);
}

LONG CClientDemo5Dlg::ExportConfigFile(char* szFileName, DH_CONFIG_FILE_TYPE emConfigFileType, fDownLoadPosCallBack cbUploadPos, DWORD dwUser)
{
	if(NULL == szFileName || NULL == cbUploadPos )
	{
		return -1;
	}
	
	return CLIENT_ExportConfigFile(m_LoginID, emConfigFileType, szFileName, cbUploadPos, dwUser);
}

BOOL g_bSignal = TRUE;
void CClientDemo5Dlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if (g_bSignal)
	{
//		OnBtnLogin();
		g_bSignal = FALSE;
	}
	else
	{
//		OnBtnLogout();
		g_bSignal = TRUE;
	}
	
	CDialog::OnTimer(nIDEvent);
}


void CClientDemo5Dlg::ResolutionSizetoInt(int nResolution, int *nWidth, int *nHeight)
{
	if(m_nVideoStandard == 0)  //PAL
	{
		switch (nResolution)
		{
			case 0: 			*nWidth = 704;	*nHeight = 576;		break;//D1
			case 1:				*nWidth = 352;	*nHeight = 576;		break;//HD1
			case 2:				*nWidth = 704;	*nHeight = 288;		break;//BCIF
			case 3:				*nWidth = 352;	*nHeight = 288;		break;//CIF
			case 4:				*nWidth = 176;	*nHeight = 144;		break;//QCIF
			case 5:				*nWidth = 640;	*nHeight = 480;		break;//VGA
			case 6:				*nWidth = 320;	*nHeight = 240;		break;//QVGA
			case 7:				*nWidth = 480;	*nHeight = 480;		break;//SVCD
			case 8:				*nWidth = 160;	*nHeight = 128;		break;//QQVGA
			case 9:				*nWidth = 1600; *nHeight = 1200;	break;//UXGA
			case 10:			*nWidth = 1920;	*nHeight = 1200;	break;//WUXGA
			default:	ASSERT(FALSE);				break;
		}
	}		
	else
	{
		switch (nResolution)
		{
			case 0:				*nWidth = 704;	*nHeight = 480;		break;//D1
			case 1:				*nWidth = 352;	*nHeight = 480;		break;//HD1
			case 2:				*nWidth = 704;	*nHeight = 240;		break;//BCIF
			case 3:				*nWidth = 352;	*nHeight = 240;		break;//CIF
			case 4:				*nWidth = 176;	*nHeight = 120;		break;//QCIF
			case 5:				*nWidth = 640;	*nHeight = 480;		break;//VGA
			case 6:				*nWidth = 320;	*nHeight = 240;		break;//QVGA
			case 7:				*nWidth = 480;	*nHeight = 480;		break;//SVCD
			case 8:				*nWidth = 160;	*nHeight = 128;		break;//QQVGA
			case 9:				*nWidth = 1600; *nHeight = 1200;	break;//UXGA
			case 10:			*nWidth = 1920;	*nHeight = 1200;	break;//WUXGA
			default:	ASSERT(FALSE);				break;
		}
	}
}

int CClientDemo5Dlg::ResolutionInttoSize(int nWidth, int nHeight)
{
	int nResolution = 0;

	if(m_nVideoStandard == 0)  //PAL
	{
		if (nWidth == 704 && nHeight == 576)
		{
			nResolution = 0;
		}
		else if (nWidth == 352 && nHeight == 576)
		{
			nResolution = 1;
		}
		else if (nWidth == 704 && nHeight == 288)
		{
			nResolution = 2;
		}
		else if (nWidth == 352 && nHeight == 288)
		{
			nResolution = 3;
		}
		else if (nWidth == 176 && nHeight == 144)
		{
			nResolution = 4;
		}
		else if (nWidth == 640 && nHeight == 480)
		{
			nResolution = 5;
		}
		else if (nWidth == 320 && nHeight == 240)
		{
			nResolution = 6;
		}
		else if (nWidth == 480 && nHeight == 480)
		{
			nResolution = 7;
		}
		else if (nWidth == 160 && nHeight == 128)
		{
			nResolution = 8;
		}
		else if (nWidth == 1600 && nHeight == 1200)
		{
			nResolution = 9;
		}
		else if (nWidth == 1920 && nHeight == 1200)
		{
			nResolution = 10;
		}
	}		
	else
	{
		if (nWidth == 704 && nHeight == 480)
		{
			nResolution = 0;
		}
		else if (nWidth == 352 && nHeight == 480)
		{
			nResolution = 1;
		}
		else if (nWidth == 704 && nHeight == 240)
		{
			nResolution = 2;
		}
		else if (nWidth == 352 && nHeight == 240)
		{
			nResolution = 3;
		}
		else if (nWidth == 176 && nHeight == 120)
		{
			nResolution = 4;
		}
		else if (nWidth == 640 && nHeight == 480)
		{
			nResolution = 5;
		}
		else if (nWidth == 320 && nHeight == 240)
		{
			nResolution = 6;
		}
		else if (nWidth == 480 && nHeight == 480)
		{
			nResolution = 7;
		}
		else if (nWidth == 160 && nHeight == 128)
		{
			nResolution = 8;
		}
		else if (nWidth == 1600 && nHeight == 1200)
		{
			nResolution = 9;
		}
		else if (nWidth == 1920 && nHeight == 1200)
		{
			nResolution = 10;
		}
	}

	return nResolution;
}

void CClientDemo5Dlg::ParseEventHandle(CFG_ALARM_MSG_HANDLE stuHandlle, DH_MSG_HANDLE_EX *pstruHandle)
{
	if (pstruHandle == NULL)
	{
		return;
	}

	for (int i = 0; i < 32; i++)
	{
		pstruHandle->byRecordChannel[i] = (stuHandlle.dwRecordMask[0]>>i)&0x01;	
		pstruHandle->byRelAlarmOut[i] = (stuHandlle.dwAlarmOutMask[0]>>i)&0x01;		
		pstruHandle->struPtzLink[i].iType = stuHandlle.stuPtzLink[i].emType;
		pstruHandle->struPtzLink[i].iValue = stuHandlle.stuPtzLink[i].nValue;
		pstruHandle->byTour[i] = (stuHandlle.dwTourMask[0]>>i)&0x01;			
		pstruHandle->bySnap[i] = (stuHandlle.dwSnapshot[0]>>i)&0x01;		
	}

	pstruHandle->dwMatrix = stuHandlle.dwMatrixMask[0];	
	
	if (stuHandlle.abRecordEnable)
	{	
		if (stuHandlle.bRecordEnable)
		{
			pstruHandle->dwActionFlag |= DH_ALARM_RECORD;	
		}
		pstruHandle->dwActionMask |= DH_ALARM_RECORD;
	} 
	
	if (stuHandlle.abAlarmOutEn)
	{
		if (stuHandlle.bAlarmOutEn)
		{
			pstruHandle->dwActionFlag |= DH_ALARM_OUT;
		}
		pstruHandle->dwActionMask |= DH_ALARM_OUT;
	}
	
	if (stuHandlle.abPtzLinkEn)
	{
		if (stuHandlle.bPtzLinkEn)
		{
			pstruHandle->dwActionFlag |= DH_ALARM_PTZ;
		}
		pstruHandle->dwActionMask |= DH_ALARM_PTZ;
	}

	if (stuHandlle.abTourEnable)
	{
		if (stuHandlle.bTourEnable)
		{
			pstruHandle->dwActionFlag |= DH_ALARM_TOUR;
		}
		pstruHandle->dwActionMask |= DH_ALARM_TOUR;
	}
	
	if (stuHandlle.abSnapshotEn)
	{
		if (stuHandlle.bSnapshotEn)
		{
			pstruHandle->dwActionFlag |= DH_ALARM_SNAP;
		}
		pstruHandle->dwActionMask |= DH_ALARM_SNAP;
	}

	if (stuHandlle.abTipEnable)
	{
		if (stuHandlle.bTipEnable)
		{
			pstruHandle->dwActionFlag |= DH_ALARM_TIP;
		}
		pstruHandle->dwActionMask |= DH_ALARM_TIP;
	}

	if (stuHandlle.abMailEnable)
	{
		if (stuHandlle.bMailEnable)
		{
			pstruHandle->dwActionFlag |= DH_ALARM_MAIL;
		}
		pstruHandle->dwActionMask |= DH_ALARM_MAIL;
	}

	if (stuHandlle.abMessageEnable)
	{
		if (stuHandlle.bMessageEnable)
		{
			pstruHandle->dwActionFlag |= DH_ALARM_UPLOAD;
		}
		pstruHandle->dwActionMask |= DH_ALARM_UPLOAD;
	}

	if (stuHandlle.abBeepEnable)
	{
		if (stuHandlle.bBeepEnable)
		{
			pstruHandle->dwActionFlag |= DH_ALARM_BEEP;
		}
		pstruHandle->dwActionMask |= DH_ALARM_BEEP;
	}

	if (stuHandlle.abVoiceEnable)
	{
		if (stuHandlle.bVoiceEnable)
		{
			pstruHandle->dwActionFlag |= DH_ALARM_VOICE;
		}
		pstruHandle->dwActionMask |= DH_ALARM_VOICE;
	}
	
	pstruHandle->bLog = stuHandlle.bLogEnable;
	pstruHandle->bMatrixEn = stuHandlle.bMatrixEnable;
	pstruHandle->dwEventLatch = stuHandlle.nEventLatch;
	pstruHandle->dwRecLatch = stuHandlle.nRecordLatch;	
	pstruHandle->dwDuration = stuHandlle.nAlarmOutLatch;	
	pstruHandle->bySnapshotTimes = stuHandlle.nSnapshotTimes;	
	pstruHandle->bMMSEn = stuHandlle.bMMSEnable;	
	pstruHandle->bMessageToNet = stuHandlle.bMessageToNetEn;	
}

void CClientDemo5Dlg::BuildEventHandle(DH_MSG_HANDLE_EX stuHandlle, CFG_ALARM_MSG_HANDLE *pstruHandle)
{
	if (pstruHandle == NULL)
	{
		return;
	}

	pstruHandle->dwRecordMask[0] = 0;
	pstruHandle->dwAlarmOutMask[0] = 0;
	pstruHandle->dwTourMask[0] = 0;
	pstruHandle->dwSnapshot[0] = 0;

	for (int i = 0; i < 32; i++)
	{
		if (stuHandlle.byRecordChannel[i])
		{
			pstruHandle->dwRecordMask[0] |= (0x01<<i);
		}
		if (stuHandlle.byRelAlarmOut[i])
		{
			pstruHandle->dwAlarmOutMask[0] |= (0x01<<i);
		}
		if (stuHandlle.byTour[i])
		{
			pstruHandle->dwTourMask[0] |= (0x01<<i);
		}
		if (stuHandlle.bySnap[i])
		{
			pstruHandle->dwSnapshot[0] |= (0x01<<i);
		}
		pstruHandle->stuPtzLink[i].emType = (CFG_LINK_TYPE)stuHandlle.struPtzLink[i].iType;
		pstruHandle->stuPtzLink[i].nValue = stuHandlle.struPtzLink[i].iValue;
	}
	
	pstruHandle->dwMatrixMask[0] = stuHandlle.dwMatrix; 
	pstruHandle->bRecordEnable = (stuHandlle.dwActionFlag&DH_ALARM_RECORD)>0?1:0;
	pstruHandle->bAlarmOutEn = (stuHandlle.dwActionFlag&DH_ALARM_OUT)>0?1:0;
	pstruHandle->bPtzLinkEn = (stuHandlle.dwActionFlag&DH_ALARM_PTZ)>0?1:0;
	pstruHandle->bTourEnable = (stuHandlle.dwActionFlag&DH_ALARM_TOUR)>0?1:0;
	pstruHandle->bSnapshotEn = (stuHandlle.dwActionFlag&DH_ALARM_SNAP)>0?1:0;
	pstruHandle->bTipEnable = (stuHandlle.dwActionFlag&DH_ALARM_TIP)>0?1:0;
	pstruHandle->bMailEnable = (stuHandlle.dwActionFlag&DH_ALARM_MAIL)>0?1:0;
	pstruHandle->bMessageEnable = (stuHandlle.dwActionFlag&DH_ALARM_UPLOAD)>0?1:0;
	pstruHandle->bBeepEnable = (stuHandlle.dwActionFlag&DH_ALARM_BEEP)>0?1:0;
	pstruHandle->abVoiceEnable = (stuHandlle.dwActionFlag&DH_ALARM_VOICE)>0?1:0;
	
	pstruHandle->bLogEnable = stuHandlle.bLog;
	pstruHandle->bMatrixEnable = stuHandlle.bMatrixEn;
	pstruHandle->nEventLatch = stuHandlle.dwEventLatch;
	pstruHandle->nRecordLatch = stuHandlle.dwRecLatch;	
	pstruHandle->nAlarmOutLatch = stuHandlle.dwDuration;	
	pstruHandle->nSnapshotTimes = stuHandlle.bySnapshotTimes;	
	pstruHandle->bMMSEnable = stuHandlle.bMMSEn;	
	pstruHandle->bMessageToNetEn = stuHandlle.bMessageToNet;
}

void CClientDemo5Dlg::Change_Utf8_Assic(unsigned char * pUTF8, char *destbuf)
{
	int nSrcStrLen = strlen((char*)pUTF8);
	int nDestStrLen = nSrcStrLen*2;
	char* tmpBuf = new char[nDestStrLen];
	memset(tmpBuf, 0, nDestStrLen);

	int nNum = MultiByteToWideChar(CP_UTF8, 0, (char*)pUTF8, nSrcStrLen, (LPWSTR)tmpBuf, nDestStrLen);
	nNum = WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)tmpBuf, nNum, destbuf, nSrcStrLen, NULL, NULL);
	
	delete tmpBuf;
	tmpBuf = NULL;
	return ;
}

void CClientDemo5Dlg::Change_Assic_UTF8(char *pStrGBKData, int nlen, char *pbuf, int buflen)
{
	int nDestStrLen = nlen*2;
	char* tmpBuf = new char[nDestStrLen];
	memset(tmpBuf, 0, nDestStrLen);

	int nNum = MultiByteToWideChar(CP_ACP, 0, (char*)pStrGBKData, nlen, (LPWSTR)tmpBuf, nDestStrLen);
	nNum = WideCharToMultiByte(CP_UTF8, 0, (LPCWSTR)tmpBuf, nNum, pbuf, buflen, NULL, NULL);
	
	delete tmpBuf;
	tmpBuf = NULL;
	return ;
}


