// WarnPopVideo.cpp : implementation file
//

#include "stdafx.h"
#include "BTSMonitor.h"
#include "WarnPopVideo.h"
#include "const.h"
#include "PopPlayView.h"
#include "include/iPlay.h"

// CWarnPopVideo dialog

IMPLEMENT_DYNAMIC(CWarnPopVideo, CDialog)

CWarnPopVideo::CWarnPopVideo(CWnd* pParent /*=NULL*/)
	: CDialog(CWarnPopVideo::IDD, pParent)
	, m_nPopIndex(0)
	, m_bFullScreen(false)
{
	m_pPopView = NULL;
}

CWarnPopVideo::~CWarnPopVideo()
{

}

void CWarnPopVideo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CWarnPopVideo, CDialog)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDOK, &CWarnPopVideo::OnBnClickedOk)
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
	// AFX_WS_DEFAULT_VIEW´ú±í(WS_BORDER | WS_VISIBLE | WS_CHILD)
	//m_vvControl.vvInfo[i].vv->Create(NULL, NULL, AFX_WS_DEFAULT_VIEW, rect, this, 1000+i);
	m_pPopView->Create(NULL, NULL, WS_VISIBLE | WS_CHILD, rect, this, ID_POPVIDEO_VIEW+m_nPopIndex);

	m_pPopView->ShowWindow(SW_SHOW);
	
	/*
	CRect rc;
	GetClientRect(&rc);
	rc.top+=1;
	rc.left+=1;

	MoveWindow(&rc);
	*/
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

void CWarnPopVideo::SetPopVideoIndex(int nIndex)
{
	m_nPopIndex = nIndex;
}

int CWarnPopVideo::GetPopVideoIndex(void)
{
	return m_nPopIndex;
}

void CWarnPopVideo::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
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
