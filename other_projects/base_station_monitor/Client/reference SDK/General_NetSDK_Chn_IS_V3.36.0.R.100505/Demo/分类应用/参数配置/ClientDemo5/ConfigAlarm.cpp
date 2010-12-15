// ConfigAlarm.cpp : implementation file
//

#include "stdafx.h"
#include "ClientDemo5.h"
#include "ConfigAlarm.h"
#include "ClientDemo5Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CConfigAlarm dialog


CConfigAlarm::CConfigAlarm(CWnd* pParent /*=NULL*/)
	: CDialog(CConfigAlarm::IDD, pParent)
{
	//{{AFX_DATA_INIT(CConfigAlarm)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pAlarmInfo = new DHDEV_ALARM_SCHEDULE_EX;
	if (m_pAlarmInfo != NULL)
	{
		memset(m_pAlarmInfo, 0 ,sizeof(DHDEV_ALARM_SCHEDULE_EX));
	}
	m_bNewProtocol = FALSE;
}
CConfigAlarm::~CConfigAlarm()
{
	if (m_pAlarmInfo != NULL)
	{
		delete m_pAlarmInfo;
		m_pAlarmInfo = NULL;
	}
	
}


void CConfigAlarm::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConfigAlarm)
	DDX_Control(pDX, IDC_TAB_ALARMBOARD, m_ctlAlarmTab);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CConfigAlarm, CDialog)
	//{{AFX_MSG_MAP(CConfigAlarm)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_ALARMBOARD, OnSelchangeTabAlarmboard)
	ON_BN_CLICKED(IDC_BUTTON_APPLY, OnButtonApply)
	ON_BN_CLICKED(IDC_BUTTON_READALL, OnButtonReadall)
	ON_WM_CANCELMODE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConfigAlarm message handlers

BOOL CConfigAlarm::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	//Initialize TabControl control
	InitTabControl();
	g_SetWndStaticText(this);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//Initialize TabControl control 
void CConfigAlarm::InitTabControl()
{
	CRect childRect;
	m_ctlAlarmTab.GetClientRect(childRect);
	childRect.top += 4;
	childRect.bottom -= 4;
	childRect.left += 4;
	childRect.right -= 30;
	
	//Create non-mode dialogue box. Specified tag control is non-mode father dialogue box
	m_TabAlarmInput.Create(IDD_CONFIG_ALARM_INPUT, &m_ctlAlarmTab);
	m_TabAlarmInput.MoveWindow(childRect);
	m_TabMotion.Create(IDD_CONFIG_ALARM_MOTION, &m_ctlAlarmTab);
	m_TabMotion.MoveWindow(childRect);
	m_TabVideoLost.Create(IDD_CONFIG_ALARM_VIDEOLOST, &m_ctlAlarmTab);
	m_TabVideoLost.MoveWindow(childRect);
	m_TabShelter.Create(IDD_CONFIG_ALARM_SHELTER, &m_ctlAlarmTab);
	m_TabShelter.MoveWindow(childRect);
	
	m_ctlAlarmTab.InsertItem(0, ConvertString("Alarm Input"));
	m_ctlAlarmTab.InsertItem(1, ConvertString("Dynamic Detecting"));
	m_ctlAlarmTab.InsertItem(2, ConvertString("Video Lost"));
	m_ctlAlarmTab.InsertItem(3, ConvertString("Video Shelter"));
	
	m_ctlAlarmTab.SetCurSel(0);
	DoTab(0);
}

