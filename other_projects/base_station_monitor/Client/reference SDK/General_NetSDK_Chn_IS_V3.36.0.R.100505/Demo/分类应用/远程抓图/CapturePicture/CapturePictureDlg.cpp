// CapturePictureDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CapturePicture.h"
#include "CapturePictureDlg.h"
#include "dhnetsdk.h"
#include "DhDecode.h"
#include <atlconv.h>
#include <assert.h>

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
// CCapturePictureDlg dialog

CCapturePictureDlg::CCapturePictureDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCapturePictureDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCapturePictureDlg)
	m_strCardNum = _T("");
	m_bEnable = FALSE;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_LoginID = 0;
	m_nChannelCount = 0;
	m_snapmode = -1;
	m_bJSON = FALSE;
}

void CCapturePictureDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCapturePictureDlg)
	DDX_Control(pDX, IDC_COMBO_SNAP_QUALITY, m_ccbQuality);
	DDX_Control(pDX, IDC_COMBO_SNAP_SIZE, m_ccbSize);
	DDX_Control(pDX, IDC_COMBO_SNAP_MODE, m_ccbSnapMode);
	DDX_Control(pDX, IDC_COMBO_SNAP_SPACE, m_ccbFrameRate);
	DDX_Control(pDX, IDC_SHOWPICTURE, m_showbmp);
	DDX_Control(pDX, IDC_COMBO_CHANNEL, m_ctlChannel);
	DDX_Text(pDX, IDC_EDIT_CARD, m_strCardNum);
	DDV_MaxChars(pDX, m_strCardNum, 16);
	DDX_Check(pDX, IDC_CHECK_TIME_ENABLE, m_bEnable);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCapturePictureDlg, CDialog)
	//{{AFX_MSG_MAP(CCapturePictureDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_LOGIN, OnButtonLogin)
	ON_BN_CLICKED(IDC_BUTTON_LOGOUT, OnButtonLogout)
	ON_BN_CLICKED(IDC_BUTTON_START, OnButtonStart)
	ON_BN_CLICKED(IDC_BTN_STARTSNAP, OnBtnStartsnap)
	ON_BN_CLICKED(IDC_BTN_STOPSNAP, OnBtnStopsnap)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_RADIO_SNAPONE, OnRadioSnapone)
	ON_BN_CLICKED(IDC_RADIO_SNAPTIME, OnRadioSnaptime)
	ON_BN_CLICKED(IDC_BTN_ENABLE, OnBtnEnable)
	ON_CBN_SELCHANGE(IDC_COMBO_CHANNEL, OnSelchangeComboChannel)
	ON_BN_CLICKED(IDC_BTN_GET, OnBtnGet)
	ON_BN_CLICKED(IDC_BTN_Set, OnBTNSet)
	ON_CBN_SELCHANGE(IDC_COMBO_SNAP_MODE, OnSelchangeComboSnapMode)
	ON_CBN_SELCHANGE(IDC_COMBO_SNAP_SIZE, OnSelchangeComboSnapSize)
	ON_CBN_SELCHANGE(IDC_COMBO_SNAP_SPACE, OnSelchangeComboSnapSpace)
	ON_CBN_SELCHANGE(IDC_COMBO_SNAP_QUALITY, OnSelchangeComboSnapQuality)
	ON_BN_CLICKED(IDC_CHECK_TIME_ENABLE, OnCheckTimeEnable)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCapturePictureDlg message handlers

