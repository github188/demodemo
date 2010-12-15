
#include "stdafx.h"
#include <mmsystem.h>
#include "player264demo.h"
#include "player264demoDlg.h"

#include "Locate.h"
#include "MultiDisplay.h"
#include "SYSINFO.h"
#include "cutfile.h"
#include "multilanguage.h"
#include "MediaIfoDlg.h"
#include <math.h>

#include "dhplay.h"

#define DEMOVERSION 0x02001208

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define WM_FILE_END WM_USER+100 
#define ISCONVERT TRUE

#define FILE_MODE_FILE			3
#define FILE_MODE_STREAM		STREAME_FILE

int m_nDecType = 3;  //Dec CallBack Stream Type
/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About
void CALLBACK DrawFun(long nPort,HDC hDc,LONG nUser) ;
void CALLBACK FileRefDone(DWORD nReserved,DWORD nUser);
void CALLBACK FileRefDoneEx(DWORD nReserved,BOOL bIndexCreated, DWORD nUser);
void CALLBACK EncTypeChange(long nPort,long User);
void CALLBACK funAudio(long nPort, char * pAudioBuf,long nSize, long nStamp, long nType, long nUser) ;
void CALLBACK DecCBFun(long nPort,char * pBuf,long nSize,FRAME_INFO * pFrameInfo, long nReserved1,long nReserved2);
void CALLBACK DecCBFunEx(long nPort,char * pBuf,long nSize,FRAME_INFO * pFrameInfo, long nReserved1,long nReserved2);

FILE* f_audio ;
FILE* f_video ;
FILE* f_complex ;

bool g_IndexCreated = false ;
int g_playspeed = 0 ;

extern CString  g_szPropName;   
extern HANDLE   g_hValue;

void CALLBACK timer_proc(UINT wTimerID, UINT msg, DWORD dwUser, DWORD dw1, DWORD dw2)
{
	CPlayer264demoDlg *pDlg = (CPlayer264demoDlg*)dwUser;
	
	if (!pDlg)
	{
		return;
	}
	
	__try 
	{		
		pDlg->StartDraw();		
	}
	
	__except (0, 1)
	{
		int zgf = 0;
	}
}

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
	virtual BOOL OnInitDialog();
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
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
DWORD WINAPI StreamFile_Proc(LPVOID pParam)
{
	CPlayer264demoDlg* pThis = (CPlayer264demoDlg*)pParam;
	
	pThis->StreamFilePlay_Thread();
	
	return 0;
}
/////////////////////////////////////////////////////////////////////////////
// CPlayer264demoDlg dialog

CPlayer264demoDlg::CPlayer264demoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPlayer264demoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPlayer264demoDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_IsPlaying    = false ;
	m_IsRepeatPlay = false ;
	m_pause        = false ;
	m_playnormal   = false ;
	m_csfileName   = ""    ;
	m_fileindexlen = 0     ;
	m_bIsFullScreen = 0    ;
	m_nPrePlayPosition = 0 ;
	m_fileindexbuf = NULL ;
}

void CPlayer264demoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPlayer264demoDlg)
	DDX_Control(pDX, IDC_BUTTON_PAUSE, m_BNPause);
	DDX_Control(pDX, IDC_SLIDER_SOUND, m_SoundSlider);
	DDX_Control(pDX, IDC_BUTTON_SETCOLOR, m_BNSetColor);
	DDX_Control(pDX, IDC_FULLSCREEN, m_BNFullScreen);
	DDX_Control(pDX, IDC_BUTTON_TOEND, m_BNToEnd);
	DDX_Control(pDX, IDC_BUTTON_TOBEGIN, m_BNToBegin);
	DDX_Control(pDX, IDC_BUTTON_STOP, m_BNStop);
	DDX_Control(pDX, IDC_BUTTON_SOUND, m_BNSound);
	DDX_Control(pDX, IDC_BUTTON_SLOW, m_BNSlow);
	DDX_Control(pDX, IDC_BUTTON_PICCATCH, m_BNPicCatch);
	DDX_Control(pDX, IDC_BUTTON_ONE, m_BNStepOne);
	DDX_Control(pDX, IDC_BUTTON_BACKONE, m_BNBackOne);
	DDX_Control(pDX, IDC_BUTTON_FAST, m_BNFast);
	DDX_Control(pDX, IDC_BUTTON_PLAY, m_BNPlay);
	DDX_Control(pDX, IDC_SLIDER_PLAY, m_sliderplay);
	DDX_Control(pDX, IDC_STATIC_SHOW, m_playregion);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPlayer264demoDlg, CDialog)
	//{{AFX_MSG_MAP(CPlayer264demoDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_PLAY, OnPlay)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_BN_CLICKED(IDC_BUTTON_FAST, OnFast)
	ON_BN_CLICKED(IDC_BUTTON_SLOW, OnSlow)
	ON_BN_CLICKED(IDC_BUTTON_BACKONE, OnBackone)
	ON_BN_CLICKED(IDC_BUTTON_ONE, OnOne)
	ON_BN_CLICKED(IDC_BUTTON_TOBEGIN, OnTobegin)
	ON_BN_CLICKED(IDC_BUTTON_TOEND, OnToend)
	ON_BN_CLICKED(IDC_BUTTON_PICCATCH, OnPiccatch)
	ON_BN_CLICKED(IDC_BUTTON_STOP, OnStop)
	ON_COMMAND(ID_FILE_CLOSE, OnFileClose)
	ON_COMMAND(ID_ABOUT, OnAbout)
	ON_WM_TIMER()
	ON_WM_HSCROLL()
	ON_COMMAND(ID_LOCATE, OnLocate)
	ON_COMMAND(ID_REPEAT, OnRepeat)
	ON_MESSAGE(WM_FILE_END, PlayMessage)	
	ON_COMMAND(ID_RESETBUF, OnResetbuf)
	ON_COMMAND(ID_EXIT, OnExit)
	ON_WM_SIZE()
	ON_WM_MOVE()
	ON_WM_DESTROY()
	ON_COMMAND(ID_PART_DISPLAY, OnPartDisplay)
	ON_COMMAND(IDC_SYSINFO, OnSysinfo)
	ON_COMMAND(IDC_VERTICALSYNC, OnVerticalSync)
	ON_COMMAND(IDC_CUTFILE, OnCutfile)
	ON_BN_CLICKED(IDC_STATIC_SHOW, OnStaticShow)
	ON_BN_CLICKED(IDC_FULLSCREEN, OnFullscreen)
	ON_WM_LBUTTONDBLCLK()
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_PLAY, OnCustomdrawSliderPlay)
	ON_WM_SHOWWINDOW()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_SOUND, OnCustomdrawSliderSound)
	ON_COMMAND(ID_DEC_AUDIO, OnDecAudio)
	ON_COMMAND(ID_DEC_VIDEO, OnDecVideo)
	ON_COMMAND(ID_DEC_COMPLEX, OnDecComplex)
	ON_COMMAND(ID_DEC_NONE, OnDecNone)
	ON_COMMAND(ID_MENUITEM_Chinese, OnLanChinese)
	ON_COMMAND(ID_MENUITEM_English, OnLanEnglish)
	ON_WM_CLOSE()
	ON_WM_DROPFILES()
	ON_COMMAND(ID_PIC_JPG, OnPicTypeSet)
	ON_COMMAND(ID_OPT_WATERMARK, OnWatermarkCheck)
	ON_COMMAND(ID_FILEMODE_FILE, OnFilePlayMode)
	ON_COMMAND(ID_FILEMODE_STREAM, OnFilePlayMode)
	ON_COMMAND(ID_INFO_MEDIA, OnMediaInfo)
	ON_COMMAND(ID_OPT_AVICONVER, OnAVIConvert)
	ON_COMMAND(ID_SET_PICPATH, OnPicPathSet)
	ON_COMMAND(ID_MENUITEM_AUDIOIN, OnCaptureSound)
	ON_BN_CLICKED(IDC_BUTTON_SOUND, OnSound)
	ON_BN_CLICKED(IDC_BUTTON_SETCOLOR, OnButtonSetcolor)
	ON_WM_VSCROLL()
	ON_COMMAND(ID_PIC_BMP, OnPicTypeSet)
	ON_BN_CLICKED(IDC_BUTTON_PAUSE, OnPause)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////

BOOL CPlayer264demoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	SetProp(m_hWnd, "dhplaydemo", (HANDLE)1);

	DragAcceptFiles(true);

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
	m_bFileOpen = FALSE;

	m_pMainMenu = GetMenu() ;

	m_bitmap.LoadBitmap(IDB_BITMAP) ;

	m_BNPlay.LoadBitmap(IDB_BITMAP_PLAY);
	m_BNPause.LoadBitmap(IDB_BITMAP_PAUSE);
	m_BNStop.LoadBitmap(IDB_BITMAP_STOP);
	m_BNToEnd.LoadBitmap(IDB_BITMAP_SEEKTOEND);
	m_BNBackOne.LoadBitmap(IDB_BITMAP_STEPBACKWARD);
	m_BNStepOne.LoadBitmap(IDB_BITMAP_STEPFORWARD);
	m_BNSlow.LoadBitmap(IDB_BITMAP_SLOW);
	m_BNFast.LoadBitmap(IDB_BITMAP_FAST);
	m_BNToBegin.LoadBitmap(IDB_BITMAP_SEEKTOSTART);
	m_BNPicCatch.LoadBitmap(IDB_BITMAP_CAMERA);
	m_BNFullScreen.LoadBitmap(IDB_BITMAP_FULLSCREEN);
	m_BNSound.LoadBitmap(IDB_BITMAP_SOUND);
	m_BNSetColor.LoadBitmap(IDB_BITMAP_SETCOLOR);

	SetButtonsState(FALSE);

	PLAY_SetFileEndMsg(PORT,m_hWnd,WM_FILE_END);
	PLAY_SetDisplayBuf(PORT,15) ;

	//Timer
	//SetTimer(1,1000,NULL) ;
	timeBeginPeriod(10);
	m_iTimerID = timeSetEvent(1000, 3, timer_proc, (unsigned int)this, TIME_PERIODIC|TIME_CALLBACK_FUNCTION);

	//File Slider Control
	
	m_sliderplay.SetRangeMin(0);
	m_sliderplay.SetRangeMax(200);
	m_sliderplay.SetPos(0);
	m_sliderplay.SetLineSize(1);
	m_sliderplay.SetPageSize(5);

	//Audio Slider Control
	m_SoundSlider.SetRangeMin(0);
	m_SoundSlider.SetRangeMax(0xffff-1);
	int volume = PLAY_GetVolume(PORT) ;
	m_SoundSlider.SetPos(volume);
	m_SoundSlider.SetLineSize(1);
	m_SoundSlider.SetPageSize(5);

	m_BMPOverlay.LoadBitmap(IDB_OVERLAY);
	m_BMPBlack.LoadBitmap(IDB_BITMAP);

	m_bNeedFileClose = 0;
	m_nDecType = 0;	

	m_nWidth = 704;
	m_nHeight = 576;