void CConfigAlarm::DoTab(int nTab)
{
	//Check nTab is within the threshold
	if(nTab>3)
	{
		nTab=3;
	}
	if(nTab<0)
	{
		nTab=0;
	}
	
	BOOL bTab[6];
	bTab[0]=bTab[1]=bTab[2]=bTab[3]=FALSE;
	bTab[nTab]=TRUE;
	
	//Hide or display dialogue box 
	SetDlgState(&m_TabAlarmInput,bTab[0]);
	SetDlgState(&m_TabMotion,bTab[1]);
	SetDlgState(&m_TabVideoLost,bTab[2]);
	SetDlgState(&m_TabShelter,bTab[3]);

	if (bTab[2])
	{
		m_TabMotion.SaveAll();
		for(int i = 0; i < DH_MAX_VIDEO_IN_NUM_EX; i++)
		{
			memcpy(m_TabVideoLost.m_VideoLostInfo[i].stSect, m_TabMotion.m_AlarmMotionInfo[i].stSect, DH_N_WEEKS*DH_N_REC_TSECT*sizeof(DH_TSECT));
			m_TabVideoLost.UpdateSchedule();
		}
	}

	if (bTab[3])
	{
		m_TabMotion.SaveAll();
		for(int i = 0; i < DH_MAX_VIDEO_IN_NUM_EX; i++)
		{
			memcpy(m_TabShelter.m_AlarmShelterInfo[i].stSect, m_TabMotion.m_AlarmMotionInfo[i].stSect, DH_N_WEEKS*DH_N_REC_TSECT*sizeof(DH_TSECT));
			m_TabShelter.UpdateSchedule();
		}
	}
}

void CConfigAlarm::SetDlgState(CWnd *pWnd, BOOL bShow)
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

void CConfigAlarm::OnSelchangeTabAlarmboard(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	int nSelect = m_ctlAlarmTab.GetCurSel();
	if(nSelect>=0)
	{
		DoTab(nSelect);
	}
	*pResult = 0;
}

//Save
void CConfigAlarm::OnButtonApply() 
{
	// TODO: Add your control notification handler code here
	if(UpdateData(TRUE) == FALSE)
	{
		return; 
	}

	if (m_bNewProtocol == FALSE)
	{
		if(m_TabAlarmInput.SaveAll() && m_TabMotion.SaveAll()
			&& m_TabShelter.SaveAll() && m_TabVideoLost.SaveAll())
		{
			int i=0;
			for(i=0;i<DH_MAX_ALARM_IN_NUM_EX;i++)
			{
				m_pAlarmInfo->struLocalAlmIn[i] = m_TabAlarmInput.m_AlarmInInfo[i];
			}
			for(i=0;i<DH_MAX_VIDEO_IN_NUM_EX;i++)
			{
				m_pAlarmInfo->struMotion[i] = m_TabMotion.m_AlarmMotionInfo[i];
				m_pAlarmInfo->struBlind[i] = m_TabShelter.m_AlarmShelterInfo[i];
				m_pAlarmInfo->struVideoLost[i] = m_TabVideoLost.m_VideoLostInfo[i];
				
				memcpy(m_pAlarmInfo->struBlind[i].stSect, m_pAlarmInfo->struMotion[i].stSect, DH_N_WEEKS*DH_N_REC_TSECT*sizeof(DH_TSECT));
				memcpy(m_pAlarmInfo->struVideoLost[i].stSect, m_pAlarmInfo->struMotion[i].stSect, DH_N_WEEKS*DH_N_REC_TSECT*sizeof(DH_TSECT));
			}
			
			((CClientDemo5Dlg*)AfxGetMainWnd())->SetAlarmConfigInfo(m_pAlarmInfo);
		}
		else
		{
			MessageBox(ConvertString("Input error!"), ConvertString("Prompt"));
		}
	}
	else//32路以上只保存某个通道
	{
		int nSelect = m_ctlAlarmTab.GetCurSel();
		switch(nSelect)
		{
		case 0://alarm input
			{
				if(m_TabAlarmInput.SaveAll())
				{
					int nChannelID = m_TabAlarmInput.GetCurChannel();
					if (nChannelID >= 0)
					{
						((CClientDemo5Dlg*)AfxGetMainWnd())->SetAlarmInConfigInfo(&m_TabAlarmInput.m_AlarmInInfo[nChannelID], nChannelID);
					}
					else
					{
						MessageBox(ConvertString("Input error!"), ConvertString("Prompt"));
					}
				}
			}
			break;
		case 1://motion
			{
				if(m_TabMotion.SaveAll())
				{
					int nChannelID = m_TabMotion.GetCurChannel();
					if (nChannelID >= 0)
					{
						((CClientDemo5Dlg*)AfxGetMainWnd())->SetMotionAlarmConfigInfo(&m_TabMotion.m_AlarmMotionInfo[nChannelID], nChannelID);
					}
					else
					{
						MessageBox(ConvertString("Input error!"), ConvertString("Prompt"));
					}
				}
			}
			break;
		case 2://video lost
			{
				if(m_TabVideoLost.SaveAll())
				{
					int nChannelID = m_TabVideoLost.GetCurChannel();
					if (nChannelID >= 0)
					{
						((CClientDemo5Dlg*)AfxGetMainWnd())->SetLossAlarmConfigInfo(&m_TabVideoLost.m_VideoLostInfo[nChannelID], nChannelID);
					}
					else
					{
						MessageBox(ConvertString("Input error!"), ConvertString("Prompt"));
					}
				}
			}
		    break;
		case 3://video blind
			{
				if(m_TabShelter.SaveAll())
				{
					int nChannelID = m_TabShelter.GetCurChannel();
					if (nChannelID >= 0)
					{
						((CClientDemo5Dlg*)AfxGetMainWnd())->SetBlindAlarmConfigInfo(&m_TabShelter.m_AlarmShelterInfo[nChannelID], nChannelID);
					}
					else
					{
						MessageBox(ConvertString("Input error!"), ConvertString("Prompt"));
					}
				}
			}
		    break;
		default:
		    break;
		}
	}	
}