BOOL CCapturePictureDlg::OnInitDialog()
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
	SetDlgItemText(IDC_IPADDRESS_IP, "20,2,1,99");
	SetDlgItemText(IDC_EDIT_PORT, "37777");
	SetDlgItemText(IDC_EDIT_USER, "admin");
	SetDlgItemText(IDC_EDIT_PWD, "admin");
	SetDlgItemInt(IDC_EDIT_SERIAL, 1);
	SetDlgItemInt(IDC_EDIT_SNAPINT, 5);
	((CButton*)GetDlgItem(IDC_RADIO_SNAPONE))->SetCheck(1);
	m_snapmode = 0;

	InitNetSDK();
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CCapturePictureDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CCapturePictureDlg::OnPaint() 
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
HCURSOR CCapturePictureDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CCapturePictureDlg::OnButtonLogin() 
{
	// TODO: Add your control notification handler code here
	int nError = 0;
	int nPort = 0;
	char szIp[32] = {0};
	char szUser[32] = {0};
	char szPwd[32] = {0};
	NET_DEVICEINFO deviceInfo;

	nPort = GetDlgItemInt(IDC_EDIT_PORT);
	GetDlgItemText(IDC_IPADDRESS_IP, szIp, 32);
	GetDlgItemText(IDC_EDIT_USER, szUser, 32);
	GetDlgItemText(IDC_EDIT_PWD, szPwd, 32);

	LONG lRet = CLIENT_Login(szIp, nPort, szUser, szPwd, &deviceInfo, &nError);
	if(0 == lRet)
	{
		//Display log in failure reason
		ShowLoginErrorReason(nError);
	}
	else
	{
		m_LoginID = lRet;
		m_nChannelCount = deviceInfo.byChanNum;

		int nIndex = -1;
		m_ctlChannel.ResetContent();
		for (int i=0; i<m_nChannelCount; i++)
		{
			CString str;
			str.Format("%d",i);
			nIndex = m_ctlChannel.AddString(str);
			m_ctlChannel.SetItemData(nIndex,i);
		}
		m_ctlChannel.SetCurSel(0);

		GetDlgItem(IDC_BUTTON_LOGIN)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_LOGOUT)->EnableWindow(TRUE);

		//query json ability.
		DH_DEV_ENABLE_INFO devEnable = {0};
		DWORD dwRetLen = 0;
		BOOL bRet = CLIENT_QuerySystemInfo(m_LoginID, ABILITY_DEVALL_INFO, (char*)&devEnable, 
										sizeof(DH_DEV_ENABLE_INFO), (int*)&dwRetLen, 1000);
		if(bRet == FALSE)
		{
			AfxMessageBox(ConvertString("Query device ability failed.")); 
			return;
		}
		m_bJSON = devEnable.IsFucEnable[EN_JSON_CONFIG]>0?TRUE:FALSE;

		if(m_bJSON == FALSE)
		{
			int nRetLen = 0;
			int nRet = CLIENT_QueryDevState(lRet, DH_DEVSTATE_SNAP, (char *)&m_stuSnapAttr, sizeof(DH_SNAP_ATTR_EN), &nRetLen, 1000);
			if(nRet == FALSE || nRetLen != sizeof(DH_SNAP_ATTR_EN))
			{
				AfxMessageBox(ConvertString("Get snap ability failled."));
				return;
			}
		}
		else//json
		{
			memset(&m_stuSnapAttrEx, 0, sizeof(m_stuSnapAttrEx));
			memset(&m_stuSnapCfg, 0, sizeof(m_stuSnapCfg));
			InitSnapConfigExUI(0);
		}

		int nRet = CLIENT_GetDevConfig(lRet, DH_DEV_SNAP_CFG, -1, (LPVOID)m_stuSnapCfg, sizeof(DHDEV_SNAP_CFG)*32, &dwRetLen, 1000);
		if(nRet < 0)
		{
			AfxMessageBox(ConvertString("Get snap configure failed."));
			return;
		}
		
		OnSelchangeComboChannel();
	}
}

void CCapturePictureDlg::OnButtonLogout() 
{
	// TODO: Add your control notification handler code here
	if(0 != m_LoginID)
	{
		BOOL bSuccess = CLIENT_Logout(m_LoginID);
		if(bSuccess)
		{
			m_LoginID = 0;
			
			m_ctlChannel.ResetContent();
			m_ccbFrameRate.ResetContent();
			m_ccbQuality.ResetContent();
			m_ccbSize.ResetContent();
			m_ccbSnapMode.ResetContent();
			
			GetDlgItem(IDC_BUTTON_LOGIN)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_LOGOUT)->EnableWindow(FALSE);
			m_bJSON = FALSE;
		}
		else
		{
			MessageBox(ConvertString("Logout failed!"), ConvertString("prompt"));
		}
	}
}

void CCapturePictureDlg::OnButtonStart() 
{
	// TODO: Add your control notification handler code here
	if (0 != m_LoginID)
	{
		BOOL bSuccess = UpdateData(TRUE);
		if (bSuccess)
		{
			if (!m_strCardNum.IsEmpty())
			{
				NET_SNAP_COMMANDINFO SnapInfo = {0};
				memcpy(SnapInfo.szCardInfo, (LPTSTR)(LPCTSTR)m_strCardNum, m_strCardNum.GetLength());
				bSuccess = CLIENT_ControlDevice(m_LoginID, DH_CTRL_CAPTURE_START, &SnapInfo, 3000);
				if (bSuccess)
				{
					MessageBox(ConvertString("trigger snap succeed!"), ConvertString("prompt"));
				}
				else
				{
					MessageBox(ConvertString("trigger snap faild!"), ConvertString("prompt"));
				}
			}
			else
			{
				MessageBox(ConvertString("please input Card ID!"), ConvertString("prompt"));
			}
		}
	}
	else
	{
		MessageBox(ConvertString("plese login first!"), ConvertString("prompt"));
	}
}

