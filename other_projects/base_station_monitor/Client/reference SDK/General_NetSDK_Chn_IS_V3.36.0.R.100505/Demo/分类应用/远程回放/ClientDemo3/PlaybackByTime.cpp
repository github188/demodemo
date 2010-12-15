// PlaybackByTime.cpp : implementation file
//

#include "stdafx.h"
#include "ClientDemo3.h"
#include "PlaybackByTime.h"
#include "ClientDemo3Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPlaybackByTime dialog


CPlaybackByTime::CPlaybackByTime(CWnd* pParent /*=NULL*/)
	: CDialog(CPlaybackByTime::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPlaybackByTime)
	m_timeFrom = 0;
	m_timeLoadFrom = 0;
	m_timeLoadTo = 0;
	m_timeTo = 0;
	m_dateFrom = COleDateTime::GetCurrentTime();
	m_dateLoadFrom = COleDateTime::GetCurrentTime();
	m_dateLoadTo = COleDateTime::GetCurrentTime();
	m_dateTo = COleDateTime::GetCurrentTime();
	//}}AFX_DATA_INIT
	m_hLoginID = 0;
	m_dwTotalSize = 0;
	m_dwCurValue = 0;
	m_nTimer = 0;
}


void CPlaybackByTime::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPlaybackByTime)
	DDX_Control(pDX, IDC_COMBO_TIMEPLAYMODE, m_ctlPlayMode);
	DDX_Control(pDX, IDC_COMBO_LOADCHANNEL, m_ctlLoadChannel);
	DDX_Control(pDX, IDC_PROGRESS_BYTIME, m_ctlLoadPro);
	DDX_Control(pDX, IDC_COMBO_CHANNEL, m_ctlChannel);
	DDX_DateTimeCtrl(pDX, IDC_TIME_FROM, m_timeFrom);
	DDX_DateTimeCtrl(pDX, IDC_TIME_LOAD_FROM, m_timeLoadFrom);
	DDX_DateTimeCtrl(pDX, IDC_TIME_LOAD_TO, m_timeLoadTo);
	DDX_DateTimeCtrl(pDX, IDC_TIME_TO, m_timeTo);
	DDX_DateTimeCtrl(pDX, IDC_DATE_FROM, m_dateFrom);
	DDX_DateTimeCtrl(pDX, IDC_DATE_LOAD_FROM, m_dateLoadFrom);
	DDX_DateTimeCtrl(pDX, IDC_DATE_LOAD_TO, m_dateLoadTo);
	DDX_DateTimeCtrl(pDX, IDC_DATE_TO, m_dateTo);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPlaybackByTime, CDialog)
	//{{AFX_MSG_MAP(CPlaybackByTime)
	ON_BN_CLICKED(IDC_BUTTON_PLAY, OnButtonPlay)
	ON_BN_CLICKED(IDC_BUTTON_DOWNLOAD, OnButtonDownload)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPlaybackByTime message handlers

//Playback 
void CPlaybackByTime::OnButtonPlay() 
{
	// TODO: Add your control notification handler code here
	BOOL bValid = UpdateData(TRUE);
	if(bValid)
	{
		//Channel number 
		int nChannelId = 0;
		int nIndex = m_ctlChannel.GetCurSel();
		nChannelId = (int)m_ctlChannel.GetItemData(nIndex);
		//Time
		NET_TIME netTimeFrom = ConvertToDateTime(m_dateFrom,m_timeFrom);
		NET_TIME netTimeTo = ConvertToDateTime(m_dateTo,m_timeTo);
		//Playback
		//To get play mode 
		nIndex = m_ctlPlayMode.GetCurSel();
		if(CB_ERR != nIndex)
		{
			DH_PlayBackMode ePlayMode = (DH_PlayBackMode)m_ctlPlayMode.GetItemData(nIndex);
			if(ePlayMode == EM_SERVERMODE)
			{
				((CClientDemo3Dlg *)AfxGetMainWnd())->PlayBackByTimeServerMode(nChannelId,&netTimeFrom,&netTimeTo);
			}
			else
			{
				((CClientDemo3Dlg *)AfxGetMainWnd())->PlayBackByTime(nChannelId,&netTimeFrom,&netTimeTo);
			}
		}
	}
}

//Download 
void CPlaybackByTime::OnButtonDownload() 
{
	// TODO: Add your control notification handler code here
	CString strText;
	GetDlgItemText(IDC_BUTTON_DOWNLOAD,strText);
	if(strText == "Download" || strText == ConvertString("Download") )
	{
		BOOL bValid = UpdateData(TRUE);
		if(bValid)
		{
			//Channel number
			int nIndex = m_ctlLoadChannel.GetCurSel();
			int nChannelId = (int)m_ctlLoadChannel.GetItemData(nIndex);
			//Time
			NET_TIME netTimeFrom = ConvertToDateTime(m_dateLoadFrom,m_timeLoadFrom);
			NET_TIME netTimeTo = ConvertToDateTime(m_dateLoadTo,m_timeLoadTo);
			BOOL bSuccess = ((CClientDemo3Dlg *)AfxGetMainWnd())->DownLoadByTime(nChannelId,&netTimeFrom,&netTimeTo,TRUE);
			if(bSuccess)
			{
				SetDlgItemText(IDC_BUTTON_DOWNLOAD, ConvertString("Stop"));
			}
		}
	}
	else
	{
		NET_TIME netTimeFrom = {0};
		NET_TIME netTimeTo = {0};
		BOOL bSuccess = ((CClientDemo3Dlg *)AfxGetMainWnd())->DownLoadByTime(0,&netTimeFrom,&netTimeTo,FALSE);
		if(bSuccess)
		{
			SetDlgItemText(IDC_BUTTON_DOWNLOAD, ConvertString("Download"));
			m_dwTotalSize = 0;
			m_dwCurValue = 0;
			long hDownLoad = ((CClientDemo3Dlg *)AfxGetMainWnd())->m_pDownLoadByTime;
			if(0 != hDownLoad)
			{
				CLIENT_StopDownload(hDownLoad);
				((CClientDemo3Dlg *)AfxGetMainWnd())->m_pDownLoadByTime = 0;
			}
			m_ctlLoadPro.SetRange32(0,0);
			m_ctlLoadPro.SetPos(0);
			Invalidate();
		}
	}
}