// 	f_audio = fopen("c:\\sys_audio.pcm","w+b") ;
// 	f_video = fopen("C:\\sys_video.264","w+b") ;
// 	f_complex = fopen("C:\\sys_complex.data","w+b") ;
	
	GetCurrentDirectory(1024, g_strCurDir.GetBuffer(1024)) ;

 	OnLanEnglish() ;

	m_dwFileLen = 0;
	m_dwCurFilePos = 0;
	m_iFilePlayMode = FILE_MODE_FILE;
	m_hPlayThread = NULL;
	m_bSrcFileChange = FALSE;

	m_tPlayState = PlayState_READY;

	m_csPicSavePath = "c:\\";

	m_pause = false;
	m_bPlaySound = TRUE;	

	m_WaterCheckDlg = NULL;
	m_AVIConvDlg = NULL;
	m_SetColorDlg = NULL;
	m_SetPicPathDlg = NULL;
	m_CaptureSoundDlg = NULL;
	m_pMainMenu->EnableMenuItem(ID_INFO_MEDIA, MF_GRAYED);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

CPlayer264demoDlg::~CPlayer264demoDlg()
{
	if (m_WaterCheckDlg != NULL)
	{
		delete m_WaterCheckDlg;
		m_WaterCheckDlg = NULL;
	}

	if (m_AVIConvDlg != NULL)
	{
		delete m_AVIConvDlg;
		m_AVIConvDlg = NULL;
	}

	if (m_SetColorDlg != NULL)
	{
		delete m_SetColorDlg;
		m_SetColorDlg = NULL;
	}
	
	if (m_SetPicPathDlg != NULL)
	{
		delete m_SetPicPathDlg;
		m_SetPicPathDlg = NULL;
	}

	if (m_CaptureSoundDlg != NULL)
	{
		delete m_CaptureSoundDlg;
		m_CaptureSoundDlg = NULL;
	}
}

void CPlayer264demoDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CPlayer264demoDlg::OnPaint() 
{
	
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

	//	SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

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
	};
	
	
// 	if(m_IsPlaying)
// 	{	
// 		PLAY_RefreshPlay(PORT);
// 		SetWindowSize();
// 	}
// 	else;	

	//PLAY_RefreshPlay(PORT);

	CString csPlayState = "";
	CString csTimeState = "";

	DrawPlayState(csPlayState, csTimeState);
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CPlayer264demoDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CPlayer264demoDlg::SetButtonsState(BOOL bEnable)
{
	m_BNPlay.EnableWindow(bEnable);
	m_BNPause.EnableWindow(bEnable);
	m_BNStop.EnableWindow(bEnable);
	m_BNToEnd.EnableWindow(bEnable);
	m_BNBackOne.EnableWindow(bEnable);
	m_BNStepOne.EnableWindow(bEnable);
	m_BNSlow.EnableWindow(bEnable);
	m_BNToBegin.EnableWindow(bEnable);
	m_BNFast.EnableWindow(bEnable);
	m_BNPicCatch.EnableWindow(bEnable);
	m_BNFullScreen.EnableWindow(bEnable);
	if (m_bPlaySound) m_BNSound.EnableWindow(bEnable);
	m_BNSetColor.EnableWindow(bEnable);
}

void CPlayer264demoDlg::OnPlay() 
{	
	if (!m_bFileOpen) return ;

	if (m_pause)
	{
		m_pause = false;
		//m_BNPlay.SetState(TRUE);
		m_BNPause.SetState(FALSE);
		PLAY_Pause(PORT, 0);
		if (g_playspeed > 0)
		{
			m_tPlayState = PlayState_FAST;
		}
		else if (g_playspeed < 0)
		{
			m_tPlayState = PlayState_Slow;
		}
		else
		{
			m_BNPlay.SetState(TRUE);
			m_tPlayState = PlayState_Playing;
		}
		
		return;
	}

	if (m_iFilePlayMode == FILE_MODE_STREAM)
	{
		if (!m_IsPlaying)
		{
			m_IsPlaying = true;
			m_hPlayThread = NULL;
			PlayStreamFile(m_csfileName);

			m_BNPlay.SetState(TRUE);
			m_BNPause.SetState(FALSE);
			m_tPlayState = PlayState_Playing;
		}
		else
		{
			m_playnormal = true;
			g_playspeed = 0;

			PLAY_Play(PORT, m_playregion.m_hWnd);

			m_BNPlay.SetState(TRUE);
			m_BNPause.SetState(FALSE);
			m_tPlayState = PlayState_Playing;		
		}

		m_BNSlow.EnableWindow(TRUE);
		m_BNFast.EnableWindow(TRUE);

		return;
	}

	if (m_tPlayState == PlayState_Playing)
	{
		m_BNPlay.SetState(TRUE);
		m_BNPause.SetState(FALSE);
		m_tPlayState = PlayState_Playing;

		return;
	}

	PLAY_SetDecCallBack(PORT,DecCBFun) ;		//Set Dec CallBack

//	PLAY_SetDecCallBack(PORT,DecCBFunEx);
	PLAY_SetDecCBStream(PORT,m_nDecType);	////////////////////////
	
// 	PLAY_SetOverlayMode(PORT,TRUE,RGB(255,0,255));

	if (m_bPlaySound) PLAY_PlaySound(PORT);

	PLAY_Play(PORT,m_playregion.m_hWnd);

	if (!m_IsPlaying)
	{
		PLAY_SetColor(PORT, 0, 64, 64, 64, 64);
	}

	UINT state = m_pMainMenu->GetMenuState(IDC_VERTICALSYNC, MF_BYCOMMAND);
	ASSERT(state != 0xFFFFFFFF);
	PLAY_VerticalSyncEnable(PORT, (state & MF_CHECKED)? TRUE : FALSE);
	
	m_IsPlaying = true ;
	m_playnormal = true ;
	g_playspeed = 0 ;

	SetButtonsState(TRUE);

	m_BNPlay.SetState(TRUE);
	m_BNPause.SetState(FALSE);
	m_tPlayState = PlayState_Playing;

	if (!g_IndexCreated)
	{
		m_BNBackOne.EnableWindow(FALSE);
	}

	m_pMainMenu->EnableMenuItem(ID_FILE_CLOSE,MF_ENABLED) ;
	m_pMainMenu->EnableMenuItem(ID_LOCATE,MF_ENABLED) ;
	m_pMainMenu->EnableMenuItem(ID_SEARCH,MF_ENABLED) ;
	m_pMainMenu->EnableMenuItem(ID_RESETBUF,MF_ENABLED) ;
	m_pMainMenu->EnableMenuItem(ID_PART_DISPLAY,MF_ENABLED) ;
	m_pMainMenu->EnableMenuItem(ID_DEC_AUDIO,MF_GRAYED) ;
	m_pMainMenu->EnableMenuItem(ID_DEC_VIDEO,MF_GRAYED) ;
	m_pMainMenu->EnableMenuItem(ID_DEC_COMPLEX,MF_GRAYED) ;
	m_pMainMenu->EnableMenuItem(ID_INFO_MEDIA,MF_ENABLED);

	
	if((HBITMAP)m_BMPOverlay!=m_playregion.GetBitmap())
		m_playregion.SetBitmap(m_BMPOverlay);
	
	m_bNeedFileClose = 1;

	UpdateData(0);
	SetWindowSize();
}

void CPlayer264demoDlg::OnFileOpen() 
{
	CString tempfilename ;

	CFileDialog FileChooser(TRUE, 
					NULL,
					NULL, 
					OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
					"All files(*.*)|*.*|h264 Files (*.264)|*.264|Mpeg4 Files (*.mp4)|*.mp4|LB Files (*.dav)|*.dav|HB Files (*.mpeg)|*.mpeg||");
	//choose file
	if (FileChooser.DoModal()==IDOK)
	{
		tempfilename = FileChooser.GetPathName() ;
	}
	else 
	{
		return ;
	}

	m_bFileOpen = TRUE;
	m_bPlaySound = TRUE;

	if (m_iFilePlayMode == FILE_MODE_FILE)
	{
		PlayFile(tempfilename);
	}
	else if (m_iFilePlayMode == FILE_MODE_STREAM)
	{
		g_IndexCreated = false;
		m_BNPlay.SetState(TRUE);
		m_BNPause.SetState(FALSE);
		m_tPlayState = PlayState_Playing;

		if (!m_IsPlaying)
		{
			m_hPlayThread = NULL;
		}

		m_IsPlaying = true;
		m_csfileName = tempfilename;
		PlayStreamFile(m_csfileName);
	}
}

void CPlayer264demoDlg::PlayFile(CString csFilePath)
{
	
	// TODO: Add your command handler code here
	if (m_IsPlaying)
	{		
		OnStop() ;					
	}

	if (m_bNeedFileClose == 1)
	{
		PLAY_CloseFile(PORT) ;
		m_bNeedFileClose = 0;
	}

	g_IndexCreated = false ;
	m_IsPlaying    = false ;
	m_pause        = false ;
	m_playnormal   = false ;		

	UpdateData(0);

//	PLAY_SetFileRefCallBack(PORT,FileRefDone,(DWORD)this);   
	PLAY_SetFileRefCallBackEx(PORT,FileRefDoneEx,(DWORD)this); 
	PLAY_SetEncTypeChangeCallBack(PORT,EncTypeChange,(DWORD)this);
//	PLAY_SetPicQuality(PORT,TRUE);
	//PLAY_SetPicQuality(PORT,FALSE);
//	BOOL ifOk;
//	PLAY_GetPictureQuality(PORT,&ifOk);

//	m_pMainMenu->EnableMenuItem(ID_FILE_OPEN,MF_GRAYED) ;

	if (csFilePath == m_csfileName && m_fileindexlen > 0)//If the file is the same as the last file，and the last file can create index
	{
		//cope the file index of last file 	
		PLAY_SetRefValue(PORT,m_fileindexbuf,m_fileindexlen) ;

		if (!PLAY_OpenFile(PORT,(LPSTR)(LPCTSTR)m_csfileName))
		{			
			AfxMessageBox(ConvertString("File Open Error"));
			return;
		}
	}
	else 
	{
		m_csfileName = csFilePath ;
		if (m_fileindexbuf != NULL)
		{
			delete m_fileindexbuf ;
			m_fileindexbuf = NULL ;
		}
		m_fileindexlen = 0 ;
		if (!PLAY_OpenFile(PORT,(LPSTR)(LPCTSTR)m_csfileName))
		{
			AfxMessageBox(ConvertString("File Open Error"));
			return;
		}
	}
	
	m_pMainMenu->EnableMenuItem(IDC_CUTFILE,MF_ENABLED) ;
	m_pMainMenu->CheckMenuItem(IDC_VERTICALSYNC, MF_UNCHECKED | MF_BYCOMMAND);

	OnPlay() ;	
}