void CCapturePictureDlg::OnBtnStartsnap() 
{
	// TODO: Add your control notification handler code here
	if (0 != m_LoginID)
	{
		UpdateData(TRUE);

		if (m_snapmode == -1)
		{
			MessageBox(ConvertString("please select snap mode!"), ConvertString("prompt"));
			return;
		}
		
		//Fill in request structure 
		SNAP_PARAMS snapparams = {0};
		snapparams.Channel = m_ctlChannel.GetCurSel();
		snapparams.mode = m_snapmode;
		snapparams.CmdSerial = GetDlgItemInt(IDC_EDIT_SERIAL);
		
		if( snapparams.mode == 1)
		{
			//Time interval for scheduled snapshot. Use snapshot setup to configure.
			//snapparams.InterSnap = GetDlgItemInt(IDC_EDIT_SNAPINT);
		}
		
		//Send out request.
		BOOL b = CLIENT_SnapPicture(m_LoginID, snapparams);
		if (!b)
		{
			MessageBox(ConvertString("begin to snap failed!"), ConvertString("prompt"));
		}
	}
}

void CCapturePictureDlg::OnBtnStopsnap() 
{
	// TODO: Add your control notification handler code here
	if (0 == m_LoginID) 
	{
		return;
	}
	
	//Fill in stop request.
	SNAP_PARAMS snapparams = {0};
	snapparams.mode = -1;
	snapparams.Channel = m_ctlChannel.GetCurSel();
	snapparams.CmdSerial = GetDlgItemInt(IDC_EDIT_SERIAL);
	
	//Send out request.
	BOOL b = CLIENT_SnapPicture(m_LoginID, snapparams);
	if (!b)
	{
		MessageBox(ConvertString("stop snap failed!"), ConvertString("prompt"));
	}
}

void CALLBACK DisConnectFunc(LONG lLoginID, char *pchDVRIP, LONG nDVRPort, DWORD dwUser)
{
	if(dwUser == 0)
	{
		return;
	}
	
	AfxMessageBox(ConvertString("Netword disconnected!"));
}

void CALLBACK SnapPicRet(LONG ILoginID, BYTE *pBuf, UINT RevLen, UINT EncodeType, DWORD CmdSerial, DWORD dwUser)
{
    CCapturePictureDlg *pThis = (CCapturePictureDlg*)dwUser;
	pThis->OnOnePicture(ILoginID,pBuf,RevLen,EncodeType,CmdSerial);
}

void CCapturePictureDlg::OnOnePicture(LONG ILoginID, BYTE *pBuf, UINT RevLen, UINT EncodeType, UINT CmdSerial)
{
	//Get file path
	char path[1000];
	int filelen = GetModuleFileName(NULL,path,1000);
	int i = filelen;
	while(path[i]!='\\')
	{
        i--;
	}
	path[i + 1] = '\0';

	//Get file name
    CString filepath(path);
	CString filename = "mpeg4.JPG";
	CString strfile = filepath + filename;
	char *pFileName = strfile.GetBuffer(200);

	/* Save image original file */
	FILE *stream;
	if( (stream = fopen((const char*) pFileName, "wb")) != NULL )
	{
		int numwritten = fwrite( pBuf, sizeof( char ), RevLen, stream );
		fclose( stream );
	}
	
	/*Veirfy image encode type. If it is an I frame of mpeg4,then call I frame to decode to BMP to display.*/
	if ( EncodeType == 0)
	{
		int iRet = IFrameToBmp("tmp.bmp",pFileName);
		if (iRet == 1)
		{
			ShowBitmap("tmp.bmp");
		}
	}
	else if (EncodeType == 10)
	{
		ShowBitmap(pFileName);
	}
}

const COLORREF RESERVED_COLOR = RGB(255, 255, 255);
 
const int   HIMETRIC_INCH = 2540;

HBITMAP CCapturePictureDlg::LoadImageFile(LPCSTR szImagePath, int &nWidth, int &nHeight)
{
	USES_CONVERSION;
	IPicture* pPicture = NULL;
	
	HBITMAP hbm = NULL;
	HRESULT hr = OleLoadPicturePath(T2OLE(szImagePath), NULL, 0, RESERVED_COLOR, IID_IPicture, (LPVOID*)&pPicture);
	
	if (pPicture)
	{
		hbm = ExtractBitmap(pPicture,nWidth, nHeight);
		pPicture->Release();
	}
	
	return hbm;
}