//Read
void CConfigAlarm::OnButtonReadall() 
{
	// TODO: Add your control notification handler code here
	//First clear
	CleanAll();
	//Call father widow to read data 
	((CClientDemo5Dlg*)AfxGetMainWnd())->GetAlarmConfigInfo();
}

/*
 *	Function：display alarm information
 *	Input parameter：
 *	pAlarmInfo：Alarm configuration structure 
 *	nAlarmIn：Alarm input amount 
 *	nAlarmOut：Alarm output amount 
 *	nChannel：Video channel amount
 *  bNewProtocol: more than 32 channels with new protocol
 */
void CConfigAlarm::InitDlgInfo(DHDEV_ALARM_SCHEDULE_EX *pAlarmInfo, BYTE nAlarmIn, BYTE nAlarmOut, BYTE nChannel, BOOL bNewProtocol)
{
	if(pAlarmInfo != NULL)
	{
		*m_pAlarmInfo = *pAlarmInfo;
		
		m_TabAlarmInput.InitDlgInfo(pAlarmInfo,nAlarmIn,nAlarmOut,nChannel,bNewProtocol);
 		m_TabMotion.InitDlgInfo(pAlarmInfo,nAlarmIn,nAlarmOut,nChannel,bNewProtocol);
 		m_TabShelter.InitDlgInfo(pAlarmInfo,nAlarmIn,nAlarmOut,nChannel,bNewProtocol);
 		m_TabVideoLost.InitDlgInfo(pAlarmInfo,nAlarmIn,nAlarmOut,nChannel,bNewProtocol);
		m_bNewProtocol = bNewProtocol;
	}
}

//Clear 
void CConfigAlarm::CleanAll()
{
	m_TabAlarmInput.CleanAll();
	m_TabMotion.CleanAll();
	m_TabShelter.CleanAll();
	m_TabVideoLost.CleanAll();
}

void CConfigAlarm::OnCancelMode() 
{
	CDialog::OnCancelMode();
	
	// TODO: Add your message handler code here
	
}








































































































































































































































































































































































































































































































