void CPlayer264demoDlg::PlayStreamFile(CString csFilePath)
{
	m_IsPlaying = true;
	SetButtonsState(TRUE);
	m_BNBackOne.EnableWindow(FALSE);
	m_BNStepOne.EnableWindow(FALSE);

	if (m_hPlayThread != NULL)
	{
		m_bSrcFileChange = TRUE;
	}
	m_csfileName = csFilePath;

	if (m_pause)
	{
		m_BNPlay.SetState(FALSE);
		m_BNPause.SetState(TRUE);
		m_tPlayState = PlayState_Pause;
	}

	DWORD dwThreadId = 0;
	if (m_hPlayThread == NULL)
	{
		m_hPlayThread = CreateThread(NULL, 0, StreamFile_Proc, this, 0, &dwThreadId);
	}

	m_pMainMenu->EnableMenuItem(ID_FILE_CLOSE,MF_ENABLED);
	m_pMainMenu->EnableMenuItem(ID_SEARCH,MF_ENABLED);
	m_pMainMenu->EnableMenuItem(ID_RESETBUF,MF_ENABLED);
	m_pMainMenu->EnableMenuItem(ID_PART_DISPLAY,MF_ENABLED);
	m_pMainMenu->EnableMenuItem(ID_DEC_AUDIO,MF_GRAYED);
	m_pMainMenu->EnableMenuItem(ID_DEC_VIDEO,MF_GRAYED);
	m_pMainMenu->EnableMenuItem(ID_DEC_COMPLEX,MF_GRAYED);
	m_pMainMenu->EnableMenuItem(ID_INFO_MEDIA,MF_ENABLED);
	
	UpdateData(0);
	SetWindowSize();
}

void CPlayer264demoDlg::StreamFilePlay_Thread()
{
	const int BUFLEN = 8 * 1024;
	BYTE InfoBuf[BUFLEN];
	DWORD nRead;

	PLAY_SetStreamOpenMode(PORT, STREAME_FILE);
	PLAY_OpenStream(PORT, NULL, 0, 1024 * 100);
	PLAY_PlaySound(PORT);
	
	if (!PLAY_Play(PORT, m_playregion.m_hWnd))
	{
		PLAY_CloseStream(PORT);
		goto end;
	}

	if (m_bSrcFileChange)
	{
		m_StreamSrcFile.Close();
	}

	if (!m_StreamSrcFile.Open(m_csfileName, CFile::modeRead | CFile::shareDenyNone) )
	{
		PLAY_Stop(PORT);
		PLAY_CloseStream(PORT);
		goto end;
	}
	
	m_dwFileLen = m_StreamSrcFile.GetLength();
	m_dwCurFilePos = 0;
	
	try
	{
		while (m_IsPlaying)
		{
			if (m_bSrcFileChange)
			{
				// 这里调用了PLAY_Stop，并PLAY_Play重新开始播放
				// 也可以不关闭，只清空相关buffer，但是这样操作很导致PLAY_GetPlayedTimeEx计算出错
				// PLAY_GetPlayedTimeEx 的时间是根据码流的时间戳来计算的，若PLAY_InputData的数据是不是来自同一实时设备或
				// 录像文件，则根据时间戳计算的时间会不准确
				PLAY_ResetBuffer(PORT ,BUF_VIDEO_RENDER);
				PLAY_ResetSourceBuffer(PORT);				
				PLAY_Stop(PORT);

				m_StreamSrcFile.Close();
				
				if (!m_StreamSrcFile.Open(m_csfileName, CFile::modeRead | CFile::shareDenyNone) )
				{
					PLAY_Stop(PORT);
					PLAY_CloseStream(PORT);
					goto end;
				}

				if (!PLAY_Play(PORT, m_playregion.m_hWnd))
				{
					m_StreamSrcFile.Close();
					PLAY_CloseStream(PORT);
					goto end;
				}
				//PLAY_ResetBuffer(PORT ,BUF_VIDEO_RENDER);
				//PLAY_ResetSourceBuffer(PORT);
				m_dwFileLen = m_StreamSrcFile.GetLength();
				m_dwCurFilePos = 0;

				m_bSrcFileChange = FALSE;
			}

			memset(InfoBuf, 0, sizeof(InfoBuf));
			m_StreamSrcFile.Seek(m_dwCurFilePos, SEEK_SET);
			nRead = m_StreamSrcFile.Read(InfoBuf, BUFLEN);
			if (nRead <= 0)
			{
				if (m_IsRepeatPlay && (m_dwCurFilePos >= m_dwFileLen))
				{
					m_dwCurFilePos = 0;
					continue;
				}

				break;
			}
			
			m_dwCurFilePos += nRead;

			if (m_IsRepeatPlay && (m_dwCurFilePos >= m_dwFileLen))
			{
				m_dwCurFilePos = 0;
			}
			
			while (m_IsPlaying && !PLAY_InputData(PORT, InfoBuf, nRead))//阻塞为false
			{
				Sleep(5);
			}
		}
	}
	catch (CException* e)
	{
		PLAY_Stop(PORT);
		PLAY_CloseStream(PORT);
		m_StreamSrcFile.Close();
		e->Delete();
	}

	//解码结束，解码出来的数据都已显示播放或回调完全
	while (m_IsPlaying)
	{
		if (PLAY_GetSourceBufferRemain(PORT) == 0)
		{
			if (PLAY_GetBufferValue(PORT, BUF_VIDEO_RENDER) < 1) 
			{
				if (PLAY_GetBufferValue(PORT, BUF_AUDIO_RENDER) < 1)
				{
					//解码播放结束
					Sleep(5);
					break;
				}
			}
		}
		else
		{
			Sleep(10);
		}
	}
	
	PLAY_Stop(PORT);
	PLAY_CloseStream(PORT);

	m_StreamSrcFile.Close();

end:
	m_playregion.SetBitmap(m_bitmap) ;
	m_pause = false;
	m_IsPlaying = false;
	SetWindowSize();
	
	m_dwCurFilePos = 0;

	m_BNPlay.SetState(FALSE);
	m_BNPause.SetState(FALSE);
	m_tPlayState = PlayState_Stop;
	
	m_pMainMenu->EnableMenuItem(ID_LOCATE,MF_GRAYED) ;
	m_pMainMenu->EnableMenuItem(ID_SEARCH,MF_GRAYED) ;
	m_pMainMenu->EnableMenuItem(ID_RESETBUF,MF_GRAYED) ;
	m_pMainMenu->EnableMenuItem(ID_PART_DISPLAY,MF_GRAYED) ;
	m_pMainMenu->EnableMenuItem(ID_DEC_AUDIO,MF_ENABLED) ;
	m_pMainMenu->EnableMenuItem(ID_DEC_VIDEO,MF_ENABLED) ;
	m_pMainMenu->EnableMenuItem(ID_DEC_COMPLEX,MF_ENABLED) ;
	m_pMainMenu->EnableMenuItem(ID_OPEN_LAST_FILE,MF_ENABLED) ;
	m_pMainMenu->EnableMenuItem(ID_INFO_MEDIA, MF_GRAYED);
}

void CPlayer264demoDlg::OnPause() 
{
	if (!m_bFileOpen || m_pause) 
	{
		m_BNPlay.SetState(FALSE);
		m_BNPause.SetState(TRUE);
		m_tPlayState = PlayState_Pause;
		return ;
	}

	if (!m_IsPlaying)
	{
		return ;
	}

	int z = PLAY_Pause(PORT, 1);
	m_pause = TRUE;
	
	m_BNPlay.SetState(FALSE);
	m_BNPause.SetState(TRUE);
	m_tPlayState = PlayState_Pause;
}

void CPlayer264demoDlg::OnFast() 
{
	if (!m_bFileOpen) return ;

	if (m_IsPlaying)
	{
		m_playnormal = false ;
		int z = PLAY_Fast(PORT) ;

		if ( (++g_playspeed) >= 4 )
		{
			g_playspeed = 4 ;
			m_BNFast.EnableWindow(FALSE) ;
		}
	
		m_BNSlow.EnableWindow(TRUE) ;

		if (g_playspeed == 0)
		{
			m_BNPlay.SetState(TRUE);
			m_BNPause.SetState(FALSE);
			m_tPlayState = PlayState_Playing;
		}
		else if (g_playspeed > 0 && m_tPlayState != PlayState_FAST)
		{
			m_BNPlay.SetState(FALSE);
			m_BNPause.SetState(FALSE);
			m_tPlayState = PlayState_FAST;
		}
	}
}

void CPlayer264demoDlg::OnSlow() 
{
	if (!m_bFileOpen) return ;

	if (m_IsPlaying)
	{
		if (PLAY_Slow(PORT) == FALSE)
		{
			return ;
		}

		m_playnormal = false ;
		
		if ( (--g_playspeed) <= -4 )
		{
			g_playspeed = -4 ;
			m_BNSlow.EnableWindow(FALSE) ;
		}
	
		m_BNFast.EnableWindow(TRUE) ;

		if (g_playspeed == 0)
		{
			m_BNPlay.SetState(TRUE);
			m_BNPause.SetState(FALSE);
			m_tPlayState = PlayState_Playing;
		}
		else if (g_playspeed < 0 && m_tPlayState != PlayState_Slow)
		{
			m_BNPlay.SetState(FALSE);
			m_BNPause.SetState(FALSE);
			m_tPlayState = PlayState_Slow;
		}
	}
}