HBITMAP CCapturePictureDlg::ExtractBitmap(IPicture *pPicture, int &nWidth, int &nHeight)
{
	ASSERT(pPicture);
	
	if (!pPicture)
		return NULL;
	
	CBitmap bmMem;
	CDC dcMem;
	CDC* pDC = CWnd::GetDesktopWindow()->GetDC();
	
	if (dcMem.CreateCompatibleDC(pDC))
	{
		long hmWidth;
		long hmHeight;
		
		pPicture->get_Width(&hmWidth);
		pPicture->get_Height(&hmHeight);
		
		nWidth = MulDiv(hmWidth, pDC->GetDeviceCaps(LOGPIXELSX), HIMETRIC_INCH);
		nHeight = MulDiv(hmHeight, pDC->GetDeviceCaps(LOGPIXELSY), HIMETRIC_INCH);
		
		if (bmMem.CreateCompatibleBitmap(pDC, nWidth, nHeight))
		{
			CBitmap* pOldBM = dcMem.SelectObject(&bmMem);
			
			HRESULT hr = pPicture->Render(dcMem, 0, 0, nWidth, nHeight, 0, hmHeight, hmWidth, -hmHeight, NULL);
			dcMem.SelectObject(pOldBM);
		}
	}
	
	CWnd::GetDesktopWindow()->ReleaseDC(pDC);
	
	return (HBITMAP)bmMem.Detach();
}

void CCapturePictureDlg::ShowBitmap(LPCSTR filename)
{
	HBITMAP hBitmap; //Define .BMP object handle.
    BITMAP BM; 
    CDC MemDC; 
	
    CClientDC ClientDC(&m_showbmp); 
	
	RECT rect;
	m_showbmp.GetClientRect(&rect);
	
    MemDC.CreateCompatibleDC(&ClientDC); 
	/*
    hBitmap=(HBITMAP)::LoadImage(AfxGetInstanceHandle(),  //Get application handle.
		filename,              // .BMP file name.
		IMAGE_BITMAP,          //File format is Windows .BMP file 
		0,0, LR_LOADFROMFILE); //Get .BMP data from the file.
	*/
	int nWidth = 0;
	int nHeight = 0;
	hBitmap = LoadImageFile(filename, nWidth, nHeight);
    MemDC.SelectObject(hBitmap);
	::GetObject(hBitmap,sizeof(BM),&BM);

	//ClientDC.FillSolidRect(&rect,this->GetDC()->GetBkColor());
    //ClientDC.BitBlt(0,0,(rect.right - rect.left),(rect.bottom - rect.top),&MemDC,0,0,SRCCOPY);
	ClientDC.StretchBlt(0,0,(rect.right - rect.left),(rect.bottom - rect.top),&MemDC,0,0,nWidth,nHeight,SRCCOPY);
	::DeleteObject(hBitmap);
}

void CCapturePictureDlg::InitNetSDK()
{
	BOOL ret = CLIENT_Init(DisConnectFunc, (DWORD)this);
	if (ret)
	{
		CLIENT_SetSnapRevCallBack(SnapPicRet,(DWORD)this);
	}
	else
	{
		MessageBox(ConvertString("initialize SDK failed!"), ConvertString("prompt"));
	}
}

void CCapturePictureDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	CLIENT_Cleanup();

	CDialog::OnClose();
}

void CCapturePictureDlg::ShowLoginErrorReason(int nError)
{
	if(1 == nError)		MessageBox(ConvertString("incorrect password!"), ConvertString("prompt"));
	else if(2 == nError)	MessageBox(ConvertString("no this account! "), ConvertString("prompt"));
	else if(3 == nError)	MessageBox(ConvertString("time out!"), ConvertString("prompt"));
	else if(4 == nError)	MessageBox(ConvertString("account have been logined!"), ConvertString("prompt"));
	else if(5 == nError)	MessageBox(ConvertString("account have been locked!"), ConvertString("prompt"));
	else if(6 == nError)	MessageBox(ConvertString("The user has listed into illegal!"), ConvertString("prompt"));
	else if(7 == nError)	MessageBox(ConvertString("The system is busy!"), ConvertString("prompt"));
	else if(9 == nError)	MessageBox(ConvertString("You Can't find the network server!"), ConvertString("prompt"));
	else	MessageBox(ConvertString("Login falied!"), ConvertString("prompt"));
}

void CCapturePictureDlg::OnRadioSnapone() 
{
	// TODO: Add your control notification handler code here
	m_snapmode = 0;
}

void CCapturePictureDlg::OnRadioSnaptime() 
{
	// TODO: Add your control notification handler code here
	m_snapmode = 1;
}

