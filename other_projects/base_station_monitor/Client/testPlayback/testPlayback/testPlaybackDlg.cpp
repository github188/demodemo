
// testPlaybackDlg.cpp : implementation file
//

#include "stdafx.h"
#include "testPlayback.h"
#include "testPlaybackDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

typedef struct{
	char *cmd;
	int nport;
}threadparm;

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CtestPlaybackDlg dialog




CtestPlaybackDlg::CtestPlaybackDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CtestPlaybackDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CtestPlaybackDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CtestPlaybackDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &CtestPlaybackDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CtestPlaybackDlg message handlers

BOOL CtestPlaybackDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
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

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CtestPlaybackDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CtestPlaybackDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

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

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CtestPlaybackDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

UINT recvThread(LPVOID param)
{
	threadparm *p=(threadparm *)param;

	CSocket *cs=new CSocket();
	cs->Create();
	if(cs->Connect(_T("127.0.0.1"), 8001)==FALSE)
	{
		MessageBox(NULL,_T("Error"),_T("Error"),MB_OK);
		return -1;
	}
	int totalNum=4094;
	BYTE buf[1024];
	//char sendcmd_start[]="video>replay?uuid=10020002\n";
	char *sendcmd_start=p->cmd;
	char sendcmd_ack[]="video>ack\n";
	cs->Send(sendcmd_start, strlen(sendcmd_start));
	bool bfinish=false;
	while(!bfinish)
	{
		int receivedNum=0;
		while(receivedNum<totalNum)
		{
			int ret=cs->Receive(buf, sizeof(buf));
			while(PLAY_InputData(p->nport,buf,sizeof(buf))==FALSE)
			{
				::Sleep(500);
			}
			if(ret>0)
			{
				receivedNum+=ret;
			}
			else
			{
				//cannot be stopped... Need a terminal signal...
				bfinish=true;
			}
		}
		cs->Send(sendcmd_ack, sizeof(sendcmd_ack));
	}
	cs->Close();
	MessageBox(NULL,_T("receive end"),_T("receive end"),MB_OK);
	return 0;
}

void CtestPlaybackDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	BOOL bOpenRet = PLAY_OpenStream(1,0,0,1024*100);
	if(bOpenRet)
	{
		HWND hwnd=GetDlgItem(IDC_PLAY)->GetSafeHwnd();
		BOOL bPlayRet=PLAY_Play(1, hwnd);
		//start a thread to receive the video information.
		threadparm *p1=new threadparm();
		p1->cmd="video>replay?uuid=10020002\n";
		p1->nport=1;
		mythread = AfxBeginThread(recvThread, p1);

		//closestream
	}

	PLAY_OpenStream(2,0,0,1024*100);
	HWND hwnd=GetDlgItem(IDC_PLAY2)->GetSafeHwnd();
	PLAY_Play(2, hwnd);
	threadparm *p2=new threadparm();
	p2->cmd="video>replay?uuid=10020001\n";
	p2->nport=2;
	AfxBeginThread(recvThread, p2);
}