void CPlayer264demoDlg::OnBackone() 
{
	if (!m_bFileOpen) return ;

	if (m_iFilePlayMode == FILE_MODE_STREAM) return ;

	if (m_IsPlaying)
	{
		m_BNPlay.SetState(FALSE);
		m_BNPause.SetState(TRUE);
		m_tPlayState = PlayState_StepBackWard;
		m_playnormal = false ;
		
		PLAY_OneByOneBack(PORT) ;
	}
}

void CPlayer264demoDlg::OnOne() 
{
	if (!m_bFileOpen) return ;

	if (m_iFilePlayMode == FILE_MODE_STREAM) return ;

	if (m_IsPlaying)
	{
		m_BNPlay.SetState(FALSE);
		m_BNPause.SetState(TRUE);
		m_tPlayState = PlayState_StepForward;

		if (g_playspeed >= 4)
		{
			m_BNFast.EnableWindow(TRUE) ;
		}
		else if (g_playspeed <= -4)
		{
			m_BNSlow.EnableWindow(TRUE) ;
		}

		g_playspeed  = 0;
		m_playnormal = false ;

		PLAY_OneByOne(PORT) ;
	}
}

void CPlayer264demoDlg::OnTobegin() 
{
	if (!m_bFileOpen) return ;

	if (!m_IsPlaying)
	{
		return ;
	}

	if (m_iFilePlayMode == FILE_MODE_STREAM)
	{
		m_dwCurFilePos = 0;
		PLAY_ResetBuffer(PORT ,BUF_VIDEO_RENDER);
		PLAY_ResetSourceBuffer(PORT);
		return;
	}

	if (g_IndexCreated)
	{
		BOOL iRet = PLAY_SetCurrentFrameNum(PORT,0) ;
		if (iRet == FALSE)
		{
			PLAY_SetPlayedTimeEx(PORT,0) ;
		}
	}
	else 
	{
		PLAY_SetPlayPos(PORT,0) ;
	}
}

void CPlayer264demoDlg::OnToend() 
{
	if (!m_bFileOpen) return ;

	if (!m_IsPlaying)
	{
		return ;
	}

	if (m_iFilePlayMode == FILE_MODE_STREAM)
	{
		m_dwCurFilePos = m_dwFileLen;
		PLAY_ResetBuffer(PORT ,BUF_VIDEO_RENDER);
		PLAY_ResetSourceBuffer(PORT);
		return;
	}

	if (g_IndexCreated)
	{
		int endframe = PLAY_GetFileTotalFrames(PORT);

		//如果已在最后位置则返回，并避免颜色参数有调整的情况下图像数据被重复修改
		if (PLAY_GetCurrentFrameNum(PORT) == (endframe-1))
		{
			return;
		}
//		while(!PLAY_SetCurrentFrameNum(PORT,endframe --))
//		{
//			//TRACE("FrameNum is :%d\n",nEndFrame);
//			if(endframe==0)
//				break;
//		}	
		BOOL iRet = PLAY_SetCurrentFrameNum(PORT,endframe - 1) ;
		if (iRet == FALSE)
		{
			PLAY_SetPlayedTimeEx(PORT, (PLAY_GetFileTime(PORT)-1)*1000 ) ;
		}
	
	}
	else 
	{
		PLAY_SetPlayPos(PORT,(float)0.99) ;
	}	

}

void CPlayer264demoDlg::OnPiccatch() 
{
	if (!m_bFileOpen) return ;

	if (!m_IsPlaying)
	{
		return ;
	}

	static int i = 0;

	UINT state = m_pMainMenu->GetMenuState(ID_PIC_JPG, MF_BYCOMMAND);
	ASSERT(state != 0xFFFFFFFF);

	CString csTemp;
	if (g_IndexCreated)
	{
		if (state & MF_CHECKED)
		{
			csTemp.Format("Frame_%d.jpg", PLAY_GetCurrentFrameNum(PORT));
		}
		else
		{
			csTemp.Format("Frame_%d.bmp", PLAY_GetCurrentFrameNum(PORT));
		}
	}
	else
	{
		if (state & MF_CHECKED)
		{
			csTemp.Format("Num%d.jpg", i++);
		}
		else
		{
			csTemp.Format("Num%d.bmp", i++);
		}
	}

	csTemp = m_csPicSavePath + "\\" +csTemp;

	PLAY_CatchPicEx(PORT, csTemp.GetBuffer(0), (state & MF_CHECKED)? PicFormat_JPEG:PicFormat_BMP);
}

void CPlayer264demoDlg::OnStop() 
{
	if (!m_bFileOpen) return ;

	if (m_iFilePlayMode == FILE_MODE_STREAM)
	{
		m_IsPlaying = false;
		return;
	}
	
	if (g_IndexCreated)
	{ 
		PLAY_GetRefValue(PORT,NULL,&m_fileindexlen) ;
		m_fileindexbuf = new BYTE[m_fileindexlen+1] ;
		PLAY_GetRefValue(PORT,m_fileindexbuf,&m_fileindexlen) ;
		
	}
	
	if (m_IsPlaying)
	{
		PLAY_Stop(PORT);
	}
	
	m_playregion.SetBitmap(m_bitmap) ;

	//m_IsPlaying	   = true;
	m_pause = false ;
	m_IsPlaying = false ;
	SetWindowSize();

	m_BNPlay.SetState(FALSE);
	m_BNPause.SetState(FALSE);
	m_tPlayState = PlayState_Stop;
	
	m_pMainMenu->EnableMenuItem(ID_LOCATE,MF_GRAYED) ;
	m_pMainMenu->EnableMenuItem(ID_SEARCH,MF_GRAYED) ;
	m_pMainMenu->EnableMenuItem(ID_RESETBUF,MF_GRAYED) ;
	m_pMainMenu->EnableMenuItem(ID_PART_DISPLAY,MF_GRAYED) ;

	m_pMainMenu->EnableMenuItem(ID_DEC_AUDIO,MF_ENABLED) ;
	m_pMainMenu->EnableMenuItem(ID_DEC_VIDEO,MF_ENABLED) ;
	m_pMainMenu->EnableMenuItem(ID_DEC_COMPLEX,MF_ENABLED) ;
	
	m_pMainMenu->EnableMenuItem(ID_OPEN_LAST_FILE,MF_ENABLED) ;

	m_pMainMenu->EnableMenuItem(ID_INFO_MEDIA, MF_GRAYED);
//	m_playregion.SetBitmap(m_BMPBlack);
}

BOOL CAboutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	DWORD nVer=PLAY_GetSdkVersion();
	DWORD nVersion=(nVer>>16)&0x000000ff;
	DWORD nBuildVer=(nVer>>8)&0x000000ff;	
	DWORD nMendVer = nVer & 0x000000ff;	
	DWORD nDemoVersion=(DEMOVERSION>>16)&0x0000ffff;
	DWORD nDemoBuild=DEMOVERSION&0x0000ffff;

	CString str ;
//	str.Format("dhplay.dll ver is %04x,build is %02x,mend is %02x\ndhplaydemo ver is%04x,build is %04x\n",
//		nVersion,nBuildVer,nMendVer,nDemoVersion,nDemoBuild) ;
	str.Format("dhplay.dll %d.%d.%d\n", nVersion,nBuildVer,nMendVer) ;

	GetDlgItem(IDC_STATIC_VER)->SetWindowText(str) ;
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPlayer264demoDlg::OnFileClose() 
{
	// TODO: Add your command handler code here

	if (!m_bFileOpen) return ;
	
	if (m_iFilePlayMode == FILE_MODE_STREAM)
	{
		m_bFileOpen = FALSE;
		g_IndexCreated = false;	
		m_pause = false;
		m_IsPlaying = false;
		m_playnormal = false;
		m_pMainMenu->EnableMenuItem(ID_FILE_CLOSE,MF_GRAYED);
		m_pMainMenu->EnableMenuItem(IDC_CUTFILE,MF_GRAYED);
		SetButtonsState(FALSE);
		m_bNeedFileClose = 0;
		return;
	}

	if (!m_IsPlaying && !m_bFileOpen)
	{
		m_pMainMenu->EnableMenuItem(ID_FILE_CLOSE,MF_GRAYED) ;
		SetButtonsState(FALSE);
		return ;
	}
	OnStop() ;
	m_bFileOpen = FALSE;
	m_bPlaySound = TRUE;
	PLAY_CloseFile(PORT) ;
	//m_IsPlaying	   = true;
	//SetWindowSize();
	//m_IsPlaying    = false ;
	g_IndexCreated = false ;	
	m_pause        = false ;
	m_playnormal   = false ;
	m_pMainMenu->EnableMenuItem(ID_FILE_CLOSE,MF_GRAYED) ;
	m_pMainMenu->EnableMenuItem(IDC_CUTFILE,MF_GRAYED) ;
	SetButtonsState(FALSE);
	//m_playregion.SetBitmap(m_BMPBlack);
	m_bNeedFileClose = 0;
}

void CPlayer264demoDlg::OnAbout() 
{
	// TODO: Add your command handler code here
	CAboutDlg dlg ;
	dlg.DoModal() ;
}

void CPlayer264demoDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
//	
	if(nIDEvent == 1)
	{
	 	StartDraw();
	}

	CDialog::OnTimer(nIDEvent);
}

void CPlayer264demoDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	DWORD nTime;
	int nBrightness, nContrast, nSaturation, nHue;
	int nPlayPosition;

	switch(GetWindowLong(pScrollBar->m_hWnd, GWL_ID))
	{
	case IDC_SLIDER_PLAY:
		if (m_IsPlaying)
		{
			nPlayPosition = m_sliderplay.GetPos();
		
			if (abs(nPlayPosition-m_nPrePlayPosition)<1 && nPlayPosition != 0)
			{
				break;
			}
			
			if (m_iFilePlayMode == FILE_MODE_FILE)
			{
				if (g_IndexCreated)
				{
					nTime= nPlayPosition * 5 * PLAY_GetFileTime(PORT);			
					PLAY_SetPlayedTimeEx(PORT,nTime);
				}
				else 
				{
					PLAY_SetPlayPos(PORT,nPlayPosition/(200.0)) ;
				}
			}
			else if (m_iFilePlayMode == FILE_MODE_STREAM)
			{
				double fPos = (double)nPlayPosition/200.0;
				m_dwCurFilePos = (DWORD)(fPos*(double)m_dwFileLen);

				PLAY_ResetBuffer(PORT ,BUF_VIDEO_RENDER);
				PLAY_ResetSourceBuffer(PORT);
			}
			
			//StartDraw();			
		}
		break ;		
	case IDC_SLIDER_SOUND:
		{
			int volume = m_SoundSlider.GetPos();
			PLAY_SetVolume(PORT,volume);
		}
		break;
	}
		
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CPlayer264demoDlg::OnLocate() 
{
	// TODO: Add your command handler code here
	if (!m_IsPlaying)
	{
		AfxMessageBox(ConvertString("Please Open File First")) ;
		return ;
	}
	Locate dlg ;
	dlg.DoModal() ;
}