void CCapturePictureDlg::OnBtnEnable() 
{
	// TODO: Add your control notification handler code here
	if (0 != m_LoginID)
	{
		int nIndex = m_ccbFrameRate.GetCurSel();
		if(nIndex == CB_ERR)
		{
			return;
		}
		int nFrame = m_ccbFrameRate.GetItemData(nIndex);

		DWORD dwRetLen = 0;
		BOOL bSuccess = CLIENT_GetDevConfig(m_LoginID, DH_DEV_SNAP_CFG, -1, m_stuSnapCfg, 16*sizeof(DHDEV_SNAP_CFG), &dwRetLen, 3000);
		if (bSuccess && 0 == dwRetLen % sizeof(DHDEV_SNAP_CFG))
		{
			int nCount = dwRetLen/sizeof(DHDEV_SNAP_CFG);
			nIndex = m_ctlChannel.GetCurSel();
			if(nIndex == CB_ERR)
			{
				return;
			}

			int nChn = m_ctlChannel.GetItemData(nIndex);
			if(nChn > nCount)
			{
				return;
			}	
			
			m_stuSnapCfg[nChn].bTimingEnable = TRUE;
			m_stuSnapCfg[nChn].struSnapEnc[0].byVideoEnable = 1;
			m_stuSnapCfg[nChn].struSnapEnc[0].byFramesPerSec =nFrame;

			bSuccess = CLIENT_SetDevConfig(m_LoginID, DH_DEV_SNAP_CFG, -1, m_stuSnapCfg, nCount*sizeof(DHDEV_SNAP_CFG));
			if (bSuccess)
			{
				MessageBox(ConvertString("Set enable successfully!"), ConvertString("prompt"));
			}
		}
	}
}

void CCapturePictureDlg::OnSelchangeComboChannel() 
{
	// TODO: Add your control notification handler code here
	int nIndex = m_ctlChannel.GetCurSel();
	if(nIndex == CB_ERR)
	{
		return;
	}
	int nChn = m_ctlChannel.GetItemData(nIndex);

	int nSnapMode = 0;
	nIndex = m_ccbSnapMode.GetCurSel();
	if(nIndex != CB_ERR)
	{
		nSnapMode = m_ccbSnapMode.GetItemData(nIndex);
	}

	
	if(m_bJSON == FALSE)
	{
		InitSnapConfigUI(nChn);
		UpdateSnapConfig(nChn, nSnapMode);
	}
	else
	{
		InitSnapConfigExUI(nChn);
		UpdateSnapConfig(nChn, nSnapMode);
	}
}

void CCapturePictureDlg::InitSnapConfigUI(int nChn)
{
	const DH_QUERY_SNAP_INFO& stuSnapInfo = m_stuSnapAttr.stuSnap[nChn];

	m_ccbSnapMode.ResetContent();
	for(int i = 0 ; i < stuSnapInfo.nSnapModeCount; i++)
	{
		if(stuSnapInfo.SnapMode[i] == 0)
		{	
			m_ccbSnapMode.SetItemData(m_ccbSnapMode.AddString(ConvertString("Timing snap")), 0);
		}
		else if(stuSnapInfo.SnapMode[i] == 1)
		{
			m_ccbSnapMode.SetItemData(m_ccbSnapMode.AddString(ConvertString("Trigger snap")), 1);
		}
	}
	//
	m_ccbFrameRate.ResetContent();
	CString csFrame;
	for(i = 0; i < stuSnapInfo.nFramesCount; i++)
	{
		if(stuSnapInfo.Frames[i] > 0)
		{
			csFrame.Format(ConvertString("%d frame pre second"), stuSnapInfo.Frames[i]);
		}
		else if(stuSnapInfo.Frames[i] < 0)
		{
			csFrame.Format(ConvertString("%d second pre frame"), abs(stuSnapInfo.Frames[i]));
		}
		
		m_ccbFrameRate.SetItemData(m_ccbFrameRate.AddString(csFrame), stuSnapInfo.Frames[i]);
	}
	//
	char szSize[32][8] = {"D1",	"HD1", "BCIF", "CIF", "QCIF", "VGA", 
		"QVGA", "SVCD", "QQVGA", "SVGA", "XVGA", 
		"WXGA", "SXGA", "WSXGA", "UXGA", "WUXGA"};
	DWORD dwMask = 0x0001;
	m_ccbSize.ResetContent();
	for(i = 0; i < 16; i++)
	{
		if( (stuSnapInfo.dwVideoStandardMask&dwMask) > 0)
		{
			m_ccbSize.SetItemData(m_ccbSize.AddString(szSize[i]), i);
		}
		dwMask <<= 1;
	}
	//
	CString csQuality;
	m_ccbQuality.ResetContent();
	for(i = 0; i < stuSnapInfo.nPicQualityCount; i++)
	{
		csQuality.Format("%d%s", stuSnapInfo.PictureQuality[i], "%");
		m_ccbQuality.SetItemData(m_ccbQuality.AddString(csQuality), stuSnapInfo.PictureQuality[i]);
	}
}