//Set channel dropdown menu 
void CPlaybackByTime::InitComboBox(int nChannel,long lLoginID)
{
	m_hLoginID = lLoginID;

	int nIndex = 0;
	int i = 0;
	CString str;
	m_ctlChannel.ResetContent();
	for(i=0;i<nChannel;i++)
	{
		str.Format("%d",i);
		nIndex = m_ctlChannel.AddString(str);
		m_ctlChannel.SetItemData(nIndex,i);
	}
	m_ctlChannel.SetCurSel(0);

	m_ctlLoadChannel.ResetContent();
	for(i=0;i<nChannel;i++)
	{
		str.Format("%d",i);
		nIndex = m_ctlLoadChannel.AddString(str);
		m_ctlLoadChannel.SetItemData(nIndex,i);
	}
	m_ctlLoadChannel.SetCurSel(0);
}

BOOL CPlaybackByTime::OnInitDialog() 
{
	CDialog::OnInitDialog();
	g_SetWndStaticText(this);
	// TODO: Add extra initialization here
	//Set current time
	CTimeSpan timeSpan(1,0,0,0);
	CTime curTime = CTime::GetCurrentTime();
	CTime dateFrom = curTime - timeSpan;
	m_dateFrom.SetDateTime(dateFrom.GetYear(), dateFrom.GetMonth(), dateFrom.GetDay(), 
		dateFrom.GetHour(), dateFrom.GetMinute(), dateFrom.GetSecond());
	m_timeFrom = curTime;
	m_dateTo.SetDateTime(curTime.GetYear(), curTime.GetMonth(), curTime.GetDay(), 
		curTime.GetHour(), curTime.GetMinute(), curTime.GetSecond());
	m_timeTo = curTime;
	CTime dateLoadFrom = curTime - timeSpan;
	m_dateLoadFrom.SetDateTime(dateLoadFrom.GetYear(), dateLoadFrom.GetMonth(), dateLoadFrom.GetDay(), 
		dateLoadFrom.GetHour(), dateLoadFrom.GetMinute(), dateLoadFrom.GetSecond());
	m_timeLoadFrom = curTime;
	m_dateLoadTo.SetDateTime(curTime.GetYear(), curTime.GetMonth(), curTime.GetDay(), 
		curTime.GetHour(), curTime.GetMinute(), curTime.GetSecond());
	m_timeLoadTo = curTime;

	m_ctlPlayMode.ResetContent();
	int nIndex = m_ctlPlayMode.AddString( ConvertString("direct"));
	m_ctlPlayMode.SetItemData(nIndex,EM_DIRECTMODE);
	nIndex = m_ctlPlayMode.AddString(ConvertString("callback"));
	m_ctlPlayMode.SetItemData(nIndex,EM_SERVERMODE);
	m_ctlPlayMode.SetCurSel(0);

	m_ctlLoadPro.SetRange32(0,(int)(m_dwTotalSize/100));
	m_ctlLoadPro.SetPos(0);
	m_nTimer = SetTimer(1, 500, NULL);
	
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//Turn time format CTime to NET_TIME type
NET_TIME CPlaybackByTime::ConvertTime(CTime date, CTime time)
{
	NET_TIME netTime = {0};
	netTime.dwYear = date.GetYear();
	netTime.dwMonth = date.GetMonth();
	netTime.dwDay = date.GetDay();
	netTime.dwHour = time.GetHour();
	netTime.dwMinute = time.GetMinute();
	netTime.dwSecond =time.GetSecond();
	return netTime;
}

void CPlaybackByTime::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent == m_nTimer)
	{
		m_ctlLoadPro.SetRange32(0,(int)(m_dwTotalSize/100));
		m_ctlLoadPro.SetPos((int)(m_dwCurValue/100));
	}
	
	CDialog::OnTimer(nIDEvent);
}

//Logg off and then clear 
void CPlaybackByTime::Clean()
{
	SetDlgItemText(IDC_BUTTON_DOWNLOAD, ConvertString("Download"));
	m_dwTotalSize = 0;
	m_dwCurValue = 0;
}

NET_TIME CPlaybackByTime::ConvertToDateTime(const COleDateTime &date, const CTime &time)
{
	NET_TIME netTime = {0};
	int year = date.GetYear();
	if (year < 2000)
	{
		netTime.dwYear = 2000;
		netTime.dwMonth = 1;
		netTime.dwDay = 1;
		netTime.dwHour = 0;
		netTime.dwMinute = 0;
		netTime.dwSecond = 0;
	}
	else
	{
		netTime.dwYear = date.GetYear();
		netTime.dwMonth = date.GetMonth();
		netTime.dwDay = date.GetDay();
		netTime.dwHour = time.GetHour();
		netTime.dwMinute = time.GetMinute();
		netTime.dwSecond =time.GetSecond();
	}
	
	return netTime;
}