void CPlayer264demoDlg::OnRepeat() 
{
	// TODO: Add your command handler code here
	if (m_IsRepeatPlay)
	{
		m_pMainMenu->CheckMenuItem(ID_REPEAT,MF_UNCHECKED );
		m_IsRepeatPlay=false;
	}
	else
	{
		m_pMainMenu->CheckMenuItem(ID_REPEAT,MF_CHECKED );
		m_IsRepeatPlay=true;
	}
}
void CPlayer264demoDlg::PlayMessage(WPARAM wParam, LPARAM lParam)
{
	MSG msgVal;
	while(PeekMessage(&msgVal,m_hWnd,WM_FILE_END,WM_FILE_END,PM_REMOVE))
		;
	
	if(m_IsRepeatPlay)
	{
		if (g_IndexCreated)
		{
			BOOL iRet = PLAY_SetCurrentFrameNum(PORT,0);
			if (iRet == FALSE)
			{
				PLAY_SetPlayedTimeEx(PORT, 0) ;
			}
		}
		else PLAY_SetPlayPos(PORT,0) ;
	}
}

void CPlayer264demoDlg::OnResetbuf() 
{
	// TODO: Add your command handler code here
	if (m_IsPlaying)
		PLAY_ResetBuffer(PORT,BUF_VIDEO_RENDER) ;
}


void CPlayer264demoDlg::OnExit() 
{
	// TODO: Add your command handler code here
	CPlayer264demoDlg::OnOK() ;
}

void CPlayer264demoDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
}

void CPlayer264demoDlg::OnMove(int x, int y) 
{
	CDialog::OnMove(x, y);
	
	// TODO: Add your message handler code here
//	if (m_IsPlaying)
//		PLAY_RefreshPlay(PORT) ;
	
}
void CALLBACK DrawFun(long nPort,HDC hDc,LONG nUser) 
{
}

void CALLBACK EncTypeChange(long nPort,long User)
{
	CPlayer264demoDlg* pOwner=(CPlayer264demoDlg*)User;
	pOwner->SetWindowSize();
}

void CALLBACK funAudio(long nPort, char * pAudioBuf,long nSize, long nStamp, long nType, long nUser)
{
//	audio_cb.Write(pAudioBuf,nSize) ;
}

void  CALLBACK DecCBFun(long nPort,char * pBuf,long nSize,FRAME_INFO * pFrameInfo, long nReserved1,long nReserved2)
{
	TRACE("nPort=%d,TYPE=%d;Width=%d;Height=%d\n",nPort,pFrameInfo->nType,pFrameInfo->nWidth,pFrameInfo->nHeight);
	char fname[100];
	
	switch (m_nDecType)
	{
	case 1:
// 		fwrite(pBuf, nSize, 1, f_video);
// 		fflush(f_video) ;
		break;
	case 2:
// 		fwrite(pBuf, nSize, 1, f_audio) ;
// 		fflush(f_audio) ;
		break;
	case 3:
// 		fwrite(pBuf, nSize, 1, f_complex) ;
// 		fflush(f_complex) ;
		break;
	default:
		return;
	}
}


void  CALLBACK DecCBFunEx(long nPort,char * pBuf,long nSize,FRAME_INFO * pFrameInfo, long nReserved1,long nReserved2)
{
	TRACE("nPort=%d,TYPE=%d;Width=%d;Height=%d\n",nPort,pFrameInfo->nType,pFrameInfo->nWidth,pFrameInfo->nHeight);
	char fname[100];
	
	switch (m_nDecType)
	{
	case 1:
// 		fwrite(pBuf, nSize, 1, f_video);
// 		fflush(f_video) ;
		break;
	case 2:
// 		fwrite(pBuf, nSize, 1, f_audio) ;
// 		fflush(f_audio) ;
		break;
	case 3:
// 		fwrite(pBuf, nSize, 1, f_complex) ;
// 		fflush(f_complex) ;
		break;
	default:
		return;
	}

}

void CALLBACK FileRefDone(DWORD nReserved,DWORD nUser)
{
	CPlayer264demoDlg * dlg = (CPlayer264demoDlg*)nUser ;

	if (dlg->m_iFilePlayMode == FILE_MODE_STREAM) return ;

	g_IndexCreated = true ;
	float fPos = PLAY_GetPlayPos(0);
	PLAY_SetPlayPos(0, fPos);

	dlg->m_BNStepOne.EnableWindow(TRUE) ;
	dlg->m_BNBackOne.EnableWindow(TRUE) ;
}

void CALLBACK FileRefDoneEx(DWORD nReserved, BOOL bIndexCreated, DWORD nUser)
{
	CPlayer264demoDlg * dlg = (CPlayer264demoDlg*)nUser ;

	if (dlg->m_iFilePlayMode == FILE_MODE_STREAM) return ;

	g_IndexCreated = bIndexCreated ;
	
	float fPos = PLAY_GetPlayPos(0);
	PLAY_SetPlayPos(0, fPos);
	
	dlg->m_BNStepOne.EnableWindow(TRUE) ;
	dlg->m_BNBackOne.EnableWindow(TRUE) ;
}

void CPlayer264demoDlg::OnDestroy() 
{
	CDialog::OnDestroy();

	OnClose();
	
	// TODO: Add your message handler code here
	
}

void CPlayer264demoDlg::OnPartDisplay() 
{
	// TODO: Add your command handler code here
	MultiDisplay dlg;
	if (dlg.nRegionNum > 1)
	{
		return;
	}
	dlg.DoModal();
}

void CPlayer264demoDlg::OnSysinfo() 
{
	// TODO: Add your command handler code here
	SYSINFO sysinfodlg ;
	sysinfodlg.DoModal() ;
}

void CPlayer264demoDlg::OnVerticalSync()
{
	UINT state = m_pMainMenu->GetMenuState(IDC_VERTICALSYNC, MF_BYCOMMAND);
	ASSERT(state != 0xFFFFFFFF);

	if (!m_IsPlaying) return;

	if (state & MF_CHECKED)
	{
		m_pMainMenu->CheckMenuItem(IDC_VERTICALSYNC, MF_UNCHECKED | MF_BYCOMMAND);
		PLAY_VerticalSyncEnable(PORT, FALSE);
	}
	else
	{
		m_pMainMenu->CheckMenuItem(IDC_VERTICALSYNC, MF_CHECKED | MF_BYCOMMAND);
		PLAY_VerticalSyncEnable(PORT, TRUE);
	}
}

void CPlayer264demoDlg::OnFilePlayMode()
{
	UINT state = m_pMainMenu->GetMenuState(ID_FILEMODE_FILE, MF_BYCOMMAND);
	ASSERT(state != 0xFFFFFFFF);
	
	if (m_IsPlaying)
	{
		OnFileClose();
	}

	if (state & MF_CHECKED)
	{
		m_iFilePlayMode = FILE_MODE_STREAM;
		m_pMainMenu->CheckMenuItem(ID_FILEMODE_FILE, MF_UNCHECKED | MF_BYCOMMAND);
		m_pMainMenu->CheckMenuItem(ID_FILEMODE_STREAM, MF_CHECKED | MF_BYCOMMAND);
	}
	else
	{
		m_iFilePlayMode = FILE_MODE_FILE;
		m_pMainMenu->CheckMenuItem(ID_FILEMODE_FILE, MF_CHECKED | MF_BYCOMMAND);
		m_pMainMenu->CheckMenuItem(ID_FILEMODE_STREAM, MF_UNCHECKED | MF_BYCOMMAND);
	}
}

void CPlayer264demoDlg::OnPicTypeSet()
{
	UINT state = m_pMainMenu->GetMenuState(ID_PIC_JPG, MF_BYCOMMAND);
	ASSERT(state != 0xFFFFFFFF);
	
	if (state & MF_CHECKED)
	{
		m_pMainMenu->CheckMenuItem(ID_PIC_JPG, MF_UNCHECKED | MF_BYCOMMAND);
		m_pMainMenu->CheckMenuItem(ID_PIC_BMP, MF_CHECKED | MF_BYCOMMAND);
	}
	else
	{
		m_pMainMenu->CheckMenuItem(ID_PIC_JPG, MF_CHECKED | MF_BYCOMMAND);
		m_pMainMenu->CheckMenuItem(ID_PIC_BMP, MF_UNCHECKED | MF_BYCOMMAND);
	}
}

void CPlayer264demoDlg::OnMediaInfo()
{
	CMediaIfoDlg tMediaInfoDlg;
	tMediaInfoDlg.DoModal();
}

void CPlayer264demoDlg::OnWatermarkCheck()
{
	if (m_WaterCheckDlg == NULL)
	{
		m_WaterCheckDlg = new CWaterCheckDlg;
		m_WaterCheckDlg->Create(IDD_DIALOG_WATERMARK, GetDesktopWindow());
	}
	m_WaterCheckDlg->CenterWindow(GetDesktopWindow());
	m_WaterCheckDlg->ShowWindow(SW_SHOW);
}

void CPlayer264demoDlg::OnAVIConvert()
{
	if (m_AVIConvDlg == NULL)
	{
		m_AVIConvDlg = new CAVIConvertDlg;
		m_AVIConvDlg->Create(IDD_DIALOG_AVICONV, GetDesktopWindow());
	}
	m_AVIConvDlg->CenterWindow(GetDesktopWindow());
	m_AVIConvDlg->ShowWindow(SW_SHOW);
}

void CPlayer264demoDlg::OnCaptureSound()
{
	if (m_CaptureSoundDlg == NULL)
	{
		m_CaptureSoundDlg = new CaptureSoundDlg;
		m_CaptureSoundDlg->Create(IDD_DIALOG_AUDIOIN, GetDesktopWindow());
	}
	m_CaptureSoundDlg->CenterWindow(GetDesktopWindow());
	m_CaptureSoundDlg->ShowWindow(SW_SHOW);
}