void CCapturePictureDlg::InitSnapConfigExUI(int nChn)
{
	if(m_stuSnapAttrEx.m_bQueried[nChn] == FALSE)
	{
		char * szSnapAttr = new char[1024*100];
		memset(szSnapAttr, 0, 1024*100);
		DWORD dwRetLen = 0;
		BOOL bRet = CLIENT_GetNewDevConfig(m_LoginID, CFG_CMD_SNAPCAPINFO, nChn, 
										  (char*)szSnapAttr, 1024*100, NULL, 1000);
		if(bRet == FALSE)
		{
			AfxMessageBox(ConvertString("Query device ability failed.")); 
			delete []szSnapAttr;
			return;
		}
		
		bRet = CLIENT_ParseData(CFG_CMD_SNAPCAPINFO, szSnapAttr, 
								(LPVOID)&m_stuSnapAttrEx.m_stuSnapAttrEx[nChn], 
								sizeof(CFG_SNAPCAPINFO_INFO),
								NULL);
		if(bRet == FALSE)
		{
			AfxMessageBox(ConvertString("Parse device ability failed.")); 
			delete []szSnapAttr;
			return;	
		}

		m_stuSnapAttrEx.m_bQueried[nChn] = TRUE;
		delete []szSnapAttr;
	}

	if(m_stuSnapAttrEx.m_bQueried[nChn] == FALSE)
	{
		return;
	}

	const CFG_SNAPCAPINFO_INFO & stuSnapInfo = m_stuSnapAttrEx.m_stuSnapAttrEx[nChn];
	int i = 0;

	m_ccbSnapMode.ResetContent();
	if((stuSnapInfo.dwMode&0x00000001) > 0)
	{	
		m_ccbSnapMode.SetItemData(m_ccbSnapMode.AddString(ConvertString("Timing snap")), 0);
	}
	if((stuSnapInfo.dwMode&0x00000002) > 0)
	{
		m_ccbSnapMode.SetItemData(m_ccbSnapMode.AddString(ConvertString("Trigger snap")), 1);
	}

	//
	CString csFrame;
	m_ccbFrameRate.ResetContent();
	for(i = 0; i < stuSnapInfo.dwFramesPerSecNum; i++)
	{
		if(stuSnapInfo.nFramesPerSecList[i] > 0)
		{
			csFrame.Format(ConvertString("%d frame pre second"), stuSnapInfo.nFramesPerSecList[i]);
		}
		else if(stuSnapInfo.nFramesPerSecList[i] < 0)
		{
			csFrame.Format(ConvertString("%d second pre frame"), abs(stuSnapInfo.nFramesPerSecList[i]));
		}
		
		m_ccbFrameRate.SetItemData(m_ccbFrameRate.AddString(csFrame), stuSnapInfo.nFramesPerSecList[i]);
	}
// 
// 		IMAGE_SIZE_D1,								// 704*576(PAL)  704*480(NTSC)
// 		IMAGE_SIZE_HD1,								// 352*576(PAL)  352*480(NTSC)
// 		IMAGE_SIZE_BCIF,							// 704*288(PAL)  704*240(NTSC)
// 		IMAGE_SIZE_CIF,								// 352*288(PAL)  352*240(NTSC)
// 		IMAGE_SIZE_QCIF,							// 176*144(PAL)  176*120(NTSC)
// 		IMAGE_SIZE_VGA,								// 640*480
// 		IMAGE_SIZE_QVGA,							// 320*240
// 		IMAGE_SIZE_SVCD,							// 480*480
// 		IMAGE_SIZE_QQVGA,							// 160*128
// 		IMAGE_SIZE_SVGA,							// 800*592
// 		IMAGE_SIZE_XVGA,							// 1024*768
// 		IMAGE_SIZE_WXGA,							// 1280*800
// 		IMAGE_SIZE_SXGA,							// 1280*1024  
// 		IMAGE_SIZE_WSXGA,							// 1600*1024  
// 		IMAGE_SIZE_UXGA,							// 1600*1200
// 		IMAGE_SIZE_WUXGA,							// 1920*1200
// 		IMAGE_SIZE_LTF,								// 240*192
// 		IMAGE_SIZE_720,								// 1280*720
// 		IMAGE_SIZE_1080,							// 1920*1080
// 		IMAGE_SIZE_1_3M,							// 1280*960
// 		IMAGE_SIZE_NR  

	//
	char szSize[32][8] = {	"D1",	"HD1",	"BCIF",	"CIF",	"QCIF", "VGA", 
							"QVGA", "SVCD",	"QQVGA","SVGA",	"XVGA", "WXGA", 
							"SXGA", "WSXGA","UXGA", "WUXGA","LTF",	"720",
							"1080",	"1_3M"};
	int nSizeIndex = 0;
	m_ccbSize.ResetContent();
	for(i = 0; i < stuSnapInfo.dwIMageSizeNum; i++)
	{
		nSizeIndex = stuSnapInfo.emIMageSizeList[i];
		m_ccbSize.SetItemData(m_ccbSize.AddString(szSize[nSizeIndex]), nSizeIndex);
	}
	//
	//IMAGE_QUALITY_Q10 = 1,							// 图像质量10%
	//IMAGE_QUALITY_Q30,								// 图像质量30%
	//IMAGE_QUALITY_Q50,								// 图像质量50%
	//IMAGE_QUALITY_Q60,								// 图像质量60%
	//IMAGE_QUALITY_Q80,								// 图像质量80%
	//IMAGE_QUALITY_Q100,								// 图像质量100%
	int nMapQuality[7] = {0, 10, 30, 50, 60, 80, 100};
	CString csQuality;
	m_ccbQuality.ResetContent();
	for(i = 0; i < stuSnapInfo.dwQualityMun; i++)
	{
		if(stuSnapInfo.emQualityList[i] > 0 && stuSnapInfo.emQualityList[i] < 7)
		csQuality.Format("%d%s", nMapQuality[stuSnapInfo.emQualityList[i]], "%");
		m_ccbQuality.SetItemData(m_ccbQuality.AddString(csQuality), nMapQuality[stuSnapInfo.emQualityList[i]]);
	}
}

