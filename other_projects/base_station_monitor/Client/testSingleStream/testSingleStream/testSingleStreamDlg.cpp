
// testSingleStreamDlg.cpp : implementation file
//

#include "stdafx.h"
#include "testSingleStream.h"
#include "testSingleStreamDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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


// CtestSingleStreamDlg dialog




CtestSingleStreamDlg::CtestSingleStreamDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CtestSingleStreamDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CtestSingleStreamDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CtestSingleStreamDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &CtestSingleStreamDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDOK, &CtestSingleStreamDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CtestSingleStreamDlg message handlers

BOOL CtestSingleStreamDlg::OnInitDialog()
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

void CtestSingleStreamDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CtestSingleStreamDlg::OnPaint()
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
HCURSOR CtestSingleStreamDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

UINT recvThread(LPVOID param)
{
	CSocket *cs=new CSocket();
	cs->Create();
	if(cs->Connect(_T("127.0.0.1"), 8083)==FALSE)
	{
		MessageBox(NULL,_T("Error"),_T("Error"),MB_OK);
		return -1;
	}
	int totalNum=4094;
	BYTE buf[1024];
	char sendcmd_start[]="test_id\n";
	char sendcmd_ack[]="OK\n";
	cs->Send(sendcmd_start, sizeof(sendcmd_start));
	FILE *fp=fopen("getstream.h264", "wb");
	bool bfinish=false;
	while(!bfinish)
	{
		int receivedNum=0;
		while(receivedNum<totalNum)
		{
			int ret=cs->Receive(buf, sizeof(buf));
			while(PLAY_InputData(1,buf,sizeof(buf))==FALSE)
			{
				::Sleep(1000);
			}
			fwrite(buf, sizeof(char), sizeof(buf), fp);
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
	fclose(fp);
	MessageBox(NULL,_T("receive end"),_T("receive end"),MB_OK);
	return 0;
}

void CtestSingleStreamDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	BOOL bOpenRet = PLAY_OpenStream(1,0,0,1024*100);
	if(bOpenRet)
	{
		HWND hwnd=GetDlgItem(IDC_PLAY)->GetSafeHwnd();
		BOOL bPlayRet=PLAY_Play(1, hwnd);
		//start a thread to receive the video information.
		mythread = AfxBeginThread(recvThread, NULL);

		//closestream
	}
}

void CtestSingleStreamDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	//PostThreadMessage(mythread->, WM_STOPMSG, NULL, NULL);
	OnOK();
}