void CPlayer264demoDlg::OnCutfile() 
{
	// TODO: Add your command handler code here
	if (g_IndexCreated == false){
		AfxMessageBox(ConvertString("File index not created")) ;
		return ;
	}
	cutfile cutfiledlg(m_csfileName) ;
	cutfiledlg.DoModal() ;
}

void CPlayer264demoDlg::OnWndFullScreen()
{
	if(m_IsPlaying)
	{
		m_bIsFullScreen = !m_bIsFullScreen;
		
	
		CButton *pButton;
		CStatic* pStatic;
		if(m_bIsFullScreen)
		{
			
			pButton = (CButton *)GetDlgItem(IDC_BUTTON_PLAY);		
			pButton->ModifyStyle(WS_VISIBLE,0,0);
			pButton = (CButton *)GetDlgItem(IDC_BUTTON_PAUSE);		
			pButton->ModifyStyle(WS_VISIBLE,0,0);
			
			pButton = (CButton *)GetDlgItem(IDC_BUTTON_FAST);		
			pButton->ModifyStyle(WS_VISIBLE,0,0);
			pButton = (CButton *)GetDlgItem(IDC_BUTTON_SLOW);		
			pButton->ModifyStyle(WS_VISIBLE,0,0);
			pButton = (CButton *)GetDlgItem(IDC_BUTTON_BACKONE);		
			pButton->ModifyStyle(WS_VISIBLE,0,0);
			pButton = (CButton *)GetDlgItem(IDC_BUTTON_ONE);		
			pButton->ModifyStyle(WS_VISIBLE,0,0);
			pButton = (CButton *)GetDlgItem(IDC_BUTTON_TOBEGIN);		
			pButton->ModifyStyle(WS_VISIBLE,0,0);
			pButton = (CButton *)GetDlgItem(IDC_BUTTON_TOEND);		
			pButton->ModifyStyle(WS_VISIBLE,0,0);
			pButton = (CButton *)GetDlgItem(IDC_BUTTON_PICCATCH);		
			pButton->ModifyStyle(WS_VISIBLE,0,0);
			pButton = (CButton *)GetDlgItem(IDC_BUTTON_STOP);		
			pButton->ModifyStyle(WS_VISIBLE,0,0);
// 			pButton = (CButton *)GetDlgItem(IDC_BUTTON_GETERROR);		
// 			pButton->ModifyStyle(WS_VISIBLE,0,0);
			pButton = (CButton *)GetDlgItem(IDC_FULLSCREEN);		
			pButton->ModifyStyle(WS_VISIBLE,0,0);
			pButton = (CButton *)GetDlgItem(IDC_BUTTON_SETCOLOR);		
			pButton->ModifyStyle(WS_VISIBLE,0,0);
			pButton = (CButton *)GetDlgItem(IDC_BUTTON_SOUND);		
			pButton->ModifyStyle(WS_VISIBLE,0,0);
			
			m_sliderplay.ModifyStyle(WS_VISIBLE,0,0);
			m_SoundSlider.ModifyStyle(WS_VISIBLE,0,0);
			
			GetDlgItem(IDC_STATIC_STATEINFO)->ModifyStyle(WS_VISIBLE,0,0);
			GetDlgItem(IDC_STATIC_PLAYSTATE)->ModifyStyle(WS_VISIBLE,0,0);
			GetDlgItem(IDC_STATIC_CURINFO)->ModifyStyle(WS_VISIBLE,0,0);

			//GetDlgItem(IDC_CHECK_SOUND)->ModifyStyle(WS_VISIBLE,0,0);



			GetWindowPlacement(&m_OldWndplacement);

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

//			m_FullScreenRect.left = 0;
//			m_FullScreenRect.top = 0; 
//			m_FullScreenRect.right = GetSystemMetrics(SM_CXSCREEN);
//			m_FullScreenRect.bottom = GetSystemMetrics(SM_CYSCREEN);

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
			GetDlgItem(IDC_STATIC_SHOW)->MoveWindow(&rc,TRUE);

		}
		else
		{
			pButton = (CButton *)GetDlgItem(IDC_BUTTON_PLAY);		
			pButton->ModifyStyle(0,WS_VISIBLE,0);
			pButton = (CButton *)GetDlgItem(IDC_BUTTON_PAUSE);		
			pButton->ModifyStyle(0,WS_VISIBLE,0);
			
			pButton = (CButton *)GetDlgItem(IDC_BUTTON_FAST);		
			pButton->ModifyStyle(0,WS_VISIBLE,0);
			pButton = (CButton *)GetDlgItem(IDC_BUTTON_SLOW);		
			pButton->ModifyStyle(0,WS_VISIBLE,0);
			pButton = (CButton *)GetDlgItem(IDC_BUTTON_BACKONE);		
			pButton->ModifyStyle(0,WS_VISIBLE,0);
			pButton = (CButton *)GetDlgItem(IDC_BUTTON_ONE);		
			pButton->ModifyStyle(0,WS_VISIBLE,0);
			pButton = (CButton *)GetDlgItem(IDC_BUTTON_TOBEGIN);		
			pButton->ModifyStyle(0,WS_VISIBLE,0);
			pButton = (CButton *)GetDlgItem(IDC_BUTTON_TOEND);		
			pButton->ModifyStyle(0,WS_VISIBLE,0);
			pButton = (CButton *)GetDlgItem(IDC_BUTTON_PICCATCH);		
			pButton->ModifyStyle(0,WS_VISIBLE,0);
			pButton = (CButton *)GetDlgItem(IDC_BUTTON_STOP);		
			pButton->ModifyStyle(0,WS_VISIBLE,0);
// 			pButton = (CButton *)GetDlgItem(IDC_BUTTON_GETERROR);		
// 			pButton->ModifyStyle(0,WS_VISIBLE,0);
			pButton = (CButton *)GetDlgItem(IDC_FULLSCREEN);		
			pButton->ModifyStyle(0,WS_VISIBLE,0);
			pButton = (CButton *)GetDlgItem(IDC_BUTTON_SETCOLOR);		
			pButton->ModifyStyle(0,WS_VISIBLE,0);
			pButton = (CButton *)GetDlgItem(IDC_BUTTON_SOUND);		
			pButton->ModifyStyle(0,WS_VISIBLE,0);

			m_sliderplay.ModifyStyle(0,WS_VISIBLE,0);
			m_SoundSlider.ModifyStyle(0,WS_VISIBLE,0);
			
			GetDlgItem(IDC_STATIC_STATEINFO)->ModifyStyle(0,WS_VISIBLE,0);
			GetDlgItem(IDC_STATIC_PLAYSTATE)->ModifyStyle(0,WS_VISIBLE,0);
			GetDlgItem(IDC_STATIC_CURINFO)->ModifyStyle(0,WS_VISIBLE,0);
			//GetDlgItem(IDC_CHECK_SOUND)->ModifyStyle(0,WS_VISIBLE,0);
			ModifyStyle(0,WS_SIZEBOX,0);
			
			
			SetWindowPlacement(&m_OldWndplacement);
			RECT rc;
			GetClientRect(&rc);
// 			rc.right  -= 1;
// 			rc.bottom -= 132;
			GetDlgItem(IDC_STATIC_SHOW)->MoveWindow(&rc,TRUE);
			SetWindowSize();
		}

		this->RedrawWindow();

		PLAY_RefreshPlay(PORT);
	}
	else;
}

void CPlayer264demoDlg::OnStaticShow() 
{
	// TODO: Add your control notification handler code here	
}

void CPlayer264demoDlg::OnFullscreen() 
{
	if (!m_bFileOpen) return ;

	OnWndFullScreen();
}

void CPlayer264demoDlg::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CRect pt;
	ClientToScreen(&point);
	GetDlgItem(IDC_STATIC_SHOW)->GetWindowRect(&pt);
	if( point.x > pt.left && point.x < pt.right && point.y > pt.top && point.y < pt.bottom)
	{
		OnWndFullScreen();
	}
	CDialog::OnLButtonDblClk(nFlags, point);
}

void CPlayer264demoDlg::OnCustomdrawSliderPlay(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}

void CPlayer264demoDlg::StartDraw()
{
	if (m_IsPlaying)
	{
		LONG nWidth, nHeight;
		PLAY_GetPictureSize(PORT, &nWidth, &nHeight);
		if (m_nWidth != nWidth || m_nHeight != nHeight)
		{
			SetWindowSize();
		}

		int volume = PLAY_GetVolume(PORT) ;

		float fTime = PLAY_GetPlayedTimeEx(PORT)/1000.0;

		float fDotNum = fTime - ((int)fTime/10)*10- (int)fTime%10;
		fTime = fDotNum >= 0.50? ((int)fTime)+1.0 : fTime;
		DWORD nCTime = fTime;
	
		DWORD nHour = (nCTime / 3600) % 24;
		DWORD nMinute = (nCTime % 3600) / 60;
		DWORD nSecond = nCTime % 60;
		int nPos = 0;
		DWORD m_nMaxFileTime = PLAY_GetFileTime(PORT);

		DWORD total_hour = m_nMaxFileTime/3600;
		DWORD total_min = (m_nMaxFileTime%3600)/60;
		DWORD total_sec = m_nMaxFileTime%60;
		
		DWORD nCFrame = PLAY_GetCurrentFrameNum(PORT)+1;

		if (PLAY_GetFileTime(PORT) && g_IndexCreated)
		{			
			nPos = (int)(fTime*200/PLAY_GetFileTime(PORT));
			
			if (m_nPrePlayPosition<nPos)
			{
				nPos = fTime*200/PLAY_GetFileTime(PORT);		
			}	
		}
		else
		{
			if (m_iFilePlayMode == FILE_MODE_FILE)
			{
				nPos = PLAY_GetPlayPos(0) * 200;
			}
			else
			{
				double fPos = (double)m_dwCurFilePos/(double)m_dwFileLen;
				nPos = fPos * 200.0;
			}
		}

		m_sliderplay.SetPos(nPos);

		UINT uiLangstate = m_pMainMenu->GetMenuState(ID_MENUITEM_Chinese, MF_BYCOMMAND);
		
		CString csTimeState, csTemp;	

		CString csBitRate;
		double frameBitRate = 0.0;
		PLAY_GetRealFrameBitRate(PORT, &frameBitRate);
		csBitRate.Format("%.3f Kbps", frameBitRate);
		csTimeState = csBitRate + "  ";
		
		if (g_IndexCreated)
		{
			csTemp.Format("%d / %d", PLAY_GetCurrentFrameNum(PORT)+1, PLAY_GetFileTotalFrames(PORT));
			csTimeState += csTemp;
			csTemp = TimeToString(nCTime);
			csTemp += " / ";
			csTemp += TimeToString(m_nMaxFileTime);
			csTimeState += "   " + csTemp;
		}
		else
		{
			int len = 0;
			MEDIA_INFO tMediaInfo;
			memset(&tMediaInfo, 0, sizeof(MEDIA_INFO));
			PLAY_QueryInfo(0, PLAY_CMD_GetMediaInfo, (char*)(&tMediaInfo), sizeof(MEDIA_INFO), &len);

			//csTemp.Format("played frames %d", PLAY_GetPlayedFrames(PORT));
			//csTimeState += csTemp;
			csTemp.Format("frame rate %d", tMediaInfo.lFrameRate);
			csTemp = TimeToString(nCTime);
			csTimeState += "   " + csTemp;
		}	

		CString csPlayState = "";
		GetPlayState(csPlayState);

		DrawPlayState(csPlayState, csTimeState);
	}
	else
	{
		m_sliderplay.SetPos(0) ;
		SetWindowText("dhplaydemo");	

		CString csPlayState = "";
		CString csTimeState = "";

		DrawPlayState(csPlayState, csTimeState);
	}
}