void CCapturePictureDlg::UpdateSnapConfig(int nChn, int nSnapMode)
{
	if(nChn < 0 || nChn > 32 || nSnapMode < 0 || nSnapMode > 1)
	{
		return;
	}
	
	if((m_stuSnapCfg[nChn].bTimingEnable <= 1 && 
		m_stuSnapCfg[nChn].bTimingEnable >= 0 && 
		0 == nSnapMode))
	{
		((CButton *)GetDlgItem(IDC_CHECK_TIME_ENABLE))->ShowWindow(TRUE);
		((CButton *)GetDlgItem(IDC_CHECK_TIME_ENABLE))->SetCheck(m_stuSnapCfg[nChn].bTimingEnable);
	}
	else
	{
		((CButton *)GetDlgItem(IDC_CHECK_TIME_ENABLE))->ShowWindow(FALSE);
	}
	
	int nCount = m_ccbSnapMode.GetCount();
	if( nCount > 0 && nCount >= nSnapMode)
	{
		for(int i = 0; i < nCount; i++)
		{
			if(m_ccbSnapMode.GetItemData(i) == nSnapMode)
			{
				m_ccbSnapMode.SetCurSel(i);
				break;
			}
		}
	}
	
	nCount = m_ccbFrameRate.GetCount();
	if(nCount > 0)
	{
		for(int i = 0; i < nCount; i++)
		{
			if(m_ccbFrameRate.GetItemData(i) == (char)m_stuSnapCfg[nChn].struSnapEnc[nSnapMode].byFramesPerSec)
			{
				m_ccbFrameRate.SetCurSel(i);
				break;
			}
		}
	}
	
	nCount = m_ccbSize.GetCount();
	if(nCount > 0)
	{
		for(int i = 0; i < nCount; i++)
		{
			if(m_ccbSize.GetItemData(i) == m_stuSnapCfg[nChn].struSnapEnc[nSnapMode].byImageSize)
			{
				m_ccbSize.SetCurSel(i);
				break;
			}
		}
	}
	
	nCount = m_ccbQuality.GetCount();
	if(nCount > 0)
	{
		for(int i = 0; i < nCount; i++)
		{
			if(m_ccbQuality.GetItemData(i) == m_stuSnapCfg[nChn].struSnapEnc[nSnapMode].byImageQlty)
			{
				m_ccbQuality.SetCurSel(i);
				break;
			}
		}
	}
}

void CCapturePictureDlg::OnBtnGet() 
{
	// TODO: Add your control notification handler code here
	DWORD dwRetLen = 0;
	int nRet = CLIENT_GetDevConfig(m_LoginID, DH_DEV_SNAP_CFG, 0, (LPVOID)m_stuSnapCfg, sizeof(DHDEV_SNAP_CFG)*32, &dwRetLen, 1000);
	if(nRet < 0)
	{
		AfxMessageBox(ConvertString("Get snap configure failed!"));
		return;
	}				

	OnSelchangeComboChannel();
}

void CCapturePictureDlg::OnBTNSet() 
{
	// TODO: Add your control notification handler code here
	int nIndex = m_ctlChannel.GetCurSel();
	if(nIndex == CB_ERR)
	{
		return;
	}
	int nChn = m_ctlChannel.GetItemData(nIndex);
	
	int nRet = CLIENT_SetDevConfig(m_LoginID, DH_DEV_SNAP_CFG, nChn, (LPVOID)m_stuSnapCfg, sizeof(DHDEV_SNAP_CFG)*32, 1000);
	if(nRet < 0)
	{
		AfxMessageBox(ConvertString("Set snap configure failed!"));
	}
}

void CCapturePictureDlg::OnSelchangeComboSnapMode() 
{
	// TODO: Add your control notification handler code here
	OnSelchangeComboChannel();
}

void CCapturePictureDlg::OnSelchangeComboSnapSize() 
{
	int nSnapMode = 0;
	int nIndex = m_ccbSnapMode.GetCurSel();
	if(nIndex != CB_ERR)
	{
		nSnapMode = m_ccbSnapMode.GetItemData(nIndex);
		
		nIndex = m_ccbSize.GetCurSel();
		if(nIndex != CB_ERR)
		{
			int nIndexChl = m_ctlChannel.GetCurSel();
			if(nIndexChl == CB_ERR)
			{
				return;
			}
			int nChl = m_ctlChannel.GetItemData(nIndexChl);
			m_stuSnapCfg[nChl].struSnapEnc[nSnapMode].byImageSize = m_ccbSize.GetItemData(nIndex);
		}
	}
}

void CCapturePictureDlg::OnSelchangeComboSnapSpace() 
{
	// TODO: Add your control notification handler code here
	int nSnapMode = 0;
	int nIndex = m_ccbSnapMode.GetCurSel();
	if(nIndex != CB_ERR)
	{
		nSnapMode = m_ccbSnapMode.GetItemData(nIndex);
		
		nIndex = m_ccbFrameRate.GetCurSel();
		if(nIndex != CB_ERR)
		{
			int nIndexChl = m_ctlChannel.GetCurSel();
			if(nIndexChl == CB_ERR)
			{
				return;
			}
			int nChl = m_ctlChannel.GetItemData(nIndexChl);
			m_stuSnapCfg[nChl].struSnapEnc[nSnapMode].byFramesPerSec = m_ccbFrameRate.GetItemData(nIndex);
		}
	}	
}

void CCapturePictureDlg::OnSelchangeComboSnapQuality() 
{
	UpdateData(TRUE);
	int nSnapMode = 0;
	int	nIndex = m_ccbSnapMode.GetCurSel();
	if(nIndex != CB_ERR)
	{
		nSnapMode = m_ccbSnapMode.GetItemData(nIndex);
		
		nIndex = m_ccbQuality.GetCurSel();
		if(nIndex != CB_ERR)
		{
			int nIndexChl = m_ctlChannel.GetCurSel();
			if(nIndex == CB_ERR)
			{
				return;
			}
			int nChl = m_ctlChannel.GetItemData(nIndexChl);
			m_stuSnapCfg[nChl].struSnapEnc[nSnapMode].byImageQlty = m_ccbQuality.GetItemData(nIndex);
		}
	}	
}

void CCapturePictureDlg::OnCheckTimeEnable() 
{
	// TODO: Add your control notification handler code here
	int nSnapMode = 0;
	int	nIndex = m_ccbSnapMode.GetCurSel();
	if(nIndex != CB_ERR)
	{
		nSnapMode = m_ccbSnapMode.GetItemData(nIndex);
		if(nSnapMode != 0)
		{
			return;
		}
		
		UpdateData(TRUE);
		
		int nIndex = m_ctlChannel.GetCurSel();
		if(nIndex == CB_ERR)
		{
			return;
		}
		int nChl = m_ctlChannel.GetItemData(nIndex);
		
		m_stuSnapCfg[nChl].bTimingEnable = m_bEnable;
		m_stuSnapCfg[nChl].struSnapEnc[0].byAudioEnable = m_bEnable;
		m_stuSnapCfg[nChl].struSnapEnc[0].byVideoEnable = m_bEnable;
		m_stuSnapCfg[nChl].struSnapEnc[1].byAudioEnable = m_bEnable;
		m_stuSnapCfg[nChl].struSnapEnc[1].byVideoEnable = m_bEnable;
	}
}