void CPlayer264demoDlg::GetPlayState(CString& csPlayState)
{
	switch (m_tPlayState)
	{
	case PlayState_Playing:
		csPlayState = "Playing";
		break;
	case PlayState_Pause:
		csPlayState = "Pause";
		break;
	case PlayState_Stop:
		csPlayState = "Stop";
		break;
	case PlayState_FAST:
		{
			CString csTemp;
			int y = abs(g_playspeed)*2;
			csTemp.Format("  %dX", y);
			
			csPlayState = "FAST" + csTemp; 
		}
		break;
	case PlayState_Slow:
		{
			CString csTemp;
			int y = pow(2,abs(g_playspeed));//   abs(g_playspeed)*2;
			csTemp.Format("  1/%dX", y);
			
			csPlayState = "Slow" + csTemp;
		}
		break;
	case PlayState_StepForward:
		csPlayState = "Step forward";
		break;
	case PlayState_StepBackWard:
		csPlayState = "Step backward";
		break;
	default:
		break;
	}
}

void CPlayer264demoDlg::DrawPlayState(CString& csPlayState, CString& csTimeState)
{
	CWnd *pWnd = GetDlgItem(IDC_STATIC_STATEINFO);   
	CRect rect;
	pWnd->GetClientRect(rect);
	CDC  *pControlDC = pWnd->GetDC();   
	pWnd->Invalidate();   
	pWnd->UpdateWindow();
	pControlDC->SetBkColor(RGB(0, 0, 0));
	pControlDC->SelectStockObject(BLACK_BRUSH); 
	pControlDC->Rectangle(rect);
	pWnd->ReleaseDC(pControlDC); 

	CWnd *pWndState = GetDlgItem(IDC_STATIC_PLAYSTATE); 
	CRect rectS;
	pWndState->GetClientRect(rectS);
	CDC *pCDC = pWndState->GetDC();   
	pWndState->Invalidate();   
	pWndState->UpdateWindow();
	pCDC->SetBkColor(RGB(0, 0, 0));
	if (csPlayState == "")
	{
		pCDC->SelectStockObject(BLACK_BRUSH); 
		pCDC->Rectangle(rectS);
	}
	else
	{
		pCDC->SetTextColor(RGB(255, 255, 255));
		pCDC->DrawText(csPlayState, &rectS, DT_LEFT|DT_NOCLIP);
	}
	pWndState->ReleaseDC(pCDC);

	pWnd = GetDlgItem(IDC_STATIC_CURINFO);   
	pWnd->GetClientRect(rect);
	//rect.bottom = 40;
	pControlDC = pWnd->GetDC();   
	pWnd->Invalidate();   
	pWnd->UpdateWindow();
	pControlDC->SetBkColor(RGB(0, 0, 0));
	if (csTimeState == "")
	{
		pControlDC->SelectStockObject(BLACK_BRUSH); 
		pControlDC->Rectangle(rect);
	}
	else
	{
		pControlDC->SetTextColor(RGB(255, 255, 255));
		pControlDC->DrawText(csTimeState, &rect, DT_RIGHT|DT_NOCLIP);
	}
	pWnd->ReleaseDC(pControlDC);
}

void CPlayer264demoDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
}

void CPlayer264demoDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	//CDialog::OnLButtonUp(nFlags, point);
}

void CPlayer264demoDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	//CDialog::OnLButtonDown(nFlags, point);
}

void CPlayer264demoDlg::SavePicTmp() 
{
	// TODO: Add your message handler code here and/or call default

}

void CPlayer264demoDlg::OnCustomdrawSliderSound(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}

void CPlayer264demoDlg::OnDecAudio() 
{
	// TODO: Add your command handler code here
	m_pMainMenu->CheckMenuItem(ID_DEC_NONE,MF_UNCHECKED );
	m_pMainMenu->CheckMenuItem(ID_DEC_AUDIO,MF_CHECKED );
	m_pMainMenu->CheckMenuItem(ID_DEC_VIDEO,MF_UNCHECKED );
	m_pMainMenu->CheckMenuItem(ID_DEC_COMPLEX,MF_UNCHECKED );
	m_nDecType = 2;
}

void CPlayer264demoDlg::OnDecVideo() 
{
	// TODO: Add your command handler code here
	m_pMainMenu->CheckMenuItem(ID_DEC_NONE,MF_UNCHECKED );
	m_pMainMenu->CheckMenuItem(ID_DEC_AUDIO,MF_UNCHECKED );
	m_pMainMenu->CheckMenuItem(ID_DEC_VIDEO,MF_CHECKED );
	m_pMainMenu->CheckMenuItem(ID_DEC_COMPLEX,MF_UNCHECKED );
	m_nDecType = 1;
}

void CPlayer264demoDlg::OnDecComplex() 
{
	// TODO: Add your command handler code here
	m_pMainMenu->CheckMenuItem(ID_DEC_NONE,MF_UNCHECKED );
	m_pMainMenu->CheckMenuItem(ID_DEC_AUDIO,MF_UNCHECKED );
	m_pMainMenu->CheckMenuItem(ID_DEC_VIDEO,MF_UNCHECKED );
	m_pMainMenu->CheckMenuItem(ID_DEC_COMPLEX,MF_CHECKED );
	m_nDecType = 3;
}

void CPlayer264demoDlg::OnDecNone() 
{
	// TODO: Add your command handler code here
	m_pMainMenu->CheckMenuItem(ID_DEC_NONE,MF_CHECKED );
	m_pMainMenu->CheckMenuItem(ID_DEC_AUDIO,MF_UNCHECKED );
	m_pMainMenu->CheckMenuItem(ID_DEC_VIDEO,MF_UNCHECKED );
	m_pMainMenu->CheckMenuItem(ID_DEC_COMPLEX,MF_UNCHECKED );
	m_nDecType = 0;
}

void CPlayer264demoDlg::SetWindowSize()
{
	if(m_IsPlaying)
	{
		if (!m_bIsFullScreen)
		{
			CRect rcWindow,rcClient;
			GetWindowRect(&rcWindow);
			if (rcWindow.top < -2000)
			{
				return;
			}
			
			DWORD oldx = (rcWindow.TopLeft()).x;
			DWORD oldy = (rcWindow.TopLeft()).y;
			
			GetClientRect(&rcClient);
			int m_nDlgEdge=(rcWindow.Width()-rcClient.Width())>>1;
			ClientToScreen(&rcClient);
			int m_nDlgTopSize=rcClient.top-rcWindow.top;
			
			int ret = PLAY_GetPictureSize(PORT, &m_nWidth, &m_nHeight);
			
			static long width = m_nWidth ;
			static long height = m_nHeight;
			if (ret==0)
			{
				m_nWidth = width;
				m_nHeight = height;
			}
			else;			
			
			DWORD m_nWindowHeight = m_nHeight + 150;
			DWORD m_nWindowWidth  = m_nWidth;

			if (m_nWindowWidth <= 450)
			{
				m_nWindowWidth = 450;
			}
			
			long x = 0;		
			long y = m_nHeight;
			
			GetDlgItem(IDC_SLIDER_PLAY)->MoveWindow(x+5, y+5, m_nWindowWidth-15, 20, TRUE);
			
			x += 8; y += 30;
			GetDlgItem(IDC_BUTTON_PLAY)->MoveWindow(x, y, 23, 23, TRUE);
			GetDlgItem(IDC_BUTTON_PAUSE)->MoveWindow(x+23+1, y, 23, 23,TRUE);
			GetDlgItem(IDC_BUTTON_STOP)->MoveWindow(x+23*2+1, y, 23, 23,TRUE);
			GetDlgItem(IDC_BUTTON_TOBEGIN)->MoveWindow(x+23*3+1, y, 23, 23,TRUE);
			GetDlgItem(IDC_BUTTON_BACKONE)->MoveWindow(x+23*4+1, y, 23, 23,TRUE);
			GetDlgItem(IDC_BUTTON_SLOW)->MoveWindow(x+23*5+1, y, 23, 23,TRUE);
			GetDlgItem(IDC_BUTTON_FAST)->MoveWindow(x+23*6+1, y, 23, 23,TRUE);
			GetDlgItem(IDC_BUTTON_ONE)->MoveWindow(x+23*7+1, y, 23, 23,TRUE);
			GetDlgItem(IDC_BUTTON_TOEND)->MoveWindow(x+23*8+1, y, 23, 23,TRUE);
			GetDlgItem(IDC_BUTTON_PICCATCH)->MoveWindow(x+23*9+1, y, 23, 23,TRUE);
			GetDlgItem(IDC_FULLSCREEN)->MoveWindow(x+23*10+1, y, 23, 23,TRUE);
			GetDlgItem(IDC_BUTTON_SETCOLOR)->MoveWindow(x+23*11+1, y, 23, 23,TRUE);
			
			GetDlgItem(IDC_SLIDER_SOUND)->MoveWindow(m_nWindowWidth - 80, y+3, 70, 20,TRUE);
			GetDlgItem(IDC_BUTTON_SOUND)->MoveWindow(m_nWindowWidth - 100, y, 23, 23,TRUE);

			GetDlgItem(IDC_STATIC_STATEINFO)->MoveWindow(1, y+30, m_nWindowWidth-1, 35,TRUE);

			GetDlgItem(IDC_STATIC_CURINFO)->MoveWindow(m_nWindowWidth - 70, y+40, 50, 0,TRUE);
			GetDlgItem(IDC_STATIC_PLAYSTATE)->MoveWindow(8, y+40, 25, 5, TRUE);
			
			GetDlgItem(IDC_STATIC_SHOW)->MoveWindow(((m_nWidth > m_nWindowWidth)? 0: (m_nWindowWidth-m_nWidth)/2) - m_nDlgEdge,
				0, m_nWidth, m_nHeight, TRUE);
			
			MoveWindow(oldx, oldy, m_nWindowWidth,	m_nWindowHeight, TRUE);
		}
		else;
	}
	else
	{
		if (!m_bIsFullScreen)
		{
			CRect rcWindow,rcClient;
			GetWindowRect(&rcWindow);
			DWORD oldx = (rcWindow.TopLeft()).x;
			DWORD oldy = (rcWindow.TopLeft()).y;

			GetClientRect(&rcClient);
			int m_nDlgEdge=(rcWindow.Width()-rcClient.Width())>>1;
			ClientToScreen(&rcClient);
			int m_nDlgTopSize=rcClient.top-rcWindow.top;
			int nWidth,nHeight;
			
			nWidth = 640;
			nHeight = 480;
			static int width = nWidth ;
			static int height = nHeight;				
			
			DWORD m_nWindowHeight = nHeight+150;
			DWORD m_nWindowWidth  = nWidth;	

			int x = 0;
			int y = nHeight;
			GetDlgItem(IDC_SLIDER_PLAY)->MoveWindow(x+5, y+5, m_nWindowWidth-15, 20, TRUE);
			
			x += 8; y += 30;
			GetDlgItem(IDC_BUTTON_PLAY)->MoveWindow(x, y, 23, 23, TRUE);
			GetDlgItem(IDC_BUTTON_PAUSE)->MoveWindow(x+23+1, y, 23, 23,TRUE);
			GetDlgItem(IDC_BUTTON_STOP)->MoveWindow(x+23*2+1, y, 23, 23,TRUE);
			GetDlgItem(IDC_BUTTON_TOBEGIN)->MoveWindow(x+23*3+1, y, 23, 23,TRUE);
			GetDlgItem(IDC_BUTTON_BACKONE)->MoveWindow(x+23*4+1, y, 23, 23,TRUE);
			GetDlgItem(IDC_BUTTON_SLOW)->MoveWindow(x+23*5+1, y, 23, 23,TRUE);
			GetDlgItem(IDC_BUTTON_FAST)->MoveWindow(x+23*6+1, y, 23, 23,TRUE);
			GetDlgItem(IDC_BUTTON_ONE)->MoveWindow(x+23*7+1, y, 23, 23,TRUE);
			GetDlgItem(IDC_BUTTON_TOEND)->MoveWindow(x+23*8+1, y, 23, 23,TRUE);
			GetDlgItem(IDC_BUTTON_PICCATCH)->MoveWindow(x+23*9+1, y, 23, 23,TRUE);
			GetDlgItem(IDC_FULLSCREEN)->MoveWindow(x+23*10+1, y, 23, 23,TRUE);
			GetDlgItem(IDC_BUTTON_SETCOLOR)->MoveWindow(x+23*11+1, y, 23, 23,TRUE);
			
			GetDlgItem(IDC_SLIDER_SOUND)->MoveWindow(m_nWindowWidth - 80, y+3, 70, 20,TRUE);
			GetDlgItem(IDC_BUTTON_SOUND)->MoveWindow(m_nWindowWidth - 100, y, 23, 23,TRUE);

			GetDlgItem(IDC_STATIC_STATEINFO)->MoveWindow(1, y+30, m_nWindowWidth-1, 30,TRUE);

			GetDlgItem(IDC_STATIC_CURINFO)->MoveWindow(m_nWindowWidth - 70, y+40, 50, 0,TRUE);
			GetDlgItem(IDC_STATIC_PLAYSTATE)->MoveWindow(8, y+40, 25, 5, TRUE);
			
			GetDlgItem(IDC_STATIC_SHOW)->MoveWindow(0,//*((m_nWidth > m_nWindowWidth)? 0: (m_nWindowWidth-m_nWidth)/2)*/,
				0, nWidth, nHeight, TRUE);
			
			MoveWindow(oldx, oldy, m_nWindowWidth,	m_nWindowHeight, TRUE);
				
		}
		else;
	}
	//CPlayer264demoDlg::OnPaint();

}

void CPlayer264demoDlg::OnLanChinese() 
{
	// TODO: Add your command handler code here
	if (strcmp("Chinese", g_pCurInterface.pLanguage.LangName) == 0)
	{
		return ;
	}

	m_pMainMenu->CheckMenuItem(ID_MENUITEM_Chinese,MF_CHECKED);
	m_pMainMenu->CheckMenuItem(ID_MENUITEM_English,MF_UNCHECKED );
	

	strcpy(g_pCurInterface.pLanguage.LangName,"Chinese") ;
	strcpy(g_pCurInterface.pLanguage.IniFile,"dhchs_play.ini") ;

	SetMenuStaticText(m_pMainMenu) ;
	SetWndStaticText(this) ;
	DrawMenuBar() ;
}

void CPlayer264demoDlg::OnLanEnglish() 
{
	// TODO: Add your command handler code here
	if (strcmp("English", g_pCurInterface.pLanguage.LangName) == 0)
	{
		return ;
	}

	m_pMainMenu->CheckMenuItem(ID_MENUITEM_Chinese,MF_UNCHECKED );
	m_pMainMenu->CheckMenuItem(ID_MENUITEM_English,MF_CHECKED );
	
	strcpy(g_pCurInterface.pLanguage.LangName,"English") ;
	strcpy(g_pCurInterface.pLanguage.IniFile,"dheng_play.ini") ;

 	SetMenuStaticText(m_pMainMenu) ;
	SetWndStaticText(this) ;
 	DrawMenuBar() ;
}

void CPlayer264demoDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	PLAY_Stop(PORT) ;
	if(m_bNeedFileClose == 1)
	{
		PLAY_CloseFile(PORT) ;
		m_bNeedFileClose = 0;
	}
	
// 	fclose(f_complex) ;
// 	fclose(f_audio) ;
// 	fclose(f_video) ;
	if (m_fileindexbuf != NULL)
	{
		delete m_fileindexbuf ;
		m_fileindexbuf = NULL ;
	}

	timeKillEvent(m_iTimerID);
	timeEndPeriod(10);
	
	CDialog::OnClose();
}

void CPlayer264demoDlg::OnDropFiles(HDROP hDropInfo)
{
	TCHAR *lpszFileName = new TCHAR[MAX_PATH];
	
	UINT nDragNum;
	UINT nFileNum;
	
	nDragNum = ::DragQueryFile(hDropInfo, 0xFFFFFFFF,lpszFileName, MAX_PATH);
	
	for (int i = 0; i < 1/*nDragNum*/; i++)
	{
		::DragQueryFile(hDropInfo, i, lpszFileName, MAX_PATH);
		
		CString csFilePath;
		csFilePath.Format("%s", lpszFileName);

		m_bFileOpen = TRUE;

		if (m_iFilePlayMode == FILE_MODE_FILE)
		{
			PlayFile(csFilePath);
		}
		else if (m_iFilePlayMode == FILE_MODE_STREAM)
		{
			m_BNPlay.SetState(TRUE);
			m_BNPause.SetState(FALSE);
			m_tPlayState = PlayState_Playing;
			
			if (!m_IsPlaying)
			{
				m_hPlayThread = NULL;
			}
			
			m_IsPlaying = true;
			m_csfileName = csFilePath;
			PlayStreamFile(m_csfileName);
		}			
	}
	
	::DragFinish (hDropInfo); //释放内存
	delete []lpszFileName;
	lpszFileName = NULL;
}

void CPlayer264demoDlg::OnSound() 
{
	if (!m_bFileOpen) return;

	m_bPlaySound = !m_bPlaySound;
	
	if (!m_bPlaySound)
	{
		PLAY_StopSound();
		m_BNSound.EnableWindow(FALSE);
	}
	else 
	{
		PLAY_PlaySound(PORT);
		m_BNSound.EnableWindow(TRUE);
	}
}

void CPlayer264demoDlg::OnButtonSetcolor() 
{
	if (!m_bFileOpen) return;

	if (m_SetColorDlg == NULL)
	{
		m_SetColorDlg = new CSetColorDlg;
		m_SetColorDlg->Create(IDD_DIALOG_COLORSET, this);
	}
	if (m_IsPlaying) m_SetColorDlg->SetColorSliders();
	m_SetColorDlg->CenterWindow(this);
	m_SetColorDlg->ShowWindow(SW_SHOW);	
}

void CPlayer264demoDlg::OnPicPathSet()
{
	if (m_SetPicPathDlg == NULL)
	{
		m_SetPicPathDlg = new CSetPicPath;
	}

	m_SetPicPathDlg->m_csPicPath = m_csPicSavePath;
	m_SetPicPathDlg->DoModal();	

	m_csPicSavePath = m_SetPicPathDlg->m_csPicPath;
}

CString CPlayer264demoDlg::TimeToString(DWORD dwTime)
{
	CString csTime, csHour, csMin, csSec;
	int hour, min, sec;
	hour = (int)(dwTime/3600)%24;
	min  = (int)( (dwTime%3600)/60 );
	sec  = (int)dwTime%60;
	
	csHour.Format("%d",hour);
	csMin.Format("%d",min);
	csSec.Format("%d",sec);
	
	if ( csHour.GetLength() <= 1 )
		csHour = "0" + csHour;
	if ( csMin.GetLength() <= 1 )
		csMin = "0" + csMin;
	if ( csSec.GetLength() <= 1 )
		csSec = "0" + csSec;
	
	csTime = csHour + ":" + csMin + ":" + csSec;
	
	return csTime;
}

void CPlayer264demoDlg::Refurish()
{
	if(m_IsPlaying)
	{	
		PLAY_RefreshPlay(PORT);
		SetWindowSize();
	}
	else;
}

BOOL CPlayer264demoDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
	{
		return TRUE;
	}
	return CDialog::PreTranslateMessage(pMsg);
}
